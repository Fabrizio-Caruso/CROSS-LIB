//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2003, 2005-2008, 2010 Peter Miller
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

#include <srecord/arglex/tool.h>
#include <srecord/output/file/motorola.h>
#include <srecord/record.h>


srecord::output_file_motorola::~output_file_motorola()
{
    write_data_count();
    // check for termination record
}


srecord::output_file_motorola::output_file_motorola(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    data_count(0),
    pref_block_size(32),
    address_length(2),
    address_shift(0),
    data_count_written(false)
{
}


srecord::output::pointer
srecord::output_file_motorola::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_motorola(a_file_name));
}


void
srecord::output_file_motorola::command_line(srecord::arglex_tool *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
        int n = cmdln->value_number();
        cmdln->token_next();

        //
        // Shift Bytes Bits
        //   0     1     8
        //   1     2    16
        //   2     4    32
        //   3     8    64
        //
        switch (n)
        {
        case 1:
        case 8:
            address_shift = 0;
            break;

        case 2:
        case 16:
            address_shift = 1;
            break;

        case 4:
        case 32:
            address_shift = 2;
            break;

        case 64:
            address_shift = 3;
            break;

        default:
            fatal_error("-motorola address multiple %d not understood", n);
            // NOTREACHED
        }
    }
}


void
srecord::output_file_motorola::write_inner(int tag, unsigned long address,
    int address_nbytes, const void *data, int data_nbytes)
{
    //
    // Make sure the line is not too long.
    //
    if (address_nbytes + data_nbytes > 254)
    {
        fatal_error
        (
            "data length (%d+%d) too long",
            address_nbytes,
            data_nbytes
        );
    }

    //
    // Assemble the data for this line.
    //
    unsigned char buffer[256];
    int line_length = address_nbytes + data_nbytes + 1;
    buffer[0] = line_length;
    srecord::record::encode_big_endian(buffer + 1, address, address_nbytes);
    if (data_nbytes)
        memcpy(buffer + 1 + address_nbytes, data, data_nbytes);

    //
    // Emit the line as hexadecimal text.
    //
    put_char('S');
    put_nibble(tag);
    checksum_reset();
    for (int j = 0; j < line_length; ++j)
        put_byte(buffer[j]);
    put_byte(~checksum_get());
    put_char('\n');
}


void
srecord::output_file_motorola::write_data_count()
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
srecord::output_file_motorola::write(const srecord::record &record)
{
    //
    // Make sure the address is nicely aligned.
    // FIXME: cope with this more elegantly.
    //
    unsigned long shifted_address = record.get_address();
    if (address_shift)
    {
        if (shifted_address & ((1 << address_shift) - 1))
        {
            fatal_error
            (
                "address 0x%04lX not aligned on %d byte boundary",
                shifted_address,
                (1 << address_shift)
            );
        }
        shifted_address >>= address_shift;
    }

    switch (record.get_type())
    {
    case srecord::record::type_header:
        if (enable_header_flag)
            write_inner(0, 0, 2, record.get_data(), record.get_length());
        break;

    case srecord::record::type_data:
        if (shifted_address < (1UL << 16) && address_length <= 2)
        {
            write_inner
            (
                1,
                shifted_address,
                2,
                record.get_data(),
                record.get_length()
            );
        }
        else if (shifted_address < (1UL << 24) && address_length <= 3)
        {
            write_inner
            (
                2,
                shifted_address,
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
                shifted_address,
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

            if (shifted_address < (1UL << 16) && address_length <= 2)
                write_inner(9, shifted_address, 2, 0, 0);
            else if (shifted_address < (1UL << 24) && address_length <= 3)
                write_inner(8, shifted_address, 3, 0, 0);
            else
                write_inner(7, shifted_address, 4, 0, 0);
        }
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_motorola::line_length_set(int linlen)
{
    //
    // Given the number of characters, figure the maximum number of
    // data baytes.
    // 'S' <tag> <size1:2> <addr1:8> ...data... <cs1:2>
    // 1 +  1 +  2 +       8 +       2*n +      2       <= linlen
    //
    int n = (linlen - 14) / 2;

    //
    // Constrain based on the file format.
    //
    // The size field (max 255) includes the size of the data,
    // the size of the address (up to 4 bytes) and the size of the
    // size (1 byte), thus 250 (255 - 4 - 1) bytes of data is
    // the safest maximum.      We could make it based on the address,
    // but that's probably overkill.
    //
    if (n < 1)
        n = 1;
    else if (n > 250)
        n = 250;

    //
    // An additional constraint is the size of the srecord::record
    // data buffer.
    //
    if (n > srecord::record::max_data_length)
        n = srecord::record::max_data_length;
    pref_block_size = n;
}


void
srecord::output_file_motorola::address_length_set(int n)
{
    if (n < 2)
        n = 2;
    else if (n > 4)
        n = 4;
    address_length = n;
}


bool
srecord::output_file_motorola::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_motorola::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srecord::output_file_motorola::format_name()
    const
{
    return "Motorola S-Record";
}
