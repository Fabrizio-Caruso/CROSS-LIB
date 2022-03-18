//
// srecord - manipulate eprom load files
// Copyright (C) 2004, 2006-2012 Peter Miller
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

#include <srecord/output/file/aomf.h>
#include <srecord/record.h>


srecord::output_file_aomf::~output_file_aomf()
{
}


srecord::output_file_aomf::output_file_aomf(const std::string &a_file_name) :
    srecord::output_file(a_file_name)
{
    if (line_termination == line_termination_native)
        line_termination = line_termination_binary;
}


srecord::output::pointer
srecord::output_file_aomf::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_aomf(a_file_name));
}


void
srecord::output_file_aomf::emit_record(int type, const unsigned char *data,
    size_t length)
{
    checksum_reset();
    put_byte(type);
    put_word_le(length + 1);
    for (size_t j = 0; j < length; ++j)
        put_byte(data[j]);
    put_byte(-checksum_get());
}


void
srecord::output_file_aomf::module_header_record(const char *name)
{
    //
    // The TRN ID describes the system which created the file, and (by
    // implication) the CPU type is 8051.  It isn't much good to us,
    // because it doesn't admit that non-Intel systems or other CPUs exist.
    //     0xFD -> ASM51
    //     0xFE -> PL/M-51
    //     0xFF -> RL51
    //
    unsigned char buffer[1 + 255 + 2];
    size_t len = strlen(name);
    if (len > 255)
        len = 255;
    buffer[0] = len;
    memcpy(buffer + 1, name, len);
    buffer[len + 1] = 0; // TRN ID
    buffer[len + 2] = 0; // must be zero
    emit_record(0x02, buffer, len + 3);
}


void
srecord::output_file_aomf::content_record(unsigned long address,
    const unsigned char *data, size_t len)
{
    size_t maxlen = 4 * srecord::record::max_data_length;
    while (len > 0)
    {
        unsigned char buffer[maxlen + 3];
        buffer[0] = address >> 16; // this byte should be zero ;-)
        buffer[1] = address; // "offset" is little-endian
        buffer[2] = address >> 8;
        int nbytes = (len > maxlen ? maxlen : len);
        memcpy(buffer + 3, data, nbytes);
        emit_record(0x06, buffer, len + 3);
        address += nbytes;
        data += nbytes;
        len -= nbytes;
    }
}


void
srecord::output_file_aomf::module_end_record(const char *name)
{
    unsigned char buffer[1 + 255 + 4];
    size_t len = strlen(name);
    if (len > 255)
        len = 255;
    buffer[0] = len;
    memcpy(buffer + 1, name, len);
    buffer[len + 1] = 0; // must be zero
    buffer[len + 2] = 0; // must be zero
    buffer[len + 3] = 0; // REG MSK
    buffer[len + 4] = 0; // must be zero
    emit_record(0x04, buffer, len + 5);
}


void
srecord::output_file_aomf::put_byte(unsigned char n)
{
    checksum_add(n);
    put_char(n);
    byte_offset++;
}


void
srecord::output_file_aomf::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        module_name.assign((const char *)record.get_data(),
            record.get_length());
        module_header_record(module_name.c_str());
        break;

    case srecord::record::type_data:
        if (record.get_length() < 1)
            return;
        if (!record.address_range_fits_into_n_bits(24))
            data_address_too_large(record, 24);

        //
        // Write the data out.
        //
        content_record
        (
            record.get_address(),
            record.get_data(),
            record.get_length()
        );
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        module_end_record(module_name.c_str());
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_aomf::line_length_set(int)
{
    // Ignore.
}


void
srecord::output_file_aomf::address_length_set(int)
{
    // Ignore (this is only a 16-bit format).
}


bool
srecord::output_file_aomf::preferred_block_size_set(int)
{
    // Irrelevant.
    return true;
}


int
srecord::output_file_aomf::preferred_block_size_get()
    const
{
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_aomf::format_name()
    const
{
    return "AOMF";
}


// vim: set ts=8 sw=4 et :
