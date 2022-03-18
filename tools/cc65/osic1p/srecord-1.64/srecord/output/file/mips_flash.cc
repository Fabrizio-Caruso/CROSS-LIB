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

#include <srecord/output/file/mips_flash.h>
#include <srecord/record.h>
#include <srecord/sizeof.h>


srecord::output_file_mips_flash::~output_file_mips_flash()
{
    buffer_flush_newline();

    if (!base_set)
        fatal_error("no data written");
    if (base == 0x1FC00000)
    {
        // Lock the flash.
        put_string(">LOCKFLSH\n");
        put_string("@1fc00000 !S\n");
        put_string("@1fc20000 !S\n");
        put_string("@1fc40000 !S\n");
        put_string("@1fc60000 !S\n");
        put_string("@1fc80000 !S\n");
        put_string("@1fca0000 !S\n");
        put_string("@1fcc0000 !S\n");
        put_string("@1fce0000 !S\n");
    }
    put_string(">#DL_DONE\n");
    put_string(">FINISHED\n");
}


srecord::output_file_mips_flash::output_file_mips_flash(
    const std::string &a_filename,
    endian_t a_endian
) :
    srecord::output_file(a_filename),
    endian(a_endian),
    address(0),
    base(0),
    base_set(false),
    buffer_length(0),
    line_length(80),
    column(0)
{
}


srecord::output::pointer
srecord::output_file_mips_flash::create_le(const std::string &a_file_name)
{
    return
        pointer
        (
            new srecord::output_file_mips_flash(a_file_name, endian_little)
        );
}


srecord::output::pointer
srecord::output_file_mips_flash::create_be(const std::string &a_file_name)
{
    return
        pointer
        (
            new srecord::output_file_mips_flash(a_file_name, endian_big)
        );
}


void
srecord::output_file_mips_flash::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_header:
        // This format can't do header records
        break;

    case srecord::record::type_data:
        if (record.get_length() < 1)
            return;
        for (size_t j = 0; j < record.get_length(); ++j)
        {
            unsigned long byte_address = record.get_address() + j;
            unsigned char byte = record.get_data(j);
            bool new_sector =
                !base_set || ((address >> 17) != (byte_address >> 17));
            bool new_address = !base_set || (address != byte_address);

            if (new_address || new_sector)
            {
                buffer_flush_newline();
                address = byte_address;
            }

            if (!base_set)
            {
                address = byte_address;
                base = address & ~0x1FFFF;
                base_set = true;

                // Reset the loader state machine.
                put_string("!R\n");

                // Remember the base address.
                if (base == 0x1FC00000)
                {
                    // Unlock the flash.
                    put_string(">1fc00xxx @1fc00000 !C\n");
                }
            }

            if (new_sector)
            {
                // Started a new sector, by erasing it.
                unsigned long sbase = address & ~0x1FFFF;
                put_stringf(">%.5lxxxx ", sbase >> 12);
                put_stringf("@%.8lx !E\n", sbase);
                if (sbase != address)
                    new_address = true;
            }

            if (new_address)
            {
                // If the address is not 4-byte aligned:
                // complain, and suggest they use a fill filter.
                address = byte_address;
                if ((address & 3) != 0)
                    fatal_alignment_error(4);

                // Set new write address.
                put_stringf("@%.8lx\n", address);
            }

            if (new_address || new_sector || (address & 0x0FFF) == 0)
            {
                // Update the display.
                put_stringf(">%.8lx\n", address);
            }

            ++address;
            buffer[buffer_length++] = byte;
            if (buffer_length >= sizeof(buffer))
                buffer_flush();
        }
        break;

    case srecord::record::type_data_count:
        // ignore, this is the input count, not the output count
        break;

    case srecord::record::type_execution_start_address:
        // ignore, this format doesn't have it
        break;

    case srecord::record::type_unknown:
        fatal_error("can't write unknown record type");
    }
}


void
srecord::output_file_mips_flash::buffer_flush_newline(void)
{
    buffer_flush();
    if (column)
    {
        put_char('\n');
        column = 0;
    }
}


void
srecord::output_file_mips_flash::buffer_flush(void)
{
    if (!buffer_length)
        return;
    if ((buffer_length & 3) != 0)
        fatal_alignment_error(4);
    const unsigned char *end = buffer + buffer_length;
    for (const unsigned char *bp = buffer; bp < end; bp += 4)
    {
        if (column)
        {
            if (column + 9 > line_length)
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
        if (endian == endian_big)
        {
            put_byte(bp[0]);
            put_byte(bp[1]);
            put_byte(bp[2]);
            put_byte(bp[3]);
        }
        else
        {
            put_byte(bp[3]);
            put_byte(bp[2]);
            put_byte(bp[1]);
            put_byte(bp[0]);
        }
        column += 8;
    }
    buffer_length = 0;
}


void
srecord::output_file_mips_flash::line_length_set(int w)
{
    // ignore (fixed line length)
    if (w > 0)
        line_length = w;
}


void
srecord::output_file_mips_flash::address_length_set(int)
{
    // ignore (this is only a 32-bit format)
}


int
srecord::output_file_mips_flash::preferred_block_size_get(void)
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    // But make sure it is an multiple of 4 bytes long.
    //
    return (srecord::record::max_data_length & ~3);
}


bool
srecord::output_file_mips_flash::preferred_block_size_set(int n)
{
    // Irrelevant.
    return ((n & 3) == 0);
}


const char *
srecord::output_file_mips_flash::format_name(void)
    const
{
    return
        (
            endian == endian_big
        ?
            "MIPS-Flash (big-endian)"
        :
            "MIPS-Flash (little-endian)"
        );
}


// vim: set ts=8 sw=4 et :
