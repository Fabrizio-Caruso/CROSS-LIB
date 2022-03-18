//
// srecord - Manipulate EPROM load files
// Copyright (C) 2012 Peter Miller
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

#include <srecord/memory/walker/alignment.h>


srecord::memory_walker_alignment::~memory_walker_alignment()
{
}


srecord::memory_walker_alignment::memory_walker_alignment(unsigned a_multiple) :
    multiple(a_multiple),
    current_address(0),
    data_seen(false),
    well_aligned(true)
{
    assert(multiple >= 2);
}


srecord::memory_walker_alignment::pointer
srecord::memory_walker_alignment::create(unsigned a_multiple)
{
    return pointer(new memory_walker_alignment(a_multiple));
}


bool
srecord::memory_walker_alignment::is_well_aligned(void)
    const
{
    return well_aligned;
}


void
srecord::memory_walker_alignment::observe(unsigned long address, const void *,
    int data_size)
{
    if (data_seen && address != current_address)
    {
        // Check end.
        if (current_address % multiple != 0)
            well_aligned = false;
    }

    if (!data_seen || address != current_address)
    {
        // Check beginning.
        if (address % multiple != 0)
            well_aligned = false;
    }

    current_address = address + data_size;
    data_seen = true;
}


void
srecord::memory_walker_alignment::observe_end(void)
{
    // Check last end.
    if (data_seen && current_address % multiple != 0)
        well_aligned = false;
}


// vim: set ts=8 sw=4 et :
