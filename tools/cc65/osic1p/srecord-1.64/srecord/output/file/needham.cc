//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2010 Peter Miller
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

#include <srecord/output/file/needham.h>
#include <srecord/record.h>


srecord::output_file_needham::~output_file_needham()
{
    if (column)
    {
        put_char('\n');
        column = 0;
    }
}


srecord::output_file_needham::output_file_needham(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    address(0),
    column(0),
    pref_block_size(16),
    address_length(4)
{
}


srecord::output::pointer
srecord::output_file_needham::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_needham(a_file_name));
}


void
srecord::output_file_needham::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // ignore
        if (!enable_optional_address_flag)
            address = (unsigned long)-1L;
        break;

    case srecord::record::type_data:
        if (address != record.get_address())
        {
            if (column + 4 > pref_block_size)
            {
                put_char('\n');
                column = 0;
            }
            else if (column)
                put_char(' ');
            address = record.get_address();
            int width = 2;
            if (address >= 0x1000000)
                width = 4;
            if (address >= 0x10000)
                width = 3;
            if (width < address_length)
                width = address_length;
            put_stringf("$A%0*lX,\n", width * 2, address);
            column = 0;
        }
        for (size_t j = 0; j < record.get_length(); ++j)
        {
            if (column)
                put_char(' ');
            put_byte(record.get_data(j));
            ++address;
            ++column;
            if (column >= pref_block_size)
            {
                put_char('\n');
                column = 0;
            }
        }
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        // ignore
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_needham::line_length_set(int linlen)
{
    int n = (linlen + 1) / 3;
    if (n < 1)
        n = 1;
    if (n > srecord::record::max_data_length)
        n = srecord::record::max_data_length;
    pref_block_size = n;
}


void
srecord::output_file_needham::address_length_set(int n)
{
    if (n < 2)
        n = 2;
    if (n > 4)
        n = 4;
    address_length = n;
}


bool
srecord::output_file_needham::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_needham::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srecord::output_file_needham::format_name()
    const
{
    return "Needham";
}
