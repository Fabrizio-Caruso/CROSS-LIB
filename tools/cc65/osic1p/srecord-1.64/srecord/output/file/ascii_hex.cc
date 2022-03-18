//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2010 Peter Miller
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

#include <srecord/output/file/ascii_hex.h>
#include <srecord/record.h>


srecord::output_file_ascii_hex::~output_file_ascii_hex()
{
    emit_end_of_file();
}


srecord::output_file_ascii_hex::output_file_ascii_hex(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    address(0),
    column(0),
    pref_block_size(16),
    line_length(pref_block_size * 3 - 1),
    address_length(2),
    start_code_emitted(false),
    end_code_emitted(false)
{
}


srecord::output::pointer
srecord::output_file_ascii_hex::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_ascii_hex(a_file_name));
}


void
srecord::output_file_ascii_hex::emit_end_of_file()
{
    if (end_code_emitted)
        return;

    if (column)
    {
        if (column + 2 > line_length)
            put_char('\n');
        else
            put_char(' ');
    }
    put_char(3);
    put_char('\n');
    column = 0;
    end_code_emitted = true;

    if (enable_footer_flag)
    {
        //
        // According to the documentation we emit the checksum after
        // the ETX, which should mean that it is ignored (?!?)
        //
        put_stringf("$S%4.4X,\n", checksum_get16());
    }
}



void
srecord::output_file_ascii_hex::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // All header data is discarded
        break;

    case srecord::record::type_data:
        if (!start_code_emitted)
        {
            put_char(2);
            ++column;
            start_code_emitted = true;

            if (!enable_optional_address_flag)
                address = (unsigned long)-1L;
        }
        if (address != record.get_address())
        {
            //
            // figure out how wide the address is going to be.
            //
            address = record.get_address();
            int address_width = 2;
            if (address >= 0x1000000)
                address_width = 4;
            if (address >= 0x10000)
                address_width = 3;
            if (address_width < address_length)
                address_width = address_length;
            address_width *= 2;

            //
            // Throw a new line if the address isn't going to fit on
            // this line.
            //
            if (column + 5 + address_width > line_length)
            {
                put_char('\n');
                column = 0;
            }
            else if (column)
            {
                put_char(' ');
                ++column;
            }

            //
            // Now write out the new address.  It is important not to
            // disturb the checksum, so don't use the put_byte method.
            //
            put_stringf("$A%0*lX,\n", address_width, address);
            column = 0;
        }
        for (size_t j = 0; j < record.get_length(); ++j)
        {
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
            put_byte(record.get_data(j));
            ++address;
            column += 2;
        }
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        emit_end_of_file();
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_ascii_hex::line_length_set(int linlen)
{
    int n = (linlen + 1) / 3;
    if (n < 1)
        n = 1;
    if (n > srecord::record::max_data_length)
        n = srecord::record::max_data_length;
    pref_block_size = n;
    line_length = pref_block_size * 3 - 1;
}


bool
srecord::output_file_ascii_hex::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > srecord::record::max_data_length)
        return false;
    pref_block_size = nbytes;
    line_length = pref_block_size * 3 - 1;
    return true;
}


void
srecord::output_file_ascii_hex::address_length_set(int n)
{
    if (n < 2)
        n = 2;
    if (n > 4)
        n = 4;
    address_length = n;
}


int
srecord::output_file_ascii_hex::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srecord::output_file_ascii_hex::format_name()
    const
{
    return "Ascii-Hex";
}
