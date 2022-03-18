//
// srecord - manipulate eprom load files
// Copyright (C) 2001-2003, 2006-2008, 2010-2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <cctype>

#include <srecord/arglex/tool.h>
#include <srecord/input/file/atmel_generic.h>
#include <srecord/record.h>


srecord::input_file_atmel_generic::~input_file_atmel_generic()
{
}


srecord::input_file_atmel_generic::input_file_atmel_generic(
        const std::string &a_file_name, endian_t a_end) :
    srecord::input_file(a_file_name),
    seen_some_input(false),
    end(a_end)
{
}


srecord::input_file::pointer
srecord::input_file_atmel_generic::create_be(const std::string &a_file_name)
{
    return create(a_file_name, endian_big);
}


srecord::input_file::pointer
srecord::input_file_atmel_generic::create(const std::string &a_file_name,
    endian_t a_end)
{
    return pointer(new srecord::input_file_atmel_generic(a_file_name, a_end));
}


bool
srecord::input_file_atmel_generic::read_inner(srecord::record &record)
{
    if (peek_char() < 0)
        return false;

    int address = get_3bytes_be();
    if (get_char() != ':')
        fatal_error("colon expected");
    unsigned char data[2];
    if (end == endian_big)
    {
        data[1] = get_byte();
        data[0] = get_byte();
    }
    else
    {
        data[0] = get_byte();
        data[1] = get_byte();
    }
    if (get_char() != '\n')
        fatal_error("end of line expected");

    record = srecord::record(srecord::record::type_data, address * 2, data, 2);
    return true;
}


bool
srecord::input_file_atmel_generic::read(srecord::record &record)
{
    if (!read_inner(record))
    {
        if (!seen_some_input)
            fatal_error("file contains no data");
        return false;
    }
    seen_some_input = true;
    return true;
}


const char *
srecord::input_file_atmel_generic::get_file_format_name()
    const
{
    return
        (
            end == endian_big
        ?
            "Atmel Generic (big-endian)"
        :
            "Atmel Generic (little-endian)"
        );
}


int
srecord::input_file_atmel_generic::format_option_number(void)
    const
{
    return
        (
            end == endian_big
        ?
            arglex_tool::token_atmel_generic_be
        :
            arglex_tool::token_atmel_generic_le
        );
}


// vim: set ts=8 sw=4 et :
