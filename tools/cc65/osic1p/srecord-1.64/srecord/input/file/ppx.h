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

#ifndef SRECORD_INPUT_FILE_PPX_H
#define SRECORD_INPUT_FILE_PPX_H

#include <srecord/input/file.h>
#include <srecord/record.h>

namespace srecord {

/**
  * The input_file_hexdump class is used to represent the processing
  * required to read in a Stag Prog Programmer hexadecimal file.
  */
class input_file_ppx:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_ppx();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to be read.
      */
    static pointer create(const std::string &filename);

protected:
    // See base class for documentation.
    bool read(class record &rec);

    // See base class for documentation.
    const char *get_file_format_name(void) const;

    // See base class for documentation.
    int format_option_number(void) const;

private:
    /**
      * The constructor.
      *
      * @param filename
      *     The name of the file to be read.
      */
    input_file_ppx(const std::string &filename);

    /**
      * The state instance variable is used to remember the current
      * processing state as the file is progressively parsed.  The parse
      * is, of course, interrupted to return data records when they are seen.
      */
    int state;

    enum token_t
    {
        token_eof,
        token_star,
        token_address,
        token_byte,
        token_end,
        token_sum,
    };

    /**
      * The token instance variable is used to remember the kind of the
      * most recent token seen.  Set by the #get_next_token method.
      */
    token_t token;

    /**
      * The token_value instance variable is used to remember the value
      * of the most recent token_byte or token_address seen.
      * Set by the #get_next_token method.
      */
    unsigned token_value;

    /**
      * The get_next_token method is used to read the next lexical token
      * from the input.
      * It will set #token with the kind of token seen.
      * It will set the #token_value instance variable for token_byte
      * and token_address
      */
    void get_next_token(void);

    /**
      * The address instance variable is used to remember the current
      * address of the next data record.  This is set and advanced by
      * the #read method.
      */
    record::address_t address;

    /**
      * The data_seen instance variable is used to remember whether or
      * not any data has been seen in the file.  This is used to issue
      * an error when there is aparrently no data in the file, and this
      * helps #guess to figure out the file is notof this type.
      */
    bool data_seen;

    /**
      * The syntax_error method is a convenience wrapper around
      * #fatal_error to complain about syntax errors.
      */
    void syntax_error(void);

    /**
      * The dsum instance variable is used to remember the simple sum of
      * the data bytes, and the data bytes alone.
      */
    unsigned short dsum;

    /**
      * The buffer instance variable is used to remember the most recent
      * #buffer_length data bytes read from the file.
      */
    record::data_t buffer[record::max_data_length];

    /**
      * The buffer_length instance variable is used to remember the
      * number of data bytes in the #buffer array.
      */
    size_t buffer_length;

    /**
      * The default constructor.  Do not use.
      */
    input_file_ppx();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_ppx(const input_file_ppx &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_ppx &operator=(const input_file_ppx &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILE_PPX_H
