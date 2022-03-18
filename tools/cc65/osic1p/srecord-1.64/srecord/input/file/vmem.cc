//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2005-2008, 2010, 2011, 2013 Peter Miller
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
#include <srecord/input/file/vmem.h>
#include <srecord/record.h>


srecord::input_file_vmem::~input_file_vmem()
{
}


srecord::input_file_vmem::input_file_vmem(const std::string &a_file_name) :
    srecord::input_file(a_file_name),
    seen_some_input(false),
    address(0)
{
}


srecord::input_file::pointer
srecord::input_file_vmem::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_vmem(a_file_name));
}


bool
srecord::input_file_vmem::read(srecord::record &record)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
        {
            if (!seen_some_input)
                fatal_error("file contains no data");
            return false;
        }
        if (c == '@')
        {
            // collect address
            address = 0;
            for (;;)
            {
                address = (address << 4) + get_nibble();
                c = peek_char();
                if (c < 0 || !isxdigit((unsigned char)c))
                    break;
            }
            continue;
        }
        if (isspace((unsigned char)c))
            continue;

        if (c == '/')
        {
            c = get_char();
            if (c == '/')
            {
                for (;;)
                {
                    c = get_char();
                    if (c == '\n' || c < 0)
                        break;
                }
                continue;
            }
            if (c == '*')
            {
                for (;;)
                {
                    for (;;)
                    {
                        c = get_char();
                        if (c < 0)
                        {
                            eof_within_comment:
                            fatal_error("end-of-file within comment");
                        }
                        if (c == '*')
                            break;
                    }
                    for (;;)
                    {
                        c = get_char();
                        if (c < 0)
                            goto eof_within_comment;
                        if (c != '*')
                            break;
                    }
                    if (c =='/')
                        break;
                }
                continue;
            }
            fatal_error("malformed comment");
        }

        // collect value
        get_char_undo(c);
        unsigned char value[5];
        size_t nbytes = 0;
        while (nbytes < sizeof(value))
        {
            value[nbytes++] = get_byte();
            c = peek_char();
            if (c < 0 || !isxdigit((unsigned char)c))
                break;
        }
        switch (nbytes)
        {
        default:
            fatal_error("value has too many bytes (%d)", (int)nbytes);

        case 1:
        case 2:
        case 4:
            record =
                srecord::record
                (
                    srecord::record::type_data,
                    address * nbytes,
                    value,
                    nbytes
                );
            break;
        }

        // This is not a byte address, it's a chunk address.
        ++address;
        seen_some_input = true;
        return true;
    }
}


const char *
srecord::input_file_vmem::get_file_format_name(void)
    const
{
    return "Verilog VMEM";
}


int
srecord::input_file_vmem::format_option_number(void)
    const
{
    return arglex_tool::token_vmem;
}


// vim: set ts=8 sw=4 et :
