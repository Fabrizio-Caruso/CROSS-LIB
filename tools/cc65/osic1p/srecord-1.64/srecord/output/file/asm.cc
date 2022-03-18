//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2010 Peter Miller
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

#include <cstdio>
#include <cstring>

#include <srecord/interval.h>
#include <srecord/arglex/tool.h>
#include <srecord/output/file/asm.h>
#include <srecord/record.h>


srecord::output_file_asm::~output_file_asm()
{
    if (!section_style && range.empty())
    {
        if (output_word)
            emit_word(0xFFFF);
        else
            emit_byte(0xFF);
    }
    if (column)
    {
        put_char('\n');
        column = 0;
    }

    //
    // write out sections data
    //
    if (section_style)
    {
        // address
        put_char('\n');
        if (dot_style)
            put_stringf("        .global %s_address\n", prefix.c_str());
        else
            put_stringf("        PUBLIC  %s_address\n", prefix.c_str());
        put_stringf("%s_address\n", prefix.c_str());
        interval x = range;
        while (!x.empty())
        {
            interval x2 = x;
            x2.first_interval_only();
            x -= x2;

            unsigned long val = x2.get_lowest();
            char buffer[20];
            // We do it this way, rather than just making argument 3 a
            // (?:) conditional, so that the compiler can tell us when
            // we get the types wrong.  (Besides, it will optimize.)
            if (hex_style)
                snprintf(buffer, sizeof(buffer), "0x%8.8lX", val);
            else
                snprintf(buffer, sizeof(buffer), "%lu", val);
            long len = strlen(buffer);

            if (column && column + len + 2 > line_length)
            {
                put_char('\n');
                column = 0;
            }
            if (column == 0)
            {
                if (dot_style)
                    put_stringf("        .long   ");
                else
                    put_stringf("        DL      ");
                column = 16;
            }
            else
            {
                put_stringf(", ");
                column += 2;
            }
            put_string(buffer);
            column += len;
        }
        if (column)
        {
            put_char('\n');
            column = 0;
        }

        // length_of_sections
        put_char('\n');
        if (dot_style)
        {
            put_stringf
            (
                "        .global %s_length_of_sections\n",
                prefix.c_str()
            );
        }
        else
        {
            put_stringf
            (
                "        PUBLIC  %s_length_of_sections\n",
                prefix.c_str()
            );
        }
        put_stringf("%s_length_of_sections\n", prefix.c_str());
        long nsections = 0;
        x = range;
        while (!x.empty())
        {
            interval x2 = x;
            x2.first_interval_only();
            x -= x2;
            ++nsections;

            unsigned long slen = x2.get_highest() - x2.get_lowest();
            if (output_word)
                slen /= 2;
            char buffer[30];
            if (hex_style)
                snprintf(buffer, sizeof(buffer), "0x%8.8lX", slen);
            else
                snprintf(buffer, sizeof(buffer), "%lu", slen);
            long len = strlen(buffer);
            if (column && column + len + 2 > line_length)
            {
                put_char('\n');
                column = 0;
            }
            if (column == 0)
            {
                if (dot_style)
                    put_stringf("        .long   ");
                else
                    put_stringf("        DL      ");
                column = 16;
            }
            else
            {
                put_stringf(", ");
                column += 2;
            }
            put_string(buffer);
            column += len;
        }
        if (column)
        {
            put_char('\n');
            column = 0;
        }

        // sections
        put_char('\n');
        if (dot_style)
            put_stringf("        .global %s_sections\n", prefix.c_str());
        else
            put_stringf("        PUBLIC  %s_sections\n", prefix.c_str());
        put_stringf("%s_sections\n", prefix.c_str());
        if (dot_style)
            put_string("        .long   ");
        else
            put_string("        DL      ");
        if (hex_style)
            put_stringf("0x%4.4lX\n", nsections);
        else
            put_stringf("%lu\n", nsections);
    }


    if (enable_footer_flag)
    {
        unsigned long hi = range.get_highest();
        put_stringf("; upper bound = 0x%4.4lX\n", hi);
        unsigned long lo = range.get_lowest();
        put_stringf("; lower bound = 0x%4.4lX\n", lo);
    }
    unsigned long len = range.get_highest() - range.get_lowest();
    put_stringf("; length =      0x%4.4lX\n", len);

    if (section_style)
    {
        if (dot_style)
            put_stringf("        .end\n");
        else
            put_stringf("        END\n");
    }
}


srecord::output_file_asm::output_file_asm(const std::string &filename) :
    srecord::output_file(filename),
    prefix("eprom"),
    taddr(0),
    column(0),
    current_address(0),
    line_length(75),
    org_warn(false),
    output_word(false),
    dot_style(false),
    section_style(false),
    hex_style(false)
{
}


srecord::output::pointer
srecord::output_file_asm::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_asm(a_file_name));
}


void
srecord::output_file_asm::command_line(srecord::arglex_tool *cmdln)
{
    if (cmdln->token_cur() == arglex::token_string)
    {
        prefix = cmdln->value_string();
        cmdln->token_next();
    }
    for (;;)
    {
        switch (cmdln->token_cur())
        {
        case srecord::arglex_tool::token_a430:
            cmdln->token_next();
            // Generate "IAR assembler compiler compliant" output.
            section_style = true;
            hex_style = true;
            break;

        case srecord::arglex_tool::token_cl430:
            cmdln->token_next();
            // Generate "Code Composer Essential compliant" output.
            dot_style = true;
            section_style = true;
            hex_style = true;
            break;

        case srecord::arglex_tool::token_style_dot:
            cmdln->token_next();
            dot_style = true;
            break;

        case srecord::arglex_tool::token_style_hexadecimal:
            cmdln->token_next();
            hex_style = true;
            break;

        case srecord::arglex_tool::token_style_hexadecimal_not:
            cmdln->token_next();
            hex_style = false;
            break;

        case srecord::arglex_tool::token_style_section:
            cmdln->token_next();
            section_style = true;
            break;

        case srecord::arglex_tool::token_output_word:
            cmdln->token_next();
            output_word = true;
            break;

        default:
            return;
        }
    }
}


