//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2006-2008, 2010 Peter Miller
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


#include <srecord/input/filter/unfill.h>


srecord::input_filter_unfill::~input_filter_unfill()
{
}


srecord::input_filter_unfill::input_filter_unfill(
        const srecord::input::pointer &a1, int a2, int a3) :
    srecord::input_filter(a1),
    fill_value(a2),
    fill_minimum(a3),
    buffer(),
    buffer_pos(0)
{
}


srecord::input::pointer
srecord::input_filter_unfill::create(const input::pointer &a_deeper, int a2,
    int a3)
{
    return pointer(new srecord::input_filter_unfill(a_deeper, a2, a3));
}


bool
srecord::input_filter_unfill::read(srecord::record &record)
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

        int first_pos = buffer_pos;
        unsigned long addr = buffer.get_address() + buffer_pos;
        unsigned char c = buffer.get_data(buffer_pos++);
        if (c == fill_value)
        {
            //
            // Let runs of fill characters go through,
            // if the run is shorter than the minimum.
            // This is so that single bytes can be left in,
            // but long patches are thrown away.
            //
            while (buffer_pos < buffer.get_length())
            {
                c = buffer.get_data(buffer_pos);
                if (c != fill_value)
                    break;
                ++buffer_pos;
            }
            if (buffer_pos - first_pos < fill_minimum)
            {
                record =
                    srecord::record
                    (
                        srecord::record::type_data,
                        addr,
                        buffer.get_data() + first_pos,
                        buffer_pos - first_pos
                    );
                return true;
            }
        }
        else
        {
            //
            // Let runs of non-fill characters go straight through.
            //
            while (buffer_pos < buffer.get_length())
            {
                c = buffer.get_data(buffer_pos);
                if (c == fill_value)
                    break;
                ++buffer_pos;
            }
            record =
                srecord::record
                (
                    srecord::record::type_data,
                    addr,
                    buffer.get_data() + first_pos,
                    buffer_pos - first_pos
                );
            return true;
        }
    }
}
