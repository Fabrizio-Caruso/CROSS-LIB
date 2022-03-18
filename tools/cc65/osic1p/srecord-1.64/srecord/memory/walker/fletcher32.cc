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

#include <srecord/memory/walker/fletcher32.h>
#include <srecord/output.h>


srecord::memory_walker_fletcher32::~memory_walker_fletcher32()
{
}


srecord::memory_walker_fletcher32::memory_walker_fletcher32()
{
}


srecord::memory_walker_fletcher32::pointer
srecord::memory_walker_fletcher32::create()
{
    return pointer(new srecord::memory_walker_fletcher32());
}


void
srecord::memory_walker_fletcher32::observe(unsigned long, const void *data,
    int length)
{
    checksum.nextbuf(data, length);
}


unsigned
srecord::memory_walker_fletcher32::get()
    const
{
    return checksum.get();
}

// vim: set ts=8 sw=4 et :
