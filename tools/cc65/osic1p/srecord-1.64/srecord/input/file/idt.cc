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
#include <srecord/input/file/idt.h>
#include <srecord/record.h>


srecord::input_file_idt::~input_file_idt()
{
}


srecord::input_file_idt::input_file_idt(
    const std::string &a_file_name
) :
    input_file(a_file_name),
    data_count(0),
    seen_some_input(false)
{
}


srecord::input_file::pointer
srecord::input_file_idt::create(const std::string &a_file_name)
{
    return pointer(new input_file_idt(a_file_name));
}


void
srecord::input_file_idt::record_format_error(void)
{
    fatal_error("record format error");
}


bool
srecord::input_file_idt::read_inner(record &result)
{
    int c = get_char();
    if (c < 0)
        return false;
    if (c != 'S')
        record_format_error();
    int tag = get_nibble();
    if (tag < 0)
        record_format_error();
    unsigned char csum = 0;
    int line_length = get_char();
    if (line_length < 0)
        record_format_error();
    csum += line_length;
    if (line_length < 1)
        fatal_error("line length invalid");
    record::data_t buffer[256];
    for (int j = 0; j < line_length; ++j)
    {
        int c = get_char();
        if (c < 0)
            record_format_error();
        buffer[j] = c;
        csum += c;
    }
    if (use_checksums() && csum != 0xFF)
        fatal_error("checksum mismatch (%02X != FF)", csum);
    --line_length;

    int naddr = 2;
    record::type_t type = record::type_unknown;
    switch (tag)
    {
    default:
        record_format_error();

    case 0:
        // header
        type = record::type_header;
        if (line_length < naddr)
        {
            // Some programs write Very short headers.
            naddr = line_length;
        }
        break;

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
        // This is documented as having 2 address bytes and
        // no data bytes.  The Green Hills toolchain happily
        // generates records with 4 address bytes.  We cope
        // with this silently.
        //
        if (line_length >= 2 && line_length <= 4)
            naddr = line_length;
        break;

    case 6:
        // data count
        type = record::type_data_count;
        //
        // Just in case some smarty-pants uses the Green Hills
        // trick, we cope with address size crap the same as S5.
        //
        naddr = 3;
        if (line_length == 4)
            naddr = line_length;
        break;

    case 7:
        // termination
        type = record::type_execution_start_address;
        naddr = 4;
        break;

    case 8:
        // termination
        type = record::type_execution_start_address;
        naddr = 3;
        break;

    case 9:
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
    record::address_t addr = record::decode_big_endian(buffer, naddr);
    result = record(type, addr, buffer + naddr, line_length - naddr);
    return true;
}


bool
srecord::input_file_idt::read(record &record)
{
    for (;;)
    {
        if (!read_inner(record))
        {
            if (!seen_some_input)
                fatal_error("file contains no data");
            return false;
        }
        seen_some_input = true;
        switch (record.get_type())
        {
        case record::type_unknown:
            fatal_error("record type not recognised");
            break;

        case record::type_header:
            if (record.get_address())
            {
                warning("address in header record ignored");
                record.set_address(0);
            }
            break;

        case record::type_data:
            ++data_count;
            if (record.get_length() == 0)
            {
                warning("empty data record ignored");
                continue;
            }
            break;

        case record::type_data_count:
            {
                record::address_t addr = record.get_address();
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
            if (record.get_length() > 0)
            {
                warning("data in termination record ignored");
                record.set_length(0);
            }
            break;
        }
        break;
    }
    return true;
}


bool
srecord::input_file_idt::is_binary(void)
    const
{
    return true;
}


const char *
srecord::input_file_idt::get_file_format_name()
    const
{
    return "IDT System Integration Manager binary";
}


int
srecord::input_file_idt::format_option_number(void)
    const
{
    return arglex_tool::token_idt;
}


// vim: set ts=8 sw=4 et :
