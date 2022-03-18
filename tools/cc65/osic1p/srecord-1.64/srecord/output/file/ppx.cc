//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011, 2012 Peter Miller
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

#include <cstring>

#include <srecord/output/file/ppx.h>
#include <srecord/record.h>


srecord::output_file_ppx::~output_file_ppx()
{
    if (column)
    {
        if (column + 7 > line_length)
        {
            put_char('\n');
            column = 0;
        }
        else
        {
            put_char(' ');
            ++column;
        }
    }
    put_string("$S");
    put_word_be(dsum);
    put_char('\n');
    column = 0;
}


srecord::output_file_ppx::output_file_ppx(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    started(false),
    address(0),
    line_length(80),
    column(0),
    dsum(0)
{
}


srecord::output::pointer
srecord::output_file_ppx::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_ppx(a_file_name));
}


void
srecord::output_file_ppx::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    default:
        // ignore
        break;

    case srecord::record::type_header:
        // ignore
        break;

    case srecord::record::type_data:
        if (!started)
        {
            put_string("*\n");
            started = true;
            column = 0;
        }
        for (size_t j = 0; j < record.get_length(); ++j)
        {
            unsigned char data = record.get_data(j);
            unsigned long data_address = record.get_address() + j;

            if (data_address >= (1uL << 16))
                data_address_too_large(record, 16);

            if (data_address != address)
            {
                if (column)
                {
                    put_char('\n');
                    column = 0;
                }
                address = data_address;
            }
            if (column)
            {
                if (column + 3 > line_length)
                {
                    put_char('\n');
                    column = 0;
                }
                else
                {
                    put_char(' ');
                    ++column;
                }
            }
            if (column == 0)
            {
                put_word_be(address);
                put_char(' ');
                column = 5;
            }
            put_byte(data);
            column += 2;
            dsum += data;
            ++address;
        }
        break;

    case srecord::record::type_execution_start_address:
        // ignore
        break;
    }
}


void
srecord::output_file_ppx::line_length_set(int ll)
{
    line_length = (ll >= 7 ? ll : 7);
}


bool
srecord::output_file_ppx::preferred_block_size_set(int nbytes)
{
    return (nbytes >= 2 && nbytes <= record::max_data_length);
}


void
srecord::output_file_ppx::address_length_set(int)
{
    // Ignore
}


int
srecord::output_file_ppx::preferred_block_size_get(void)
    const
{
    //
    // Use the largest we can get,
    //
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_ppx::format_name(void)
    const
{
    return "Stag Prom Programmer Hexadecimal";
}


// vim: set ts=8 sw=4 et :
