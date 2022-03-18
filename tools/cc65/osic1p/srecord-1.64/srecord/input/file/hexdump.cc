//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/arglex/tool.h>
#include <srecord/input/file/hexdump.h>
#include <srecord/record.h>


srecord::input_file_hexdump::~input_file_hexdump()
{
}


srecord::input_file_hexdump::input_file_hexdump(const std::string &filename) :
    input_file(filename),
    address(0),
    data_seen(false)
{
}


srecord::input_file_hexdump::pointer
srecord::input_file_hexdump::create(const std::string &filename)
{
    return pointer(new input_file_hexdump(filename));
}


srecord::input_file_hexdump::token_t
srecord::input_file_hexdump::get_next_token(void)
{
    for (;;)
    {
        int sc = get_char();
        if (sc < 0)
            return token_eof;
        unsigned char c = sc;
        switch (c)
        {
        case '\n':
            return token_eoln;

        case '#':
        case ';':
            // comments
            return (discard_rest_of_line() ? token_eoln : token_eof);

        case ' ':
        case '\t':
        case '\f':
        case '\r':
        case '\v':
            break;

        case ':':
            return token_colon;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            {
                int c1 = get_nibble_value(c);
                int c2 = get_nibble();
                current_token_value = (c1 << 4) | c2;
                return token_byte;
            }

        default:
            return token_junk;
        }
    }
}


bool
srecord::input_file_hexdump::discard_rest_of_line(void)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return false;
        if (c == '\n')
            return true;
    }
}


bool
srecord::input_file_hexdump::read(record &result)
{
    // file
    //     : line
    //     | file line
    //     ;
    // line
    //     : address EOLN
    //     : address data EOLN
    //     | data EOLN
    //     ;
    // address
    //     : data COLON
    //     ;
    // data
    //     : BYTE
    //     | data BYTE
    //     ;
    for (;;)
    {
        token_t tok = get_next_token();
        switch (tok)
        {
        case token_eof:
            return false;

        default:
            fatal_error("expected address");

        case token_eoln:
            // Blank lines are OK.
            // Comments arrive here, too.
            continue;

        case token_byte:
            break;
        }

        unsigned char data[16];
        unsigned nbytes = 0;
        bool could_be_an_address = true;
        for (;;)
        {
            data[nbytes++] = current_token_value;
            if (nbytes >= 16)
            {
                tok = discard_rest_of_line() ? token_eoln : token_eof;
                break;
            }
            if (nbytes > sizeof(address))
                could_be_an_address = false;

            tok = get_next_token();
            if (tok == token_colon && could_be_an_address)
            {
                // it's actually an address
                address = 0;
                for (unsigned d = 0; d < nbytes; ++d)
                    address = (address << 8) | data[d];
                nbytes = 0;
                could_be_an_address = false;
                tok = get_next_token();
                if (tok == token_byte)
                    continue;
            }
            if (tok == token_eoln || tok == token_eof)
                break;
            if (tok != token_byte)
            {
                tok = discard_rest_of_line() ? token_eoln : token_eof;
                break;
            }
        }

        if (tok == token_eof)
        {
            if (!data_seen)
                fatal_error("file contains no data");
            if (nbytes == 0)
                return false;
            goto yes;
        }

        if (nbytes > 0)
        {
            yes:
            result = record(record::type_data, address, data, nbytes);
            address += nbytes;
            data_seen = true;
            return true;
        }
    }
}


const char *
srecord::input_file_hexdump::get_file_format_name(void)
    const
{
    return "hexdump";
}


int
srecord::input_file_hexdump::format_option_number(void)
    const
{
    return arglex_tool::token_hexdump;
}


// vim: set ts=8 sw=4 et :
