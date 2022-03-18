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

#include <srecord/output/file/emon52.h>
#include <srecord/record.h>


srecord::output_file_emon52::~output_file_emon52()
{
    // check for termination record
}


srecord::output_file_emon52::output_file_emon52(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    pref_block_size(16)
{
}


srecord::output::pointer
srecord::output_file_emon52::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_emon52(a_file_name));
}


void
srecord::output_file_emon52::write(const srecord::record & record)
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
        put_byte(record.get_length());
        put_char(' ');
        put_word_be(record.get_address());
        put_char(':');
        checksum_reset();
        for (size_t j = 0; j < record.get_length(); ++j)
        {
            put_byte(record.get_data(j));
            put_char(' ');
        }
        put_word_be(checksum_get16());
        put_char('\n');
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        // This format can't do execution start address records
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_emon52::line_length_set(int linlen)
{
    //
    // Given the number of characters, figure the maximum number of
    // data baytes.
    //
    int n = (linlen - 11) / 3;

    //
    // Constrain based on the file format.
    //
    if (n < 1)
        n = 1;
    else if (n > 255)
        n = 255;

    //
    // An additional constraint is the size of the srecord::record
    // data buffer.
    //
    if (n > srecord::record::max_data_length)
        n = srecord::record::max_data_length;
    pref_block_size = n;
}


void
srecord::output_file_emon52::address_length_set(int)
{
    // ignore (this is only a 16-bit format)
}


bool
srecord::output_file_emon52::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_emon52::preferred_block_size_get() const
{
    return pref_block_size;
}


const char *
srecord::output_file_emon52::format_name()
    const
{
    return "Emon52";
}


// vim: set ts=8 sw=4 et :
