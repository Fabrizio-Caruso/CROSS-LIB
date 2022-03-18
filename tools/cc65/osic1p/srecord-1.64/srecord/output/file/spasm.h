//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_SPASM_H
#define SRECORD_OUTPUT_FILE_SPASM_H

#include <srecord/endian.h>
#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_spasm class is used to represent an output
  * files in SPASM format.
  */
class output_file_spasm:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_spasm();

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      * @param end
      *     The byte order of the file
      */
    output_file_spasm(const std::string &file_name, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be written.
      * @param end
      *     The byte order of the file
      */
    static pointer create(const std::string &file_name,
        endian_t end = endian_big);

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
    const char *format_name() const;

private:
    /**
      * The end instance variable is used to remember the byte order of
      * the file.
      */
    endian_t end;

    /**
      * The default constructor.  Do not use.
      */
    output_file_spasm();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_spasm(const output_file_spasm &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_spasm &operator=(const output_file_spasm &);
};

};

#endif // SRECORD_OUTPUT_FILE_SPASM_H
