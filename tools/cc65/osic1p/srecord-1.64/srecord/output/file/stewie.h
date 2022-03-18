//
// srecord - manipulate eprom load files
// Copyright (C) 2004, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_STEWIE_H
#define SRECORD_OUTPUT_FILE_STEWIE_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_stewie class is used to represent the current
  * state of an output file in an undocumented binary format loosely
  * based on the Motorola format.
  */
class output_file_stewie:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_stewie();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_stewie(const std::string &file_name);

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
    void put_byte(unsigned char);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
      * The data_count instance variable is used to remember how many
      * data records have occurred so far in the output file.
      */
    unsigned long data_count;

    /**
      * The address_length instance variable is used to remember the
      * minimum number of bytes to use for addresses.
      */
    int address_length;

    /**
      * The preferred_block_size instance variable is used to remember
      * the prefrred block size for records.
      */
    int preferred_block_size;

    /**
      * Write a data record.
      */
    void write_inner(int, unsigned long, int, const void *, int);

    /**
      * The default constructor.  Do not use.
      */
    output_file_stewie();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_stewie(const output_file_stewie &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_stewie &operator=(const output_file_stewie &);
};

};

#endif // SRECORD_OUTPUT_FILE_STEWIE_H
