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

#include <srecord/output/file/atmel_generic.h>
#include <srecord/record.h>


srecord::output_file_atmel_generic::~output_file_atmel_generic()
{
}


srecord::output_file_atmel_generic::output_file_atmel_generic(
    const std::string &a_file_name,
    endian_t a_end
) :
    srecord::output_file(a_file_name),
    end(a_end)
{
}


srecord::output::pointer
srecord::output_file_atmel_generic::create(const std::string &a_file_name,
    endian_t a_end)
{
    return pointer(new srecord::output_file_atmel_generic(a_file_name, a_end));
}


void
srecord::output_file_atmel_generic::write(const srecord::record &record)
{
    // This format can't do header records or termination records
    if (record.get_type() != srecord::record::type_data)
        return;

    if (!record.address_range_fits_into_n_bits(17))
        data_address_too_large(record, 17);

    long address = record.get_address();
    if ((address & 1) || (record.get_length() & 1))
        fatal_alignment_error(2);
    for (size_t j = 0; j < record.get_length(); j += 2)
    {
        put_3bytes_be(address / 2);
        put_char(':');
        if (end == endian_big)
        {
            put_byte(record.get_data(j + 1));
            put_byte(record.get_data(j));
        }
        else
        {
            put_byte(record.get_data(j));
            put_byte(record.get_data(j + 1));
        }
        put_char('\n');
        address += 2;
    }
}


void
srecord::output_file_atmel_generic::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srecord::output_file_atmel_generic::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


bool
srecord::output_file_atmel_generic::preferred_block_size_set(int nbytes)
{
    return (nbytes == 2);
}


int
srecord::output_file_atmel_generic::preferred_block_size_get()
    const
{
    //
    // Use the largest we can get,
    // but make sure it is an even number of bytes long.
    //
    return (srecord::record::max_data_length & ~1);
}


const char *
srecord::output_file_atmel_generic::format_name()
    const
{
    return "Atmel-Generic";
}


// vim: set ts=8 sw=4 et :
