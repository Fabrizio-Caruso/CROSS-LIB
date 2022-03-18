//
// srecord - Manipulate EPROM load files
// Copyright (C) 2012 Peter Miller
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
#include <srecord/output/file/coe.h>
#include <srecord/record.h>


srecord::output_file_coe::~output_file_coe()
{
    emit_header();
    if (got_data)
    {
        put_stringf(";\n");
        got_data = false;
    }
    if (enable_header_flag && actual_depth != depth)
    {
        put_stringf
        (
            "; depth = %lu; 0x%04lX\n",
            actual_depth / width_in_bytes,
            actual_depth / width_in_bytes
        );
    }
}


srecord::output_file_coe::output_file_coe(const std::string &a_file_name) :
    srecord::output_file(a_file_name),
    address(0),
    column(0),
    depth(0),
    width(8),
    width_in_bytes(1),
    actual_depth(0),
    header_done(false),
    pref_blk_sz(32),
    got_data(false)
{
    line_length_set(80);
}


srecord::output::pointer
srecord::output_file_coe::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_coe(a_file_name));
}


void
srecord::output_file_coe::command_line(srecord::arglex_tool *cmdln)
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
srecord::output_file_coe::notify_upper_bound(unsigned long addr)
{
    depth = addr;
    actual_depth = addr;
}


void
srecord::output_file_coe::emit_header(void)
{
    if (header_done)
        return;
    if (enable_header_flag)
    {
        put_stringf
        (
            ";\n"
            "; Generated automatically by %s -o --coe %d\n"
            ";\n",
            progname_get(),
            width
        );
        if (actual_depth != 0)
        {
            put_stringf
            (
                "; depth = %ld; 0x%04lX\n",
                actual_depth / width_in_bytes,
                actual_depth / width_in_bytes
            );
            put_stringf
            (
                "; width = %d; 0x%02X\n",
                width_in_bytes * 8,
                width_in_bytes * 8
            );
        }
        put_stringf("memory_initialization_radix = 16;\n");
        put_stringf("memory_initialization_vector =\n");

    }
    header_done = true;
}


void
srecord::output_file_coe::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_unknown:
        // Ignore.
        break;

    case srecord::record::type_header:
        if (enable_header_flag && record.get_length() > 0)
        {
            put_string("; ");
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
                    put_string("\n; ");
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

            if (address != record.get_address())
                fatal_hole_error(address, record.get_address());
            if
            (
                (record.get_address() % width_in_bytes) != 0
            ||
                (record.get_length() % width_in_bytes) != 0
            )
                fatal_alignment_error(width_in_bytes);

            for (unsigned j = 0; j < len; ++j)
            {
                if (got_data && (j % width_in_bytes) == 0)
                    put_stringf(",\n");
                put_stringf("%02X", record.get_data(j));
                got_data = true;
            }

            unsigned long d = addr + len;
            if (actual_depth < d)
                actual_depth = d;
        }
        break;

    case srecord::record::type_data_count:
        if (enable_data_count_flag)
        {
            if (got_data)
            {
                put_stringf(";\n");
                got_data = false;
            }
            unsigned long addr = record.get_address();
            put_stringf("; data record count = %lu\n", addr);
        }
        break;

    case srecord::record::type_execution_start_address:
        if (enable_goto_addr_flag)
        {
            if (got_data)
            {
                put_stringf(";\n");
                got_data = false;
            }
            unsigned long addr = record.get_address();
            put_stringf("; start address = %04lX\n", addr);
        }
        break;
    }
}


void
srecord::output_file_coe::line_length_set(int len)
{
    int pref_mult = (len - 6) / (1 + 2 * width_in_bytes);
    if (pref_mult < 1)
        pref_mult = 1;
    pref_blk_sz = pref_mult * width_in_bytes;
}


void
srecord::output_file_coe::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_coe::preferred_block_size_set(int nbytes)
{
    if (nbytes < 1 || nbytes > record::max_data_length)
        return false;
    if ((nbytes % width_in_bytes) != 0)
        return false;
    pref_blk_sz = nbytes;
    return true;
}


int
srecord::output_file_coe::preferred_block_size_get(void)
    const
{
    return pref_blk_sz;
}


const char *
srecord::output_file_coe::format_name(void)
    const
{
    return "Coefficient (.COE) Files (Xilinx)";
}


// vim: set ts=8 sw=4 et :
