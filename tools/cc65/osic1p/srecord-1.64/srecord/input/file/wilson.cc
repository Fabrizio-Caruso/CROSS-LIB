//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2005-2008, 2010, 2011, 2013 Peter Miller
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
#include <srecord/input/file/wilson.h>
#include <srecord/record.h>


srecord::input_file_wilson::~input_file_wilson()
{
}


srecord::input_file_wilson::input_file_wilson(const std::string &a_file_name) :
    input_file(a_file_name),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false)
{
}


srecord::input_file::pointer
srecord::input_file_wilson::create(const std::string &a_file_name)
{
    return pointer(new input_file_wilson(a_file_name));
}


int
srecord::input_file_wilson::get_byte(void)
{
    int n = -1;
    int c = get_char();
    if (c >= 0x40 && c < 0xE0)
        n = (c - 0x40);
    else if (c >= 0xE0)
        n = c;
    else if (c >= 0x3A && c < 0x3E)
    {
        int c2 = get_char();
        if (c2 >= 0x30 && c2 < 0x40)
            n = ((c - 0x30) << 4) + (c2 - 0x30);
        else
            goto bomb;
    }
    else
    {
        bomb:
        fatal_error("illegal character");
        // NOTREACHED
    }
    checksum_add(n);
    return n;
}


bool
srecord::input_file_wilson::read_inner(record &result)
{
    int c;
    for (;;)
    {
        c = get_char();
        if (c < 0)
            return false;
        if (c == '#' || c == '\'')
            break;
        if (c == '\n')
            continue;
        if (!garbage_warning)
        {
            warning("ignoring garbage lines");
            garbage_warning = true;
        }
        for (;;)
        {
            c = get_char();
            if (c < 0)
                return false;
            if (c == '\n')
                break;
        }
    }
    int tag = c;
    checksum_reset();
    int line_length = get_byte();
    if (line_length < 1)
        fatal_error("line length invalid");
    unsigned char buffer[256];
    for (int j = 0; j < line_length; ++j)
        buffer[j] = get_byte();
    if (use_checksums())
    {
        int n = checksum_get();
        if (n != 0xFF)
            fatal_error("checksum mismatch (%02X != FF)", n);
    }
    if (get_char() != '\n')
        fatal_error("end-of-line expected");
    --line_length;

    int naddr = 4;
    record::type_t type = record::type_unknown;
    switch (tag)
    {
    case '#':
        // data
        type = record::type_data;
        break;

    case '\'':
        // termination
        type = record::type_execution_start_address;
        break;
    }
    if (line_length < naddr)
    {
        fatal_error
        (
            "data length too short (%d < %d) for data type (%x)",
            line_length,
            naddr,
            tag
        );
    }
    result =
        record
        (
            type,
            record::decode_big_endian(buffer, naddr),
            buffer + naddr,
            line_length - naddr
        );
    return true;
}


bool
srecord::input_file_wilson::read(record &record)
{
    for (;;)
    {
        if (!read_inner(record))
        {
                if (!seen_some_input)
                        fatal_error("file contains no data");
                if (!termination_seen)
                {
                        warning("no execution start address record");
                        termination_seen = true;
                }
                return false;
        }
        seen_some_input = true;
        if
        (
                record.get_type() != record::type_execution_start_address
        &&
                termination_seen
        )
        {
                warning("termination record should be last");
                termination_seen = false;
        }
        switch (record.get_type())
        {
        case record::type_unknown:
        case record::type_header:
        case record::type_data_count:
            fatal_error("record type not recognised");
            break;

        case record::type_data:
            if (record.get_length() == 0)
            {
                    warning("empty data record ignored");
                    continue;
            }
            break;

        case record::type_execution_start_address:
            if (record.get_length() > 0)
            {
                    warning("data in execution start address record ignored");
                    record.set_length(0);
            }
            if (termination_seen)
                    warning("redundant execution start address record");
            termination_seen = true;
            break;
        }
        break;
    }
    return true;
}


bool
srecord::input_file_wilson::is_binary(void)
    const
{
    return true;
}


const char *
srecord::input_file_wilson::get_file_format_name(void)
    const
{
    return "Wilson (anyone know this format's real name?)";
}


int
srecord::input_file_wilson::format_option_number(void)
    const
{
    return arglex_tool::token_wilson;
}


// vim: set ts=8 sw=4 et :
