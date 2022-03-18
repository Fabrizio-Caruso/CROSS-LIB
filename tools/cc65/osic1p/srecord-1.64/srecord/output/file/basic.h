//
// srecord - manipulate eprom load files
// Copyright (C) 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_BASIC_H
#define SRECORD_OUTPUT_FILE_BASIC_H

#include <srecord/output/file.h>
#include <srecord/interval.h>

namespace srecord
{

/**
  * The srecord::output_file_basic class is used to emit record in BASIc
  * format, using DATA statements.
  */
class output_file_basic:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_basic();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_basic(const std::string &file_name);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for docum,entation.
    void write(const record &);

    // See base class for docum,entation.
    void line_length_set(int);

    // See base class for docum,entation.
    void address_length_set(int);

    // See base class for docum,entation.
    int preferred_block_size_get() const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The taddr instance variabel is used to remember the
      * termination address, to be emitted in the footer.
      */
    unsigned long taddr;

    /**
      * The range instance variable is used to remember the range
      * of addresses present in the output.
      */
    interval range;

    /**
      * The column instance variable is used to remember the current
      * printing column on the line.
      */
    int column;

    /**
      * The current_address instance variabel is used to remember
      * the current address that the file is positioned at.  This is
      * used to know whether we need to add padding.
      */
    unsigned long current_address;

    /**
      * The line_length instance variable is used to remember the
      * maximum line length.  The output usually does not exceed it.
      */
    int line_length;

    /**
      * The emit_byte method is used to emit a single byte.  It uses
      * column to track the position, so as not to exceed line_length.
      */
    void emit_byte(int);

    /**
      * The default constructor.  Do not use.
      */
    output_file_basic();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_basic(const output_file_basic &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_basic &operator=(const output_file_basic &);
};

};

#endif // SRECORD_OUTPUT_FILE_BASIC_H
