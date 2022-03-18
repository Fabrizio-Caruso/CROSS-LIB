//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2008, 2010, 2013, 2014 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>
#include <cstdlib>

#include <srecord/input/catenate.h>
#include <srecord/input/file.h>
#include <srecord/memory.h>
#include <srecord/memory/walker/writer.h>
#include <srecord/output.h>
#include <srecord/output/file.h>
#include <srecord/output/filter/reblock.h>
#include <srecord/record.h>
#include <srecord/string.h>

#include <srec_cat/arglex3.h>


int
main(int argc, char **argv)
{
    srec_cat_arglex3 cmdline(argc, argv);
    cmdline.token_first();
    srecord::input::pointer infile;
    srecord::output::pointer outfile;
    int line_length = 0;
    int address_length = 0;
    std::string header;
    bool header_set = false;
    unsigned long execution_start_address = 0;
    bool execution_start_address_set = false;
    int output_block_size = 0;
    bool output_block_packing = false;
    bool output_block_align = false;
    while (cmdline.token_cur() != srecord::arglex::token_eoln)
    {
        switch (cmdline.token_cur())
        {
        default:
            cmdline.default_command_line_processing();
            continue;

        case srecord::arglex_tool::token_paren_begin:
        case srecord::arglex_tool::token_string:
        case srecord::arglex_tool::token_stdio:
        case srecord::arglex_tool::token_generator:
            {
                srecord::input::pointer ip = cmdline.get_input();
                if (infile)
                    infile = srecord::input_catenate::create(infile, ip);
                else
                    infile = ip;
            }
            continue;

        case srecord::arglex_tool::token_output:
            if (outfile)
                cmdline.usage();
            outfile = cmdline.get_output();
            continue;

        case srec_cat_arglex3::token_line_length:
            if (line_length > 0)
                cmdline.usage();
            if (cmdline.token_next() != srecord::arglex::token_number)
                cmdline.usage();
            line_length = cmdline.value_number();
            if (line_length <= 0)
            {
                std::cerr << "the line length " << line_length << " is invalid"
                    << std::endl;
                exit(EXIT_FAILURE);
            }
            break;

        case srec_cat_arglex3::token_output_block_size:
            if (output_block_size > 0)
                cmdline.usage();
            if (cmdline.token_next() != srecord::arglex::token_number)
                cmdline.usage();
            output_block_size = cmdline.value_number();
            if
            (
                output_block_size <= 0
            ||
                output_block_size > srecord::record::max_data_length
            )
            {
                std::cerr << "the block size " << output_block_size
                    << " is invalid" << std::endl;
                exit(EXIT_FAILURE);
            }
            break;

        case srec_cat_arglex3::token_address_length:
            if (address_length > 0)
                cmdline.usage();
            if (cmdline.token_next() != srecord::arglex::token_number)
                cmdline.usage();
            address_length = cmdline.value_number();
            if (address_length <= 0 || address_length > (int)sizeof(long))
            {
                std::cerr << "the address length " << address_length
                    << " is invalid" << std::endl;
                exit(EXIT_FAILURE);
            }
            break;

        case srec_cat_arglex3::token_data_only:
            srecord::output_file::enable_header(false);
            srecord::output_file::enable_data_count(false);
            srecord::output_file::enable_goto_addr(false);
            srecord::output_file::enable_footer(false);
            break;

        case srec_cat_arglex3::token_enable:
        case srec_cat_arglex3::token_disable:
            {
                int tok = cmdline.token_cur();
                bool yesno = (tok == srec_cat_arglex3::token_enable);
                cmdline.token_next();
                std::string name = cmdline.get_string(cmdline.token_name(tok));
                if (!srecord::output_file::enable_by_name(name, yesno))
                {
                    std::cerr << "argument of " << cmdline.token_name(tok)
                        << "=" << name << " unknown" << std::endl;
                    cmdline.usage();
                }
            }
            continue;

        case srec_cat_arglex3::token_crlf:
            srecord::output_file::line_termination_by_name("crlf");
            break;

        case srec_cat_arglex3::token_line_termination:
            {
                int tok = cmdline.token_cur();
                cmdline.token_next();
                std::string name = cmdline.get_string(cmdline.token_name(tok));
                if (!srecord::output_file::line_termination_by_name(name))
                {
                    std::cerr << "line termination \"" << name << "\" unknown"
                        << std::endl;
                    cmdline.usage();
                }
            }
            continue;

        case srec_cat_arglex3::token_header:
            if (cmdline.token_next() != srecord::arglex::token_string)
            {
                std::cerr << "the -header option requires a string argument"
                    << std::endl;
                exit(EXIT_FAILURE);
            }
            header = cmdline.value_string();
            header_set = true;

            //
            // The users may use %nn on the command line, but we store
            // a byte 0xnn in the header.  The motivation or this a use
            // case that wanted to insert a trailing NUL character.
            //
            header = srecord::string_url_decode(header);

            srecord::output_file::enable_header(true);
            break;

        case srec_cat_arglex3::token_execution_start_address:
            if (execution_start_address_set)
            {
                std::cerr << "too many -execution-start-address options "
                    "specified" << std::endl;
                exit(EXIT_FAILURE);
            }
            cmdline.token_next();
            execution_start_address =
                cmdline.get_number("-Execution_Start_Address");
            execution_start_address_set = true;
            srecord::output_file::enable_goto_addr(true);
            continue;

        case srec_cat_arglex3::token_output_block_packing:
            output_block_packing = true;
            break;

        case srec_cat_arglex3::token_output_block_align:
            output_block_align = true;
            break;
        }
        cmdline.token_next();
    }
    if (!infile)
        infile = cmdline.get_input();
    if (!outfile)
        outfile = cmdline.get_output();

    if (output_block_packing || output_block_align)
    {
        //
        // Reblock the output so that it matches the output file's block
        // size exactly.  That way SRecord's internal memory chunk size
        // does not cause output artifacts.  This requires lost of
        // memory copying back and forth, so only do it if they asked
        // for it.
        //
        // This filter makes no semantic difference to the output.
        // (If it does, it's a bug.)
        //
        outfile =
            srecord::output_filter_reblock::create(outfile, output_block_align);
    }

    if (address_length > 0)
        outfile->address_length_set(address_length);
    if (line_length > 0)
        outfile->line_length_set(line_length);
    if (output_block_size > 0)
    {
        if (!outfile->preferred_block_size_set(output_block_size))
        {
            std::cerr << "output block size " << output_block_size
                << " was rejected by " << outfile->format_name() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    //
    // Read the input into memory.  This allows the data to be
    // consolidated on output, and warnings to be issued for
    // duplicates.
    //
    // It is assumed the data will all fit into memory.  This is
    // usually reasonable, because these utilities are used for
    // EPROMs which are usually smaller than the available virtual
    // memory of the development system.
    //
    srecord::memory m;
    if (header_set)
    {
        // Only the first header is used, even if you have N input
        // files.  Being set before reading any of the input files, the
        // command line takes precedence.
        m.set_header(header);
    }
    m.reader
    (
        infile,
        cmdline.get_redundant_bytes(),
        cmdline.get_contradictory_bytes()
    );
    if (execution_start_address_set)
        m.set_execution_start_address(execution_start_address);

    //
    // Write the remembered data out to the output.
    //
    srecord::memory_walker::pointer w =
        srecord::memory_walker_writer::create(outfile);
    m.walk(w);

    //
    // success
    //
    return EXIT_SUCCESS;
}


// vim: set ts=8 sw=4 et :
