//
// srecord - Manipulate EPROM load files
// Copyright (C) 2010, 2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
#include <cstring>

#include <srecord/output/filter/reblock.h>
#include <srecord/record.h>


srecord::output_filter_reblock::~output_filter_reblock()
{
    if (buffer_pos != 0)
        flush_buffer(false);
    delete [] buffer;
}


srecord::output_filter_reblock::output_filter_reblock(
    const output::pointer &a_deeper,
    bool a_align
) :
    output_filter(a_deeper),
    align(a_align),
    block_size(0),
    buffer_address(0),
    buffer(0),
    buffer_pos(0),
    buffer_max(0)
{
    track_block_size();

    // We allocate a large-ish buffer to start with.  The idea is we
    // will fill it up before we deeper->write it, this will minimize
    // the number of memmove() calls.
    buffer_max = 1 << 14;
    buffer = new unsigned char [buffer_max];

    assert(buffer_max > 2 * record::max_data_length);
}


srecord::output_filter_reblock::pointer
srecord::output_filter_reblock::create(const output::pointer &a_deeper,
    bool a_align)
{
    return pointer(new output_filter_reblock(a_deeper, a_align));
}


void
srecord::output_filter_reblock::track_block_size(void)
{
    block_size = output_filter::preferred_block_size_get();
}


void
srecord::output_filter_reblock::line_length_set(int n)
{
    output_filter::line_length_set(n);
    track_block_size();
}


void
srecord::output_filter_reblock::address_length_set(int n)
{
    output_filter::address_length_set(n);
    track_block_size();
}


int
srecord::output_filter_reblock::preferred_block_size_get(void)
    const
{
    return record::max_data_length;
}


bool
srecord::output_filter_reblock::preferred_block_size_set(int nbytes)
{
    if (!output_filter::preferred_block_size_set(nbytes))
        return false;
    track_block_size();
    return true;
}


void
srecord::output_filter_reblock::write(const record &r)
{
    if (r.get_type() != record::type_data)
    {
        flush_buffer(false);
        output_filter::write(r);
        return;
    }
    if (r.get_length() == 0)
        return;
    if (buffer_pos != 0)
    {
        if (r.get_address() != buffer_address + buffer_pos)
        {
            flush_buffer(false);
        }
        else if (buffer_pos + r.get_length() > buffer_max)
        {
            flush_buffer(true);
        }
    }
    assert(buffer_pos + r.get_length() <= buffer_max);
    if (buffer_pos == 0)
    {
        buffer_address = r.get_address();
    }
    assert(buffer);
    memcpy(buffer + buffer_pos, r.get_data(), r.get_length());
    buffer_pos += r.get_length();
    assert(buffer_pos <= buffer_max);

    // We don't call the deeper->write method immediately, that could
    // cause one memmove call per write (expensive).  Instead we collect
    // up a whole bunch that can be done sequentially, leaving fewer
    // than block_size bytes to shuffle.
}


void
srecord::output_filter_reblock::flush_buffer(bool partial)
{
    if (buffer_pos == 0)
        return;

    size_t p = 0;
    if (align)
    {
        unsigned residual = (buffer_address % block_size);
        if (residual)
        {
            size_t size = block_size - residual;
            if (size > buffer_pos)
            {
                size = buffer_pos;
                record r2(record::type_data, buffer_address, buffer, size);
                output_filter::write(r2);
                buffer_pos = 0;
                buffer_address = 0;
                return;
            }
            record r2(record::type_data, buffer_address, buffer, size);
            output_filter::write(r2);
            p += size;
        }
    }

    while (p < buffer_pos)
    {
        size_t size = block_size;
        if (p + size > buffer_pos)
        {
            if (partial)
                break;
            size = buffer_pos - p;
        }
        record r2(record::type_data, buffer_address + p, buffer + p, size);
        output_filter::write(r2);
        p += size;
    }
    if (p == buffer_pos)
    {
        buffer_pos = 0;
        buffer_address = 0;
    }
    else
    {
        // Shuffle down.
        buffer_pos -= p;
        buffer_address += p;
        memmove(buffer, buffer + p, buffer_pos);
    }
}


// vim: set ts=8 sw=4 et :
