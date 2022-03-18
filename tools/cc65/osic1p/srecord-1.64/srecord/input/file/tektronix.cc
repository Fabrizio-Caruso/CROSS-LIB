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
#include <srecord/input/file/tektronix.h>
#include <srecord/record.h>


srecord::input_file_tektronix::~input_file_tektronix()
{
    // make sure the termination record is done
}


srecord::input_file_tektronix::input_file_tektronix(
    const std::string &a_filename
) :
    srecord::input_file(a_filename),
    data_record_count(0),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false)
{
}


srecord::input_file::pointer
srecord::input_file_tektronix::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_tektronix(a_file_name));
}


int
srecord::input_file_tektronix::get_nibble(void)
{
    int n = srecord::input_file::get_nibble();
    checksum_add(n);
    return n;
}


int
srecord::input_file_tektronix::get_byte(void)
{
    // this differs from the srecord::input_file method only in that we
    // don't add to the checksum.
    int c1 = get_nibble();
    int c2 = get_nibble();
    return ((c1 << 4) | c2);
}


bool
srecord::input_file_tektronix::read_inner(srecord::record &record)
{
    for (;;)
    {
        int c = get_char();
        if (c < 0)
            return false;
        if (c == '/')
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

    if (peek_char() == '/')
    {
        //
        // lines beginning with "//" are end-of-file records
        //
        seek_to_end();
        return false;
    }

    unsigned char buffer[255+5];
    checksum_reset();
    buffer[0] = get_byte();
    buffer[1] = get_byte();
    buffer[2] = get_byte();
    int nibble_checksum = checksum_get();
    buffer[3] = get_byte();
    if (use_checksums() && nibble_checksum != buffer[3])
    {
        fatal_error
        (
            "header checksum mismatch (%02X != %02X)",
            nibble_checksum,
            buffer[3]
        );
    }
    if (buffer[2])
    {
        checksum_reset();
        for (int j = 0; j < buffer[2]; ++j)
            buffer[4 + j] = get_byte();
        int data_checksum_calc = checksum_get();
        int data_checksum_file = get_byte();
        if (use_checksums() && data_checksum_calc != data_checksum_file)
        {
            fatal_error
            (
                "data checksum mismatch (%02X != %02X)",
                data_checksum_calc,
                data_checksum_file
            );
        }
    }
    if (get_char() != '\n')
        fatal_error("end-of-line expected");

    record =
        srecord::record
        (
            (
                buffer[2] == 0
            ?
                srecord::record::type_execution_start_address
            :
                srecord::record::type_data
            ),
            srecord::record::decode_big_endian(buffer, 2),
            buffer + 4,
            buffer[2]
        );
    return true;
}


bool
srecord::input_file_tektronix::read(srecord::record &record)
{
    for (;;)
    {
        if (!read_inner(record))
        {
            if (!seen_some_input && garbage_warning)
                fatal_error("file contains no data");
            if (data_record_count <= 0)
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
        default:
            // impossible
            continue;

        case srecord::record::type_data:
            ++data_record_count;
            break;

        case srecord::record::type_execution_start_address:
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
srecord::input_file_tektronix::get_file_format_name(void)
    const
{
    return "Tektronix (16-bit)";
}


int
srecord::input_file_tektronix::format_option_number(void)
    const
{
    return arglex_tool::token_tektronix;
}


// vim: set ts=8 sw=4 et :
