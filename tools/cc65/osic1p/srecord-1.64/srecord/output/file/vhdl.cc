//
// srecord - manipulate eprom load files
// Copyright (C) 2000 Hendrik De Vloed <hendrik.devloed@ugent.be>
// Copyright (C) 2006-2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/arglex/tool.h>
#include <srecord/interval.h>
#include <srecord/output/file/vhdl.h>
#include <srecord/record.h>
#include <srecord/progname.h>


srecord::output_file_vhdl::~output_file_vhdl()
{
    emit_header();
    put_stringf("  others => %s_dont_care\n" " );\n", prefix.c_str());
    if (enable_footer_flag)
    {
        put_stringf("end package body %s_pack;\n", prefix.c_str());
    }
}


srecord::output_file_vhdl::output_file_vhdl(const std::string &a_file_name) :
    srecord::output_file(a_file_name),
    bytes_per_word(1),
    prefix("eprom"),
    header_done(false)
{
}


srecord::output::pointer
srecord::output_file_vhdl::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_vhdl(a_file_name));
}


void
srecord::output_file_vhdl::command_line(srecord::arglex_tool *cmdln)
{
    if (cmdln->token_cur() == arglex::token_number)
    {
        int a1 = cmdln->value_number();
        cmdln->token_next();

        if (a1 > 0)
        {
            unsigned a2 = (unsigned)a1;
            if (a2 > sizeof(unsigned long))
                a2 = sizeof(unsigned long);
            bytes_per_word = a2;
        }
    }
    if (cmdln->token_cur() == arglex::token_string)
    {
        prefix = cmdln->value_string();
        cmdln->token_next();
    }
}


void
srecord::output_file_vhdl::emit_header()
{
    if (header_done)
        return;
    if (enable_header_flag)
    {
        put_stringf
        (
            "--\n"
            "-- Generated automatically by %s -VHDL - do not edit\n"
            "--\n",
            progname_get()
        );
        put_stringf
        (
            "library IEEE;\n"
            "use IEEE.numeric_std.all;\n"
            "use work.%s_defs_pack.all;\n\n",
            prefix.c_str()
        );
        put_stringf("package %s_pack is\n", prefix.c_str());
        put_stringf
        (
            " constant %s_rom : %s_rom_array;\n",
            prefix.c_str(),
            prefix.c_str()
        );
        put_stringf("end package %s_pack;\n\n", prefix.c_str());
        put_stringf("package body %s_pack is\n", prefix.c_str());
    }
    put_stringf
    (
        " constant %s_rom : %s_rom_array := %s_rom_array'(\n",
        prefix.c_str(),
        prefix.c_str(),
        prefix.c_str()
    );
    header_done = true;
}


void
srecord::output_file_vhdl::write(const srecord::record &record)
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
                put_stringf("%08lX: ", addr);
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
        //
        // Make sure the data is aligned.
        //
        if
        (
            bytes_per_word > 1
        &&
            (
                (record.get_address() % bytes_per_word)
            ||
                (record.get_length() % bytes_per_word)
            )
        )
        {
            fatal_alignment_error(bytes_per_word);
        }

        emit_header();
        for (size_t j = 0; j < record.get_length(); j += bytes_per_word)
        {
            srecord::record::address_t current_word = 0;
            for (unsigned k = 0; k < bytes_per_word; ++k)
                current_word = (current_word << 8) + record.get_data(j + k);
            put_stringf
            (
                "  %lu => %s_entry(%lu),\n",
                (unsigned long)((record.get_address() + j) / bytes_per_word),
                prefix.c_str(),
                (unsigned long)current_word
            );
        }
        break;

    case srecord::record::type_data_count:
        // Ignore.
        break;

    case srecord::record::type_execution_start_address:
        // Ignore.
        break;
    }
}


void
srecord::output_file_vhdl::line_length_set(int)
{
    // ignore
}


void
srecord::output_file_vhdl::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_vhdl::preferred_block_size_set(int nbytes)
{
    if (nbytes > 1 || nbytes > record::max_data_length)
        return false;
    if (bytes_per_word > 1 && 0 != (nbytes % bytes_per_word))
        return false;
    return true;
}


int
srecord::output_file_vhdl::preferred_block_size_get()
    const
{
    //
    // Use the largest we can get, but it has to be a multiple of our
    // word size.
    //
    int n = srecord::record::max_data_length;
    if (bytes_per_word > 1)
        n -= (n % bytes_per_word);
    return n;
}


const char *
srecord::output_file_vhdl::format_name()
    const
{
    return "VHDL";
}
