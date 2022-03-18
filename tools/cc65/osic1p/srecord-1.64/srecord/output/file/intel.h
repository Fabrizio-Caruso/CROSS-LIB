//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2003, 2006-2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_INTEL_H
#define SRECORD_OUTPUT_FILE_INTEL_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_intel class is used to write an EPROM load file
  * in Intel Hex format.
  */
class output_file_intel:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_intel();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_intel(const std::string &file_name);

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
    int preferred_block_size_get(void) const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    const char *format_name(void) const;

private:
    /**
      * The write_inner method is used to write a single line to the
      * output file.
      */
    void write_inner(int, unsigned long, const void *, int);

    /**
      * The address_base instance variable is used to remember the
      * current position within the output file
      */
    unsigned long address_base;

    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of bytes on each line of the output file.
      */
    int pref_block_size;

    enum mode_t
    {
        mode_linear, // aka i32hex
        mode_segmented, // aka i16hex
        mode_i8hex
    };

    /**
      * The mode instance variable is used to remember what addressing
      * mode the file is currently in.  If set to "segmented" (via the
      * address_length_set method) you get 16-bit MCS-86 output (record
      * type 2, extended segment address record).  The default value of
      * "linear" gets you 32-bit output (record type 4, extended linear
      * address record).
      */
    mode_t mode;

    /**
      * The default constructor.  Do not use.
      */
    output_file_intel();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_intel(const output_file_intel &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_intel &operator=(const output_file_intel &);
};

};

#endif // SRECORD_OUTPUT_FILE_INTEL_H
