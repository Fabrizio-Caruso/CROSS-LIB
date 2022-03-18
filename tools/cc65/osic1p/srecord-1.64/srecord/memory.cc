//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2003, 2006-2014 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <cstring>

#include <srecord/input.h>
#include <srecord/memory.h>
#include <srecord/memory/walker/alignment.h>
#include <srecord/memory/walker/compare.h>
#include <srecord/memory/walker/continuity.h>
#include <srecord/record.h>
#include <srecord/string.h>


srecord::memory::memory() :
    nchunks(0),
    nchunks_max(0),
    chunk(0),
    cache(0),
    find_next_chunk_index(0),
    header(0),
    execution_start_address(0)
{
}


srecord::memory::memory(const srecord::memory &rhs) :
    nchunks(0),
    nchunks_max(0),
    chunk(0),
    cache(0),
    find_next_chunk_index(0),
    header(0),
    execution_start_address(0)
{
    copy(rhs);
}


srecord::memory &
srecord::memory::operator=(const srecord::memory &rhs)
{
    if (&rhs != this)
    {
        clear();
        copy(rhs);
    }
    return *this;
}


srecord::memory::~memory()
{
    clear();
}


void
srecord::memory::clear(void)
{
    delete header;
    header = 0;
    delete execution_start_address;
    execution_start_address = 0;
    for (int j = 0; j < nchunks; ++j)
        delete chunk[j];
    if (chunk)
        delete [] chunk;
    nchunks = 0;
    nchunks_max = 0;
    chunk = 0;
    cache = 0;
}


void
srecord::memory::copy(const srecord::memory &rhs)
{
    delete header;
    header = 0;
    if (rhs.header)
        header = new srecord::record(*rhs.header);

    delete execution_start_address;
    execution_start_address = 0;
    if (rhs.execution_start_address)
    {
        execution_start_address =
            new srecord::record(*rhs.execution_start_address);
    }

    nchunks = rhs.nchunks;
    while (nchunks_max < nchunks)
        nchunks_max = nchunks_max * 2 + 4;
    chunk = new srecord::memory_chunk * [nchunks_max];
    for (int j = 0; j < nchunks; ++j)
    {
        // use copy-new to make the copies
        chunk[j] = new srecord::memory_chunk(*(rhs.chunk[j]));
    }
}


srecord::memory_chunk *
srecord::memory::find(unsigned long address)
    const
{
    //
    // Speed things up if we've been there recently.
    //
    if (cache && cache->get_address() == address)
        return cache;

    //
    // Binary chop to find the appropriate chunk.
    //
    int min = 0;
    int max = nchunks - 1;
    srecord::memory_chunk *mcp = 0;
    while (min <= max)
    {
        int mid = (min + max) / 2;
        mcp = chunk[mid];
        if (mcp->get_address() == address)
        {
            cache = mcp;
            return cache;
        }
        if (address < mcp->get_address())
            max = mid - 1;
        else
            min = mid + 1;
    }

    //
    // We need a new row.  Make sure there is enough room.
    //
    if (nchunks >= nchunks_max)
    {
        nchunks_max = nchunks_max * 2 + 4;
        srecord::memory_chunk **tmp =
            new srecord::memory_chunk * [nchunks_max];
        for (int j = 0; j < nchunks; ++j)
            tmp[j] = chunk[j];
        delete [] chunk;
        chunk = tmp;
    }

    //
    // Insert the new chunk.
    //
    mcp = new srecord::memory_chunk(address);
    for (int up = nchunks; up > min; --up)
        chunk[up] = chunk[up - 1];
    ++nchunks;
    chunk[min] = mcp;

    cache = mcp;
    return mcp;
}


void
srecord::memory::set(unsigned long address, int datum)
{
    unsigned long address_hi = address / srecord::memory_chunk::size;
    unsigned long address_lo = address % srecord::memory_chunk::size;
    srecord::memory_chunk *mcp = find(address_hi);
    mcp->set(address_lo, datum);
}


int
srecord::memory::get(unsigned long address)
    const
{
    unsigned long address_hi = address / srecord::memory_chunk::size;
    unsigned long address_lo = address % srecord::memory_chunk::size;
    srecord::memory_chunk *mcp = find(address_hi);
    return mcp->get(address_lo);
}


bool
srecord::memory::set_p(unsigned long address)
    const
{
    unsigned long address_hi = address / srecord::memory_chunk::size;
    unsigned long address_lo = address % srecord::memory_chunk::size;
    srecord::memory_chunk *mcp = find(address_hi);
    return mcp->set_p(address_lo);
}


bool
srecord::memory::equal(const srecord::memory &lhs, const srecord::memory &rhs)
{
    if (lhs.nchunks != rhs.nchunks)
        return false;
    for (int j = 0; j < lhs.nchunks; ++j)
        if (lhs.chunk[j][0] != rhs.chunk[j][0])
            return false;
    return true;
}



bool
srecord::memory::compare(const srecord::memory &lhs, const srecord::memory &rhs)
{
    srecord::memory_walker_compare::pointer wlhs =
        srecord::memory_walker_compare::create(rhs, true);
    lhs.walk(wlhs);
    wlhs->print("Left");
    srecord::memory_walker_compare::pointer wrhs =
        srecord::memory_walker_compare::create(lhs, false);
    rhs.walk(wrhs);
    wrhs->print("Right");
    return (!wlhs->same() || !wrhs->same());
}


unsigned long
srecord::memory::get_lower_bound()
    const
{
    if (nchunks == 0)
        return 0;
    return chunk[0]->get_lower_bound();
}


unsigned long
srecord::memory::get_upper_bound(void)
    const
{
    if (nchunks == 0)
        return 0;
    return chunk[nchunks - 1]->get_upper_bound();
}


