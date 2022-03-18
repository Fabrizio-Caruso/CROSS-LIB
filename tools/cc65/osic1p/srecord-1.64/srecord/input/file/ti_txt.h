//
// srecord - manipulate eprom load files
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

#ifndef SRECORD_INPUT_FILE_ti_txt_H
#define SRECORD_INPUT_FILE_ti_txt_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_ti_txt class is used to repesent the parse
  * state when reading a ti_txt (MSP430) format input file.
  */
class input_file_ti_txt:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_ti_txt();

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
    input_file_ti_txt(const std::string &file_name);

    /**
      * The garbage_warning instance variable is used to remember whether
      * a warning has already been issued if the file contains garbage.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * any data has been seen in the input to date.
      */
    bool seen_some_input;

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file, so it may be associated with data bytes.
      */
    unsigned long address;

    enum token_t
    {
        token_start_up,
        token_at,
        token_end_of_file,
        token_junk,
        token_number,
        token_q
    };

    /**
      * the token instance variable is used to remember the type of the
      * most recent token, as determined by the get_next_token method.
      */
    token_t token;

    /**
      * the token_value instance variable is used to remember the value
      * of the most recent token, as determined by the get_next_token
      * method.  Only meaningful for token_number, zero otherwise.
      */
    unsigned long token_value;

    /**
      * The address_warning instance variable is used to remember
      * whether or not we have already warned about addresses which are
      * too large.
      */
    bool address_warning;

    /**
      * The get_next_token method is used to partition the input into
      * the next symbol.  All the digits of a hexadecimal number are
      * considered a single symbol.
      */
    void get_next_token(void);

    /**
      * The default constructor.  Do not use.
      */
    input_file_ti_txt();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_ti_txt(const input_file_ti_txt &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_ti_txt &operator=(const input_file_ti_txt &);
};

};

#endif // SRECORD_INPUT_FILE_ti_txt_H
// vim: set ts=8 sw=4 et :
