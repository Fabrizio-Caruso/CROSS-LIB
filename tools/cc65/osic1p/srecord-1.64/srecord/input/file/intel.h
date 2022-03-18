//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2000, 2002, 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
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

#ifndef SRECORD_INPUT_FILE_INTEL_H
#define SRECORD_INPUT_FILE_INTEL_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The srecord::input_file_intel class is used to represent the parse state
  * of an Intel Hex formatted file.
  */
class input_file_intel:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_intel();

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
    input_file_intel(const std::string &file_name);

    /**
      * Read one record from the file.  The read method is a wrapper
      * around this method.
      */
    bool read_inner(record &);

    /**
      * The data_record_count instance variable is used to remember the
      * number of data records seen to date.
      */
    int data_record_count;

    /**
      * The garbage_warning instance variable is used to remember wherther
      * or not a warning has already been issued about garbage lines
      * of input.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to remember whether
      * or not the file contains any data.
      */
    bool seen_some_input;

    /**
      * The termination_seen instance variable is used to remember
      * whether or not a termination record has been seen yet.
      */
    bool termination_seen;

    enum mode_t
    {
        mode_linear, // aka i32hex
        mode_segmented, // aka i16hex
        mode_i8hex
    };

    /**
      * The mode instance variable is used to remember what addressing
      * mode the file is currently in.
      */
    mode_t mode;

    /**
      * The address_base instance variable is used to remember the
      * segment base address when in segmented addressing mode.
      */
    unsigned long address_base;

    /**
      * The pushback instance variable is used to remember the previous
      * record in the file.  This is needed in some instances, but not always.
      */
    record *pushback;

    /**
      * The end_seen instance variable is used to remember whether or
      * not the end of file has been seen yet.
      */
    bool end_seen;

    /**
      * The default constructor.  Do not use.
      */
    input_file_intel();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_intel(const input_file_intel &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_intel &operator=(const input_file_intel &);
};

};

#endif // SRECORD_INPUT_FILE_INTEL_H
// vim: set ts=8 sw=4 et :
