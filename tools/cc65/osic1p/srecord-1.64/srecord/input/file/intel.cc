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
#include <srecord/input/file/intel.h>
#include <srecord/record.h>


srecord::input_file_intel::~input_file_intel()
{
    if (pushback)
        delete pushback;
}


srecord::input_file_intel::input_file_intel(const std::string &a_file_name) :
    srecord::input_file(a_file_name),
    data_record_count(0),
    garbage_warning(false),
    seen_some_input(false),
    termination_seen(false),
    mode(mode_i8hex),
    address_base(0),
    pushback(0),
    end_seen(false)
{
}


srecord::input_file::pointer
srecord::input_file_intel::create(const std::string &a_file_name)
{
    return pointer(new srecord::input_file_intel(a_file_name));
}


bool
srecord::input_file_intel::read_inner(srecord::record &record)
{
    if (pushback)
    {
        record = *pushback;
        delete pushback;
        pushback = 0;
        return true;
    }

    //
    // keep reading lines until something returnable comes along
    //
    for (;;)
    {
        //
        // grab the first character of the line
        //
        int c = get_char();

        //
        // end of file means we are done
        //
        if (c < 0)
            return false;

        //
        // quietly ignore blank lines
        //
        if (c == '\n')
            continue;

        //
        // If it doesn't start with a colon, it's a garbage line.
        // Warn, and then ignore it.
        //
        if (c != ':')
        {
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
            continue;
        }

        //
        // Looks like a real Intel-hex line.
        //
        unsigned char buffer[255+5];
        checksum_reset();
        buffer[0] = get_byte();
        buffer[1] = get_byte();
        buffer[2] = get_byte();
        buffer[3] = get_byte();
        for (int j = 0; j <= buffer[0]; ++j)
            buffer[4 + j] = get_byte();
        if (use_checksums())
        {
            int n = checksum_get();
            if (n != 0)
                fatal_error("checksum mismatch (%02X != 00)", n);
        }
        if (get_char() != '\n')
            fatal_error("end-of-line expected");

        srecord::record::address_t address_field =
            srecord::record::decode_big_endian(buffer + 1, 2);

        srecord::record::type_t type = srecord::record::type_unknown;
        switch (buffer[3])
        {
        case 0:
            //
            // data
            //
            switch (mode)
            {
            case mode_linear:
                //
                // linear addressing model
                //
                if
                (
                    (
                        (long long)address_base + address_field
                    <
                        ((long long)1 << 32)
                    )
                &&
                    (
                        (long long)address_base + address_field + buffer[0]
                    >
                        ((long long)1 << 32)
                    )
                )
                {
                    int split =
                        ((long long)1 << 32) - address_base - address_field;
                    pushback =
                        new srecord::record
                        (
                            srecord::record::type_data,
                            0L,
                            buffer + 4 + split,
                            buffer[0] - split
                        );
                    buffer[0] = split;
                }
                break;

            case mode_segmented:
                //
                // segmented addressing model
                //
                if (address_field + buffer[0] > (1L << 16))
                {
                    //
                    // If the record would cross a segment boundary,
                    // wrap around to the beginning of the segment.  We
                    // do this by using "pushback" to remember additonal
                    // data.
                    //
                    int split = (1L << 16) - address_field;
                    pushback =
                        new srecord::record
                        (
                            srecord::record::type_data,
                            address_base + ((address_field + split) & 0xFFFF),
                            buffer + 4 + split,
                            buffer[0] - split
                        );
                    buffer[0] = split;
                }
                break;

            case mode_i8hex:
                break;
            }
            type = srecord::record::type_data;
            break;

        case 1:
            //
            // end-of-file record
            //
            if (buffer[0] != 0)
                fatal_error("EOF data length field must be zero");
            end_seen = true;
            seek_to_end();
            switch (mode)
            {
            case mode_i8hex:
                if (address_field != 0)
                {
                    record =
                        srecord::record
                        (
                            srecord::record::type_execution_start_address,
                            address_field,
                            0,
                            0
                        );
                    return true;
                }
                break;

            case mode_linear:
            case mode_segmented:
                if (address_field != 0)
                {
                    warning
                    (
                        "end-of-file record's address field should be zero, "
                            "not 0x%04X",
                        address_field
                    );
                }
                break;
            }
            return false;

        case 2:
            //
            // extended segment address record
            //
            // Set the base address and addressing mode,
            // and then loop for another record, this one
            // isn't visable to the caller.
            //
            if (buffer[0] != 2)
                fatal_error("length field must be 2");
            if (address_field != 0)
                fatal_error("address field must be zero");
            address_field =
                srecord::record::decode_big_endian(buffer + 4, 2);
            address_base = address_field << 4;
            mode = mode_segmented;
            continue;

        case 3:
            //
            // start segment address record
            //
            if (buffer[0] != 4)
                fatal_error("length field must be 4");
            if (address_field != 0)
                fatal_error("address field must be zero");
            address_field =
                srecord::record::decode_big_endian(buffer + 4, 2) * 16 +
                srecord::record::decode_big_endian(buffer + 6, 2);
            record =
                srecord::record
                (
                    srecord::record::type_execution_start_address,
                    address_field,
                    0,
                    0
                );
            mode = mode_segmented;
            return true;

        case 4:
            //
            // extended linear address record
            //
            // Set the base address and addressing mode,
            // and then loop for another record, this one
            // isn't visable to the caller.
            //
            if (buffer[0] != 2)
                fatal_error("length field must be 2");
            if (address_field != 0)
                fatal_error("address field must be zero");
            address_field =
                srecord::record::decode_big_endian(buffer + 4, 2);
            address_base = address_field << 16;
            mode = mode_linear;
            continue;

        case 5:
            //
            // start linear address record
            //
            if (buffer[0] != 4)
                fatal_error("length field must be 4");
            if (address_field != 0)
                fatal_error("address field must be zero");
            address_field = srecord::record::decode_big_endian(buffer + 4, 4);
            record =
                srecord::record
                (
                    srecord::record::type_execution_start_address,
                    address_field,
                    0,
                    0
                );
            mode = mode_linear;
            return true;
        }

        //
        // data record or unknown
        //
        record =
            srecord::record
            (
                type,
                address_base + address_field,
                buffer + 4,
                buffer[0]
            );
        return true;
    }
}


