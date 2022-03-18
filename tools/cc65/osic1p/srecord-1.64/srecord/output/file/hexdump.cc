//
// srecord - The "srecord" program.
// Copyright (C) 2007-2011 Peter Miller
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

#include <cstring>

#include <srecord/output/file/hexdump.h>
#include <srecord/record.h>


srecord::output_file_hexdump::~output_file_hexdump()
{
    row_cache_print();
    delete [] row_cache;
}


srecord::output_file_hexdump::output_file_hexdump(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    number_of_columns(0),
    row_cache_address((unsigned long)-1),
    row_cache_address_mask(0),
    row_cache_size(0),
    row_cache(0),
    address_length(4)
{
    line_length_set(80);
}


srecord::output::pointer
srecord::output_file_hexdump::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_hexdump(a_file_name));
}


void
srecord::output_file_hexdump::command_line(srecord::arglex_tool *)
{
    // nothing (yet?)
}


void
srecord::output_file_hexdump::row_cache_print(void)
{
    if (row_cache_address == (unsigned long)(-1))
        return;
    char *cp = row_cache;
    char *ep = cp + row_cache_size;
    while (ep > cp && ep[-1] == ' ')
        --ep;
    while (cp < ep)
        put_char(*cp++);
    put_char('\n');
    memset(row_cache, ' ', row_cache_size);
    row_cache_address = (unsigned long)-1;
}


static char
hex_nybble(int n)
{
    return "0123456789ABCDEF"[n & 15];
}


static void
hex_byte(char *buffer, int n)
{
    buffer[0] = hex_nybble(n >> 4);
    buffer[1] = hex_nybble(n);
}


static void
hex(char *buffer, long value, int nbytes)
{
    buffer += nbytes * 2;
    while (nbytes > 0)
    {
        buffer -= 2;
        hex_byte(buffer, value);
        value >>= 8;
        --nbytes;
    }
}


void
srecord::output_file_hexdump::emit_byte(unsigned long address,
    unsigned char data)
{
    if (row_cache_address != (unsigned long)(-1))
    {
        if (row_cache_address != (address & ~row_cache_address_mask))
        {
            // need to start a new row
            row_cache_print();
        }
    }
    if (row_cache_address == (unsigned long)(-1))
    {
        row_cache_address = address & ~row_cache_address_mask;
        hex(row_cache, row_cache_address, address_length);
        row_cache[address_length * 2] = ':';
        row_cache[address_length * 2 + 3 + 3 * number_of_columns] = '#';
    }
    address &= row_cache_address_mask;
    hex_byte(row_cache + address_length * 2 + 2 + 3 * address, data);
    data &= 0x7F;
    if (data < ' ' || data > '~')
        data = '.';
    row_cache[address_length * 2 + 4 + 3 * number_of_columns + address] = data;
}


void
srecord::output_file_hexdump::write(const srecord::record &record)
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
        {
            unsigned long a = record.get_address();
            for (size_t j = 0; j < record.get_length(); ++j)
                emit_byte(a + j, record.get_data(j));
        }
        break;

    case srecord::record::type_execution_start_address:
        // ignore
        break;
    }
}


int
srecord::output_file_hexdump::columns_to_line_length(int cols)
{
    // 0000: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
    //      ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^^
    return (address_length * 2 + 4 + 4 * cols);
}


void
srecord::output_file_hexdump::line_length_set(int line_length)
{
    int n = 8;
    while (columns_to_line_length(n << 1) <= line_length)
        n <<= 1;
    number_of_columns = n;
    delete [] row_cache;
    row_cache_size = columns_to_line_length(number_of_columns);
    row_cache = new char [row_cache_size];
    memset(row_cache, ' ', row_cache_size);
    row_cache_address_mask = number_of_columns - 1;
}


bool
srecord::output_file_hexdump::preferred_block_size_set(int nbytes)
{
    if (nbytes < 2 || nbytes > record::max_data_length)
        return false;
    if ((nbytes & -nbytes) != nbytes)
        return false;
    number_of_columns = nbytes;
    row_cache_address_mask = number_of_columns - 1;
    return true;
}


void
srecord::output_file_hexdump::address_length_set(int)
{
    // Ignore
}


int
srecord::output_file_hexdump::preferred_block_size_get(void)
    const
{
    //
    // Use the largest we can get,
    //
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_hexdump::format_name(void)
    const
{
    return "hexdump";
}
