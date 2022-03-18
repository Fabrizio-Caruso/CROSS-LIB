//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2003, 2005-2011, 2013 Peter Miller
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
#include <srecord/input/file/motorola.h>
#include <srecord/record.h>


srecord::input_file_motorola::~input_file_motorola()
{
}


srecord::input_file_motorola::input_file_motorola(
    const std::string &a_file_name
) :
    input_file(a_file_name),
    data_count(0),
    garbage_warning(false),
    seen_some_input(false),
    header_seen(false),
    termination_seen(false),
    address_shift(0)
{
}


srecord::input_file::pointer
srecord::input_file_motorola::create(const std::string &a_file_name)
{
    return pointer(new input_file_motorola(a_file_name));
}


void
srecord::input_file_motorola::command_line(arglex_tool *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
        int n = cmdln->value_number();
        cmdln->token_next();

        //
        // Shift Bytes Bits
        //   0     1     8
        //   1     2    16
        //   2     4    32
        //   3     8    64
        //
        switch (n)
        {
        case 1:
        case 8:
            address_shift = 0;
            break;

        case 2:
        case 16:
            address_shift = 1;
            break;

        case 4:
        case 32:
            address_shift = 2;
            break;

        case 64:
            address_shift = 3;
            break;

        default:
            fatal_error
            (
                "-motorola address multiple %d not understood (do "
                    "you need parentheses to group the expressions "
                    "differently?)",
                n
            );
            // NOTREACHED
        }
    }
}


bool
srecord::input_file_motorola::read_inner(record &result)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return false;
        if (c == 'S')
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
    int tag = get_nibble();
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

    int naddr = 2;
    record::type_t type = record::type_unknown;
    switch (tag)
    {
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
    long tmp_addr = record::decode_big_endian(buffer, naddr);
    if (address_shift && type != record::type_data_count)
        tmp_addr <<= address_shift;
    result = record(type, tmp_addr, buffer + naddr, line_length - naddr);
    return true;
}


bool
srecord::input_file_motorola::read(record &record)
{
    for (;;)
    {
        if (!read_inner(record))
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
        if (record.get_type() != record::type_header && !header_seen)
        {
            warning("no header record");
            header_seen = true;
        }
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
            fatal_error("record type not recognised");
            break;

        case record::type_header:
            if (header_seen)
                warning("redundant header record");
            if (record.get_address())
            {
                warning("address in header record ignored");
                record.set_address(0);
            }
            header_seen = true;
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
            if (termination_seen)
                warning("redundant termination record");
            termination_seen = true;
            break;
        }
        break;
    }
    return true;
}


const char *
srecord::input_file_motorola::get_file_format_name(void)
    const
{
    return "Motorola S-Record";
}


int
srecord::input_file_motorola::format_option_number(void)
    const
{
    return arglex_tool::token_motorola;
}


// vim: set ts=8 sw=4 et :
