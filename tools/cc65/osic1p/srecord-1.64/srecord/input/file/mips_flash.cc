//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011, 2013 Peter Miller
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

#include <srecord/arglex/tool.h>
#include <srecord/input/file/mips_flash.h>
#include <srecord/record.h>


srecord::input_file_mips_flash::~input_file_mips_flash()
{
}


srecord::input_file_mips_flash::input_file_mips_flash(
    const std::string &a_file_name,
    endian_t a_endian
) :
    input_file(a_file_name),
    seen_some_input(false),
    endian(a_endian),
    address(0),
    token(token_eof),
    token_value(0),
    seen_reset(false)
{
}


srecord::input_file::pointer
srecord::input_file_mips_flash::create_be(const std::string &a_file_name)
{
    return pointer(new input_file_mips_flash(a_file_name, endian_big));
}


srecord::input_file::pointer
srecord::input_file_mips_flash::create_le(const std::string &a_file_name)
{
    return pointer(new input_file_mips_flash(a_file_name, endian_little));
}


void
srecord::input_file_mips_flash::tokenizer(void)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
        {
            token = token_eof;
            return;
        }
        switch (c)
        {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            break;

        case '!':
            switch (get_char())
            {
            case 'C':
                token = token_unlock;
                return;

            case 'E':
                token = token_erase;
                return;

            case 'R':
                token = token_reset;
                return;

            case 'S':
                token = token_lock;
                return;

            default:
                goto junk;
            }

        case '@':
            token = token_at;
            return;

        case '>':
            // throw away 8 characters
            for (int j = 0; j < 8; ++j)
                get_char();
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            // get 8 digit hex number
            get_char_undo(c);
            token_value = 0;
            for (int j = 0; j < 8; ++j)
                token_value = (token_value << 4) | get_nibble();
            token = token_number;
            return;

        default:
            junk:
            fatal_error("hexadecimal number expected");
        }
    }
}


bool
srecord::input_file_mips_flash::read_inner(record &result)
{
    if (!seen_reset)
    {
        tokenizer();
        if (token != token_reset)
            fatal_error("not a MIPS-Flash format file");
        seen_reset = true;
        tokenizer();
    }
    for (;;)
    {
        switch (token)
        {
        case token_eof:
            return false;

        case token_at:
            tokenizer();
            if (token != token_number)
                fatal_error("@ must be followed by a number");
            address = token_value;
            tokenizer();
            break;

        case token_number:
            if (endian == endian_big)
            {
                unsigned char data[4];
                data[0] = token_value >> 24;
                data[1] = token_value >> 16;
                data[2] = token_value >> 8;
                data[3] = token_value;
                result = record(record::type_data, address, data, 4);
            }
            else
            {
                unsigned char data[4];
                data[0] = token_value;
                data[1] = token_value >> 8;
                data[2] = token_value >> 16;
                data[3] = token_value >> 24;
                result = record(record::type_data, address, data, 4);
            }
            address += 4;
            tokenizer();
            return true;

        case token_erase:
        case token_lock:
        case token_unlock:
            // we ignore this
            tokenizer();
            break;

        default:
            fatal_error("syntax error");
        }
    }
}


bool
srecord::input_file_mips_flash::read(record &result)
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
srecord::input_file_mips_flash::get_file_format_name(void)
    const
{
    return
        (
            endian == endian_big
        ?
            "MIPS Flash (big-endian)"
        :
            "MIPS Flash (little-endian)"
        );
}


int
srecord::input_file_mips_flash::format_option_number(void)
    const
{
    return
        (
            endian == endian_big
        ?
            arglex_tool::token_mips_flash_be
        :
            arglex_tool::token_mips_flash_le
        );
}


// vim: set ts=8 sw=4 et :
