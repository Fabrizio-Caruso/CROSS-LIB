//
// srecord - manipulate eprom load files
// Copyright (C) 2009-2011, 2013 Peter Miller
//
// Code contribution by David Kozub <zub@linux.fjfi.cvut.cz>
// Copyright assigned to Peter Miller 27-Jan-2010.
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

#include <algorithm>
#include <limits>
#include <cstring>

#include <srecord/arglex/tool.h>
#include <srecord/input/file/msbin.h>


srecord::input_file_msbin::~input_file_msbin()
{
    if (first_record_read)
    {
        // Check if data from file header were correct
        if (lowest_address != image_start)
        {
            warning
            (
                "image address header field is wrong (header = 0x%08lX, "
                    "actual = 0x%08lX)",
                (unsigned long)image_start,
                (unsigned long)lowest_address
            );
        }
        if (highest_address - lowest_address + 1 != image_length)
        {
            warning
            (
                "image length header field is wrong (header = 0x%08lX, "
                    "actual = 0x%08lX)",
                (unsigned long)image_length,
                (unsigned long)(highest_address - lowest_address + 1)
            );
        }
    }
}


srecord::input_file_msbin::input_file_msbin(const std::string &a_file_name) :
    input_file(a_file_name),
    header_read(false),
    first_record_read(false),
    execution_start_record_seen(false),
    last_record_warning(false),
    address(0),
    remaining(0),
    record_checksum(0),
    running_checksum(0)
{
}


srecord::input_file::pointer
srecord::input_file_msbin::create(const std::string &a_file_name)
{
    return pointer(new input_file_msbin(a_file_name));
}


uint32_t
srecord::input_file_msbin::read_dword_le(void)
{
    unsigned char c[sizeof(uint32_t)];

    for (size_t i = 0; i < sizeof(c); ++i)
    {
        int j = get_char();
        if (j < 0)
            fatal_error("short input file");

        assert(j <= std::numeric_limits<unsigned char>::max());
        c[i] = (unsigned char)j;
    }

    return record::decode_little_endian(c, sizeof(c));
}


void
srecord::input_file_msbin::read_file_header(void)
{
    // Optional magic
    static const unsigned char Magic[7] =
        { 'B', '0', '0', '0', 'F', 'F', '\n' };

    // +1 so that buff can be reused for two dwords in case the magic is missing
    unsigned char buff[sizeof(Magic) + 1];
    for (size_t i = 0; i < sizeof(Magic); ++i)
    {
        int j = get_char();
        if (j < 0)
            fatal_error("short input file");

        assert(j <= std::numeric_limits<unsigned char>::max());
        buff[i] = j;
    }

    BOOST_STATIC_ASSERT(sizeof(buff) >= sizeof(Magic));
    if (memcmp(Magic, buff, sizeof(Magic)))
    {
        // Ok, there's no magic in the header. But it's optional anyway.

        // Fill up to two dwords
        BOOST_STATIC_ASSERT(sizeof(buff) == 2 * sizeof(uint32_t));
        int j = get_char();
        if (j < 0)
            fatal_error("short input file");
        buff[sizeof(buff) - 1] = j;

        // Read first dword
        image_start = record::decode_little_endian(buff, sizeof(uint32_t));

        // Read second dword
        image_length =
            record::decode_little_endian
            (
                buff + sizeof(uint32_t),
                sizeof(uint32_t)
            );
    }
    else
    {
        image_start = read_dword_le();
        image_length = read_dword_le();
    }

    // What shall we do with the (useless) file header?
    // Throw it away?
    //warning("image_start = %08x", (unsigned int)image_start);
    //warning("image_length = %08x", (unsigned int)image_length);
}


uint32_t
srecord::input_file_msbin::checksum(const unsigned char *data, size_t len)
{
    uint32_t sum = 0;

    for (size_t i = 0; i < len; ++i)
        sum += data[i];

    return sum;
}


bool
srecord::input_file_msbin::read(record &result)
{
    // Read the file header if we haven't read it yet.
    if (!header_read)
    {
        read_file_header();
        header_read = true;
    }

    if (remaining == 0)
    {
        // No remaining data from any previous record. => Try to read
        // next record header, if present.

        if (peek_char() < 0)
        {
            // Check if we have seen the execution start address record.
            if (!execution_start_record_seen)
                warning("input file is missing the execution start record");

            return false; // end of file
        }

        if (execution_start_record_seen && !last_record_warning)
        {
            warning
            (
                "the execution start record is not the last record; "
                "reading further records"
            );
            last_record_warning = true;
        }

        // Read header of the next record
        address = read_dword_le();
        remaining = read_dword_le();
        record_checksum = read_dword_le();

        // Reset running checksum
        running_checksum = 0;

        // Bookkeeping for tracking the address range - but ignore
        // the execution start address record, as it has a special
        // format!
        if (address != 0)
        {
            if (!first_record_read)
            {
                lowest_address = address;
                highest_address = address + remaining - 1;
                first_record_read = true;
            }
            else
            {
                lowest_address = std::min(lowest_address, address);
                highest_address =
                    std::max(highest_address, address + remaining - 1);
            }
        }
    }

    if (address == 0)
    {
        // This is a special record specifying the execution start address.
        if (record_checksum != 0 && use_checksums())
        {
            fatal_error
            (
                "checksum of the execution start record is not 0, as "
                    "required by specification (0x%08lX != 0x00000000)",
                (unsigned long)record_checksum
            );
        }

        result = record(record::type_execution_start_address, remaining, 0, 0);

        // This should be the last record - but if it was not, we try to read
        // further and produce a warning.
        remaining = 0;
        execution_start_record_seen = true;
        return true;
    }

    // Data record
    // Read (part) of the record
    unsigned char data[record::max_data_length];
    const size_t to_read =
        std::min(remaining, (uint32_t)record::max_data_length);

    int c = get_char();
    if (c < 0)
    {
        fatal_error("short input file");
        return false;
    }

    size_t read = 0;
    while (read < to_read)
    {
        assert(c <= std::numeric_limits<unsigned char>::max());
        data[read++] = c;
        if (read >= to_read)
            break;
        c = get_char();
        if (c < 0)
        {
            fatal_error("short input file");
            return false;
        }
    }

    result = record(record::type_data, address, data, read);
    address += read;
    assert(remaining >= read);
    remaining -= read;
    running_checksum += checksum(data, read);

    if (remaining == 0)
    {
        // All data in a record was read. => We can verify the checksum
        // now.
        if (running_checksum != record_checksum && use_checksums())
        {
            fatal_error
            (
                "wrong record checksum (0x%08lX != 0x%08lX)",
                (unsigned long)running_checksum,
                (unsigned long)record_checksum
            );
        }
    }

    return true;
}


bool
srecord::input_file_msbin::is_binary(void)
    const
{
    return true;
}


const char *
srecord::input_file_msbin::get_file_format_name(void)
    const
{
    return "Windows CE Binary Image Data Format";
}


int
srecord::input_file_msbin::format_option_number(void)
    const
{
    return arglex_tool::token_msbin;
}


// vim: set ts=8 sw=4 et :
