//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2003, 2005-2012 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_H
#define SRECORD_OUTPUT_FILE_H

#include <string>
#include <srecord/output.h>
#include <srecord/format_printf.h>

namespace srecord {

/**
  * The srecord::output_file class is used to represent a generic output file.
  * It provides a numnber of services useful to many output file formats.
  */
class output_file:
    public output
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file();

    /**
      * The default constructor.
      * Output will be sent to the standard output.
      */
    output_file();

    /**
      * The constructor.  The output will be sent to the named file (or
      * the standard output if the file name is "-").
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file(const std::string &file_name);

    // See base class for documentation.
    virtual const std::string filename(void) const;

    /**
      * The enable_header class method is used to enable or disable
      * the writing of header records into output file, if the format
      * supports optional header records.
      *
      * @param yesno
      *     true if header records should be written, false if not
      */
    static void enable_header(bool yesno);

    /**
      * The enable_data_count class method is used to enable or disable
      * the writing of data record count records into output file, if
      * the format supports optional data record count records.
      *
      * @param yesno
      *     true if data record count records should be written, false
      *     if not
      */
    static void enable_data_count(bool yesno);

    /**
      * The enable_goto_addr class method is used to enable or disable
      * the writing of execution start address records into output file,
      * if the format supports optional execution start address records.
      *
      * @param yesno
      *     true if execution start address records should be written,
      *     false if not
      */
    static void enable_goto_addr(bool yesno);

    /**
      * The enable_footer class method is used to enable or disable
      * the writing of footer records into output file, if the format
      * supports optional footer records.
      *
      * @param yesno
      *     true if footer records should be written, false if not
      */
    static void enable_footer(bool yesno);

    /**
      * The enable_by_name class method is used to enable or disable
      * the writing of various records into output file.
      *
      * @param name
      *     the name of the record type to enable or disable
      * @param yesno
      *     true if should be enable, false if sjould be disabled
      * @returns
      *     true if name understood, false if not (to trigger diagnostic
      *     error message)
      */
    static bool enable_by_name(const std::string &name, bool yesno);

    /**
      * The line_termination_by_name method is used to force line
      * termination to a particular style, rather than the current
      * operating system's native text format.
      *
      * @param name
      *     The name of the line termination style to be used,
      *     e.g. "crlf" or "nl".
      * @returns
      *     true if successful, false if name unknown
      */
    static bool line_termination_by_name(const std::string &name);

protected:
    /**
      * The put_char method is used to send a character to the output.
      * Usually, this is sufficient, however derived classes may
      * over-ride it if they have a special case.  Over-ride with
      * caution, as it affects many other methods.
      */
    virtual void put_char(int c);

    /**
      * The put_nibble method is used to send a hexadecimal digit (0..9,
      * A..F) to the output.  It calls put_char to send the output.
      */
    void put_nibble(int value);

    /**
      * The put_byte method is used to send a byte value to the output.
      * The default implementation is to call the put_nibble method
      * twice, big-endian (most significant nibble first).
      *
      * The value of the byte will be added to the running checksum, via
      * the #checksum_add method.
      *
      * Usually, this #put_byte method implemention is sufficient for
      * most output classes, however derived classes may over-ride it if
      * they have a special case.  Over-ride with caution, as it affects
      * many other methods.
      */
    virtual void put_byte(unsigned char value);

    /**
      * The put_word_be method is used to send a 16-bit value to the
      * output.  The #put_byte method is called twice, and the two byte
      * values are sent big-endian (most significant byte first).
      */
    virtual void put_word_be(int value);

    /**
      * The put_word_le method is used to send a 16-bit value to the
      * output.  The #put_byte method is called twice, and the two byte
      * values are sent little-endian (least significant byte first).
      */
    virtual void put_word_le(int value);

    /**
      * The put_3bytes_be method is used to send a 24-bit value to the
      * output.  The #put_byte method is called three times, and the
      * three byte values are sent big-endian (most significant byte
      * first).
      */
    virtual void put_3bytes_be(unsigned long value);

    /**
      * The put_3bytes_le method is used to send a 24-bit value to the
      * output.  The #put_byte method is called three times, and the
      * three byte values are sent little-endian (least significant byte
      * first).
      */
    virtual void put_3bytes_le(unsigned long value);

    /**
      * The put_4bytes_be method is used to send a 32-bit value to the
      * output.  The #put_byte method is called four times, and the
      * four byte values are sent big-endian (most significant byte
      * first).
      */
    virtual void put_4bytes_be(unsigned long value);

    /**
      * The put_4bytes_le method is used to send a 32-bit value to the
      * output.  The #put_byte method is called four times, and the
      * four byte values are sent little-endian (least significant byte
      * first).
      */
    virtual void put_4bytes_le(unsigned long value);

    /**
      * The checksum_reset method is used to set the running checksum to
      * zero.
      */
    void checksum_reset(void);

    /**
      * The checksum_add method is used to add another 8-bit value to
      * the running checksum.
      *
      * The default implementation uses a simple addition.  Derived
      * classesmay override if they need to.  Do this with caution, as
      * it affects other methods.
      */
    virtual void checksum_add(unsigned char n);

    /**
      * The checksum_get method is used to get the current value of the
      * running checksum (added to by the #checksum_add method, usually
      * called by the #put_byte method).  Only the lower 8 bits of the
      * sum are returned.
      */
    int checksum_get(void);

    /**
      * The checksum_get16 method is used to get the current value of
      * the running checksum (added to by the #checksum_add method,
      * usually called by the #put_byte method).  Only the lower 16
      * bits of the sum are returned.
      */
    int checksum_get16(void);

