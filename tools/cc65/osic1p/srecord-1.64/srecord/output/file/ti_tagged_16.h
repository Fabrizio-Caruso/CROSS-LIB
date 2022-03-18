//
// srecord - The "srecord" program.
// Copyright (C) 2007, 2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_TI_TAGGED_16_H
#define SRECORD_OUTPUT_FILE_TI_TAGGED_16_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_ti_tagged_16 class is used to represent the
  * output state of a file in Texas Instruments SDSMAC (320) format.
  */
class output_file_ti_tagged_16:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_ti_tagged_16();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_ti_tagged_16(const std::string &file_name);

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
    // See base class for documentation.
    void write(const record &);

    // See base class for documentation.
    void line_length_set(int);

    // See base class for documentation.
    void address_length_set(int);

    // See base class for documentation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    void put_char(int);

    // See base class for documentation.
    const char *format_name() const;

private:
    typedef output_file inherited;

    /**
      * The address instance variable is used to remember the current
      * address within the output file.
      */
    unsigned long address;

    /**
      * The column instance variable is used to remember the current
      * text column within the output file.  This is so that we can
      * decide when to throw a new line.
      */
    int column;

    /**
      * The line_length instance variable is used to remember the maximum
      * permitted text line length.  This is so that we can decide when
      * to throw a new line.
      */
    int line_length;

    /**
      * The csum instance variable is used to remember the 16-bit running
      * total of the bytes emitted so far.  It gets reset by put_eoln.
      */
    int csum;

    /**
      * The put_eoln method is used to output the line termination,
      * which includes a checksum.
      */
    void put_eoln();

    /**
      * The default constructor.  Do not use.
      */
    output_file_ti_tagged_16();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_ti_tagged_16(const output_file_ti_tagged_16 &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_ti_tagged_16 &operator=(const output_file_ti_tagged_16 &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_TI_TAGGED_16_H
