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

#ifndef SRECORD_INPUT_FILE_COSMAC_H
#define SRECORD_INPUT_FILE_COSMAC_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_cosmac class is used to represent the parse
  * state when reading an RCS Cosmac formatted file.
  */
class input_file_cosmac:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_cosmac();

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
    input_file_cosmac(const std::string &file_name);

    /**
      * The state instance variable is used to remember where we are in
      * parsing the !M command.
      *
      *   \n  no command seen yet
      *   !   have seen the ! character
      *   M   have seen the !M characters, want an address
      *   sp  have seen !Maaaa, want data bytes
      *   ,   have seen , charcter, discarding to \n, then resume sp state
      *   ;   have seen , charcter, discarding to \n, then resume M state
      */
    char state;

    /**
      * The address instance variable is used to remember what address
      * we are up to in the input.
      */
    unsigned long address;

    /**
      * The seen_some_input instance variable is used to remember whether
      * the file has any contents or not.
      */
    bool seen_some_input;

    /**
      * The default constructor.  Do not use.
      */
    input_file_cosmac();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_cosmac(const input_file_cosmac &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_cosmac &operator=(const input_file_cosmac &);
};

};

#endif // SRECORD_INPUT_FILE_COSMAC_H
// vim: set ts=8 sw=4 et :
