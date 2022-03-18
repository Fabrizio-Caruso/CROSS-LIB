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

#include <cassert>

#include <srecord/arglex/tool.h>
#include <srecord/input/file/ppx.h>
#include <srecord/record.h>


srecord::input_file_ppx::~input_file_ppx()
{
}


srecord::input_file_ppx::input_file_ppx(const std::string &filename) :
    input_file(filename),
    state(0),
    token(token_eof),
    token_value(0),
    address(0),
    data_seen(false),
    dsum(0),
    buffer_length(0)
{
}


srecord::input_file_ppx::pointer
srecord::input_file_ppx::create(const std::string &filename)
{
    return pointer(new input_file_ppx(filename));
}


void
srecord::input_file_ppx::get_next_token(void)
{
    for (;;)
    {
        int sc = get_char();
        if (sc < 0)
        {
            token = token_eof;
            return;
        }
        unsigned char c = sc;
        switch (c)
        {
        case '*':
            token = token_star;
            return;

        case '$':
            token = token_end;
            return;

        case 'S':
            token = token_sum;
            return;

        case ' ':
        case '\t':
        case '\f':
        case '\r':
        case '\v':
        case '\n':
            break;

        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            {
                token_value = get_nibble_value(c);
                int ndigits = 1;
                for (;;)
                {
                    int sc = get_char();
                    if (sc < 0)
                        break;
                    c = sc;
                    switch (c)
                    {
                    case '0': case '1': case '2': case '3': case '4':
                    case '5': case '6': case '7': case '8': case '9':
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                        token_value = (token_value << 4) | get_nibble_value(c);
                        ++ndigits;
                        continue;

                    default:
                        get_char_undo(c);
                        break;
                    }
                    break;
                }
                if (ndigits > 2)
                {
                    token = token_address;
                    return;
                }
                token = token_byte;
                return;
            }

        default:
            syntax_error();
            break;
        }
    }
}


void
srecord::input_file_ppx::syntax_error(void)
{
    fatal_error("syntax error");
}


bool
srecord::input_file_ppx::read(record &result)
{
    //
    // file
    //     : STAR lines end
    //     ;
    // lines
    //     : line
    //     | lines line
    //     ;
    // line
    //     : ADDRESS bytes
    //     ;
    // bytes
    //     : BYTE
    //     | bytes BYTE
    //     ;
    // end
    //     : END
    //     | END SUM ADDRESS
    //     ;
    //
    for (;;)
    {
        switch (state)
        {
        case 0:
            get_next_token();
            // file: . ASTERISK lines end;
            switch (token)
            {
            case token_star:
                get_next_token();
                state = 1;
                break;

            default:
                syntax_error();
            }
            break;

        case 1:
            // file: ASTERISK . lines end;
            // file: ASTERISK lines . end;
            // lines: . line
            // lines: lines . line
            // lines: lines line .
            // line: . ADDRESS bytes
            // end: . END
            // end: . END SUM ADDRESS
            switch (token)
            {
            case token_address:
                if (address != token_value)
                    assert(buffer_length == 0);
                address = token_value;
                get_next_token();
                state = 3;
                break;

            case token_end:
                get_next_token();
                state = 2;
                break;

            default:
                syntax_error();
            }
            break;

        case 2:
            // end: END .
            // end: END . SUM ADDRESS
            switch (token)
            {
            case token_eof:
                state = 5;
                break;

            case token_sum:
                get_next_token();
                state = 4;
                break;

            default:
                syntax_error();
            }
            break;

        case 3:
            // line: ADDRESS . bytes
            // line: ADDRESS bytes .
            // bytes: . BYTE
            // bytes: bytes . BYTE
            // bytes: bytes BYTE .
            switch (token)
            {
            case token_address:
            case token_end:
                state = 1;

                if (buffer_length)
                {
                    return_data_record:
                    result =
                        record
                        (
                            record::type_data,
                            address - buffer_length,
                            buffer,
                            buffer_length
                        );
                    buffer_length = 0;
                    return true;
                }
                break;

            case token_byte:
                dsum += token_value;
                buffer[buffer_length++] = token_value;
                ++address;
                get_next_token();
                data_seen = true;

                if (buffer_length >= sizeof(buffer))
                    goto return_data_record;
                break;

            default:
                syntax_error();
            }
            break;

        case 4:
            // end: END SUM . ADDRESS
            switch (token)
            {
            case token_address:
                if (use_checksums() && dsum != token_value)
                {
                    fatal_error
                    (
                        "checksum mismatch (calculated 0x%04X, given 0x%04X)",
                        dsum,
                        token_value
                    );
                }
                get_next_token();
                state = 5;
                break;

            default:
                syntax_error();
            }
            break;

        case 5:
            // file: ASTERISK lines end .
            switch (token)
            {
            case token_eof:
                if (!data_seen)
                    fatal_error("no data seen");
                return false;

            default:
                syntax_error();
            }
            break;
        }
    }
}


const char *
srecord::input_file_ppx::get_file_format_name(void)
    const
{
    return "Stag Prom Programmer hexadecimal (PPX)";
}


int
srecord::input_file_ppx::format_option_number(void)
    const
{
    return arglex_tool::token_ppx;
}


// vim: set ts=8 sw=4 et :
