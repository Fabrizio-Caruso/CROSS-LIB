//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2014 Peter Miller
// Copyright (C) 2014 Scott Finneran
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

#include <iostream>

#include <srecord/arglex/tool.h>
#include <srecord/input/file.h>


srecord::arglex_tool::arglex_tool(int argc, char **argv) :
    arglex(argc, argv),
    stdin_used(false),
    stdout_used(false),
    issue_sequence_warnings(-1),
    redundant_bytes(srecord::defcon_warning),
    contradictory_bytes(srecord::defcon_fatal_error)
{
    static const table_ty table[] =
    {
        // sort using "!LC_ALL=C sort -f"
        { "(", token_paren_begin, },
        { ")", token_paren_end, },
        { "+", token_union, },
        { "-A430", token_a430, },
        { "-Absolute_Object_Module_Format", token_aomf, },
        { "-Adler_16_Big_Endian", token_adler16_be, },
        { "-Adler_16_Little_Endian", token_adler16_le, },
        { "-Adler_32_Big_Endian", token_adler32_be, },
        { "-Adler_32_Little_Endian", token_adler32_le, },
        { "-Adler_Checksum_16_Big_Endian", token_adler16_be, },
        { "-Adler_Checksum_32_big_Endian", token_adler32_be, },
        { "-AND", token_and, },
        { "-Ascii_Hexadecimal", token_ascii_hex, },
        { "-Ascii_Space_Hexadecimal", token_ascii_hex, },
        { "-ASseMbler", token_assembler, },
        { "-Atmel_Generic", token_atmel_generic_le, },
        { "-Atmel_Generic_Big_Endian", token_atmel_generic_be, },
        { "-Atmel_Generic_Little_Endian", token_atmel_generic_le, },
        { "-AUGment", token_crc16_augment },
        { "-BAsic", token_basic_data, },
        { "-BIG", token_big, },
        { "-BINary", token_binary, },
        { "-Bit_REVerse", token_bitrev },
        { "-BROken", token_crc16_broken, },
        { "-Byte_Swap", token_byte_swap, },
        { "-B_Record", token_brecord, },
        { "-Checksum_Adler_16_Big_Endian", token_adler16_be, },
        { "-Checksum_Adler_16_Little_Endian", token_adler16_le, },
        { "-Checksum_Adler_32_Big_Endian", token_adler32_be, },
        { "-Checksum_Adler_32_Little_Endian", token_adler32_le, },
        { "-Checksum_BitNot_Big_Endian", token_checksum_be_bitnot, },
        { "-Checksum_BitNot_Little_Endian", token_checksum_le_bitnot, },
        { "-Checksum_Fletchers_16_Little_Endian", token_fletcher16_le, },
        { "-Checksum_Fletchers_32_Little_Endian", token_fletcher32_le, },
        { "-Checksum_Negative_Big_Endian", token_checksum_be_negative, },
        { "-Checksum_Negative_Little_Endian", token_checksum_le_negative, },
        { "-Checksum_Positive_Big_Endian", token_checksum_be_positive, },
        { "-Checksum_Positive_Little_Endian", token_checksum_le_positive, },
        { "-CL430", token_cl430, },
        { "-COE", token_xilinx_coefficient_file },
        { "-Comite_Consultatif_International_Telephonique_et_Telegraphique",
            token_crc16_ccitt,},
        { "-CONSTant", token_constant, },
        { "-CONSTant_Big_Endian", token_constant_be, },
        { "-CONSTant_Little_Endian", token_constant_le, },
        { "-Contradictory_Bytes", token_contradictory_bytes },
        { "-COsmac", token_cosmac, },
        { "-CRop", token_crop, },
        { "-Cyclic_Redundancy_Check_16_Big_Endian", token_crc16_be, },
        { "-Cyclic_Redundancy_Check_16_Little_Endian", token_crc16_le,},
        { "-Cyclic_Redundancy_Check_16_XMODEM", token_crc16_xmodem,},
        { "-Cyclic_Redundancy_Check_32_Big_Endian", token_crc32_be, },
        { "-Cyclic_Redundancy_Check_32_Little_Endian", token_crc32_le,},
        { "-C_Array", token_c_array, },
        { "-C_COMpressed", token_c_compressed, },
        { "-DECimal_STyle", token_style_hexadecimal_not, },
        { "-Dec_Binary", token_dec_binary, },
        { "-DIFference", token_minus, },
        { "-Disable_Sequence_Warnings", token_sequence_warnings_disable, },
        { "-Dot_STyle", token_style_dot, },
        { "-EEPROM", token_eeprom, },
        { "-Elektor_Monitor52", token_emon52, },
        { "-Enable_Sequence_Warnings", token_sequence_warnings_enable, },
        { "-Exclude", token_exclude, },
        { "-Exclusive_Length", token_exclusive_length, },
        { "-Exclusive_Length_Big_Endian", token_exclusive_length_be, },
        { "-Exclusive_Length_Little_Endian", token_exclusive_length_le, },
        { "-Exclusive_MAximum", token_exclusive_maximum, },
        { "-Exclusive_MAximum_Big_Endian", token_exclusive_maximum_be, },
        { "-Exclusive_MAximum_Little_Endian", token_exclusive_maximum_le, },
        { "-Exclusive_MInimum", token_exclusive_minimum, },
        { "-Exclusive_MInimum_Big_Endian",token_exclusive_minimum_be, },
        { "-Exclusive_MInimum_Little_Endian", token_exclusive_minimum_le, },
        { "-FAIrchild", token_fairchild, },
        { "-Fast_Load", token_fast_load, },
        { "-Fill", token_fill, },
        { "-Fletchers_16_Big_Endian", token_fletcher16_be, },
        { "-Fletchers_16_Little_Endian", token_fletcher16_le, },
        { "-Fletchers_32_Big_Endian", token_fletcher32_be, },
        { "-Fletchers_32_Little_Endian", token_fletcher32_le, },
        { "-Fletchers_Checksum_16_Big_Endian", token_fletcher16_be, },
        { "-Fletchers_Checksum_32_Big_Endian", token_fletcher32_be, },
        { "-Formatted_Binary", token_formatted_binary, },
        { "-FORTH", token_forth, },
        { "-Four_Packed_Code", token_four_packed_code, },
        { "-GENerate", token_generator },
        { "-GENerator", token_generator },
        { "-Gnu_CRypt", token_gcrypt }, // undocumented
        { "-GUess", token_guess, },
        { "-HAVal", token_haval, },
        { "-HEXadecimal_Dump", token_hexdump, },
        { "-HEXadecimal_STyle", token_style_hexadecimal, },
        { "-IGnore_Checksums", token_ignore_checksums, },
        { "-INCLude", token_include, },
        { "-Integrated_Device_Technology", token_idt, },
        { "-Intel", token_intel, },
        { "-INtel_HeXadecimal_16", token_intel16, },
        { "-International_Telecommunication_Union", token_crc16_ccitt,},
        { "-INTERSection", token_intersection, },
        { "-Lattice_Memory_Initialization_Format",
            token_lattice_memory_initialization_format },
        { "-Least_To_Most", token_crc16_least_to_most },
        { "-Length", token_length, },
        { "-Length_Big_Endian", token_length_be, },
        { "-Length_Little_Endian", token_length_le, },
        { "-LOGisim", token_logisim },
        { "-MAximum-Address", token_maximum_address, },
        { "-MAximum_Big_Endian", token_maximum_be, },
        { "-MAximum_Little_Endian", token_maximum_le, },
        { "-MEM", token_lattice_memory_initialization_format },
        { "-Memory_Initialization_File", token_memory_initialization_file },
        { "-Message_Digest_2", token_md2 },
        { "-Message_Digest_5", token_md5 },
        { "-MInimum-Address", token_minimum_address, },
        { "-MInimum_Big_Endian",token_minimum_be, },
        { "-MInimum_Little_Endian", token_minimum_le, },
        { "-MINUs", token_minus, },
        { "-Mips_Flash_Big_Endian", token_mips_flash_be, },
        { "-Mips_Flash_Little_Endian", token_mips_flash_le, },
        { "-Most_To_Least", token_crc16_most_to_least },
        { "-MOS_Technologies", token_mos_tech, },
        { "-Motorola", token_motorola, },
        { "-MsBin", token_msbin, },
        { "-MULTiple", token_multiple, },
        { "-Needham_Hexadecimal", token_needham_hex, },
        { "-NOT", token_not, },
        { "-Not_AUGment", token_crc16_augment_not },
        { "-Not_CONSTant", token_constant_not, },
        { "-Not_INCLude", token_include_not, },
        { "-OFfset", token_offset, },
        { "-Ohio_Scientific", token_ohio_scientific, },
        { "-Ohio_Scientific65v", token_ohio_scientific, },
        { "-OR", token_or, },
        { "-Output", token_output, },
        { "-Output_Words", token_output_word, },
        { "-OVer", token_over, },
        { "-POLYnomial", token_polynomial },
        { "-PREfix", token_prefix, },
        { "-POSTfix", token_postfix, },
        { "-Prom_Pogrammer_Binary", token_ppb, },
        { "-Prom_Pogrammer_heXadecimal", token_ppx, },
        { "-RAM", token_ram, },
        { "-RANDom", token_random, },
        { "-Random_Fill", token_random_fill, },
        { "-RAnge_PADding", token_range_padding, },
        { "-RAw", token_binary, },
        { "-Redundant_Bytes", token_redundant_bytes },
        { "-REPeat_Data", token_repeat_data, },
        { "-REPeat_String", token_repeat_string, },
        { "-Ripe_Message_Digest_160", token_rmd160 },
        { "-Round", token_round_nearest, },
        { "-Round_Down", token_round_down, },
        { "-Round_Nearest", token_round_nearest, },
        { "-Round_Off", token_round_nearest, },
        { "-Round_Up", token_round_up, },
        { "-Section_STyle", token_style_section, },
        { "-Secure_Hash_Algorithm_1", token_sha1 },
        { "-Secure_Hash_Algorithm_224", token_sha224 },
        { "-Secure_Hash_Algorithm_256", token_sha256 },
        { "-Secure_Hash_Algorithm_384", token_sha384 },
        { "-Secure_Hash_Algorithm_512", token_sha512 },
        { "-Signed_BiNary", token_stewie, },
        { "-SIGnetics", token_signetics, },
        { "-SPAsm_Big_Endian", token_spasm_be, },
        { "-SPAsm_Little_Endian", token_spasm_le, },
        { "-SPAsm", token_spasm_be, }, // put after others
        { "-SPEctrum", token_spectrum, },
        { "-SPlit", token_split, },
        { "-Stag_Binary", token_ppb, },
        { "-Stag_Hexadecimal", token_ppx, },
        { "-STewie", token_stewie, },
        { "-STM32_Big_Endian", token_stm32_crc_be },
        { "-STM32_Little_Endian", token_stm32_crc_le },
        { "-STM32", token_stm32_crc_le }, // put after others
        { "-SUBtract", token_minus, },
        { "-S_record", token_motorola, },
        { "-Tektronix", token_tektronix, },
        { "-Tektronix_EXtended", token_tektronix_extended, },
        { "-Texas_Instruments_Tagged", token_ti_tagged, },
        { "-Texas_Instruments_Tagged_16", token_ti_tagged_16, },
        { "-Texas_Instruments_TeXT", token_ti_txt, },
        { "-TIGer", token_tiger },
        { "-TRS80", token_trs80 },
        { "-UNIon", token_union, },
        { "-Un_Fill", token_unfill, },
        { "-Un_SPlit", token_unsplit, },
        { "-VHdl", token_vhdl, },
        { "-VMem", token_vmem, },
        { "-WHIrlpool", token_whirlpool },
        { "-WILson", token_wilson, },
        { "-Within", token_within, },
        { "-Xilinx_Coefficient_File", token_xilinx_coefficient_file },
        { "-XOR", token_xor, },
        { "-X_MODEM", token_crc16_xmodem, },
        { "[", token_paren_begin, },
        { "]", token_paren_end, },
        { "{", token_paren_begin, },
        { "}", token_paren_end, },

        // The deprecated options go at the end of the table, so that
        // the preferred name will always be earlier in the table.
        { "-Checksum_Big_Endian", token_checksum_be_positive, },
        { "-Checksum_Little_Endian", token_checksum_le_positive, },
        { "-INtel_16", token_intel16, },
        { "-MAximum", token_maximum_address, },
        { "-MInimum", token_minimum_address, },

        SRECORD_ARGLEX_END_MARKER
    };

    table_set(table);
    deprecated_option("-Checksum_Big_Endian");
    deprecated_option("-Checksum_Little_Endian");
    deprecated_option("-INtel_16");
    deprecated_option("-MAximum");
    deprecated_option("-MInimum");
}


