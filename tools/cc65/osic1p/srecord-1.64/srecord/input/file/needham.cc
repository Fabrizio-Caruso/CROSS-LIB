//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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
#include <srecord/input/file/needham.h>
#include <srecord/record.h>


srecord::input_file_needham::~input_file_needham()
{
}


srecord::input_file_needham::input_file_needham(
    const std::string &a_file_name
) :
    srecord::input_file(a_file_name),
    seen_some_input(false),
    address(0)
{
}


srecord::input_file::pointer
srecord::input_file_needham::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_needham(a_file_name));
}


bool
srecord::input_file_needham::read(srecord::record &record)
{
    for (;;)
    {
        int c = peek_char();
        if (c < 0)
        {
            if (!seen_some_input)
                fatal_error("file contains no data");
            return false;
        }
        if (isxdigit(c))
        {
            unsigned char c = get_byte();
            record =
                srecord::record
                (
                    srecord::record::type_data,
                    address,
                    &c,
                    1
                );
            seen_some_input = true;
            ++address;
            return true;
        }
        c = get_char();
        switch (c)
        {
        case ' ':
        case '\t':
        case '\r':
        case '\n':
        case '\f':
            break;

        case '\32':
            // ignore ^Z characters
            break;

        default:
            fatal_error("illegal character");

        case '$':
            if (get_char() != 'A')
                fatal_error("unknown command");
            address = 0;
            int n = 0;
            for (n = 0; n < 4 && peek_char() != ','; ++n)
            {
                int b = get_byte();
                address = (address << 8) + b;
            }
            if (n < 2)
                fatal_error("short address");
            if (get_char() != ',')
                fatal_error("comma expected");
            break;
        }
    }
}


const char *
srecord::input_file_needham::get_file_format_name(void)
    const
{
    return "Needham";
}


int
srecord::input_file_needham::format_option_number(void)
    const
{
    return arglex_tool::token_needham_hex;
}


// vim: set ts=8 sw=4 et :
