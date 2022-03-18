//
// srecord - manipulate eprom load files
// Copyright (C) 2012 Peter Miller
//
// Code contribution by Eric Smith <eric@brouhaha.com>
// Copyright assigned to Peter Miller 15-Mar-2012.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/output/file/trs80.h>
#include <srecord/record.h>


srecord::output_file_trs80::~output_file_trs80()
{
    if (!termination_seen)
    {
        put_byte(0x03);  // record type - END w/o transfer
        put_byte(0x02);  // length
        put_word_le(0x0000);
    }
}


srecord::output_file_trs80::output_file_trs80(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    termination_seen(false),
    pref_block_size(253)
{
}


srecord::output::pointer
srecord::output_file_trs80::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_trs80(a_file_name));
}


void
srecord::output_file_trs80::put_byte(unsigned char n)
{
    put_char(n);
    ++byte_offset;
}


void
srecord::output_file_trs80::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // This format can't do header record,
        // but we will borrow the comment type.
        {
            size_t len = record.get_length();
            if (len > 0)
            {
                if (len > 255)
                    len = 255;
                put_byte(0x05);
                put_byte(len);
                for (size_t j = 0; j < len; ++j)
                    put_byte(record.get_data(j));
            }
        }
        break;

    case srecord::record::type_data:
        if (record.get_length() < 1)
            return;
        if (!record.address_range_fits_into_n_bits(16))
            data_address_too_large(record, 16);

        //
        // Write the data out.
        //
        put_byte(0x01);
        put_byte((record.get_length() + 2) & 0xFF);
        put_word_le(record.get_address());
        for (size_t j = 0; j < record.get_length(); ++j)
            put_byte(record.get_data(j));
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            put_byte(0x02);  // record type - END w/ transfer
            put_byte(0x02);  // length
            put_word_le(record.get_address());
        }
        termination_seen = true;
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_trs80::line_length_set(int)
{
    // Ignore.
}


void
srecord::output_file_trs80::address_length_set(int)
{
    // Ignore (this is only a 16-bit format).
}


bool
srecord::output_file_trs80::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > 256)
        return false;
    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_trs80::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srecord::output_file_trs80::format_name()
    const
{
    return "TRS80";
}


bool
srecord::output_file_trs80::is_binary(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
