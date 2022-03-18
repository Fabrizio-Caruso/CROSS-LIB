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

#ifndef SRECORD_INPUT_FILE_SPECTRUM_H
#define SRECORD_INPUT_FILE_SPECTRUM_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_spectrum class is used to represent the parse
  * state when reading from a file in Spectrum format.
  */
class input_file_spectrum:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_spectrum();

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
    input_file_spectrum(const std::string &file_name);

    /**
      * The header_seen instance variable is used to remember whether
      * the file header (the start character STX=0x02) has been seen yet.
      */
    bool header_seen;

    /**
      * The trailer_seen instance variable is used to remember whether
      * the file trailer (the end character ETX=0x03) has been seen yet.
      */
    bool trailer_seen;

    /**
      * The file_contains_data instance variable is used to remember
      * whether any file data has been seen yet.
      */
    bool file_contains_data;

    /**
      * The get_decimal method is used to get a decimal number from
      * the input.  It must have at least one digit.
      */
    int get_decimal(void);

    /**
      * The get_binary method is used to get a binary number from
      * the input.  It must have at least one digit.
      */
    int get_binary(void);

    /**
      * The default constructor.  Do not use.
      */
    input_file_spectrum();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_spectrum(const input_file_spectrum &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_spectrum &operator=(const input_file_spectrum &);
};

};

#endif // SRECORD_INPUT_FILE_SPECTRUM_H
// vim: set ts=8 sw=4 et :
