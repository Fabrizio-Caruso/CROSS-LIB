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

#ifndef SRECORD_ARGLEX_TOOL_H
#define SRECORD_ARGLEX_TOOL_H

#include <srecord/arglex.h>
#include <srecord/defcon.h>
#include <srecord/endian.h>
#include <srecord/input.h>
#include <srecord/output.h>


namespace srecord
{

class interval; // forward

/**
  * The srecord::arglex_tool is used to parse command line with srec-specific
  * arguments.
  */
class arglex_tool:
    public arglex
{
public:
    enum
    {
        token_a430 = arglex::token_MAX,
        token_adler16_be,
        token_adler16_le,
        token_adler32_be,
        token_adler32_le,
        token_and,
        token_aomf,
        token_ascii_hex,
        token_assembler,
        token_atmel_generic_be,
        token_atmel_generic_le,
        token_basic_data,
        token_big,
        token_binary,
        token_bitrev,
        token_brecord,
        token_byte_swap,
        token_c_array,
        token_c_compressed,
        token_checksum_be_bitnot,
        token_checksum_be_negative,
        token_checksum_be_positive,
        token_checksum_le_bitnot,
        token_checksum_le_negative,
        token_checksum_le_positive,
        token_cl430,
        token_constant,
        token_constant_be,
        token_constant_le,
        token_constant_not,
        token_contradictory_bytes,
        token_cosmac,
        token_crc16_augment,
        token_crc16_augment_not,
        token_crc16_be,
        token_crc16_broken,
        token_crc16_ccitt,
        token_crc16_le,
        token_crc16_least_to_most,
        token_crc16_most_to_least,
        token_crc16_xmodem,
        token_crc32_be,
        token_crc32_le,
        token_crop,
        token_dec_binary,
        token_eeprom,
        token_emon52,
        token_exclude,
        token_exclusive_length,
        token_exclusive_length_be,
        token_exclusive_length_le,
        token_exclusive_maximum,
        token_exclusive_maximum_be,
        token_exclusive_maximum_le,
        token_exclusive_minimum,
        token_exclusive_minimum_be,
        token_exclusive_minimum_le,
        token_fairchild,
        token_fast_load,
        token_fill,
        token_fletcher16_be,
        token_fletcher16_le,
        token_fletcher32_be,
        token_fletcher32_le,
        token_formatted_binary,
        token_forth,
        token_four_packed_code,
        token_gcrypt,
        token_generator,
        token_guess,
        token_haval,
        token_hexdump,
        token_idt,
        token_ignore_checksums,
        token_lattice_memory_initialization_format,
        token_logisim,
        token_include,
        token_include_not,
        token_intel,
        token_intel16,
        token_intersection,
        token_length,
        token_length_be,
        token_length_le,
        token_maximum_address,
        token_maximum_be,
        token_maximum_le,
        token_md2,
        token_md5,
        token_memory_initialization_file,
        token_minimum_address,
        token_minimum_be,
        token_minimum_le,
        token_minus,
        token_mips_flash_be,
        token_mips_flash_le,
        token_mos_tech,
        token_motorola,
        token_msbin,
        token_multiple,
        token_needham_hex,
        token_not,
        token_offset,
        token_ohio_scientific,
        token_or,
        token_output,
        token_output_word,
        token_over,
        token_paren_begin,
        token_paren_end,
        token_polynomial,
        token_postfix,
        token_ppb,
        token_ppx,
        token_prefix,
        token_ram,
        token_random,
        token_random_fill,
        token_range_padding,
        token_redundant_bytes,
        token_repeat_data,
        token_repeat_string,
        token_rmd160,
        token_round_down,
        token_round_nearest,
        token_round_up,
        token_sequence_warnings_disable,
        token_sequence_warnings_enable,
        token_sha1,
        token_sha224,
        token_sha256,
        token_sha384,
        token_sha512,
        token_signetics,
        token_spasm_be,
        token_spasm_le,
        token_spectrum,
        token_split,
        token_stewie,
        token_stm32_crc_be,
        token_stm32_crc_le,
        token_style_dot,
        token_style_hexadecimal,
        token_style_hexadecimal_not,
        token_style_section,
        token_tektronix,
        token_tektronix_extended,
        token_tiger,
        token_ti_tagged,
        token_ti_tagged_16,
        token_ti_txt,
        token_trs80,
        token_unfill,
        token_union,
        token_unsplit,
        token_vhdl,
        token_vmem,
        token_whirlpool,
        token_wilson,
        token_within,
        token_xilinx_coefficient_file,
        token_xor,
        token_MAX
    };

    /**
      * The destructor.
      */
    virtual ~arglex_tool();

    /**
      * The constructor.  Pass the argc and argv as given to main;
      * there is not need to change the values at all.
      */
    arglex_tool(int argc, char **argv);

    /**
      * The get_input method is used to parse an input specification
      * (filename, file format, filters, everything) from the
      * command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    input::pointer get_input(void);

    /**
      * The get_output method is used to parse an output specification
      * (filename and file format) from the command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    output::pointer get_output(void);

    /**
      * The get_number method is used to parse a numeric value from the
      * command line.
      */
    unsigned long get_number(const char *caption);

    /**
      * The get_number method is used to parse a numeric value
      * from the command line, and check it agains a specified range.
      *
      * @param caption
      *     for the error message, if necessary
      * @param min
      *     The minimum acceptable value (inclusive)
      * @param max
      *     The maximum acceptable value (inclusive)
      */
    unsigned long get_number(const char *caption, long min, long max);

    /**
      * The can_get_number method is used to determine if it is possible
      * to parse a number from the next token on the command line.
      */
    bool can_get_number(void) const;

    /**
      * The get_interval method is used to parse an interval
      * set form the command line.  It consists of as many
      * get_interval_inner()s as possible.
      *
      * Used by the get_input method to parse the address intervals used
      * by various filters.  It is the lowest precedence level, and
      * handsles set union (the implicit operator) and set difference
      * (the - operator).
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    interval get_interval(const char *err_msg_caption);

    /**
      * The get_interval_small method may be used to parse an interval
      * set form the command line.  It checks that the interval is
      * <=1GB, and errors if it is not, with a --big override.  Commonly
      * used to sanity check things like --fill.
      */
    interval get_interval_small(const char *err_msg_caption);

    /**
      * The get_string method may be used to get a string from the
      * command line, or issue a fatal error if one is not available.
      *
      * @param caption
      *     The text for the error message.
      */
    std::string get_string(const char *caption);

    // See base class for documentation.
    void default_command_line_processing(void);

    defcon_t get_redundant_bytes(void) const { return redundant_bytes; }
    defcon_t get_contradictory_bytes(void) const { return contradictory_bytes; }

private:
    /**
      * The get_interval_factor method is used to parse a single
      * interval from the command line (usually, a pair of number
      * representing the [lower, upper) bounds, but it could be
      * -over or -within, too).
      *
      * This method parses the highest precedence operators in the range
      * parsing.
      *
      * This method should only every be called by the get_interval_term
      * method.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    interval get_interval_factor(const char *err_msg_caption);

    /**
      * The get_interval_term method is used to parse set-intersection
      * precedence intervals from the command line.  This method parses
      * the middle precedence operators in the range parsing.
      *
      * This method should only every be called by the get_interval_term
      * method.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    interval get_interval_term(const char *err_msg_caption);

    /**
      * The get_address method is used to parse an address from the
      * command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    void get_address(const char *err_msg_caption, unsigned long &addr);

    /**
      * The get_address_and_nbytes method is used to parse an address
      * and a byte count from the command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    void get_address_and_nbytes(const char *err_msg_caption,
            unsigned long &addr, int &nbytes);

    /**
      * The get_address_nbytes_width method is used to parse an address
      * a byte count and a width from the command line.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    void get_address_nbytes_width(const char *err_msg_caption,
        unsigned long &addr, int &nbytes, int &width);

    /**
      * The stdin_used instance variable is used to remember whether
      * or not the standard input has been used by a filter, yet.
      * Only one use of the standard input may be made; the second
      * use will result in a fatal error.
      */
    bool stdin_used;

    /**
      * The stdout_used instance variable is used to remember whether
      * or not the standard output has been used by a filter, yet.
      * Only one use of the standard output may be made; the second
      * use will result in a fatal error.
      */
    bool stdout_used;

    /**
      * The issue_sequence_warnings instance variable is used to
      * remember whether or not to issue data sequence warnings when
      * data records are not in strictly ascending address order.
      *
      * Negative means not set from the command line, zero means diabled
      * from the command line, positive means enabled on the command
      * line.
      */
    int issue_sequence_warnings;

    /**
      * The get_simple_input method is used to parse an input filename
      * or generator from the command line.  It shall only be used by
      * the #get_input method.
      *
      * If the parse is unsuccessful (is not present on command
      * line) a fatal error will be issued and the method call will
      * not return.
      */
    input::pointer get_simple_input();

    /**
      * The get_endian_by_token method is sued to obtain the endian-ness
      * of a given token.  This is for when there are big-endian and
      * little-endian variants of filters and file formats.
      *
      * @param tok
      *     the noken the endien-ness is required for.
      */
    endian_t get_endian_by_token(int tok) const;

    /**
      * The get_endian_by_token method is sued to obtain the endian-ness
      * of the current token.
      */
    endian_t
    get_endian_by_token(void)
        const
    {
        return get_endian_by_token(token_cur());
    }

    /**
      * The get_inclusive_by_token method is used to determine whether
      * or not a token is inclusive (e.g. token_length_be) or exclusive
      * (e.g. token_exclusive_length_be).
      *
      * @param tok
      *     The token to examine.
      */
    bool get_inclusive_by_token(int tok) const;

    /**
      * The get_inclusive_by_token method is used to determine whether
      * or not the current token is inclusive or exclusive.
      */
    bool
    get_inclusive_by_token(void)
        const
    {
        return get_inclusive_by_token(token_cur());
    }

    /**
      * The redundant_bytes instance variable is used to remember what
      * to do when faced with multiple identical byte values for a
      * memory address.
      */
    defcon_t redundant_bytes;

    /**
      * The vontradivtory_bytes instance variable is used to remember what
      * to do when faced with multiple different byte values for a
      * memory address.
      */
    defcon_t contradictory_bytes;

    /**
      * The default constructor.  Do not use.
      */
    arglex_tool();

    /**
      * The copy constructor.  Do not use.
      */
    arglex_tool(const arglex_tool &);

    /**
      * The assignment operator.  Do not use.
      */
    arglex_tool &operator=(const arglex_tool &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_ARGLEX_TOOL_H
