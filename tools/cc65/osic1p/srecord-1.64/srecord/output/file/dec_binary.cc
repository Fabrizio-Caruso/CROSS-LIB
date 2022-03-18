//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2006-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <srecord/output/file/dec_binary.h>
#include <srecord/record.h>

#define BLOCK_SIZE 512
#define BLOCK_SIZE_MASK (BLOCK_SIZE - 1)


srecord::output_file_dec_binary::~output_file_dec_binary()
{
    // Round off to a whole multiple of BLOCK_SIZE bytes.
    while (byte_offset & BLOCK_SIZE_MASK)
        put_byte(0);
}


srecord::output_file_dec_binary::output_file_dec_binary(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    pref_block_size(preferred_block_size_calculate())
{
    if (line_termination == line_termination_native)
        line_termination = line_termination_binary;
}


srecord::output::pointer
srecord::output_file_dec_binary::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_dec_binary(a_file_name));
}


void
srecord::output_file_dec_binary::put_byte(unsigned char n)
{
    checksum_add(n);
    put_char(n);
    byte_offset++;
}


void
srecord::output_file_dec_binary::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // This format can't do header records
        break;

    case srecord::record::type_data:
        if (record.get_length() < 1)
            return;
        if (!record.address_range_fits_into_n_bits(16))
            data_address_too_large(record, 16);

#if 0
        //
        // Don't span block boundaries.
        //
        if
        (
            (byte_offset & ~BLOCK_SIZE_MASK)
        !=
            ((byte_offset + 6 + record.get_length())
                & ~BLOCK_SIZE_MASK)
        )
        {
            fprintf(stderr, "%s: %d: spans block %4.4lX/%4.4lX\n", __FILE__,
                __LINE__, byte_offset, byte_offset + 6 + record.get_length());
            while (byte_offset & BLOCK_SIZE_MASK)
                put_byte(0);
        }
#endif

        //
        // Write the data out.
        //
        checksum_reset();
        put_word_le(1);
        put_word_le(record.get_length() + 6);
        put_word_le(record.get_address());
        for (size_t j = 0; j < record.get_length(); ++j)
            put_byte(record.get_data(j));
        put_byte(-checksum_get());
        if (byte_offset & 1)
            put_byte(0);
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            checksum_reset();
            put_word_le(1);
            put_word_le(6);
            put_word_le(record.get_address());
            put_byte(-checksum_get());
        }
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_dec_binary::line_length_set(int)
{
    // Ignore.
}


void
srecord::output_file_dec_binary::address_length_set(int)
{
    // Ignore (this is only a 16-bit format).
}


bool
srecord::output_file_dec_binary::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_dec_binary::preferred_block_size_get()
    const
{
    return pref_block_size;
}


int
srecord::output_file_dec_binary::preferred_block_size_calculate()
{
#if 0
    //
    // Because we don't cross BLOCK_SIZE boundaries, we need a number
    // less than srecord::record::max_data_length that, when you add 7
    // and double, gives
    //
    int max = BLOCK_SIZE;
    for (;;)
    {
        if (max - 7 <= srecord::record::max_data_length)
            return (max - 7);
        max >>= 1;
    }
    return 1;
#else
    return srecord::record::max_data_length;
#endif
}


const char *
srecord::output_file_dec_binary::format_name()
    const
{
    return "DEC-Binary";
}


bool
srecord::output_file_dec_binary::is_binary(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