    /**
      * The seek_to method is used to move the output position to the
      * specified location in the output file.
      */
    void seek_to(unsigned long);

    /**
      * The put_string method is used to send a nul-terminated C string
      * to the output.  Multiple calls to #put_char are made.
      */
    void put_string(const char *text);

    /**
      * The put_string method is used to send C++ string
      * to the output.  Multiple calls to #put_char are made.
      *
      * @param text
      *     The string to print.
      */
    void put_string(const std::string &text);

    /**
      * The put_stringf method is used to send a formatted string to the
      * output.  The format and operation ios similar to the standard
      * printf function.  Multiple calls to #put_char are made.
      */
    void put_stringf(const char *, ...)             FORMAT_PRINTF(2, 3);

    /**
      * The enable_header_flag class variable is set by the
      * #enable_header method, to remember whether or not header
      * records are to be emitted (if the format supports optional
      * header records).
      */
    static bool enable_header_flag;

    /**
      * The enable_data_count_flag class variable is set by the
      * #enable_data_count method, to remember whether or not data
      * record count records are to be emitted (if the format supports
      * optional data record count records).
      */
    static bool enable_data_count_flag;

    /**
      * The enable_goto_addr_flag class variable is set by the
      * #enable_goto_addr method, to remember whether or not execution
      * start address records are to be emitted (if the format supports
      * optional execution start address records).
      */
    static bool enable_goto_addr_flag;

    /**
      * The enable_footer_flag class variable is set by the
      * #enable_footer method, to remember whether or not footer
      * records (end-of-file record) are to be emitted (if the format
      * supports optional footer records).
      */
    static bool enable_footer_flag;

    /**
      * The enable_optional_address_records class variable is used
      * to remember whether or not to emit optional address records.
      * Defaults to true.  Set by the #enable_by_name method.
      */
    static bool enable_optional_address_flag;

    /**
      * The enable_footer class method is used to enable or disable
      * emitting optional address records.
      *
      * @param yesno
      *     true if records should be written, false if not
      */
    static void enable_optional_address(bool yesno);

    enum line_termination_t
    {
        line_termination_native,
        line_termination_crlf,
        line_termination_nl,
        line_termination_binary = line_termination_nl,
        line_termination_cr,
        line_termination_primos
    };

    /**
      * The line_termination class variable is used to remember the
      * desired line termination style.  Defaults to the native style of
      * the current operating system.
      */
    static line_termination_t line_termination;

    /**
      * The line_termination_guess class method is used to figure out
      * the line termination style of the host environment.
      */
    static line_termination_t line_termination_guess(void);

private:
    /**
      * The file_name instance variable is used by the #filename and
      * filename_and_line methods to report the name of the input
      * file.  This makes for informative error mesages.
      */
    std::string file_name;

    /**
      * The line_number instance variable is used by the get_char
      * method to remember the current line number.  It us used by the
      * filename_and_line method to report the current file location.
      */
    int line_number;

    /**
      * The vfp instance variable is used by the #get_fp method to
      * remember the file pointer.  You need to cast it to FILE* befor
      * you use it.  Never access this instance variable directly,
      * always go via the #get_fp method.  This ensures the file has
      * been opened first!
      */
    void *vfp;

protected:
    /**
      * The checksum instance variable is used record the running
      * checksum.  NEVER access this variable directly.  Always use the
      * #checksum_reset method to set it mack to its initial state.
      * Always use the #checksum_add method to add a byte to it.
      * Always use the #checksum_get or #checksum_get16 methods to
      * read its value.
      */
    int checksum;

    /**
      * The fatal_alignment_error method is used to report problems
      * with unaligned data in formats that require aligned data.  It
      * suggests a fill to fix the problem.
      *
      * @param alignment
      *     The necessary byte alignment
      */
    void fatal_alignment_error(int alignment);

    /**
      * The fatal_hole_error method is used to report problems with
      * holes in the data, for formats that cannot cope with them.
      */
    void fatal_hole_error(unsigned long lo, unsigned long hi);

    /**
      * The data_address_too_large method is used to report fatal
      * problems with data records, in the case where some or all of the
      * address span of a record falls outside the format's ability to
      * represent addresses.
      *
      * @param record
      *     The offending data record.
      * @param nbits
      *     The number of bits in the available address range
      */
    void data_address_too_large(const record &record, unsigned nbits) const;

private:
    /**
      * The position instance variable is used to remember the
      * current position within the output file.  Set by the put_char
      * method, and the seek_to method.  Used by the seek_to method.
      */
    unsigned long position;

    /**
      * The is_regular instance variable is used to remember whther
      * or not the file is a regular file.  This is set by the
      * set_is_regular method.  It is used by the seek_to method.
      */
    bool is_regular;

    /**
      * The set_is_regular method shall be used whenever vfp is assigned,
      * to estanblish whther the output file is a regular file or a
      * special file (like a pipe).
      */
    void set_is_regular(void);

    /**
      * The get_fp method is used to get the stdio file pointer
      * associated with this input file.  (By avoiding a FILE*
      * declaraion, we avoid having to include <stdio.h> for not
      * particularly good reason.  Take care when casting.)
      *
      * If the file has not been opened yet, it will be opened by this
      * method.
      */
    void *get_fp(void);

    /**
      * The is_binary method is used to to determine whether or not
      * a file format is binary (true) of text (false).  The default
      * implementation always returns false (text).
      */
    virtual bool is_binary(void) const;

    /**
      * The copy constructor.  Do not use.
      */
    output_file(const output_file &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file &operator=(const output_file &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_H
