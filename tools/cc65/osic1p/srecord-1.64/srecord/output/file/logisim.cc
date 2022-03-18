//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
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

#include <cstdio> // for snprintf
#include <cstring>
#include <cassert>

#include <srecord/output/file/logisim.h>
#include <srecord/record.h>


srecord::output_file_logisim::~output_file_logisim()
{
    if (column)
        put_char('\n');
}


srecord::output_file_logisim::output_file_logisim(
    const std::string &a_file_name
) :
    output_file(a_file_name),
    address(0),
    line_length(80),
    column(0)
{
    put_string("v2.0 raw\n\n");
}


srecord::output::pointer
srecord::output_file_logisim::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_logisim(a_file_name));
}


void
srecord::output_file_logisim::emit(int count, int value)
{
    assert(count > 0);
    char buffer[100];
    if (count == 1)
        snprintf(buffer, sizeof(buffer), "%X", value);
    else
        snprintf(buffer, sizeof(buffer), "%d*%X", count, value);
    int buffer_length = strlen(buffer);

    address += count;

    if (column == 0)
    {
        // even if the word is 80 chars long, print it out
        // if we are at the start of  a new line
    }
    else if (column + 1 + buffer_length <= line_length)
    {
        // separate each datum by white space.
        put_char(' ');
        ++column;
    }
    else
    {
        // will not fit on the current line,
        // so throw a newline.
        put_char('\n');
        column = 0;
    }
    put_string(buffer);
    column += buffer_length;
}


void
srecord::output_file_logisim::write(const srecord::record &record)
{
    // This format can't do header records or termination records
    if (record.get_type() != srecord::record::type_data)
        return;

    if (!record.address_range_fits_into_n_bits(16))
        data_address_too_large(record, 16);

    assert (address <= record.get_address());
    while (address < record.get_address())
    {
#if 1
        fatal_hole_error(address, record.get_address() - 1);
#else
        // fill holes with 0x00
        // this format has no provision for switching adddress mid stream.
        int count = record.get_address() - address;
        if (count >= 512)
            count = 512;
        emit(count, 0x00);
#endif
    }

    for (size_t j = 0; j < record.get_length(); ++j)
    {
        int value = record.get_data(j);

        // look for runs, at least of length 4.
        int count = 1;
        while
        (
            j + count < record.get_length()
        &&
            record.get_data(j + count) == value
        )
        {
             ++count;
        }
        // Logisim only uses counts when the runs are longer than 4.
        if (count < 4)
            count = 1;
        j += count - 1;

        emit(count, value);
    }
}


void
srecord::output_file_logisim::line_length_set(int x)
{
    line_length = (x <= 0 ? 80 : (x < 20 ? 20 : x));
}


void
srecord::output_file_logisim::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


bool
srecord::output_file_logisim::preferred_block_size_set(int nbytes)
{
    (void)nbytes;
    return false;
}


int
srecord::output_file_logisim::preferred_block_size_get(void)
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return (srecord::record::max_data_length);
}


const char *
srecord::output_file_logisim::format_name(void)
    const
{
    return "Logisim";
}


// vim: set ts=8 sw=4 et :
