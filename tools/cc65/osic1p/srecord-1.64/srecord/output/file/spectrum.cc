//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010 Peter Miller
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

#include <srecord/output/file/spectrum.h>
#include <srecord/record.h>


srecord::output_file_spectrum::~output_file_spectrum()
{
    if (enable_footer_flag)
        put_char(3);
}


srecord::output_file_spectrum::output_file_spectrum(
    const std::string &a_filename
) :
    srecord::output_file(a_filename)
{
}


srecord::output::pointer
srecord::output_file_spectrum::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_spectrum(a_file_name));
}


void
srecord::output_file_spectrum::put_decimal(unsigned long n)
{
    put_stringf("%4.4lu", n);
}


void
srecord::output_file_spectrum::put_binary(unsigned char n)
{
    for (unsigned char bit = 0x80; bit; bit >>= 1)
    {
        put_char((n & bit) ? '1' : '0');
    }
}


void
srecord::output_file_spectrum::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        if (enable_header_flag)
            put_char(2);
        break;

    case srecord::record::type_data:
        {
            unsigned long address = record.get_address();
            int length = record.get_length();
            for (int j = 0; j < length; ++j)
            {
                put_decimal(address + j);
                put_char(' ');
                put_binary(record.get_data(j));
                put_char('\n');
            }
        }
        break;

    case srecord::record::type_execution_start_address:
    case srecord::record::type_unknown:
    case srecord::record::type_data_count:
        // ignore
        break;
    }
}


void
srecord::output_file_spectrum::line_length_set(int)
{
    // ignore
}


void
srecord::output_file_spectrum::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_spectrum::preferred_block_size_set(int nbytes)
{
    return (nbytes == 1);
}


int
srecord::output_file_spectrum::preferred_block_size_get()
    const
{
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_spectrum::format_name()
    const
{
    return "Spectrum";
}
