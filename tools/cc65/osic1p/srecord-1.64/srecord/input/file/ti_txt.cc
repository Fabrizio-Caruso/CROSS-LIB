//
// srecord - manipulate eprom load files
// Copyright (C) 2006-2008, 2010, 2011, 2013 Peter Miller
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
#include <srecord/input/file/ti_txt.h>
#include <srecord/record.h>


srecord::input_file_ti_txt::~input_file_ti_txt()
{
}


srecord::input_file_ti_txt::input_file_ti_txt(const std::string &a_file_name) :
    input_file(a_file_name),
    seen_some_input(false),
    address(0),
    token(token_start_up),
    token_value(0),
    address_warning(false)
{
}


srecord::input_file::pointer
srecord::input_file_ti_txt::create(const std::string &a_file_name)
{
    return pointer(new input_file_ti_txt(a_file_name));
}


void
srecord::input_file_ti_txt::get_next_token(void)
{
    token_value = 0;
    for (;;)
    {
        int c = get_char();
        if (c < 0)
        {
            token = token_end_of_file;
            return;
        }
        switch (c)
        {
        case ' ':
        case '\t':
        case '\f':
            // ignore all white space
            break;

        case '\r':
        case '\n':
            // ignore end of line
            break;

        case '\32':
            // ignore ^Z characters
            break;

        case '@':
            token = token_at;
            return;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            for (;;)
            {
                int n = get_nibble_value(c);
                // assert(n >= 0);
                token_value = (token_value << 4) | n;
                c = get_char();
                switch (c)
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                    break;

                default:
                    get_char_undo(c);
                    token = token_number;
                    return;
                }
            }

        case 'Q':
        case 'q':
            token = token_q;
            return;

        default:
            token = token_junk;
            return;
        }

    }
}


bool
srecord::input_file_ti_txt::read(record &result)
{
    for (;;)
    {
        switch (token)
        {
        case token_start_up:
            get_next_token();
            if (token != token_at)
                fatal_error("data must start with an address record");
            break;

        case token_at:
            get_next_token();
            if (token != token_number)
                fatal_error("@ must be followed by an address");
            address = token_value;
#if 0
            //
            // Despite // http://www.ti.com/lit/pdf/slau101 section A.2
            // stating that "the execution start address must be even",
            // it doesn't have to be.
            //
            if (address & 1)
                warning("addresses should be even");
#endif
            if (address >= (1 << 20) && !address_warning)
            {
                warning("addresses (0x%08lX) too large", address);
                address_warning = true;
            }
            get_next_token();
            break;

        case token_number:
            {
                seen_some_input = true;
                unsigned char buffer[record::max_data_length];
                size_t n = 0;
                for (;;)
                {
                    if (token_value >= 256)
                        fatal_error("byte value (%ld) too large", token_value);
                    buffer[n++] = token_value;
                    get_next_token();
                    if (n >= record::max_data_length)
                        break;
                    if (token != token_number)
                        break;
                }
                if (address >= (1 << 20) && !address_warning)
                {
                    warning("addresses (0x%08lX) too large", address);
                    address_warning = true;
                }
                result = record(record::type_data, address, buffer, n);
                address += n;
                return true;
            }

        case token_q:
        case token_end_of_file:
            if (!seen_some_input)
                fatal_error("file contains no data");
            return false;

        case token_junk:
            fatal_error("illegal character");
        }
    }
}


const char *
srecord::input_file_ti_txt::get_file_format_name(void)
    const
{
    return "ti-txt (MSP430)";
}


int
srecord::input_file_ti_txt::format_option_number(void)
    const
{
    return arglex_tool::token_ti_txt;
}


// vim: set ts=8 sw=4 et :
