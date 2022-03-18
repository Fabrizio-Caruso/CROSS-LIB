//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2006-2013 Peter Miller
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
#include <srecord/input/file/ti_tagged.h>
#include <srecord/record.h>


srecord::input_file_ti_tagged::~input_file_ti_tagged()
{
}


srecord::input_file_ti_tagged::input_file_ti_tagged(
    const std::string &a_filename
) :
    input_file(a_filename),
    address(0),
    csum(0)
{
}


srecord::input_file::pointer
srecord::input_file_ti_tagged::create(const std::string &a_file_name)
{
    return pointer(new input_file_ti_tagged(a_file_name));
}


int
srecord::input_file_ti_tagged::get_char(void)
{
    int c = inherited::get_char();
    if (c < 0 || c == '\n')
        csum = 0;
    else
        csum += c;
    return c;
}


bool
srecord::input_file_ti_tagged::read(record &result)
{
    for (;;)
    {
        int c = get_char();
        switch (c)
        {
        default:
            fatal_error
            (
                (isprint(c) ? "unknown tag '%c'" : "unknown tag (%02X)"),
                c
            );

        case -1:
            return false;

        case '*':
            {
                // followed by 2 data characters (1 data byte)
                unsigned char data[1];
                data[0] = get_byte();
                result = record(record::type_data, address, data, 1);
                ++address;
                return true;
            }

        case ':':
            // end of file
            while (get_char() >= 0)
                ;
            return false;

        case '0':
            {
                // file header
                // 4 char: word count of the rest of the file (hex)
                // 8 char: file name (ascii)
                // we will ignore
                get_word_be();
                for (int n = 0; n < 8; ++n)
                    get_char();
            }
            break;

        case '7':
            {
                // checksum which the programmer verifies
                int n = (-csum) & 0xFFFF;
                int n2 = get_word_be();
                if (n != n2)
                {
                    fatal_error
                    (
                        "checksum mismatch (calculated %04X, file has %04X)",
                        n,
                        n2
                    );
                }
            }
            break;

        case '8':
            // a checksum which the programmer ignores.
            get_word_be();
            break;

        case '9':
            // load address which represents a word location.
            address = get_word_be();
            break;

        case 'B':
            {
                unsigned char data[2];
                data[0] = get_byte();
                data[1] = get_byte();
                result = record(record::type_data, address, data, 2);
                address += 2;
                return true;
            }

        case 'F':
            // denotes the end of a data record.
            if (get_char() != '\n')
                fatal_error("end of line expected");
            break;

        case 'K':
            {
                // file description
                int n = get_word_be();
                if (n < 5)
                {
                    bad_desc:
                    fatal_error("broken description");
                }
                n -= 5;
                int max = 250;
                unsigned char *buffer = new unsigned char [max];
                for (int j = 0; j < n; ++j)
                {
                    c = get_char();
                    if (c < 0 || c == '\n')
                            goto bad_desc;
                    if (j < max)
                            buffer[j] = c;
                }
                if (n > max)
                    n = max;
                result = record(record::type_header, 0, buffer, n);
                delete [] buffer;
            }
            return true;
        }
    }
}


const char *
srecord::input_file_ti_tagged::get_file_format_name(void)
    const
{
    return "Texas Instruments Tagged (SDSMAC)";
}


int
srecord::input_file_ti_tagged::format_option_number(void)
    const
{
    return arglex_tool::token_ti_tagged;
}


// vim: set ts=8 sw=4 et :
