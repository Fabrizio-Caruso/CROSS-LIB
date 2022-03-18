//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2003, 2006-2008, 2010 Peter Miller
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
#include <srecord/memory/chunk.h>
#include <srecord/memory/walker.h>


srecord::memory_chunk::memory_chunk(unsigned long arg) :
    address(arg)
{
    memset(data, 0, sizeof(data));
    memset(mask, 0, sizeof(mask));
}


srecord::memory_chunk::memory_chunk(const srecord::memory_chunk &arg) :
    address(arg.address)
{
    memcpy(data, arg.data, sizeof(data));
    memcpy(mask, arg.mask, sizeof(mask));
}


srecord::memory_chunk &
srecord::memory_chunk::operator=(const srecord::memory_chunk &arg)
{
    if (this != &arg)
    {
        address = arg.address;
        memcpy(data, arg.data, sizeof(data));
        memcpy(mask, arg.mask, sizeof(mask));
    }
    return *this;
}


srecord::memory_chunk::~memory_chunk()
{
}


void
srecord::memory_chunk::set(unsigned long offset, int datum)
{
    data[offset] = datum;
    mask[offset >> 3] |= (1 << (offset & 7));
}


void
srecord::memory_chunk::walk(srecord::memory_walker::pointer w)
    const
{
    for (int j = 0; j < size; ++j)
    {
        if (!set_p(j))
            continue;
        int k;
        for (k = j + 1; k < size && set_p(k); ++k)
            ;
        w->observe(address * size + j, data + j, k - j);
        j = k;
    }
}


bool
srecord::memory_chunk::find_next_data(unsigned long &ret_addr, void *ret_data,
        size_t &nbytes)
    const
{
    for (unsigned j = ret_addr % size; j < size; ++j)
    {
        if (!set_p(j))
            continue;
        size_t max = j + nbytes;
        if (max > size)
            max = size;
        unsigned k;
        for (k = j + 1; k < max && set_p(k); ++k)
            ;
        nbytes = k - j;
        memcpy(ret_data, data + j, nbytes);
        ret_addr = address * size + j;
        return true;
    }
    return false;
}


int
srecord::memory_chunk::get(unsigned long offset)
{
    return data[offset];
}


bool
srecord::memory_chunk::set_p(unsigned long offset)
    const
{
    return (0 != (mask[offset >> 3] & (1 << (offset & 7))));
}


bool
srecord::memory_chunk::equal(const srecord::memory_chunk &lhs,
    const srecord::memory_chunk &rhs)
{
    return
    (
        lhs.address == rhs.address
    &&
        0 == memcmp(lhs.data, rhs.data, sizeof(lhs.data))
    &&
        0 == memcmp(lhs.mask, rhs.mask, sizeof(lhs.mask))
    );
}


unsigned long
srecord::memory_chunk::get_upper_bound()
    const
{
    for (size_t j = size; j > 0; --j)
    {
        if (set_p(j - 1))
            return (address * size + j);
    }
    // can't happen?
    return (address * size);
}


unsigned long
srecord::memory_chunk::get_lower_bound()
    const
{
    for (size_t j = 0; j < size; ++j)
    {
        if (set_p(j))
            return (address * size + j);
    }
    // can't happen?
    return (address * size);
}


bool
srecord::operator == (const srecord::memory_chunk &lhs,
    const srecord::memory_chunk &rhs)
{
    return srecord::memory_chunk::equal(lhs, rhs);
}


bool
srecord::operator != (const srecord::memory_chunk &lhs,
    const srecord::memory_chunk &rhs)
{
    return !srecord::memory_chunk::equal(lhs, rhs);
}


// vim: set ts=8 sw=4 et :
