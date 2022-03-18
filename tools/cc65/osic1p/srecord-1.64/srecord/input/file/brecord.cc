//
// srecord - The "srecord" program.
// Copyright (C) 2007, 2008, 2010-2013 Peter Miller
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

#include <srecord/arglex/tool.h>
#include <srecord/input/file/brecord.h>
#include <srecord/record.h>


srecord::input_file_brecord::~input_file_brecord()
{
}


srecord::input_file_brecord::input_file_brecord(
    const std::string &a_file_name
) :
    input_file(a_file_name),
    seen_some_input(false)
{
}


srecord::input_file::pointer
srecord::input_file_brecord::create(const std::string &a_file_name)
{
    return pointer(new input_file_brecord(a_file_name));
}


bool
srecord::input_file_brecord::read_inner(record &result)
{
    if (peek_char() < 0)
        return false;

    unsigned long address = get_4bytes_be();
    unsigned char length = get_byte();
    if (length & 0x20)
        fatal_error("read mode not supported");
    length &= 0x1F;
    unsigned char data[32];
    for (unsigned j = 0; j < length; ++j)
        data[j] = get_byte();
    if (get_char() != '\n')
        fatal_error("end of line expected");

    if (length == 0)
    {
        result = record(record::type_execution_start_address, address, 0, 0);
    }
    else
    {
        result = record(record::type_data, address, data, length);
    }
    return true;
}


bool
srecord::input_file_brecord::read(record &result)
{
    if (!read_inner(result))
    {
        if (!seen_some_input)
            fatal_error("file contains no data");
        return false;
    }
    seen_some_input = true;
    return true;
}


const char *
srecord::input_file_brecord::get_file_format_name(void)
    const
{
    return "Motorola MC68EZ328 bootstrap b-record";
}


int
srecord::input_file_brecord::format_option_number(void)
    const
{
    return arglex_tool::token_brecord;
}


// vim: set ts=8 sw=4 et :
