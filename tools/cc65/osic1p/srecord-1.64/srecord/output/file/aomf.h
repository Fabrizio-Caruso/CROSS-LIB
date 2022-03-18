//
// srecord - manipulate eprom load files
// Copyright (C) 2004, 2006-2008, 2010, 2012 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_AOMF_H
#define SRECORD_OUTPUT_FILE_AOMF_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_aomf class is used to represent the output
  * state of a file in Intel Absolute Object Module Format (AOMF).
  */
class output_file_aomf:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_aomf();

private:
    /**
      * A constructor.  The input will be read from the named file
      * (or the standard input if the file name is "-").
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_aomf(const std::string &file_name);

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
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    int preferred_block_size_get(void) const;

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    const char *format_name(void) const;

private:
    /**
      * The emit_record method is used to emit records in the AOMF format.
      * Each has an 8-but type, a 16-bit little-endian length, a payload,
      * and an 8-bit 2s complement checksum.
      */
    void emit_record(int, const unsigned char *, size_t);

    /**
      * The module_header_record method is used to write an AOMF Module
      * Header Record.
      */
    void module_header_record(const char*);

    /**
      * The content_record method is used to write an AOMF Content Record.
      */
    void content_record(unsigned long address, const unsigned char *data,
        size_t length);

    /**
      * The module_header_record method is used to write an AOMF Module
      * End Record.
      */
    void module_end_record(const char*);

    /**
      * See base class for documentation.  We are over-riding it
      * because we use raw binary, so we call the #put_char method.
      * This method also tracks the byte_offset, so that we can
      * align to specific boundaries.  Calls the #checksum_add method.
      */
    void put_byte(unsigned char);

    /**
      * The byte_offset instance variable is used to track the location
      * in the output file.  Maintained by the #put_byte method.
      */
    unsigned long byte_offset;

    /**
      * The module_name instance variable is used to remember the
      * information form the Module Header Record for reproduction in
      * the Module End Record (they are required to agree).
      */
    std::string module_name;

    /**
      * The copy constructor.  Do not use.
      */
    output_file_aomf(const output_file_aomf &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_aomf &operator=(const output_file_aomf &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_AOMF_H
