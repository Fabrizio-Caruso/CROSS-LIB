//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_FAIRCHILD_H
#define SRECORD_OUTPUT_FILE_FAIRCHILD_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_fairchild class is used to represent the running
  * state when writing a file in Fairchild Fairbug format.
  */
class output_file_fairchild:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_fairchild();

private:
    /**
      * The constructor.  It is private on purpose, use the create class
      * method instead.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_fairchild(const std::string &file_name);

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
    int preferred_block_size_get() const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The put_nibble method puts a single hex-digit on the output.
      * We override the one in the base class because the checksum is
      * nibble-based, not byte-based.
      */
    void put_nibble(unsigned);

    /**
      * The put_byte method puts two hex-digits to the output.
      * We override the one in the base class because the checksum is
      * nibble-based, not byte-based.
      */
    void put_byte(unsigned char);

    /**
      * The address instance variable is used to remember the current
      * memory position within the output.
      */
    unsigned long address;

    /**
      * The default constructor.  Do not use.
      */
    output_file_fairchild();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_fairchild(const output_file_fairchild &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_fairchild &operator=(const output_file_fairchild &);
};

};

#endif // SRECORD_OUTPUT_FILE_FAIRCHILD_H