void
srecord::memory::walk(srecord::memory_walker::pointer w)
    const
{
    w->notify_upper_bound(get_upper_bound());
    w->observe_header(get_header());
    for (int j = 0; j < nchunks; ++j)
        chunk[j]->walk(w);
    w->observe_end();

    // Only write an execution start address record if we were given one.
    if (execution_start_address)
        w->observe_start_address(get_execution_start_address());
}


void
srecord::memory::reader(const srecord::input::pointer &ifp,
    defcon_t redundant_bytes,
    defcon_t contradictory_bytes)
{
    srecord::record record;
    while (ifp->read(record))
    {
        switch (record.get_type())
        {
        case srecord::record::type_header:
            if (!header)
            {
                header = new srecord::record(record);
            }
            break;

        case srecord::record::type_unknown:
        case srecord::record::type_data_count:
            break;

        case srecord::record::type_data:
            //
            // For each data byte, we have to check for duplicates.  We
            // issue warnings for redundant settings, and we issue error
            // for contradictory settings.
            //
            for (size_t j = 0; j < record.get_length(); ++j)
            {
                srecord::record::address_t address = record.get_address() + j;
                int n = record.get_data(j);
                if (set_p(address))
                {
                    int old = get(address);
                    if (n == old)
                    {
                        // duplicate
                        switch (redundant_bytes)
                        {
                        default:
                        case defcon_ignore:
                            break;

                        case defcon_warning:
                            ifp->warning
                            (
                                "redundant 0x%08lX value (0x%02X)",
                                (long)address,
                                n
                            );
                            break;

                        case defcon_fatal_error:
                            ifp->fatal_error
                            (
                                "redundant 0x%08lX value (0x%02X)",
                                (long)address,
                                n
                            );
                            break;
                        }
                    }
                    else
                    {
                        // contradicts
                        switch (contradictory_bytes)
                        {
                        case defcon_ignore:
                            break;

                        case defcon_warning:
                            ifp->warning
                            (
                                "multiple 0x%08lX values (previous = 0x%02X, "
                                    "this one = 0x%02X)",
                                (long)address,
                                old,
                                n
                            );
                            break;

                        case defcon_fatal_error:
                            ifp->fatal_error
                            (
                                "multiple 0x%08lX values (previous = 0x%02X, "
                                    "this one = 0x%02X)",
                                (long)address,
                                old,
                                n
                            );
                            break;
                        }
                    }
                }
                set(address, n);
            }
            break;

        case srecord::record::type_execution_start_address:
            if (!execution_start_address)
            {
                execution_start_address = new srecord::record(record);
            }
            break;
        }
    }
}


bool
operator == (const srecord::memory &lhs, const srecord::memory &rhs)
{
    return srecord::memory::equal(lhs, rhs);
}


bool
operator != (const srecord::memory &lhs, const srecord::memory &rhs)
{
    return !srecord::memory::equal(lhs, rhs);
}


srecord::memory_chunk *
srecord::memory::find_next_chunk(unsigned long address)
    const
{
    //
    // This method is generally called sequentially, to visit each
    // and every byte, in cases where walk() is not appropriate.
    // As such, a binary chop isn't necessary.
    //
    if (find_next_chunk_index < nchunks)
    {
        srecord::memory_chunk *mcp = chunk[find_next_chunk_index];
        if (mcp->get_address() > address)
            find_next_chunk_index = 0;
    }
    else
    {
        find_next_chunk_index = 0;
    }

    while (find_next_chunk_index < nchunks)
    {
        srecord::memory_chunk *mcp = chunk[find_next_chunk_index];
        if (mcp->get_address() >= address)
            return mcp;
        find_next_chunk_index++;
    }
    return 0;
}


bool
srecord::memory::find_next_data(unsigned long &address, void *data,
    size_t &nbytes) const
{
    unsigned long address_hi = address / srecord::memory_chunk::size;
    for (;;)
    {
        srecord::memory_chunk *mcp = find_next_chunk(address_hi);
        if (!mcp)
            return false;
        if (mcp->find_next_data(address, data, nbytes))
            return true;
        address_hi = mcp->get_address() + 1;
        address = address_hi * srecord::memory_chunk::size;
    }
}


srecord::record *
srecord::memory::get_header(void)
    const
{
    return header;
}


void
srecord::memory::set_header(const std::string &text)
{
    // We do NOT call string_url_encode here, we decoded the command
    // line, so that weird characters can be placed into the header.  Such
    // as a terminating NUL character.  Re-encoding defeats the purpose.
    delete header;
    size_t len = text.size();
    if (len > srecord::record::max_data_length)
        len = srecord::record::max_data_length;
    header =
        new srecord::record
        (
            srecord::record::type_header,
            0,
            (srecord::record::data_t *)text.c_str(),
            len
        );
}


srecord::record *
srecord::memory::get_execution_start_address(void)
    const
{
    return execution_start_address;
}


void
srecord::memory::set_execution_start_address(unsigned long addr)
{
    delete execution_start_address;
    execution_start_address =
        new srecord::record
        (
            srecord::record::type_execution_start_address,
            addr,
            0,
            0
        );
}


bool
srecord::memory::has_holes(void)
    const
{
    srecord::memory_walker_continuity::pointer sniffer =
        srecord::memory_walker_continuity::create();
    walk(sniffer);
    return (!sniffer->is_continuous());
}


bool
srecord::memory::is_well_aligned(unsigned multiple)
    const
{
    if (multiple < 2)
        return true;
    srecord::memory_walker_alignment::pointer sniffer =
        srecord::memory_walker_alignment::create(multiple);
    walk(sniffer);
    return sniffer->is_well_aligned();
}


// vim: set ts=8 sw=4 et :
