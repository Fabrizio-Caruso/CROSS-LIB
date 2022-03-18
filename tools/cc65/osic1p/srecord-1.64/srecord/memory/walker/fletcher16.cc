//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
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

#include <srecord/memory/walker/fletcher16.h>
#include <srecord/output.h>


srecord::memory_walker_fletcher16::~memory_walker_fletcher16()
{
}


srecord::memory_walker_fletcher16::memory_walker_fletcher16(
    int a_sum1,
    int a_sum2,
    int a_answer,
    endian_t a_end
) :
    checksum(a_sum1, a_sum2, a_answer, a_end)
{
}


srecord::memory_walker_fletcher16::pointer
srecord::memory_walker_fletcher16::create(int a_sum1, int a_sum2,
    int a_answer, endian_t a_end)
{
    return
        pointer(new memory_walker_fletcher16(a_sum1, a_sum2, a_answer, a_end));
}


void
srecord::memory_walker_fletcher16::observe(unsigned long, const void *data,
    int data_size)
{
    checksum.nextbuf(data, data_size);
}


unsigned
srecord::memory_walker_fletcher16::get(void)
    const
{
    return checksum.get();
}


// vim: set ts=8 sw=4 et :
