//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <iostream>

#include <srecord/quit.h>
#include <srecord/arglex/tool.h>
#include <srecord/input/catenate.h>
#include <srecord/input/file/aomf.h>
#include <srecord/input/file/ascii_hex.h>
#include <srecord/input/file/atmel_generic.h>
#include <srecord/input/file/binary.h>
#include <srecord/input/file/brecord.h>
#include <srecord/input/file/cosmac.h>
#include <srecord/input/file/dec_binary.h>
#include <srecord/input/file/emon52.h>
#include <srecord/input/file/fairchild.h>
#include <srecord/input/file/fastload.h>
#include <srecord/input/file/formatted_binary.h>
#include <srecord/input/file/four_packed_code.h>
#include <srecord/input/file/hexdump.h>
#include <srecord/input/file/idt.h>
#include <srecord/input/file/intel.h>
#include <srecord/input/file/intel16.h>
#include <srecord/input/file/logisim.h>
#include <srecord/input/file/mif.h>
#include <srecord/input/file/mips_flash.h>
#include <srecord/input/file/mos_tech.h>
#include <srecord/input/file/motorola.h>
#include <srecord/input/file/msbin.h>
#include <srecord/input/file/needham.h>
#include <srecord/input/file/os65v.h>
#include <srecord/input/file/ppb.h>
#include <srecord/input/file/ppx.h>
#include <srecord/input/file/signetics.h>
#include <srecord/input/file/spasm.h>
#include <srecord/input/file/spectrum.h>
#include <srecord/input/file/stewie.h>
#include <srecord/input/file/tektronix.h>
#include <srecord/input/file/tektronix_extended.h>
#include <srecord/input/file/ti_tagged.h>
#include <srecord/input/file/ti_tagged_16.h>
#include <srecord/input/file/ti_txt.h>
#include <srecord/input/file/trs80.h>
#include <srecord/input/file/vmem.h>
#include <srecord/input/file/wilson.h>
#include <srecord/input/filter/and.h>
#include <srecord/input/filter/bitrev.h>
#include <srecord/input/filter/byte_swap.h>
#include <srecord/input/filter/checksum/bitnot.h>
#include <srecord/input/filter/checksum/negative.h>
#include <srecord/input/filter/checksum/positive.h>
#include <srecord/input/filter/crop.h>
#include <srecord/input/filter/fill.h>
#include <srecord/input/filter/interval/length.h>
#include <srecord/input/filter/interval/maximum.h>
#include <srecord/input/filter/interval/minimum.h>
#include <srecord/input/filter/message/adler16.h>
#include <srecord/input/filter/message/adler32.h>
#include <srecord/input/filter/message/crc16.h>
#include <srecord/input/filter/message/crc32.h>
#include <srecord/input/filter/message/stm32.h>
#include <srecord/input/filter/message/fletcher16.h>
#include <srecord/input/filter/message/fletcher32.h>
#include <srecord/input/filter/message/gcrypt.h>
#include <srecord/input/filter/not.h>
#include <srecord/input/filter/offset.h>
#include <srecord/input/filter/or.h>
#include <srecord/input/filter/random_fill.h>
#include <srecord/input/filter/sequence.h>
#include <srecord/input/filter/split.h>
#include <srecord/input/filter/unfill.h>
#include <srecord/input/filter/unsplit.h>
#include <srecord/input/filter/xor.h>
#include <srecord/input/generator.h>
#include <srecord/input/interval.h>


srecord::endian_t
srecord::arglex_tool::get_endian_by_token(int tok)
    const
{
    switch (tok)
    {
    case token_adler16_be:
    case token_adler32_be:
    case token_atmel_generic_be:
    case token_checksum_be_bitnot:
    case token_checksum_be_negative:
    case token_checksum_be_positive:
    case token_crc16_be:
    case token_crc32_be:
    case token_exclusive_length_be:
    case token_exclusive_maximum_be:
    case token_exclusive_minimum_be:
    case token_fletcher16_be:
    case token_fletcher32_be:
    case token_length_be:
    case token_maximum_be:
    case token_minimum_be:
    case token_mips_flash_be:
    case token_spasm_be:
    case token_stm32_crc_be:
        return endian_big;

    case token_adler16_le:
    case token_adler32_le:
    case token_atmel_generic_le:
    case token_checksum_le_bitnot:
    case token_checksum_le_negative:
    case token_checksum_le_positive:
    case token_crc16_le:
    case token_crc32_le:
    case token_exclusive_length_le:
    case token_exclusive_maximum_le:
    case token_exclusive_minimum_le:
    case token_fletcher16_le:
    case token_fletcher32_le:
    case token_length_le:
    case token_logisim:     // no way to tell (not documented)
    case token_maximum_le:
    case token_minimum_le:
    case token_msbin:
    case token_mips_flash_le:
    case token_spasm_le:
    case token_stm32_crc_le:
        return endian_little;

    default:
        quit_default.fatal_error
        (
            "%s: %d: add %s to switch (bug)",
            __FILE__,
            __LINE__,
            token_name(tok)
        );
        return endian_big;
    }
}