void
srecord::output_file_asm::emit_byte(int n)
{
    char buffer[8];
    if (hex_style)
        sprintf(buffer, "0x%2.2X", (unsigned char)n);
    else
        sprintf(buffer, "%u", (unsigned char)n);
    int len = strlen(buffer);
    if (column && (column + 1 + len) > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (!column)
    {
        if (dot_style)
        {
            put_string("        .byte   ");
            column = 16;
        }
        else
        {
            put_string("        DB      ");
            column = 16;
        }
    }
    else
    {
        put_char(',');
        ++column;
    }
    put_string(buffer);
    column += len;
    ++current_address;
}


void
srecord::output_file_asm::emit_word(unsigned int n)
{
    char buffer[16];
    if (hex_style)
        snprintf(buffer, sizeof(buffer), "0x%4.4X", (unsigned short)n);
    else
        snprintf(buffer, sizeof(buffer), "%u", (unsigned short)n);
    int len = strlen(buffer);
    if (column && (column + 1 + len) > line_length)
    {
        put_char('\n');
        column = 0;
    }
    if (!column)
    {
        if (dot_style)
        {
            put_string("        .short      ");
            column = 20;
        }
        else
        {
            put_string("        DW      ");
            column = 16;
        }
    }
    else
    {
        put_char(',');
        ++column;
    }
    put_string(buffer);
    column += len;
    current_address += 2;
}


void
srecord::output_file_asm::write(const srecord::record & record)
{
    switch (record.get_type())
    {
    default:
        // ignore
        break;

    case srecord::record::type_header:
        // emit header records as comments in the file
        {
            bool bol = true;
            const unsigned char *cp = record.get_data();
            const unsigned char *ep = cp + record.get_length();
            while (cp < ep)
            {
                int c = *cp++;
                if (c == '\n')
                {
                    put_char(c);
                    bol = true;
                    continue;
                }
                if (bol)
                    put_string("; ");
                if (isprint(c))
                    put_char(c);
                bol = false;
            }
            if (!bol)
                put_char('\n');
        }
        break;

    case srecord::record::type_data:
        //
        // emit the data prelude, if we have not done so already
        //
        if (section_style && range.empty())
        {
            if (dot_style)
            {
                put_stringf("        .global %s\n", prefix.c_str());
                put_stringf("        .text\n");
                put_stringf("%s\n", prefix.c_str());
            }
            else
            {
                put_stringf("        PUBLIC  %s\n", prefix.c_str());
                put_stringf("        RSEG    CODE\n");
                put_stringf("%s\n", prefix.c_str());
            }

            if (!enable_optional_address_flag)
                current_address = (unsigned long)-1L;
        }

        if (current_address != record.get_address())
        {
            current_address = record.get_address();

            if (!section_style)
            {
                if (column)
                {
                    put_char('\n');
                    column = 0;
                }

                const char *org = dot_style ? ".org" : "ORG";
                if (range.empty())
                {
                    put_stringf
                    (
                        "; To avoid this next %s directive, use the "
                            "--offset -0x%lX filter.\n",
                        org,
                        current_address
                    );
                }
                else if (!org_warn)
                {
                    org_warn = true;
                    put_stringf
                    (
                        "; To avoid this next %s directive, use the "
                            "--fill filter.\n",
                        org
                    );
                }
                put_stringf("        %-7s %lu\n", org, current_address);
            }
        }
        if (output_word)
        {
            int len = record.get_length();
            if (len & 1)
                fatal_alignment_error(2);
            range += interval(record.get_address(), record.get_address() + len);

            //
            // No attempt is made to align the data on even byte
            // boundaries, use the --fill --range-pad filter for that.
            //
            for (int j = 0; j < len; j += 2)
            {
                unsigned char n1 = record.get_data(j);
                unsigned char n2 = record.get_data(j + 1);
                // little-endian
                unsigned short n = n1 + (n2 << 8);
                emit_word(n);
            }
        }
        else
        {
            range +=
                interval
                (
                    record.get_address(),
                    record.get_address() + record.get_length()
                );

            for (size_t j = 0; j < record.get_length(); ++j)
            {
                emit_byte(record.get_data(j));
            }
        }
        break;

    case srecord::record::type_execution_start_address:
        taddr = record.get_address();
        if (enable_goto_addr_flag)
        {
            if (column)
            {
                put_char('\n');
                column = 0;
            }
            put_stringf("; execution start address = 0x%4.4lX\n", taddr);
        }
        break;
    }
}


void
srecord::output_file_asm::line_length_set(int n)
{
    line_length = n;
}


void
srecord::output_file_asm::address_length_set(int)
{
    // ignore
}


bool
srecord::output_file_asm::preferred_block_size_set(int nbytes)
{
    if (nbytes <= 0)
        return false;
    if (!output_word)
        return true;
    return ((nbytes & 1) == 0);
}


int
srecord::output_file_asm::preferred_block_size_get()
    const
{
    //
    // Use the largest we can get.
    //
    if (output_word)
        return (srecord::record::max_data_length & ~1);
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_asm::format_name()
    const
{
    return (output_word ? "Assembler (16-bit)" : "Assembler (8-bit)");
}
