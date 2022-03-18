//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_ASCII_HEX_H
#define SRECORD_OUTPUT_FILE_ASCII_HEX_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_ascii_hex class is used to write a file in
  * Ascii-Hex-Space format.
  */
class output_file_ascii_hex:
    public output_file
{
public:
    /**
      * The destrutcor.
      */
    virtual ~output_file_ascii_hex();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_ascii_hex(const std::string &file_name);

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
    const char *format_name() const;

private:
    /**
      * The address instance variable is used to remember where we are
      * up to in the output.  Used to limit the number of $A line emitted.
      */
    unsigned long address;

    /**
      * The column instance variable is used to remember which column
      * we are up to in the output.  Used to limit the length of lines
      * in the output.
      */
    int column;

    /**
      * The pref_block_size instance variable is used to remember the
      * number of bytes in the preferred block size.
      */
    int pref_block_size;

    /**
      * The line_length instance variable is used to remember the
      * maximum length of text lines.
      */
    int line_length;

    /**
      * The address_length instance variable is used to remember
      * how many bytes of addresses are are to emit.
      */
    int address_length;

    /**
      * The start_code_emitted instance variable is used to remember
      * whether or not the start code (Ctrl-B) has been emitted.
      */
    bool start_code_emitted;

    /**
      * The end_code_emitted instance variable is used to remember
      * whether or not the end code (Ctrl-C) has been emitted.
      */
    bool end_code_emitted;

    /**
      * The emit_end_of_file method is used to write the ETX and
      * checksum out to the file.  It is safe top call this method more
      * than once, only one ETX will be written.
      */
    void emit_end_of_file();

    /**
      * The default constructor.  Do not use.
      */
    output_file_ascii_hex();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_ascii_hex(const output_file_ascii_hex &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_ascii_hex &operator=(const output_file_ascii_hex &);
};

};

#endif // SRECORD_OUTPUT_FILE_ASCII_HEX_H
