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

#ifndef SRECORD_OUTPUT_FILE_PPB_H
#define SRECORD_OUTPUT_FILE_PPB_H

#include <srecord/output/file.h>

namespace srecord {

/**
  * The output_file_ppb class is used to represent the processing
  * required to write a Stag Prom Programmer binary file.
  */
class output_file_ppb:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_ppb();

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

    // See base class for documentation.
    bool is_binary(void) const;

private:
    /**
      * The constructor.  It is private on purpose, use the #create
      * class method instead.
      *
      * @param file_name
      *     The name of the file to be written.  The special name "-"
      *     indicates the standard output is to be used.
      */
    output_file_ppb(const std::string &file_name);

    /**
      * The address instance variable is used to remember the address of
      * the next data byte to be parsed.
      */
    unsigned long address;

    /**
      * The buffer instance variable is used to remember the accumulated
      * data bytes to be written to the file.
      */
    unsigned char buffer[8192];

    /**
      * The buffer_length instance variable is used to remember how many
      * bytes are valid in the #buffer array.
      */
    unsigned buffer_length;

    /**
      * The seen_some_data instance variable is used to remember whether
      * or not any data has been written to the file yet.
      */
    bool seen_some_data;

    /**
      * The buffer_flush method is used to write out the #buffer_length
      * bytes in the #buffer instance variable.
      */
    void buffer_flush(void);

    /**
      * The packet method is used to write out the #buffer as an
      * appropriately constructed packet.
      */
    void packet(unsigned long address, const unsigned char *data,
        size_t data_size);

    /**
      * The put_bin_4be method is used to write out 4 binary bytes of a
      * 32-bit value, big endian ordering.
      */
    void put_bin_4be(unsigned long value);

    /**
      * The default constructor.  Do not use.
      */
    output_file_ppb();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_ppb(const output_file_ppb &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_ppb &operator=(const output_file_ppb &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_PPB_H
