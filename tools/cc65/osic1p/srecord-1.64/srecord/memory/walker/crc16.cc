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

#include <srecord/memory/walker/crc16.h>
#include <srecord/output.h>


srecord::memory_walker_crc16::~memory_walker_crc16()
{
    delete checksum;
}


srecord::memory_walker_crc16::memory_walker_crc16(crc16::seed_mode_t seed_mode,
    bool augment_flag, unsigned short polynomial, crc16::bit_direction_t bitdir)
{
    checksum = new crc16(seed_mode, augment_flag, polynomial, bitdir);
}


srecord::memory_walker_crc16::pointer
srecord::memory_walker_crc16::create(crc16::seed_mode_t arg1, bool a_augment,
    unsigned short polynomial, crc16::bit_direction_t a_bitdir)
{
    return
        pointer
        (
            new srecord::memory_walker_crc16
            (
                arg1,
                a_augment,
                polynomial,
                a_bitdir
            )
        );
}


void
srecord::memory_walker_crc16::observe(unsigned long, const void *data,
    int length)
{
    checksum->nextbuf(data, length);
}


unsigned
srecord::memory_walker_crc16::get()
    const
{
    return checksum->get();
}
