//
// srecord - manipulate eprom load files
// Copyright (C) 2012, 2013 Peter Miller
//
// Code contribution by Eric Smith <eric@brouhaha.com>
// Copyright assigned to Peter Miller 15-Mar-2012.
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_TRS80_H
#define SRECORD_INPUT_FILE_TRS80_H


#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_trs80 class is used to parse a Radio Shack
  * TRS-80 load module file.
  */
class input_file_trs80:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_trs80();

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
    bool read(record &result);

    // See base class for documentation.
    const char *get_file_format_name(void) const;

    // See base class for documentation.
    bool is_binary(void) const;

    // See base class for documentation.
    int format_option_number(void) const;

private:
    /**
      * The termination_seen instance variable is used to remember
      * whether or not a termination record has been seen yet.
      */
    bool termination_seen;

    /**
      * The data_seen instance variable is used to remember
      * whether or not a data record has been seen yet.
      */
    bool data_seen;

    /**
      * A constructor.  The input is read from the named file (or
      * the standard input if the file anme is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_trs80(const std::string &file_name);

    /**
      * The get_byte method is used to fetch a byte of input, and
      * update the checksum.  We over-ride the base implementation,
      * because we use raw bytes rather than two hex digits.
      */
    int get_byte(void);

    /**
      * The pending instance variable is used to remember the second
      * half of large data packets, in the case where they must be splt.
      */
    record *pending;

    /**
      * The copy constructor.  Do not use.
      */
    input_file_trs80(const input_file_trs80 &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_trs80 &operator=(const input_file_trs80 &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILE_TRS80_H
