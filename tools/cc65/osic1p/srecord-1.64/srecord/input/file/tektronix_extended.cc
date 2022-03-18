//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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
#include <srecord/input/file/tektronix_extended.h>
#include <srecord/record.h>


srecord::input_file_tektronix_extended::~input_file_tektronix_extended()
{
}


srecord::input_file_tektronix_extended::input_file_tektronix_extended(
    const std::string &a_file_name
) :
    srecord::input_file(a_file_name),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false),
    nibble_sum(0)
{
}


srecord::input_file::pointer
srecord::input_file_tektronix_extended::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_tektronix_extended(a_file_name));
}


int
srecord::input_file_tektronix_extended::get_nibble(void)
{
    int n = inherited::get_nibble();
    nibble_sum += n;
    return n;
}


bool
srecord::input_file_tektronix_extended::read_inner(srecord::record &record)
{
    for (;;)
    {
        for (;;)
        {
            int c = get_char();
            if (c < 0)
                return false;
            if (c == '%')
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
        nibble_sum = 0;
        int length = get_byte();
        if (length < 6)
        {
            // 2(length) + 1(tag) + 2(csum) + 1(addrlen)
            fatal_error("line length invalid (%d < 6)", length);
        }
        length -= 2;

        int tag = get_nibble();
        --length;

        unsigned char csum_expected = get_byte();
        // except the checksum itself
        nibble_sum -= ((csum_expected >> 4) & 15) + (csum_expected & 15);
        length -= 2;

        int addr_len = get_nibble();
        if (addr_len == 0)
            addr_len = 16;
        --length;
        int addr_len_max = 2 * sizeof(srecord::record::address_t);
        if (addr_len > addr_len_max)
        {
            fatal_error
            (
                "address length too big (%d > %d)",
                addr_len,
                addr_len_max
            );
        }
        if (length < addr_len)
        {
            fatal_error
            (
                "address length exceeds line length (%d > %d)",
                addr_len,
                length
            );
        }

        unsigned long address = 0;
        while (addr_len > 0)
        {
            int n = get_nibble();
            address = address * 16 + n;
            --addr_len;
            --length;
        }
        if (length & 1)
            fatal_error("data length invalid (%d is odd)", length);

        unsigned char buffer[125];
        for (int j = 0; j * 2 < length; ++j)
        {
            int n = get_byte();
            buffer[j] = n;
        }
        if (csum_expected != nibble_sum)
        {
            fatal_error
            (
                "checksum mismatch (file says 0x%02X, expected 0x%02X)",
                csum_expected,
                nibble_sum
            );
        }
        if (get_char() != '\n')
            fatal_error("end-of-line expected");

        srecord::record::type_t type = srecord::record::type_unknown;
        switch (tag)
        {
        default:
            fatal_error("unknown tag (%X)", tag);
            continue;

        case 3:
            // symbol record, ignore
            continue;

        case 6:
            // data
            type = srecord::record::type_data;
            break;

        case 8:
            // termination
            type = srecord::record::type_execution_start_address;
            break;
        }
        record = srecord::record(type, address, buffer, length >> 1);
        return true;
    }
}


bool
srecord::input_file_tektronix_extended::read(srecord::record &record)
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
            record.get_type() != srecord::record::type_execution_start_address
        &&
            termination_seen
        )
        {
            warning("termination record should be last");
            termination_seen = false;
        }
        switch (record.get_type())
        {
        case srecord::record::type_unknown:
        case srecord::record::type_header:
        case srecord::record::type_data_count:
            fatal_error("record type not recognised");
            break;

        case srecord::record::type_data:
            ++data_record_count;
            if (record.get_length() == 0)
            {
                warning("empty data record ignored");
                continue;
            }
            break;

        case srecord::record::type_execution_start_address:
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


const char *
srecord::input_file_tektronix_extended::get_file_format_name(void)
    const
{
    return "Tektronix Extended";
}


int
srecord::input_file_tektronix_extended::format_option_number(void)
    const
{
    return arglex_tool::token_tektronix_extended;
}


// vim: set ts=8 sw=4 et :
