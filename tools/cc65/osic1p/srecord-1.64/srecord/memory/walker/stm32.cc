//
// srecord - manipulate eprom load files
// Copyright (C) 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
//      From: Hendrik Beijeman <hbeyeman@gmail.com>
//      To: pmiller@opensource.org.au
//      Subject: Re: patch for srecord 1.59
//      Date: Sat, 05 May 2012 06:26:02 +0200 (05/05/12 14:26:02)
//
//      I hereby formally assign all copyright to the author of srecord,
//      Peter Miller.
//

#include <cassert>
#include <cstdio>

#include <srecord/arglex.h>
#include <srecord/memory/walker/stm32.h>
#include <srecord/output.h>


srecord::memory_walker_stm32::~memory_walker_stm32()
{
}


srecord::memory_walker_stm32::memory_walker_stm32()
{
}


srecord::memory_walker_stm32::pointer
srecord::memory_walker_stm32::create(void)
{
    return pointer(new srecord::memory_walker_stm32());
}


void
srecord::memory_walker_stm32::observe(unsigned long, const void *data,
    int length)
{
    checksum.nextbuf(data, length);
}


unsigned
srecord::memory_walker_stm32::get(void)
    const
{
    return checksum.get();
}


// vim: set ts=8 sw=4 et :