srecord::arglex_tool::~arglex_tool()
{
}


bool
srecord::arglex_tool::can_get_number(void)
    const
{
    switch (token_cur())
    {
    case token_paren_begin:
    case token_number:
    case token_minimum_address:
    case token_maximum_address:
    case token_length:
        return true;

    default:
        return false;
    }
}


void
srecord::arglex_tool::get_address(const char *name, unsigned long &address)
{
    if (!can_get_number())
    {
        fatal_error("the %s filter requires an address", name);
        // NOTREACHED
    }
    address = get_number("address");
}


void
srecord::arglex_tool::get_address_and_nbytes(const char *name,
    unsigned long &address, int &nbytes)
{
    if (!can_get_number())
    {
        fatal_error("the %s filter requires an address and a byte count", name);
        // NOTREACHED
    }
    address = get_number("address");
    nbytes = 4;
    if (can_get_number())
    {
        nbytes = get_number("byte count", 1, 8);
    }
    if ((long long)address + nbytes > (1LL << 32))
    {
        fatal_error
        (
            "the %s address (0x%8.8lX) and byte count (%d) may not span the "
                "top of memory",
            name,
            address,
            nbytes
        );
        // NOTREACHED
    }
}


void
srecord::arglex_tool::get_address_nbytes_width(const char *name,
    unsigned long &address, int &nbytes, int &width)
{
    address = get_number("address");
    nbytes = 4;
    width = 1;
    if (can_get_number())
    {
        nbytes = get_number("byte count", 1, 8);
        if (can_get_number())
        {
            width = get_number("width", 1, nbytes);
        }
    }
    if ((long long)address + nbytes > (1LL << 32))
    {
        fatal_error
        (
            "the %s address (0x%8.8lX) and byte count (%d) may not span the "
                "top of memory",
            name,
            address,
            nbytes
        );
        // NOTREACHED
    }
}


