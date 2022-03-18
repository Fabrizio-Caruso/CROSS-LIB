//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2003, 2005-2008, 2010 Peter Miller
// Copyright (C) 2014 Scott Finneran
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

#ifndef SRECORD_OUTPUT_FILE_C_H
#define SRECORD_OUTPUT_FILE_C_H

#include <srecord/output/file.h>
#include <srecord/interval.h>

namespace srecord
{

/**
  * The srecord::output_file_c class is used to represent an output file
  * which emits C code.
  */
class output_file_c:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_c();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    output_file_c(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    void command_line(arglex_tool *cmdln);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The prefix instance variable is used to remember the variable
      * name prefix to be used in the output.
      */
    std::string prefix;

    /**
      * The prefix instance variable is used to remember the definition
      * prefix to be emitted in the header.
      */
    std::string header_prefix;

    /**
      * The postfix instance variable is used to remember the definition
      * postfix to be emitted in the header.
      */
    std::string header_postfix;

    /**
      * The taddr instance variable is used to remember the
      * termination address, to be emitted in the footer.
      */
    unsigned long taddr;

    /**
      * The range instance variable is used to remember the range
      * of addresses present in the output.
      */
    interval range;

    /**
      * The header_done instance variable is used t remember whether
      * the emit_header method has been called.
      */
    bool header_done;

    /**
      * The column instance variable is used to remember the current
      * printing column on the line.
      */
    int column;

    /**
      * The current_address instance variabel is used to remember
      * the current address that the file is positioned at.  This is
      * used to know whether we need to add padding.
      */
    unsigned long current_address;

    /**
      * The line_length instance variable is used to remember the
      * maximum line length.  The output usually does not exceed it.
      */
    int line_length;

    /**
      * The address_length instance variable is used toremember how
      * many bytes to emit when emitting addresses.
      */
    int address_length;

    /**
      * The constant instance variable is used to remember whether or
      * not to use the "const" keyword.
      */
    bool constant;

    /**
      * The include instance variable is used to remember whether or not
      * to generate an include file.
      */
    bool include;

    /**
      * The include_file_name instance variable is used to remember the
      * name of the include file to be generated.
      */
    std::string include_file_name;

    /**
      * The output_word instance variable is used to remember whether or not
      * the input bytes should be emitted as word.
      */
    bool output_word;

    /**
      * The hex_style instance variable is used to remember whether or
      * not we are to output number in hexadecimal (true) or decimal
      * (false).
      */
    bool hex_style;

    /**
      * The section_style instance variable is used to remember whether
      * or not the output is to contain "sections".
      *
      * In non-section output, padding of 0xFF is used to pad the data
      * for correct addressing.  In section output, tables of addresses
      * and lenthgs are emitted, and the actual data is intended to be
      * relocated at run time.
      */
    bool section_style;

    /**
      * The emit_header method is used to emit the initial portion
      * of the array declaration.  It does nothing if header_done
      * is true.
      */
    void emit_header();

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_byte(int);

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_word(unsigned int);

    /**
      * The format_address method is used to format an address, taking
      * the hex_style and address_length instance variable settings.
      *
      * @param addr
      *     The adress to be formatted
      */
    std::string format_address(unsigned long addr);

    /**
      * The default constructor.  Do not use.
      */
    output_file_c();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_c(const output_file_c &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_c &operator=(const output_file_c &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_C_H
