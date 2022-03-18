//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009, 2010, 2012, 2014 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
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

#include <srecord/progname.h>
#include <srecord/arglex/tool.h>
#include <srecord/output/file/mem.h>
#include <srecord/record.h>


srecord::output_file_mem::~output_file_mem()
{
    emit_header();
    if (column)
        put_char('\n');
    if (enable_header_flag && actual_depth != depth)
        put_stringf("#Depth=%lu;\n", actual_depth / width_in_bytes);
}


srecord::output_file_mem::output_file_mem(const std::string &a_file_name) :
    srecord::output_file(a_file_name),
    address(0),
    column(0),
    depth(0),
    width(8),
    width_in_bytes(1),
    actual_depth(0),
    header_done(false),
    pref_blk_sz(32)
{
    line_length_set(80);
}


srecord::output::pointer
srecord::output_file_mem::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_mem(a_file_name));
}


void
srecord::output_file_mem::command_line(srecord::arglex_tool *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
        width = cmdln->value_number();
        cmdln->token_next();
        switch (width)
        {
        case 1:
        case 8:
            width = 8; // ambiguous
            width_in_bytes = 1;
            break;

        case 2:
        case 16:
            width = 16;
            width_in_bytes = 2;
            break;

        case 4:
        case 32:
            width = 32;
            width_in_bytes = 4;
            break;

        case 64:
            width = 64;
            width_in_bytes = 8; // ambiguous
            break;

        default:
            fatal_error("-mem address multiple %d not understood", width);
        }
        line_length_set(80);
    }
}


void
srecord::output_file_mem::notify_upper_bound(unsigned long addr)
{
    depth = addr;
    actual_depth = addr;
}


void
srecord::output_file_mem::emit_header(void)
{
    if (header_done)
        return;
    if (enable_header_flag)
    {
        if (column != 0)
        {
            put_char('\n');
            column = 0;
        }
        put_stringf("#Format=Hex\n"); // Bin | Hex | AddrHex
        if (actual_depth != 0)
        {
            put_stringf("#Depth=%lu\n", actual_depth / width_in_bytes);
        }
        put_stringf("#Width=%d\n", width);
        // 0: binary, 1: octal, 3: decimal, 3: hexadecimal
        put_stringf("#AddrRadix=3\n");
        put_stringf("#DataRadix=3\n");
        put_stringf("#Data\n");

        // Comments may be added to any of the data, but never add
        // comments to the header.  The header ends here, so it is safe
        // to add some additional information.
        //
        // Note that some widths are ambiguous
        //    8 bits, 1 byte
        //   16 bits, 2 bytes
        //   32 bits, 4 bytes
        //   64 bits, 8 bytes
        // thus "8" is ambiguous
        put_stringf
        (
            "#\n"
            "# Generated automatically by %s -o --MEM %d\n"
            "#\n",
            progname_get(),
            (width == 8 ? 64 : width)
        );
    }
    header_done = true;
}


void
srecord::output_file_mem::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_unknown:
        // Ignore.
        break;

    case srecord::record::type_header:
        if (enable_header_flag && record.get_length() > 0)
        {
            // Output the header record as a comment.
            emit_header();

            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
                if (c == '\n')
                {
                    if (column == 0)
                        put_char('#');
                    put_char('\n');
                    column = 0;
                    continue;
                }
                if (!isprint(c))
                    c = ' ';
                if (column == 0)
                {
                    put_string("# ");
                    column = 2;
                    if (record.get_address() != 0)
                    {
                        unsigned long addr = record.get_address();
                        put_stringf("%04lX: ", addr);
                        column += 6;
                    }
                }
                put_char(c);
                ++column;
            }
            if (column > 0)
            {
                put_char('\n');
                column = 0;
            }
        }
        break;

    case srecord::record::type_data:
        {
            unsigned long addr = record.get_address();
            unsigned len = record.get_length();
            if (address != addr)
                fatal_hole_error(address, addr);
            if ((addr % width_in_bytes) || (len % width_in_bytes))
                fatal_alignment_error(width_in_bytes);
            emit_header();

            for (unsigned j = 0; j < len; ++j)
            {
                if (column > 0 && (j % width_in_bytes) == 0)
                {
                    put_char('\n');
                    column = 0;
                }
                put_byte(record.get_data(j));
                column += 2;

                if
                (
                    ((j + 1) % width_in_bytes) == 0
                ||
                    column + 1 + width_in_bytes * 2 > 80
                )
                {
                    put_char('\n');
                    column = 0;
                }
            }

            address = addr + len;
            if (actual_depth < address)
                actual_depth = address;
        }
        break;

    case srecord::record::type_data_count:
        if (enable_data_count_flag)
        {
            if (column > 0)
            {
                put_char('\n');
                column = 0;
            }
            unsigned long addr = record.get_address();
            put_stringf("# data record count = %lu\n", addr);
        }
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            if (column > 0)
            {
                put_char('\n');
                column = 0;
            }
            unsigned long addr = record.get_address();
            put_stringf("# execution start address = %04lX\n", addr);
        }
        break;
    }
}


void
srecord::output_file_mem::line_length_set(int len)
{
    int pref_mult = (len - 6) / (1 + 2 * width_in_bytes);
    if (pref_mult < 1)
        pref_mult = 1;
    pref_blk_sz = pref_mult * width_in_bytes;
}


void
srecord::output_file_mem::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_mem::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    if (nbytes % width_in_bytes)
        return false;
    pref_blk_sz = nbytes;
    return true;
}


int
srecord::output_file_mem::preferred_block_size_get(void)
    const
{
    return pref_blk_sz;
}


const char *
srecord::output_file_mem::format_name(void)
    const
{
    return "Lattice Memory Initialization Format (.mem)";
}


// vim: set ts=8 sw=4 et :
