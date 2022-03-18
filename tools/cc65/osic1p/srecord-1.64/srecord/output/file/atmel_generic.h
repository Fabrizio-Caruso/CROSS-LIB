//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_ATMEL_GENERIC_H
#define SRECORD_OUTPUT_FILE_ATMEL_GENERIC_H

#include <srecord/endian.h>
#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_ascii_hex class is used to write a file in
  * Atmel Generic format.
  */
class output_file_atmel_generic:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_atmel_generic();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The file to write the data to, or "-" to mean the standard
      *     output.
      * @param end
      *     The byte order of the output.
      */
    output_file_atmel_generic(const std::string &file_name, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      * @param end
      *     The byte order of the output.
      */
    static pointer create(const std::string &file_name, endian_t end);

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
      * The end instance variable is used to remember the byte order
      * of the output, big-endian or little-endian.
      */
    endian_t end;

    /**
      * The default constructor.  Do not use.
      */
    output_file_atmel_generic();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_atmel_generic(const output_file_atmel_generic &);

    /**
      * The copy constructor.  Do not use.
      */
    output_file_atmel_generic &operator=(const output_file_atmel_generic &);
};

};

#endif // SRECORD_OUTPUT_FILE_ATMEL_GENERIC_H
