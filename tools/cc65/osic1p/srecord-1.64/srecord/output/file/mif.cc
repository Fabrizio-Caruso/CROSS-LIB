//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009, 2010 Peter Miller
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
#include <srecord/output/file/mif.h>
#include <srecord/record.h>


srecord::output_file_mif::~output_file_mif()
{
    emit_header();
    put_stringf("END;\n");
    if (enable_header_flag && actual_depth != depth)
        put_stringf("-- DEPTH = %lu;\n", actual_depth / width_in_bytes);
}


srecord::output_file_mif::output_file_mif(const std::string &a_file_name) :
    srecord::output_file(a_file_name),
    depth(65536),
    width(8),
    width_in_bytes(1),
    actual_depth(0),
    header_done(false),
    pref_blk_sz(32)
{
    line_length_set(80);
}


srecord::output::pointer
srecord::output_file_mif::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_mif(a_file_name));
}


void
srecord::output_file_mif::command_line(srecord::arglex_tool *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
        width = cmdln->value_number();
        cmdln->token_next();
        switch (width)
        {
        default:
            // yuck

        case 1:
        case 8:
            width = 8;
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
            width_in_bytes = 8;
            break;
        }
        line_length_set(80);
    }
}


void
srecord::output_file_mif::notify_upper_bound(unsigned long addr)
{
    depth = addr;
    actual_depth = addr;
}


void
srecord::output_file_mif::emit_header()
{
    if (header_done)
        return;
    if (enable_header_flag)
    {
        put_stringf
        (
            "--\n"
            "-- Generated automatically by %s -o --mif\n"
            "--\n",
            progname_get()
        );
        if (actual_depth != 0)
        {
            put_stringf("DEPTH = %lu;\n", actual_depth / width_in_bytes);
        }
        else
        {
            put_stringf
            (
                "DEPTH = %lu; "
                    "-- see comment at end of file for the actual size\n",
                depth / width_in_bytes
            );
        }
        put_stringf("WIDTH = %d;\n", width);
        put_stringf("ADDRESS_RADIX = HEX;\n");
        put_stringf("DATA_RADIX = HEX;\n");
    }
    put_stringf("CONTENT BEGIN\n");
    header_done = true;
}


void
srecord::output_file_mif::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_unknown:
        // Ignore.
        break;

    case srecord::record::type_header:
        if (enable_header_flag && record.get_length() > 0)
        {
            //
            // Output the header record as a comment.
            //
            put_string("-- ");
            if (record.get_address() != 0)
            {
                unsigned long addr = record.get_address();
                put_stringf("%04lX: ", addr);
            }
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                unsigned char c = *cp++;
                if (c == '\n')
                {
                    put_string("\n-- ");
                    continue;
                }
                if (!isprint(c))
                    c = ' ';
                put_char(c);
            }
            put_char('\n');
        }
        break;

    case srecord::record::type_data:
        {
            unsigned long addr = record.get_address();
            unsigned len = record.get_length();
            if ((addr % width_in_bytes) || (len % width_in_bytes))
                fatal_alignment_error(width_in_bytes);
            emit_header();
            put_stringf("%04lX:", addr / width_in_bytes);
            for (unsigned j = 0; j < len; ++j)
            {
                if ((j % width_in_bytes) == 0)
                    put_stringf(" ");
                put_stringf("%02X", record.get_data(j));
            }
            put_stringf(";\n");

            unsigned long d = addr + len;
            if (actual_depth < d)
                actual_depth = d;
        }
        break;

    case srecord::record::type_data_count:
        if (enable_data_count_flag)
        {
            unsigned long addr = record.get_address();
            put_stringf("-- data record count = %lu\n", addr);
        }
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            unsigned long addr = record.get_address();
            put_stringf("-- start address = %04lX\n", addr);
        }
        break;
    }
}


void
srecord::output_file_mif::line_length_set(int len)
{
    int pref_mult = (len - 6) / (1 + 2 * width_in_bytes);
    if (pref_mult < 1)
        pref_mult = 1;
    pref_blk_sz = pref_mult * width_in_bytes;
}


void
srecord::output_file_mif::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_mif::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    if (nbytes % width_in_bytes)
        return false;
    pref_blk_sz = nbytes;
    return true;
}


int
srecord::output_file_mif::preferred_block_size_get()
    const
{
    return pref_blk_sz;
}


const char *
srecord::output_file_mif::format_name()
    const
{
    return "Memory Initialization File (MIF, Altera)";
}
