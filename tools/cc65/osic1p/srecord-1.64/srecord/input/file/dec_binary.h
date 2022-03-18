//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2003, 2006-2008, 2010-2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_DEC_BINARY_H
#define SRECORD_INPUT_FILE_DEC_BINARY_H


#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_dec_binary class is used to parse a DEC
  * Binary (PDP 11 absolute loader XXDP) format file.
  */
class input_file_dec_binary:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_dec_binary();

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
    int format_option_number(void) const;

private:
    /**
      * A constructor.  The input is read from the named file (or
      * the standard input if the file anme is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_dec_binary(const std::string &file_name);

    /**
      * The get_byte method is used to fetch a byte of input, and
      * update the checksum.  We over-ride the base implementation,
      * because we use raw bytes rather than two hex digits.
      */
    int get_byte();

    /**
      * This format has NUL characters for optional badding around
      * blocks.  This method is used to skip past such padding.
      * Returns true if there is more inoput available, or false at
      * end of file.
      */
    bool skip_nul();

    /**
      * The current_pos instance variable is used to track out
      * position within the current record.  We need todo this
      * becase DEC Binary records can be significantly longer than
      * other formats.
      */
    unsigned long current_pos;

    /**
      * The current_length instance variable is used to remember
      * the length of the current record.  It is zero if there is no
      * "current" record.
      */
    unsigned long current_length;

    /**
      * The current_address instance variable is used to track the
      * load address of the current record.  It is updated each time
      * we return a partial block, so that we alsoways return the
      * correct load address.
      */
    unsigned long current_address;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_dec_binary(const input_file_dec_binary &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_dec_binary &operator=(const input_file_dec_binary &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILE_DEC_BINARY_H
