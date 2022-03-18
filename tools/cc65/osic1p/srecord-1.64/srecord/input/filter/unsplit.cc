//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2006-2008, 2010 Peter Miller
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

#include <srecord/input/filter/unsplit.h>
#include <srecord/record.h>


srecord::input_filter_unsplit::~input_filter_unsplit()
{
}


srecord::input_filter_unsplit::input_filter_unsplit(
        const srecord::input::pointer &a1, int a2, int a3, int a4) :
    srecord::input_filter(a1),
    modulus(a2),
    offset(a3),
    width(a4),
    buffer(),
    buffer_pos(0)
{
}


srecord::input::pointer
srecord::input_filter_unsplit::create(const input::pointer &a_deeper, int a2,
    int a3, int a4)
{
    return pointer(new srecord::input_filter_unsplit(a_deeper, a2, a3, a4));
}


bool
srecord::input_filter_unsplit::read(srecord::record &record)
{
    for (;;)
    {
        while
        (
            buffer.get_type() != srecord::record::type_data
        ||
            buffer_pos >= buffer.get_length()
        )
        {
            if (!srecord::input_filter::read(buffer))
                return false;
            if (buffer.get_type() != srecord::record::type_data)
            {
                record = buffer;
                return true;
            }
            buffer_pos = 0;
        }

        unsigned long addr = buffer.get_address() + buffer_pos;
        unsigned char c = buffer.get_data(buffer_pos++);
        int phase = addr % width;
        addr = (addr / width) * modulus + phase + offset;
        record = srecord::record(srecord::record::type_data, addr, &c, 1);
        return true;
    }
}
