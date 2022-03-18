//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_MIPS_FLASH_H
#define SRECORD_INPUT_FILE_MIPS_FLASH_H

#include <srecord/endian.h>
#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_mips_flash class is used to represent the
  * parse state of a MIPS-Flash formatted input file.
  */
class input_file_mips_flash:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_mips_flash();

    /**
      * The create_be class method is used to create new dynamically
      * allocated instances of this class, big-endian byte order.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create_be(const std::string &file_name);

    /**
      * The create_le class method is used to create new dynamically
      * allocated instances of this class, little-endian byte order.
      * This is used by the --guess format.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create_le(const std::string &file_name);

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
      * @param end
      *     The byte order.
      */
    input_file_mips_flash(const std::string &file_name,
        endian_t end = endian_big);

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
      * The endian instance variable is used to remember whether the
      * file is big-endian or little-endian.
      */
    endian_t endian;

    /**
      * The address instance variable is used to remember where we are
      * up to in the input file, for when we build the next data record.
      */
    unsigned long address;

    /**
      * The tokenizer method is used to determine the next token in the
      * input token stream.  It sets #token and #token_value before
      * it returns.  If it finds an error it will issue a #fatal_error
      * message and not return.
      */
    void tokenizer(void);

    enum token_t
    {
        token_eof,
        token_at,
        token_erase,
        token_lock,
        token_number,
        token_reset,
        token_unlock,
    };

    /**
      * The token instance variable is used to remember the kind of
      * token was found by the preceeding #tokenizer call.
      */
    token_t token;

    /**
      * The token_value instance variable is used to remember the value
      * of the number, if the preceeding #tokenizer call saw a number.
      * Otherwise, its value is undefined.
      */
    unsigned long token_value;

    /**
      * The seen_reset instance variable is used to remember
      * whether or not the initial '!R' token has already been seen.
      */
    bool seen_reset;

    /**
      * The default constructor.  Do not use.
      */
    input_file_mips_flash();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_mips_flash(const input_file_mips_flash &);

    /**
      * The assigmne toperator.  Do not use.
      */
    input_file_mips_flash &operator=(const input_file_mips_flash &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILE_MIPS_FLASH_H
