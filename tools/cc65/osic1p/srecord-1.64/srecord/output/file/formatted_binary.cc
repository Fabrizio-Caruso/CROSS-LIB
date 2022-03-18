//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010, 2011 Peter Miller
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

#include <srecord/output/file/formatted_binary.h>
#include <srecord/record.h>


srecord::output_file_formatted_binary::~output_file_formatted_binary()
{
    // assert(address == upper_bound);
    if (address != upper_bound)
        fatal_error("upper bound stuffed");

    put_char(0);
    put_char(0);
    put_char(check_sum >> 8);
    put_char(check_sum);
}


srecord::output_file_formatted_binary::output_file_formatted_binary(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    upper_bound(0),
    address(0),
    check_sum(0)
{
    if (line_termination == line_termination_native)
        line_termination = line_termination_binary;
}


srecord::output::pointer
srecord::output_file_formatted_binary::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_formatted_binary(a_file_name));
}


void
srecord::output_file_formatted_binary::notify_upper_bound(unsigned long arg)
{
    upper_bound = arg;
    if (upper_bound == 0)
        fatal_error("can't write file with no data");
}


void
srecord::output_file_formatted_binary::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // assert(upper bound != 0);
        if (upper_bound == 0)
            fatal_error("must call srecord::output::notify_upper_bound first");
        if (upper_bound < (1uL << 16))
        {
            put_char(0x08); //     *
            put_char(0x1C); //    ***
            put_char(0x2A); //   * * *
            put_char(0x49); //  *  *  *
            put_char(0x08); //     *
            put_char(0x00);
            put_char((upper_bound >> 12) & 0x0F);
            put_char((upper_bound >>  8) & 0x0F);
            put_char((upper_bound >>  4) & 0x0F);
            put_char( upper_bound        & 0x0F);
        }
        else
        {
            put_char(0x08); //     *
            put_char(0x1C); //    ***
            put_char(0x3E); //   *****
            put_char(0x6B); //  ** * **
            put_char(0x08); //     *
            put_char(0x00);
            put_char((upper_bound >> 28) & 0x0F);
            put_char((upper_bound >> 24) & 0x0F);
            put_char((upper_bound >> 20) & 0x0F);
            put_char((upper_bound >> 16) & 0x0F);
            put_char((upper_bound >> 12) & 0x0F);
            put_char((upper_bound >>  8) & 0x0F);
            put_char((upper_bound >>  4) & 0x0F);
            put_char( upper_bound        & 0x0F);
        }
        put_char(0xFF);
        break;

    case srecord::record::type_data:
        {
            while (address < record.get_address())
            {
                put_char(0xFF);
                ++address;
            }
            const unsigned char *data = record.get_data();
            int length = record.get_length();
            while (length-- > 0)
            {
                // assert(address < upper_bound);
                if (address >= upper_bound)
                    fatal_error("upper bound stuffed");

                check_sum += *data;
                put_char(*data++);
                ++address;
            }
        }
        break;

    case srecord::record::type_execution_start_address:
        // This format can't do execution start address records
        break;

    case srecord::record::type_unknown:
    case srecord::record::type_data_count:
        // ignore
        break;
    }
}


void
srecord::output_file_formatted_binary::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srecord::output_file_formatted_binary::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


bool
srecord::output_file_formatted_binary::preferred_block_size_set(int nbytes)
{
    return (nbytes >= 1 || nbytes <= record::max_data_length);
}


int
srecord::output_file_formatted_binary::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_formatted_binary::format_name()
    const
{
    return "Formatted-Binary";
}


bool
srecord::output_file_formatted_binary::is_binary(void)
    const
{
    return true;
}
