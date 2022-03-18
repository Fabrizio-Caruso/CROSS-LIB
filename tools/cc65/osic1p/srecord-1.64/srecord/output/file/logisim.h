//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
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

#ifndef SRECORD_OUTPUT_FILE_LOGISIM_H
#define SRECORD_OUTPUT_FILE_LOGISIM_H

#include <srecord/output/file.h>

namespace srecord {

/**
  * The output_file_logisim class is used to represent
  * the processing neccessry to write a Logisim ROM file.
  */
class output_file_logisim:
    public output_file
{
public:
    /**
      * The destructor.
      */
    virtual ~output_file_logisim();

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
      * The constructor.
      * Use #create instead.
      */
    output_file_logisim(const std::string &file_name);

    long address;
    int line_length;
    int column;

    void emit(int count, int value);

    /**
      * The default constructor.
      * Do not use.
      */
    output_file_logisim();

    /**
      * The copy constructor.
      * Do not use.
      */
    output_file_logisim(const output_file_logisim &rhs);

    /**
      * The assignment operator.
      * Do not use.
      */
    output_file_logisim &operator=(const output_file_logisim &rhs);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_OUTPUT_FILE_LOGISIM_H
