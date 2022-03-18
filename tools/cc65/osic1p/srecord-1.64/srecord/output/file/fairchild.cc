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

#include <srecord/output/file/fairchild.h>
#include <srecord/record.h>


srecord::output_file_fairchild::~output_file_fairchild()
{
}


srecord::output_file_fairchild::output_file_fairchild(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    address(~0uL)
{
}


srecord::output::pointer
srecord::output_file_fairchild::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_fairchild(a_file_name));
}


void
srecord::output_file_fairchild::put_nibble(unsigned n)
{
    srecord::output_file::put_nibble(n);
    checksum_add(n & 15);
}


void
srecord::output_file_fairchild::put_byte(unsigned char n)
{
    // This differs from srecord::output_file::put_byte only in that it
    // doesn't add to the checksum.
    put_nibble(n >> 4);
    put_nibble(n);
}


void
srecord::output_file_fairchild::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        if (!enable_optional_address_flag)
            address = (unsigned long)-1L;
        break;

    case srecord::record::type_unknown:
    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_data:
        {
            int len = record.get_length();
            unsigned long new_addr = record.get_address();
            if ((new_addr & 7) || (len & 7))
                fatal_alignment_error(8);
            if (address != new_addr)
            {
                put_stringf("S%4.4lX\n", new_addr);
                address = new_addr;
            }
            for (int j = 0; j < len; j += 8)
            {
                put_char('X');
                checksum_reset();

                //
                // Emit the one line of data.
                //
                put_byte(record.get_data(j));
                put_byte(record.get_data(j + 1));
                put_byte(record.get_data(j + 2));
                put_byte(record.get_data(j + 3));
                put_byte(record.get_data(j + 4));
                put_byte(record.get_data(j + 5));
                put_byte(record.get_data(j + 6));
                put_byte(record.get_data(j + 7));
                address += 8;

                put_nibble(checksum_get());
                put_char('\n');
            }
        }
        break;

    case srecord::record::type_execution_start_address:
        put_string("*\n");
        break;
    }
}


void
srecord::output_file_fairchild::line_length_set(int)
{
    // ignore
}


void
srecord::output_file_fairchild::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_fairchild::preferred_block_size_set(int nbytes)
{
    return (nbytes == 8);
}


int
srecord::output_file_fairchild::preferred_block_size_get()
    const
{
    return 8;
}


const char *
srecord::output_file_fairchild::format_name()
    const
{
    return "Fairchild";
}
