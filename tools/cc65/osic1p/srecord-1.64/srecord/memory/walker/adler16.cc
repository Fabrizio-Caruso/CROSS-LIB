//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2006-2010 Peter Miller
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

#include <srecord/memory/walker/adler16.h>
#include <srecord/output.h>


srecord::memory_walker_adler16::~memory_walker_adler16()
{
}


srecord::memory_walker_adler16::memory_walker_adler16()
{
}


srecord::memory_walker_adler16::pointer
srecord::memory_walker_adler16::create()
{
    return pointer(new srecord::memory_walker_adler16());
}


void
srecord::memory_walker_adler16::observe(unsigned long, const void *data,
    int length)
{
    checksum.nextbuf(data, length);
}


unsigned
srecord::memory_walker_adler16::get()
    const
{
    return checksum.get();
}