void
srecord::arglex_tool::default_command_line_processing(void)
{
    switch (token_cur())
    {
    default:
        arglex::default_command_line_processing();
        break;

    case token_ignore_checksums:
        input_file::ignore_all_checksums();
        token_next();
        break;

    case token_sequence_warnings_enable:
        issue_sequence_warnings = 1;
        token_next();
        break;

    case token_sequence_warnings_disable:
        issue_sequence_warnings = 0;
        token_next();
        break;

    case token_multiple:
        // This one is intentionally not documented.
        // Use one of the -rb or -cb options.
        redundant_bytes = defcon_ignore;
        contradictory_bytes = defcon_ignore;
        token_next();
        break;

    case token_redundant_bytes:
        {
            if (token_next() != token_string)
            {
                fatal_error
                (
                    "the %s option requires a string argument",
                    token_name(token_redundant_bytes)
                );
            }
            int x = defcon_from_text(value_string().c_str());
            if (x < 0)
            {
                fatal_error
                (
                    "the %s=%s option value is unknown",
                    token_name(token_redundant_bytes),
                    value_string().c_str()
                );
            }
            redundant_bytes = (defcon_t)x;
            token_next();
        }
        break;

    case token_contradictory_bytes:
        {
            if (token_next() != token_string)
            {
                fatal_error
                (
                    "the %s option requires a string argument",
                    token_name(token_contradictory_bytes)
                );
            }
            int x = defcon_from_text(value_string().c_str());
            if (x < 0)
            {
                fatal_error
                (
                    "the %s=%s option value is unknown",
                    token_name(token_contradictory_bytes),
                    value_string().c_str()
                );
            }
            contradictory_bytes = (defcon_t)x;
            token_next();
        }
        break;
    }
}


// vim: set ts=8 sw=4 et :
