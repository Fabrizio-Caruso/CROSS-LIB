//
// srecord - manipulate eprom load files
// Copyright (C) 2002, 2006-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_OS65V_H
#define SRECORD_OUTPUT_FILE_OS65V_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_os65v class is used to represent the file state
  * of an OS65V output file, in OS65V format.
  */
class output_file_os65v:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_os65v();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_os65v(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation
    void write(const record &);

    // See base class for documentation
    void line_length_set(int);

    // See base class for documentation
    void address_length_set(int);

    // See base class for documentation
    int preferred_block_size_get() const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation
    const char *mode(void) const;

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The address instance variable is used to remember the current
      * file location.
      */
    unsigned long address;

    /**
      * The output mode is either address mode ('.') or data mode ('/').
      * Initially the file is in an unidentified mode (NUL).
      */
    char state;

    /**
      * The seen_start_address instance variable is used to remember
      * whether or not an execution start address has been seen.
      * Normally this is only given at the end of data.  This variable
      * is used to determine whether to emit thre "return to monitor"
      * sequence, if a "GO" command has not been issued.
      */
    bool seen_start_address;

    /**
      * The default constructor.  Do not use.
      */
    output_file_os65v();

    /**
      * Copy constructor.  Do not use.
      */
    output_file_os65v(const output_file_os65v &);

    /**
      * Assignment operator.  Do not use.
      */
    output_file_os65v &operator=(const output_file_os65v &);
};

};

#endif // SRECORD_OUTPUT_FILE_OS65V_H
