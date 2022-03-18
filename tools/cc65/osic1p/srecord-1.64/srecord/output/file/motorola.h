//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2005-2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_MOTOROLA_H
#define SRECORD_OUTPUT_FILE_MOTOROLA_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_motorola class is used to represent an output
  * file which emits Motorola S-Record format.
  *
  * @sa
  *     #srecord::output_file_idt,
  *     #srecord::output_file_wilson,
  */
class output_file_motorola:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_motorola();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The file name to open to write data to.  The name "-" is
      *     understood to mean the standard output.
      */
    output_file_motorola(const std::string &file_name);

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
    void command_line(arglex_tool *cmdln);

    // See base class for documentation.
    const char *format_name(void) const;

private:
    /**
      * The data_count instance variable is used to remember the total
      * number of ouput data lines have occurred to date.  Ths is used
      * at the end of the file to emit an S5 record.
      */
    unsigned long data_count;

    /**
      * The pref_block_size instance variable is used to remember the
      * preferred number of data bytes (NOT encoded hex characters) to
      * be placed in each output line.
      */
    int pref_block_size;

    /**
      * The address_length instance variable is used to remember the
      * minimum number of address bytes to be emitted into output lines.
      */
    int address_length;

    /**
      * The address_shift method is used to remember how far to the left
      * addresses need to be shifted to become byte addresses.
      * The default is zero (0).
      *
      * This is because of some poorly though out "extentions" to this
      * file format, for 16-bit and 32-bit data busses.  I say "poory
      * thought out" because the no way (zero, zip, nada) of discovering
      * this just by looking at the data.
      */
    int address_shift;

    /**
      * The data_count_written instance variable is used to remember
      * whether or not we have written out the data count record.
      * Usually this is done with the start arrdess record, but there
      * are circumstances where it will be needed when the file is
      * closed.
      */
    bool data_count_written;

    /**
      * The write_data_count method is used to write out a data count
      * record, if one is required.
      */
    void write_data_count(void);

    /**
      * The write_inner method is used to write a line of output.
      *
      * @param tag
      *     The tag value at the start of the line.  For example, and S1
      *     line would have a tag of 1.
      * @param address
      *     The address of the first byte of data on the line.
      * @param address_nbytes
      *     The number of bytes of address to emit.
      * @param data
      *     The palyload of this line.
      * @param data_nbytes
      *     The number of bytes of payload.
      */
    void write_inner(int tag, unsigned long address, int address_nbytes,
        const void *data, int data_nbytes);

    /**
      * The copy constructor.  Do not use.
      */
    output_file_motorola(const output_file_motorola &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_motorola &operator=(const output_file_motorola &);
};

};

#endif // SRECORD_OUTPUT_FILE_MOTOROLA_H
