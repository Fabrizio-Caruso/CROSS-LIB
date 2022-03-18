//
// srecord - manipulate eprom load files
// Copyright (C) 2007-2012 Peter Miller
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

#include <srecord/output/file/ti_tagged_16.h>
#include <srecord/record.h>

#include <cctype>


srecord::output_file_ti_tagged_16::~output_file_ti_tagged_16()
{
    if (column)
        put_eoln();
    if (enable_footer_flag)
    {
        put_char(':');
        put_char('\n');
    }
}


srecord::output_file_ti_tagged_16::output_file_ti_tagged_16(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    address(0),
    column(0),
    line_length(74),
    csum(0)
{
}


srecord::output::pointer
srecord::output_file_ti_tagged_16::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_ti_tagged_16(a_file_name));
}


void
srecord::output_file_ti_tagged_16::put_char(int c)
{
    if (c == '\n')
    {
        csum = 0;
        column = 0;
    }
    else
    {
        csum += (unsigned char)c;
        ++column;
    }
    inherited::put_char(c);
}


void
srecord::output_file_ti_tagged_16::put_eoln()
{
    put_char('7');
    put_word_be(-csum);
    put_char('F');
    put_char('\n');
}


void
srecord::output_file_ti_tagged_16::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        if (enable_header_flag)
        {
            put_stringf("K%4.4X", (int)(5 + record.get_length()));
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
                if (!isprint(c))
                    c = ' ';
                put_char(c);
            }
        }
        if (!enable_optional_address_flag)
            address = (unsigned long)-1L;
        break;

    case srecord::record::type_data:
        {
            if (!record.address_range_fits_into_n_bits(17))
                data_address_too_large(record, 17);

            //
            // we can't start at an odd address,
            // but don't test for odd lengths
            //
            if (record.get_address() & 1)
                fatal_alignment_error(2);

            // assert(record.get_length() > 0);
            if (record.get_length() == 0)
                break;
            if (address != record.get_address())
            {
                address = record.get_address();
                if (column + 5 > line_length)
                    put_eoln();
                put_char('9');
                put_word_be(address >> 1);
            }
            size_t pos = 0;
            for (; pos + 2 <= record.get_length(); pos += 2)
            {
                if (column + 5 > line_length)
                    put_eoln();
                put_char('B');
                put_byte(record.get_data(pos));
                put_byte(record.get_data(pos + 1));
                address += 2;
            }
            for (; pos < record.get_length(); ++pos)
            {
                if (column + 3 > line_length)
                    put_eoln();
                put_char('*');
                put_byte(record.get_data(pos));
                ++address;
            }
        }
        break;

    case srecord::record::type_data_count:
    case srecord::record::type_execution_start_address:
        // ignore
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_ti_tagged_16::line_length_set(int linlen)
{
    // reduce the line length by 6 characters, to account for the
    // checksum, so we don't keep subtracting it later.
    line_length = linlen - 6;

    // make sure the line is long enough to do anything useful
    if (line_length < 5)
        line_length = 5;
}


void
srecord::output_file_ti_tagged_16::address_length_set(int)
{
    // ignore (addresses are always 16 bits)
}


bool
srecord::output_file_ti_tagged_16::preferred_block_size_set(int nbytes)
{
    if (nbytes < 2 || nbytes > record::max_data_length)
        return false;
    if (nbytes & 1)
        return false;
    line_length = 5 * (nbytes / 2);
    return true;
}


int
srecord::output_file_ti_tagged_16::preferred_block_size_get()
    const
{
    int n = (line_length / 5) * 2;
    if (n < 2)
        n = 2;
    if (n > srecord::record::max_data_length)
        n = srecord::record::max_data_length;
    return (n & ~1);
}


const char *
srecord::output_file_ti_tagged_16::format_name()
    const
{
    return "TI-Tagged-16";
}


// vim: set ts=8 sw=4 et :