bool
srecord::input_file_intel::read(srecord::record &record)
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
                //
                // Eric Weddington:  ''From looking at the "Hexadecimal
                // Object File Format Specification" from Intel,
                // Revision A, January 6, 1988, the Extended Segment
                // Address Record (0x02) and the Extended Linear Address
                // Record (0x04) are both optional. The respective
                // addresses default to 0 until either type of record is
                // encountered.''
                //
                termination_seen = true;
#if 0
                //
                // We could synthesize an execution start address, but
                // that means the input and output don't agree when a
                // round-trip occurs.
                //
                record =
                    srecord::record
                    (
                        srecord::record::type_execution_start_address,
                        0,
                        0,
                        0
                    );
                return true;
#endif
            }
            if (!end_seen)
            {
                warning("no end-of-file record");
                end_seen = true;
            }
            return false;
        }
        seen_some_input = true;
        switch (record.get_type())
        {
        case srecord::record::type_unknown:
            fatal_error("record type not recognised");
            break;

        default:
            // impossible
            continue;

        case srecord::record::type_data:
            ++data_record_count;
            if (record.get_length() == 0)
            {
                warning("empty data record ignored");
                continue;
            }
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
srecord::input_file_intel::get_file_format_name()
    const
{
    return "Intel Hexadecimal (MCS-86)";
}


int
srecord::input_file_intel::format_option_number(void)
    const
{
    return arglex_tool::token_intel;
}


// vim: set ts=8 sw=4 et :
