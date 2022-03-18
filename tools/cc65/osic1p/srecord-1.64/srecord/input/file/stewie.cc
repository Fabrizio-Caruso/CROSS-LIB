//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2011, 2013 Peter Miller
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
#include <srecord/input/file/stewie.h>
#include <srecord/record.h>


srecord::input_file_stewie::~input_file_stewie()
{
}


srecord::input_file_stewie::input_file_stewie(const std::string &a_file_name) :
    input_file(a_file_name),
    data_count(0),
    garbage_warning(false),
    seen_some_input(false),
    header_seen(false),
    termination_seen(false)
{
}


srecord::input_file::pointer
srecord::input_file_stewie::create(const std::string &a_file_name)
{
    return pointer(new input_file_stewie(a_file_name));
}


int
srecord::input_file_stewie::get_byte()
{
    int n = get_char();
    if (n < 0)
        fatal_error("premature end-of-file");
    checksum_add(n);
    return n;
}


bool
srecord::input_file_stewie::read_inner(record &result)
{
    if (termination_seen)
        return false;
    int c = get_char();
    if (c < 0)
        return false;
    if (c != 'S')
    {
        fatal_error("'S' expected");
        return false;
    }
    int tag = get_nibble();
    switch (tag)
    {
    case 0:
        // Header records are not like, Motorola hex.
        // The header record is literally "S003"
        if (get_char() != '0' || get_char() != '3')
            fatal_error("format error");
        result = record(record::type_header, 0, 0, 0);
        return true;

    case 7:
    case 8:
    case 9:
        result = record(record::type_execution_start_address, 0, 0, 0);
        return true;
    }
    checksum_reset();
    int line_length = get_byte();
    if (line_length < 1)
        fatal_error("record length invalid");
    unsigned char buffer[256];
    for (int j = 0; j < line_length; ++j)
        buffer[j] = get_byte();
    if (use_checksums())
    {
        int n = checksum_get();
        if (n != 0xFF)
            fatal_error("checksum mismatch (%02X != FF)", n);
    }
    --line_length;

    int naddr = 2;
    record::type_t type = record::type_unknown;
    switch (tag)
    {
    case 1:
        // data
        type = record::type_data;
        break;

    case 2:
        // data
        type = record::type_data;
        naddr = 3;
        break;

    case 3:
        // data
        type = record::type_data;
        naddr = 4;
        break;

    case 5:
        // data count
        type = record::type_data_count;
        //
        // Just in case some smarty-pants uses the Green Hills trick, we
        // cope with address size crap the same as Motorola S-Record.
        //
        if (line_length >= 2 && line_length <= 4)
            naddr = line_length;
        break;

    case 6:
        // data count
        type = record::type_data_count;
        //
        // Just in case some smarty-pants uses the Green Hills trick, we
        // cope with address size crap the same as Motorola S-Record.
        //
        naddr = 3;
        if (line_length == 4)
            naddr = line_length;
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
srecord::input_file_stewie::read(record &result)
{
    for (;;)
    {
        if (!read_inner(result))
        {
            if (!seen_some_input && garbage_warning)
                fatal_error("file contains no data");
            if (!header_seen)
            {
                warning("no header record");
                header_seen = true;
            }
            if (data_count <= 0)
                warning("file contains no data");
            if (!termination_seen)
            {
                warning("no execution start address record");
                termination_seen = true;
            }
            return false;
        }
        seen_some_input = true;
        if (result.get_type() != record::type_header && !header_seen)
        {
            warning("no header record");
            header_seen = true;
        }
        switch (result.get_type())
        {
        case record::type_unknown:
            fatal_error("record type not recognised");
            break;

        case record::type_header:
            if (header_seen)
                warning("redundant header record");
            if (result.get_address())
            {
                warning("address in header record ignored");
                result.set_address(0);
            }
            header_seen = true;
            break;

        case record::type_data:
            ++data_count;
            if (result.get_length() == 0)
            {
                warning("empty data record ignored");
                continue;
            }
            break;

        case record::type_data_count:
            {
                record::address_t addr = result.get_address();
                record::address_t mask = 0xFFFF;
                while (addr > mask)
                    mask = ~(~mask << 8);
                mask &= data_count;
                if (addr != mask)
                {
                    fatal_error
                    (
                        "data record count mismatch (file %ld, read %ld)",
                        (long)addr,
                        (long)mask
                    );
                }
            }
            continue;

        case record::type_execution_start_address:
            if (result.get_length() > 0)
            {
                warning("data in termination record ignored");
                result.set_length(0);
            }
            if (termination_seen)
                warning("redundant termination record");
            termination_seen = true;
            break;
        }
        break;
    }
    return true;
}


bool
srecord::input_file_stewie::is_binary(void)
    const
{
    return true;
}


const char *
srecord::input_file_stewie::get_file_format_name()
    const
{
    return "mobile phone signed binary (SBN)";
}


int
srecord::input_file_stewie::format_option_number(void)
    const
{
    return arglex_tool::token_stewie;
}


// vim: set ts=8 sw=4 et :
