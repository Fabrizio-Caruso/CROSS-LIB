//
// srecord - The "srecord" program.
// Copyright (C) 2007, 2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_HEXDUMP_H
#define SRECORD_OUTPUT_FILE_HEXDUMP_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_hexdump class is used to represent an output
  * file which emits a hexadecimal dump (including ASCII) of the data.
  */
class output_file_hexdump:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_hexdump();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_hexdump(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
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
    int preferred_block_size_get(void) const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    void command_line(arglex_tool *cmdln);

    // See base class for documentation.
    const char *format_name(void) const;

private:
    /**
      * The number_of_columns instance variable is used to remember how
      * many columns of hex bytes to display on a single line.  It is
      * always a power of two.
      */
    int number_of_columns;

    /**
      * The row_cache_address instance variable is used to remember
      * where we are up to in printing our rows of data.  It is the
      * address of the beginning of the row, NOT the current byte.
      * The lower "number_of_columns" bits are always zero.
      */
    unsigned long row_cache_address;

    /**
      * The row_cache_address_mask instance variable is used to remember
      * the mask to calculate the column within the output line, give a
      * byte address.
      */
    unsigned long row_cache_address_mask;

    /**
      * The row_cache_size instance variable is used to remember the
      * number of printing columns wide the output is.
      */
    size_t row_cache_size;

    /**
      * The row_cache instance variable is used to remember the text of
      * the line to be printed.  It contains ASCII hex representations
      * of the bytes "written" to this output.
      */
    char *row_cache;

    /**
      * The address_length instance variable is used to remember how
      * many bytes to emit when emitting addresses.
      */
    int address_length;

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_byte(unsigned long address, unsigned char data);

    /**
      * The row_cache_print method is used to print the row cache to the
      * file and then erase the cache in preparation for another row.
      */
    void row_cache_print(void);

    /**
      * The columns_to_line_length method is used to
      */
    int columns_to_line_length(int cols);

    /**
      * The default constructor.  Do not use.
      */
    output_file_hexdump();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_hexdump(const output_file_hexdump &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_hexdump &operator=(const output_file_hexdump &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_HEXDUMP_H
