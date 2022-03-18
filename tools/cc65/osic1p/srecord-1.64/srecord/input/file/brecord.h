//
// srecord - The "srecord" program.
// Copyright (C) 2007, 2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_BRECORD_H
#define SRECORD_INPUT_FILE_BRECORD_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_brecord class is used to represent the parse
  * state of a Motorola MC68EZ328 bootstrap b-record format input file.
  */
class input_file_brecord:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_brecord();

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
    int format_option_number(void) const;

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_brecord(const std::string &file_name);

    /**
      * The read_inner method is used to read a record from the file.
      * The read method is a wrapper around it.
      */
    bool read_inner(record &);

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not any data has been seen from this file to date.
      */
    bool seen_some_input;

    /**
      * The default constructor.  Do not use.
      */
    input_file_brecord();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_brecord(const input_file_brecord &);

    /**
      * The assigmne toperator.  Do not use.
      */
    input_file_brecord &operator=(const input_file_brecord &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILE_BRECORD_H
