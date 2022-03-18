//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2006-2010 Peter Miller
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

#include <srecord/output/file/fastload.h>
#include <srecord/record.h>


srecord::output_file_fastload::~output_file_fastload()
{
    if (bytes_since_checksum)
    {
        put_command('C', checksum_get16(), 3);
        bytes_since_checksum = 0;
    }
    if (enable_footer_flag)
        put_command('E', 0, 2);
    if (column)
    {
        put_char('\n');
        column = 0;
    }
}


srecord::output_file_fastload::output_file_fastload(
    const std::string &a_filename
) :
    srecord::output_file(a_filename),
    line_length(0),
    address(~0uL),
    column(0),
    bytes_since_checksum(0),
    max_since_checksum(0),
    prev_was_command(false)
{
    line_length_set(80);
}


srecord::output::pointer
srecord::output_file_fastload::create(const std::string &a_filename)
{
    return pointer(new srecord::output_file_fastload(a_filename));
}


void
srecord::output_file_fastload::put_number(unsigned long n, int min_digits)
{
    unsigned char buffer[20];
    unsigned char *bp = buffer;
    while (n || min_digits > 0)
    {
        *bp++ = n & 63;
        --min_digits;
        n >>= 6;
    }

    static char digit[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqr"
        "stuvwxyz0123456789,.";
    while (bp > buffer)
    {
        put_char(digit[*--bp]);
        ++column;
    }
}


static int
number_width(unsigned long n)
{
    int result = 0;
    while (n)
    {
        result++;
        n >>= 6;
    }
    return result;
}


void
srecord::output_file_fastload::put_command(int c, unsigned long n, int ndigits)
{
    int width = number_width(n);
    if (width < ndigits)
        width = ndigits;
    width += 2;
    if (column + width > line_length)
    {
        put_char('\n');
        column = 0;
    }

    put_char('/');
    put_char(c);
    column += 2;
    put_number(n, ndigits);
    prev_was_command = true;
}


void
srecord::output_file_fastload::write(const srecord::record &record)
{
    size_t j;

    switch (record.get_type())
    {
    case srecord::record::type_header:
        // This format can't do header records
        if (!enable_optional_address_flag)
            address = (unsigned long)-1L;
        break;

    case srecord::record::type_data:
        if (record.get_length() < 1)
            return;
        if (record.get_address() != address)
        {
            address = record.get_address();
            put_command('A', address, 3);
        }
        if (record.is_all_zero())
        {
            // These bytes don't add anything to the checksum.
            put_command('Z', record.get_length(), 2);
            address += record.get_length();
            break;
        }
        for (j = 0; j + 3 <= record.get_length(); j += 3)
        {
            if (bytes_since_checksum >= max_since_checksum)
            {
                put_command('C', checksum_get16(), 3);
                put_command('K', 0, 2);
                checksum_reset();
                bytes_since_checksum = 0;
            }
            unsigned char n1 = record.get_data(j);
            checksum_add(n1);
            unsigned char n2 = record.get_data(j + 1);
            checksum_add(n2);
            unsigned char n3 = record.get_data(j + 2);
            checksum_add(n3);
            unsigned long n = (n1 << 16) | (n2 << 8) | n3;
            if (column + 4 > line_length || prev_was_command)
            {
                put_char('\n');
                column = 0;
            }
            put_number(n, 4);
            bytes_since_checksum += 3;
            prev_was_command = false;
        }
        for (; j < record.get_length(); ++j)
        {
            unsigned char n = record.get_data(j);
            checksum_add(n);
            put_command('B', (unsigned long)n, 2);
            bytes_since_checksum++;
        }
        address += record.get_length();
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            if (bytes_since_checksum)
            {
                put_command('C', checksum_get16(), 3);
                bytes_since_checksum = 0;
            }
            if (address != record.get_address())
            {
                address = record.get_address();
                put_command('A', address, 3);
            }
        }
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_fastload::line_length_set(int linlen)
{
    line_length = linlen;
    if (line_length < 10)
        line_length = 10;

    // Don't go bigger than this, or you get undetectable errors.
    enum { MAX = 256 };

    int bytes_on_last_line = ((line_length - 9) / 4) * 3;
    if (bytes_on_last_line > MAX)
        bytes_on_last_line = MAX;
    else if (bytes_on_last_line < 0)
        bytes_on_last_line = 0;

    int bytes_on_other_lines = (line_length / 4) * 3;
    if (bytes_on_other_lines > MAX)
        bytes_on_other_lines = MAX;
    else if (bytes_on_other_lines < 1)
        bytes_on_other_lines = 1;

    int num_other_lines =
        (MAX - bytes_on_last_line) / bytes_on_other_lines;

    max_since_checksum =
        num_other_lines * bytes_on_other_lines + bytes_on_last_line;
}


void
srecord::output_file_fastload::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_fastload::preferred_block_size_set(int nbytes)
{
    if (nbytes > srecord::record::max_data_length)
        return false;

    // Don't go bigger than this, or you get undetectable errors.
    enum { MAX = 256 };

    int ll = (nbytes / 3) * 4;

    int bytes_on_last_line = ((ll - 9) / 4) * 3;
    if (bytes_on_last_line > MAX)
        return false;
    else if (bytes_on_last_line < 0)
        return false;

    int bytes_on_other_lines = (ll / 4) * 3;
    if (bytes_on_other_lines > MAX)
        return false;
    else if (bytes_on_other_lines < 1)
        return false;

    int num_other_lines =
        (MAX - bytes_on_last_line) / bytes_on_other_lines;

    max_since_checksum =
        num_other_lines * bytes_on_other_lines + bytes_on_last_line;

    line_length = ll;
    return true;
}


int
srecord::output_file_fastload::preferred_block_size_get()
    const
{
    // Prefer a multiple of 3
    return ((srecord::record::max_data_length / 3) * 3);
}


const char *
srecord::output_file_fastload::format_name()
    const
{
    return "FastLoad";
}
