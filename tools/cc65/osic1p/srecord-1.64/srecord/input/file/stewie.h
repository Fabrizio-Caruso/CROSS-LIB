//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2000, 2002-2004, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef LIB_INPUT_FILE_STEWIE_H
#define LIB_INPUT_FILE_STEWIE_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_stewie class is used to represent the parse
  * state of an input file in an undocumented binary format loosely
  * based on the Motorola format.
  */
class input_file_stewie:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_stewie();

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
    bool read(record &record);

    // See base class for documentation.
    const char *get_file_format_name(void) const;

    // See base class for documentation.
    bool is_binary(void) const;

    // See base class for documentation.
    int get_byte(void);

    // See base class for documentation.
    int format_option_number(void) const;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_stewie(const std::string &file_name);

    /**
      * The data_count instance variable is used to remember the number
      * of data lines has occurred so far in the input file.
      */
    unsigned long data_count;

    /**
      * The read_inner method is used to read a record of input.
      * The read method is a wrapper around this method.
      */
    bool read_inner(record &);

    /**
      * The garbage_warning instance variable is used to remember whether
      * or not a warning about garbage input lines has been issued yet.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember where
      * any data has been seen in this file yet.
      */
    bool seen_some_input;

    /**
      * The header_seen instance variable is used to remember whether
      * or not the header record has been seen yet.
      */
    bool header_seen;

    /**
      * The termination_seen instance variable is used to remember
      * whether or not the termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The default constructor.  Do not use.
      */
    input_file_stewie();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_stewie(const input_file_stewie &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_stewie &operator=(const input_file_stewie &);
};

};

#endif // LIB_INPUT_FILE_STEWIE_H
// vim: set ts=8 sw=4 et :