bool
srecord::arglex_tool::get_inclusive_by_token(int tok)
    const
{
    switch (tok)
    {
    case token_exclusive_length:
    case token_exclusive_length_be:
    case token_exclusive_length_le:
    case token_exclusive_maximum:
    case token_exclusive_maximum_be:
    case token_exclusive_maximum_le:
    case token_exclusive_minimum:
    case token_exclusive_minimum_be:
    case token_exclusive_minimum_le:
        return false;

    case token_length:
    case token_length_be:
    case token_length_le:
    case token_maximum_be:
    case token_maximum_le:
    case token_minimum_be:
    case token_minimum_le:
        return true;

    default:
        quit_default.fatal_error
        (
            "%s: %d: add %s to switch (bug)",
            __FILE__,
            __LINE__,
            token_name(tok)
        );
        return true;
    }
}


srecord::input::pointer
srecord::arglex_tool::get_simple_input(void)
{
    std::string fn = "-";
    switch (token_cur())
    {
    case token_paren_begin:
        {
            token_next();
            input::pointer ifp = get_input();
            for (;;)
            {
                switch (token_cur())
                {
                case arglex_tool::token_paren_begin:
                case arglex_tool::token_string:
                case arglex_tool::token_stdio:
                case arglex_tool::token_generator:
                    ifp = input_catenate::create(ifp, get_input());
                    break;

                case token_paren_end:
                    token_next();
                    return ifp;

                default:
                    fatal_error
                    (
                        "closing parenthesis expected before %s",
                        token_name(token_cur())
                    );
                    // NOTREACHED
                }
            }
        }

    case token_generator:
        // Don't need a file name,
        // but do NOT discard this token, yet.
        break;

    case token_string:
        fn = value_string();
        token_next();
        break;

    case token_stdio:
        token_next();
        // fall through...

    default:
        if (stdin_used)
        {
            fatal_error
            (
                "the standard input may only be named once on the command line"
            );
            // NOTREACHED
        }
        stdin_used = true;
        break;
    }

    //
    // determine the file format
    // and open the input file
    //
    input::pointer ifp;
    switch (token_cur())
    {
    case token_motorola:
        token_next();
        // fall through...

    default:
        ifp = input_file_motorola::create(fn);
        break;

    case token_aomf:
        token_next();
        ifp = input_file_aomf::create(fn);
        break;

    case token_ascii_hex:
        token_next();
        ifp = input_file_ascii_hex::create(fn);
        break;

    case token_atmel_generic_be:
    case token_atmel_generic_le:
        {
            endian_t end = get_endian_by_token();
            token_next();
            ifp = input_file_atmel_generic::create(fn, end);
        }
        break;

    case token_binary:
        token_next();
        ifp = input_file_binary::create(fn);
        break;

    case token_brecord:
        token_next();
        ifp = input_file_brecord::create(fn);
        break;

    case token_cosmac:
        token_next();
        ifp = input_file_cosmac::create(fn);
        break;

    case token_dec_binary:
        token_next();
        ifp = input_file_dec_binary::create(fn);
        break;

    case token_emon52:
        token_next();
        ifp = input_file_emon52::create(fn);
        break;

    case token_fairchild:
        token_next();
        ifp = input_file_fairchild::create(fn);
        break;

    case token_fast_load:
        token_next();
        ifp = input_file_fastload::create(fn);
        break;

    case token_formatted_binary:
        token_next();
        ifp = input_file_formatted_binary::create(fn);
        break;

    case token_four_packed_code:
        token_next();
        ifp = input_file_four_packed_code::create(fn);
        break;

    case token_generator:
        token_next();
        ifp = input_generator::create(this);
        break;

    case token_guess:
        token_next();
        ifp = input_file::guess(fn, *this);
        break;

    case token_hexdump:
        token_next();
        ifp = input_file_hexdump::create(fn);
        break;

    case token_idt:
        token_next();
        ifp = input_file_idt::create(fn);
        break;

    case token_intel:
        token_next();
        ifp = input_file_intel::create(fn);
        break;

    case token_intel16:
        token_next();
        ifp = input_file_intel16::create(fn);
        break;

    case token_logisim:
        token_next();
        ifp = input_file_logisim::create(fn);
        break;

    case token_memory_initialization_file:
        token_next();
        ifp = input_file_mif::create(fn);
        break;

    case token_mips_flash_be:
        token_next();
        ifp = input_file_mips_flash::create_be(fn);
        break;

    case token_mips_flash_le:
        token_next();
        ifp = input_file_mips_flash::create_le(fn);
        break;

    case token_mos_tech:
        token_next();
        ifp = input_file_mos_tech::create(fn);
        break;

    case token_msbin:
        token_next();
        ifp = input_file_msbin::create(fn);
        break;

    case token_needham_hex:
        token_next();
        ifp = input_file_needham::create(fn);
        break;

    case token_ohio_scientific:
        token_next();
        ifp = input_file_os65v::create(fn);
        break;

    case token_ppx:
        token_next();
        ifp = input_file_ppx::create(fn);
        break;

    case token_ppb:
        token_next();
        ifp = input_file_ppb::create(fn);
        break;

    case token_signetics:
        token_next();
        ifp = input_file_signetics::create(fn);
        break;

    case token_spasm_be:
    case token_spasm_le:
        {
            endian_t end = get_endian_by_token();
            token_next();
            ifp = input_file_spasm::create(fn, end);
        }
        break;

    case token_spectrum:
        token_next();
        ifp = input_file_spectrum::create(fn);
        break;

    case token_stewie:
        token_next();
        ifp = input_file_stewie::create(fn);
        break;

    case token_tektronix:
        token_next();
        ifp = input_file_tektronix::create(fn);
        break;

    case token_tektronix_extended:
        token_next();
        ifp = input_file_tektronix_extended::create(fn);
        break;

    case token_ti_tagged:
        token_next();
        ifp = input_file_ti_tagged::create(fn);
        break;

    case token_ti_tagged_16:
        token_next();
        ifp = input_file_ti_tagged_16::create(fn);
        break;

    case token_ti_txt:
        token_next();
        ifp = input_file_ti_txt::create(fn);
        break;

    case token_trs80:
        token_next();
        ifp = input_file_trs80::create(fn);
        break;

    case token_vmem:
        token_next();
        ifp = input_file_vmem::create(fn);
        break;

    case token_wilson:
        token_next();
        ifp = input_file_wilson::create(fn);
        break;
    }
    // assert(ifp);

    //
    // Process any additional format-specfic command line options.
    //
    ifp->command_line(this);

    //
    // Ignore checksums if asked to.
    //
    if (token_cur() == token_ignore_checksums)
    {
        ifp->disable_checksum_validation();
        token_next();
    }

    //
    // warn about data record sequences, if asked to
    //
    if (issue_sequence_warnings != 0)
        ifp = input_filter_sequence::create(ifp);

    //
    // report success
    //
    return ifp;
}


