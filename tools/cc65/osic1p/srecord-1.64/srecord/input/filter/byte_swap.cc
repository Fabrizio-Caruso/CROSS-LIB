//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2005-2008, 2010 Peter Miller
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

#include <srecord/arglex/tool.h>
#include <srecord/input/filter/byte_swap.h>
#include <srecord/record.h>


srecord::input_filter_byte_swap::~input_filter_byte_swap()
{
}


srecord::input_filter_byte_swap::input_filter_byte_swap(
        const srecord::input::pointer &arg) :
    srecord::input_filter(arg),
    buffer(),
    buffer_pos(0),
    mask(1)
{
}


srecord::input::pointer
srecord::input_filter_byte_swap::create(const srecord::input::pointer &a_deeper)
{
    return pointer(new srecord::input_filter_byte_swap(a_deeper));
}


bool
srecord::input_filter_byte_swap::read(srecord::record &record)
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
                record.set_address(record.get_address() ^ mask);
                return true;
            }
            buffer_pos = 0;
        }

        unsigned long addr = (buffer.get_address() + buffer_pos) ^ mask;
        unsigned char c = buffer.get_data(buffer_pos++);
        record = srecord::record(srecord::record::type_data, addr, &c, 1);
        return true;
    }
}


void
srecord::input_filter_byte_swap::command_line(srecord::arglex_tool *cmdln)
{
    if (cmdln->can_get_number())
    {
        int width = cmdln->get_number("width");
        switch (width)
        {
        case 2:
        case 4:
        case 8:
            break;

        case 16:
            width = 2;
            break;

        case 32:
            width = 4;
            break;

        case 64:
            width = 8;
            break;

        case 128:
            width = 16;
            break;

        case 256:
            width = 32;
            break;

        default:
            fatal_error("width %d not understood", width);
            // NOTREACHED
        }
        mask = width - 1;
    }
}
