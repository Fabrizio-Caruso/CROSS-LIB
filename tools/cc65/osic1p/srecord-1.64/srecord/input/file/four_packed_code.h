//
// srecord - manipulate eprom load files
// Copyright (C) 2001-2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_FOUR_PACKED_CODE_H
#define SRECORD_INPUT_FILE_FOUR_PACKED_CODE_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_four_packed_code represents an input file in
  * the four packed code (FPC) format.  For extra points: who invented
  * this format?  Where is it used?
  */
class input_file_four_packed_code:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_four_packed_code();

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
      * The constructor.  The input will be read from the named file
      * (or the standard input if the name is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_four_packed_code(const std::string &file_name);

    /**
      * The read_inner method is used to read a record (one line)
      * from the input file.  Lines which don't start with "$"
      * will be ignored.
      */
    bool read_inner(record &);

    /**
      * See base class for documentation.  We override the default
      * implementation because we have to decode 5 characters (four
      * bytes) at a time.  The usual get_word (etc) continue to work
      * without additional modifications.
      */
    int get_byte(void);

    /**
      * The get_digit method is used to fetch one base85 digit from
      * the input.
      */
    int get_digit(void);

    /**
      * The get_byte_pos instance variable is used by the get_byte
      * method to track where were are positioned within the
      * 5-character (4-byte) input multiple.  (Only the get_byte
      * method may use this instance variable.)
      */
    unsigned get_byte_pos;

    /**
      * The get_byte_value instance variable is used by the bet_byte
      * method to method to hold the value of a 5-character (4-byte)
      * input multiple.  (Only the get_byte method may use this
      * instance variable.)
      */
    unsigned long get_byte_value;

    /**
      * The garbage_waring instance variable is used by the read
      * method to record whether or not a warning about non-format
      * lines has been issued (only one warning is issued per file).
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used by the read
      * and read_inner methods to record whether or not any valid
      * input has been seen.
      */
    bool seen_some_input;

    /**
      * The running_address instance variable method is used by the
      * read_inner method to record the current address.  This is
      * so that record types 1 and 2 can be processed accurately.
      */
    unsigned long running_address;

    /**
      * The default constructor.  Do not use.
      */
    input_file_four_packed_code();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_four_packed_code(const input_file_four_packed_code &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_four_packed_code &operator=(const input_file_four_packed_code &);
};

};

#endif // SRECORD_INPUT_FILE_FOUR_PACKED_CODE_H
// vim: set ts=8 sw=4 et :
