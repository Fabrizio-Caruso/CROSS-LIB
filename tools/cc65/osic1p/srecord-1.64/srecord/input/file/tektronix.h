//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_TEKTRONIX_H
#define SRECORD_INPUT_FILE_TEKTRONIX_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_tektronix clas sis used to represent the parse
  * state of an input file in Tektronix (non-extended) format.
  */
class input_file_tektronix:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_tektronix();

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
    input_file_tektronix(const std::string &file_name);

    /**
      * The get_nibble method gets a single hex-digit from input.
      * We override the one in the base class because the checksum is
      * nibble-based, not byte-based.
      */
    int get_nibble(void);

    /**
      * The get_byte method gets a two hex-digit from input and assembles
      * them (big-endian) into a byte.  We override the one in the base
      * class because the checksum is nibble-based, not byte-based.
      */
    int get_byte(void);

    /**
      * The read_inner methos is used to read a single record from
      * the input.  The read method is a wrapper around this method.
      */
    bool read_inner(record &);

    /**
      * The data_record_count instance variable is used to remember the
      * number of data records seen so far.
      */
    int data_record_count;

    /**
      * The garbage_warning instance variable is used to remember whether
      * a warning about input line containing garbage has been issued yet.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember wheter
      * the file contains any data so far.
      */
    bool seen_some_input;

    /**
      * The termination_seen instance variable is used to remember
      * whether the termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The default constructor.  Do not use.
      */
    input_file_tektronix();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_tektronix(const input_file_tektronix &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_tektronix &operator=(const input_file_tektronix &);
};

};

#endif // SRECORD_INPUT_FILE_TEKTRONIX_H
// vim: set ts=8 sw=4 et :
