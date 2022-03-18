//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2003, 2006, 2007, 2010, 2012 Peter Miller
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


#include <srecord/memory/walker.h>


srecord::memory_walker::memory_walker()
{
}


srecord::memory_walker::~memory_walker()
{
}


void
srecord::memory_walker::notify_upper_bound(unsigned long)
{
    // Do nothing.
}


void
srecord::memory_walker::observe_header(const srecord::record *)
{
    // Do nothing.
}


void
srecord::memory_walker::observe_start_address(const srecord::record *)
{
    // Do nothing.
}


void
srecord::memory_walker::observe_end(void)
{
    // Do nothing.
}


// vim: set ts=8 sw=4 et :
