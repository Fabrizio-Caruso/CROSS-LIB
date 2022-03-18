//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011 Peter Miller
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

#include <srecord/output/file/ppb.h>
#include <srecord/record.h>


srecord::output_file_ppb::~output_file_ppb()
{
    if (!seen_some_data)
        fatal_error("no data records");
    if (buffer_length > 0)
        buffer_flush();
    packet(0, 0, 0);
}


srecord::output_file_ppb::output_file_ppb(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    address(-1uL),
    buffer_length(0),
    seen_some_data(false)
{
}


srecord::output::pointer
srecord::output_file_ppb::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_ppb(a_file_name));
}


void
srecord::output_file_ppb::write(const srecord::record &record)
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
        for (size_t j = 0; j < record.get_length(); ++j)
        {
            unsigned char data = record.get_data(j);
            unsigned long data_address = record.get_address() + j;

            if (data_address != address)
            {
                buffer_flush();
                address = data_address;
            }
            buffer[buffer_length++] = data;
            ++address;
            if (buffer_length >= sizeof(buffer))
                buffer_flush();
            seen_some_data = true;
        }
        break;

    case srecord::record::type_execution_start_address:
        // ignore
        break;
    }
}


void
srecord::output_file_ppb::line_length_set(int)
{
    // Ignore.
}


bool
srecord::output_file_ppb::preferred_block_size_set(int nbytes)
{
    return (nbytes >= 2 && nbytes <= record::max_data_length);
}


void
srecord::output_file_ppb::address_length_set(int)
{
    // Ignore
}


int
srecord::output_file_ppb::preferred_block_size_get(void)
    const
{
    // Use the largest we can get.
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_ppb::format_name(void)
    const
{
    return "Stag Prom Programmer Binary";
}


void
srecord::output_file_ppb::put_bin_4be(unsigned long value)
{
    put_char(value >> 24);
    put_char(value >> 16);
    put_char(value >> 8);
    put_char(value);
}


void
srecord::output_file_ppb::packet(unsigned long address,
    const unsigned char *data, size_t data_size)
{
    enum { SOH = 1 };
    enum { CSLEN = 1024 };

    put_char(SOH);
    put_bin_4be(data_size);
    put_bin_4be(address);
    unsigned char chksum = 0;
    for (size_t j = 0; j < data_size; ++j)
    {
        if (j > 0 && (j % CSLEN) == 0)
            put_char(-chksum);
        put_char(data[j]);
        chksum += data[j];
    }
    put_char(-chksum);
}


void
srecord::output_file_ppb::buffer_flush(void)
{
    if (buffer_length > 0)
    {
        packet(address - buffer_length, buffer, buffer_length);
        buffer_length = 0;
    }
}


bool
srecord::output_file_ppb::is_binary(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :
