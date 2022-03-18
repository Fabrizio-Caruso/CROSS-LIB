//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_FORMATTED_BINARY_H
#define SRECORD_INPUT_FILE_FORMATTED_BINARY_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_formatted_binary class is used to represent the
  * parse state when reading a file in the Formatted Binary format.
  */
class input_file_formatted_binary:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_formatted_binary();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    bool read (record &record);

    // See base class for documentation.
    const char *get_file_format_name(void) const;

    // See base class for documentation.
    bool is_binary(void) const;

    // See base class for documentation.
    int format_option_number(void) const;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_formatted_binary(const std::string &file_name);

    /**
      * The header_seen instance variable is used to remember whether
      * the file header has been seen yet.
      */
    bool header_seen;

    /**
      * The upper_bound instance variable is used to remember how long
      * the header said the file was going to be.
      */
    unsigned long upper_bound;

    /**
      * The address instance variable is used to remember where we are
      * up to in extracting the data from the file.
      */
    unsigned long address;

    /**
      * The trailer_seen instance variable is used to remember whether
      * the file trailer has been seen yet.
      */
    bool trailer_seen;

    /**
      * The check_sum instance variable is used to remember the simple
      * sum of the data bytes in the file.
      */
    unsigned short check_sum;

    /**
      * The default constructor.
      */
    input_file_formatted_binary();

    /**
      * The copy constructor.
      */
    input_file_formatted_binary(const input_file_formatted_binary &);

    /**
      * The assignment operator.
      */
    input_file_formatted_binary &operator=(const input_file_formatted_binary &);
};

};

#endif // SRECORD_INPUT_FILE_FORMATTED_BINARY_H
// vim: set ts=8 sw=4 et :
