//
// srecord - Manipulate EPROM load files
// Copyright (C) 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_PPX_H
#define SRECORD_OUTPUT_FILE_PPX_H

#include <srecord/output/file.h>

namespace srecord {

/**
  * The output_file_ppx class is used to represent the processing
  * required to write a file in PPX or Stag Hex format.
  */
class output_file_ppx:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_ppx();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    void write(const record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get(void) const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    const char *format_name(void) const;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_ppx(const std::string &file_name);

    /**
      * The started instance variable is used to remember whether or not
      * we have emitted the start sequence.
      */
    bool started;

    /**
      * The address instance variable is used to remember the address of
      * the next data byte to be parsed.
      */
    unsigned long address;

    /**
      * The line_length instance variable is used to remember how long
      * the output lines are to be.
      */
    int line_length;

    /**
      * The column instance variable is used to remember the current
      * output column.  Used for determining when to start a new line.
      */
    int column;

    /**
      * The dsum instance variable is used to remember the simple sum of
      * all the data bytes, but not the address bytes.
      */
    unsigned short dsum;

    /**
      * The default constructor.  Do not use.
      */
    output_file_ppx();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_ppx(const output_file_ppx &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_ppx &operator=(const output_file_ppx &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_PPX_H
