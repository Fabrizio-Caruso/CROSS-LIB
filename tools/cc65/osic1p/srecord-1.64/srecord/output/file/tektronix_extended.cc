//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2008, 2010, 2012, 2013 Peter Miller
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

#include <srecord/output/file/tektronix_extended.h>
#include <srecord/record.h>


srecord::output_file_tektronix_extended::~output_file_tektronix_extended()
{
    // check for termination record
}


srecord::output_file_tektronix_extended::output_file_tektronix_extended(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    pref_block_size(32),
    address_length(4)
{
}


srecord::output::pointer
srecord::output_file_tektronix_extended::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_tektronix_extended(a_file_name));
}


void
srecord::output_file_tektronix_extended::write_inner(int tag,
    unsigned long addr, int addr_nbytes, const void *data_p, int data_nbytes)
{
    if (addr_nbytes < address_length)
        addr_nbytes = address_length;
    unsigned char buf[260];
    int record_length = 6 + (addr_nbytes + data_nbytes) * 2;
    if (record_length >= 256)
    {
        fatal_error
        (
            "record too long (%d > 255, dmax=%d)",
            record_length,
            (125 - 2 * addr_nbytes)/2
        );
    }
    int csum = 0;
    int pos = 0;
    csum += buf[pos++] = 0; // lengh hi, fill in later
    csum += buf[pos++] = 0; // lengh lo, fill in later
    csum += buf[pos++] = tag;
    csum += buf[pos++] = 0; // checksum hi, fill in later
    csum += buf[pos++] = 0; // checksum lo, fill in later
    csum += buf[pos++] = addr_nbytes * 2;  // size of addr, in nybbles
    int j;
    for (j = 0; j < 2 * addr_nbytes; ++j)
        csum += buf[pos++] = (addr >> (4 * (2*addr_nbytes-1 - j))) & 15;
    const unsigned char *data = (const unsigned char *)data_p;
    for (j = 0; j < data_nbytes; ++j)
    {
        csum += buf[pos++] = (data[j] >> 4) & 15;
        csum += buf[pos++] =  data[j]       & 15;
    }

    // now insert the record length
    csum += buf[0] = (pos >> 4) & 15;
    csum += buf[1] =  pos       & 15;

    // now insert the checksum...
    buf[3] = (csum >> 4) & 15;
    buf[4] =  csum       & 15;

    // emit the line
    put_char('%');
    for (j = 0; j < pos; ++j)
        put_nibble(buf[j]);
    put_char('\n');
}


static int
addr_width(unsigned long n)
{
    if (n < (1uL << 16))
        return 2;
    if (n < (1uL << 24))
        return 3;
    return 4;
}


void
srecord::output_file_tektronix_extended::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // This format can't do header records
        break;

    case srecord::record::type_data:
        write_inner
        (
            6,
            record.get_address(),
            addr_width(record.get_address()),
            record.get_data(),
            record.get_length()
        );
        break;

    case srecord::record::type_data_count:
        // ignore
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            write_inner
            (
                8,
                record.get_address(),
                addr_width(record.get_address()),
                0,
                0
            );
        }
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_tektronix_extended::line_length_set(int linlen)
{
    //
    // Given the number of characters, figure the maximum number of
    // data baytes.
    //
    int n = (linlen - 15) / 2;

    //
    // Constrain based on the file format.
    //
    // The size field (max 255 nibbles) includes the size of the
    // data and the size of the address (up to 9 nibbles).  Thus 123
    // ((255 - 9)/2) bytes of data is the safest maximum.  We could
    // make it based on the address, but that's probably overkill.
    //
    if (n < 1)
        n = 1;
    else if (n > 123)
        n = 123;

    //
    // An additional constraint is the size of the srecord::record
    // data buffer.
    //
    if (n > srecord::record::max_data_length)
        n = srecord::record::max_data_length;
    pref_block_size = n;
}


void
srecord::output_file_tektronix_extended::address_length_set(int n)
{
    if (n < 2)
        n = 2;
    if (n > 4)
        n = 4;
    address_length = n;
}


bool
srecord::output_file_tektronix_extended::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    if (nbytes > 123)
        return false;
    pref_block_size = nbytes;
    return true;
}


int
srecord::output_file_tektronix_extended::preferred_block_size_get()
    const
{
    return pref_block_size;
}


const char *
srecord::output_file_tektronix_extended::format_name()
    const
{
    return "Tektronix-Extended";
}


// vim: set ts=8 sw=4 et :
