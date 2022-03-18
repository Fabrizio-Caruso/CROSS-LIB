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

#include <srecord/output/file/idt.h>
#include <srecord/record.h>


srecord::output_file_idt::~output_file_idt()
{
    write_data_count();
}


srecord::output_file_idt::output_file_idt(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    data_count(0),
    pref_block_size(128),
    address_length(4),
    data_count_written(false)
{
}


srecord::output::pointer
srecord::output_file_idt::create(const std::string &a_file_name)
{
    return pointer(new output_file_idt(a_file_name));
}


void
srecord::output_file_idt::write_inner(int tag, unsigned long address,
    unsigned address_nbytes, const unsigned char *data, size_t data_nbytes)
{
    //
    // Make sure the line is not too long.
    //
    if (address_nbytes + data_nbytes > 254)
    {
        fatal_error
        (
            "data length (%d+%ld>254) too long",
            address_nbytes,
            (unsigned long)data_nbytes
        );
    }

    // intro
    put_char('S');
    put_nibble(tag);
    unsigned char line_length = address_nbytes + data_nbytes + 1;
    put_char(line_length);
    unsigned char csum = line_length;

    // address
    for (unsigned j = 0; j < address_nbytes; ++j)
    {
        unsigned char c = address >> (8 * (address_nbytes - 1 - j));
        put_char(c);
        csum += c;
    }

    // data
    for (unsigned j = 0; j < data_nbytes; ++j)
    {
        unsigned char c = data[j];
        put_char(c);
        csum += c;
    }

    // checksum
    put_char(~csum);
}


void
srecord::output_file_idt::write_data_count(void)
{
    if (data_count_written)
        return;

    if (enable_data_count_flag)
    {
        if (data_count < (1L << 16))
            write_inner(5, data_count, 2, 0, 0);
        else
            write_inner(6, data_count, 3, 0, 0);
    }
    data_count_written = true;

    //
    // It is not clear in the spec whether the data count should be
    // reset at this point.  It will not happen for srecord::cat, so it
    // probably isn't a problem.
    //
    data_count = 0;
}


void
srecord::output_file_idt::write(const srecord::record &record)
{
    //
    // Make sure the address is nicely aligned.
    //
    unsigned long addr = record.get_address();

    switch (record.get_type())
    {
    case srecord::record::type_header:
        if (enable_header_flag)
            write_inner(0, 0, 2, record.get_data(), record.get_length());
        break;

    case srecord::record::type_data:
        if (addr < (1uL << 16) && address_length <= 2)
        {
            write_inner
            (
                1,
                addr,
                2,
                record.get_data(),
                record.get_length()
            );
        }
        else if (addr < (1UL << 24) && address_length <= 3)
        {
            write_inner
            (
                2,
                addr,
                3,
                record.get_data(),
                record.get_length()
            );
        }
        else
        {
            write_inner
            (
                3,
                addr,
                4,
                record.get_data(),
                record.get_length()
            );
        }
        ++data_count;
        data_count_written = false;
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            write_data_count();

            if (addr < (1UL << 16) && address_length <= 2)
                write_inner(9, addr, 2, 0, 0);
            else if (addr < (1UL << 24) && address_length <= 3)
                write_inner(8, addr, 3, 0, 0);
            else
                write_inner(7, addr, 4, 0, 0);
        }
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_idt::line_length_set(int)
{
    // This is a binary format.  Ignore.
}


void
srecord::output_file_idt::address_length_set(int n)
{
    if (n < 2)
        n = 2;
    else if (n > 4)
        n = 4;
    address_length = n;
}


bool
srecord::output_file_idt::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length - 4)
        return false;
    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_idt::preferred_block_size_get(void)
    const
{
    return pref_block_size;
}


const char *
srecord::output_file_idt::format_name(void)
    const
{
    return "IDT System Integration Manager binary";
}


bool
srecord::output_file_idt::is_binary(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
