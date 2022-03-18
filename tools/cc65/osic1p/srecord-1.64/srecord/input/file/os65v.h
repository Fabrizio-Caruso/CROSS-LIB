//
// srecord - manipulate eprom load files
// Copyright (C) 2002, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_OS65V_H
#define SRECORD_INPUT_FILE_OS65V_H


#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_os65v class is used to represent the input state
  * of an Ohio Scientific hexadecimal formatted file.
  */
class input_file_os65v:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_os65v();

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
    input_file_os65v(const std::string &file_name);

    /**
      * The default constructor.  Do not use.
      */
    input_file_os65v();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_os65v(const input_file_os65v &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_os65v &operator=(const input_file_os65v &);

    /**
      * The read_inner method is used by the read method to get
      * another portion of input.
      */
    bool read_inner(record &);

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not some valid inpout has been seen.
      */
    bool seen_some_input;

    /**
      * The address instance variable is used to remember the current
      * address.  It is advanced for every byte read.
      */
    unsigned long address;

    /**
      * The state instance variable is used to member the current input
      * state (unknown, address, data).
      */
    char state;

    /**
      * The ignore_the_rest instance variable is used to remember when
      * it is time to ignore the rest of the file.  This happens after a
      * "return to monitor" sequence, or after a "GO" command.
      */
    bool ignore_the_rest;
};

};

#endif // SRECORD_INPUT_FILE_OS65V_H
// vim: set ts=8 sw=4 et :