srecord::input::pointer
srecord::arglex_tool::get_input()
{
    input::pointer ifp = get_simple_input();

    //
    // apply any filters specified
    //
    for (;;)
    {
        // Keep this switch sorted in alphabetical order.
        switch (token_cur())
        {
        case token_adler16_be:
        case token_adler16_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                get_address(name, address);
                ifp =
                    input_filter_message_adler16::create
                    (
                        ifp,
                        address,
                        end
                    );
            }
            break;

        case token_adler32_be:
        case token_adler32_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                get_address(name, address);
                ifp =
                    input_filter_message_adler32::create
                    (
                        ifp,
                        address,
                        end
                    );
            }
            break;

        case token_and:
            {
                token_next();
                int filler = get_number("--and", 0, 255);
                if (filler < 0 || filler >= 256)
                {
                    fatal_error("-and value %d out of range (0..255)", filler);
                    // NOTREACHED
                }
                ifp = input_filter_and::create(ifp, filler);
            }
            break;

        case token_bitrev:
            token_next();
            if (can_get_number())
            {
                // Let the byte swap filter re-arrange the bytes.
                ifp = input_filter_byte_swap::create(ifp);
                ifp->command_line(this);
            }
            ifp = input_filter_bitrev::create(ifp);
            break;

        case token_byte_swap:
            token_next();
            ifp = input_filter_byte_swap::create(ifp);
            break;

        case token_checksum_be_bitnot:
        case token_checksum_le_bitnot:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width(name, address, nbytes, width);
                ifp =
                    input_filter_checksum_bitnot::create
                    (
                        ifp,
                        address,
                        nbytes,
                        end,
                        width
                    );
            }
            break;

        case token_checksum_be_negative:
        case token_checksum_le_negative:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width(name, address, nbytes, width);
                ifp =
                    input_filter_checksum_negative::create
                    (
                        ifp,
                        address,
                        nbytes,
                        end,
                        width
                    );
            }
            break;

        case token_checksum_be_positive:
        case token_checksum_le_positive:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width(name, address, nbytes, width);
                ifp =
                    input_filter_checksum_positive::create
                    (
                        ifp,
                        address,
                        nbytes,
                        end,
                        width
                    );
            }
            break;

        case token_crc16_be:
        case token_crc16_le:
            {
                endian_t end = get_endian_by_token();
                const char *name = token_name();
                token_next();
                unsigned long address;
                get_address(name, address);
                ifp =
                    input_filter_message_crc16::create(ifp, address, end);
            }
            break;

        case token_crc32_be:
        case token_crc32_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                get_address(name, address);
                ifp = input_filter_message_crc32::create(ifp, address, end);
            }
            break;

        case token_crop:
            token_next();
            ifp = input_filter_crop::create(ifp, get_interval("-Crop"));
            break;

        case token_exclude:
            token_next();
            ifp = input_filter_crop::create(ifp, -get_interval("-Exclude"));
            break;

        case token_exclusive_length:
            fatal_error
            (
                "Use --big-endian-exclusive-length or "
                    "--little-endian-exclusive-length"
            );
            // NOTREACHED

        case token_exclusive_maximum:
            fatal_error
            (
                "Use --big-endian-exclusive-maximum or "
                    "--little-endian-exclusive-maximum"
            );
            // NOTREACHED

        case token_exclusive_minimum:
            fatal_error
            (
                "Use --big-endian-exclusive-minimum or "
                    "--little-endian-exclusive-minimum"
            );
            // NOTREACHED

        case token_fill:
            {
                token_next();
                int filler = get_number("--Fill", 0, 255);
                interval range = get_interval_small("--Fill");
                ifp = input_filter_fill::create(ifp, filler, range);
            }
            break;

        case token_fletcher16_be:
        case token_fletcher16_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                get_address(name, address);
                ifp =
                    input_filter_message_fletcher16::create
                    (
                        ifp,
                        address,
                        end
                    );
            }
            break;

        case token_fletcher32_be:
        case token_fletcher32_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                token_next();
                unsigned long address;
                get_address(name, address);
                ifp =
                    input_filter_message_fletcher32::create
                    (
                        ifp,
                        address,
                        end
                    );
            }
            break;

        case token_haval:
            {
                // Undocumented, no gcrypt implementation, yet.
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_haval
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_gcrypt:
            {
                const char *name = token_name();
                token_next();
                std::string algo = get_string(name);
                unsigned long address = 0;
                get_address(name, address);
                bool hmac = false;
                ifp =
                    input_filter_message_gcrypt::create
                    (
                        ifp,
                        address,
                        algo.c_str(),
                        hmac
                    );
            }
            break;

        case token_length:
            fatal_error("Use --big-endian-length or --little-endian-length");
            // NOTREACHED

        case token_length_be:
        case token_length_le:
        case token_exclusive_length_be:
        case token_exclusive_length_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                bool inclusive = get_inclusive_by_token();
                token_next();
                unsigned long address;
                int nbytes, width;
                get_address_nbytes_width(name, address, nbytes, width);
                ifp =
                    input_filter_interval_length::create
                    (
                        ifp,
                        address,
                        nbytes,
                        end,
                        width,
                        inclusive
                    );
            }
            break;

        case token_maximum_be:
        case token_maximum_le:
        case token_exclusive_maximum_be:
        case token_exclusive_maximum_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                bool inclusive = get_inclusive_by_token();
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes(name, address, nbytes);
                ifp =
                    input_filter_interval_maximum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        end,
                        inclusive
                    );
            }
            break;

        case token_minimum_be:
        case token_minimum_le:
        case token_exclusive_minimum_be:
        case token_exclusive_minimum_le:
            {
                const char *name = token_name();
                endian_t end = get_endian_by_token();
                bool inclusive = get_inclusive_by_token();
                token_next();
                unsigned long address;
                int nbytes;
                get_address_and_nbytes(name, address, nbytes);
                ifp =
                    input_filter_interval_minimum::create
                    (
                        ifp,
                        address,
                        nbytes,
                        end,
                        inclusive
                    );
            }
            break;

        case token_md2:
            {
                // Undocumented, no gcrypt implementation, yet.
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp = input_filter_message_gcrypt::create_md2(ifp, address);
            }
            break;

        case token_md5:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp = input_filter_message_gcrypt::create_md5(ifp, address);
            }
            break;

        case token_not:
            token_next();
            ifp = input_filter_not::create(ifp);
            break;

        case token_offset:
            {
                token_next();
                unsigned long amount = get_number("--offset");
                ifp = input_filter_offset::create(ifp, amount);
            }
            break;

        case token_or:
            {
                token_next();
                int filler = get_number("--or value", 0, 255);
                ifp = input_filter_or::create(ifp, filler);
            }
            break;

        case token_random_fill:
            {
                token_next();
                interval range = get_interval_small("-Random_Fill");
                ifp = input_filter_random_fill::create(ifp, range);
            }
            break;

        case token_rmd160:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_rmd160
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_sha1:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp = input_filter_message_gcrypt::create_sha1(ifp, address);
            }
            break;

        case token_sha224:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_sha224
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_sha256:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_sha256
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_sha384:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_sha384
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_sha512:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_sha512
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_split:
            {
                token_next();
                int split_modulus = get_number("--split modulus");
                if (split_modulus < 2)
                {
                    fatal_error("the -split modulus must be two or more");
                    // NOTREACHED
                }
                int split_offset = 0;
                if (can_get_number())
                {
                    split_offset =
                        get_number("split offset", 0, split_modulus - 1);
                }
                int split_width = 1;
                if (can_get_number())
                {
                    split_width =
                        get_number("split width", 1, split_modulus - 1);
                }
                ifp =
                    input_filter_split::create
                    (
                        ifp,
                        split_modulus,
                        split_offset,
                        split_width
                    );
            }
            break;

        case token_stm32_crc_be:
        case token_stm32_crc_le:
            {
                const char *name = token_name();
                endian_t endian = get_endian_by_token();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp = input_filter_message_stm32::create(ifp, address, endian);
            }
            break;

        case token_tiger:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_tiger
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_unfill:
            {
                token_next();
                int fill_value = get_number("--unfill value", 0, 255);
                int fill_minimum = 1;
                if (can_get_number())
                {
                    fill_minimum = get_number("--unfill minimum", 1, 16);
                }
                ifp =
                    input_filter_unfill::create
                    (
                        ifp,
                        fill_value,
                        fill_minimum
                    );
            }
            break;

        case token_unsplit:
            {
                token_next();
                int split_modulus = get_number("--unsplit modulus");
                if (split_modulus < 2)
                {
                    fatal_error("the -unsplit modulus must be two or more");
                    // NOTREACHED
                }
                int split_offset = 0;
                if (can_get_number())
                {
                    split_offset =
                        get_number("--unsplit offset", 0, split_modulus - 1);
                }
                int split_width = 1;
                if (can_get_number())
                {
                    split_width =
                        get_number("--unsplit width", 1, split_modulus - 1);
                }
                ifp =
                    input_filter_unsplit::create
                    (
                        ifp,
                        split_modulus,
                        split_offset,
                        split_width
                    );
            }
            break;

        case token_whirlpool:
            {
                const char *name = token_name();
                token_next();
                unsigned long address = 0;
                get_address(name, address);
                ifp =
                    input_filter_message_gcrypt::create_whirlpool
                    (
                        ifp,
                        address
                    );
            }
            break;

        case token_xor:
            {
                token_next();
                int filler = get_number("--xor", 0, 255);
                ifp = input_filter_xor::create(ifp, filler);
            }
            break;

        default:
            //
            // return the input stream determined
            //
            return ifp;
        }

        //
        // Process any filter-specific command line options.
        //
        ifp->command_line(this);
    }
}


// vim: set ts=8 sw=4 et :
