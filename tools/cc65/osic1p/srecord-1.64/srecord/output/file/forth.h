//
// srecord - manipulate eprom load files
// Copyright (C) 2008, 2010 Peter Miller
// Copyright (C) 2008 Torsten Sadowski
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_OUTPUT_FILE_FORTH_H
#define SRECORD_OUTPUT_FILE_FORTH_H

#include <srecord/output/file.h>

namespace srecord
{

/**
  * The srecord::output_file_forth class is used to write a file that
  * can be read by a FORTH system
  */
class output_file_forth:
    public output_file
{
public:
    /**
      * The destrutcor.
      */
    virtual ~output_file_forth();

private:
    /**
      * The constructor.
      *
      * @param file_name
      *     The name of the file to be written.
      */
    output_file_forth(const std::string &file_name);

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
    int preferred_block_size_get() const;

    // See base class for documentation.
    bool preferred_block_size_set(int nbytes);

    // See base class for documentation.
    void command_line(arglex_tool *cmdln);

    // See base class for documentation.
    const char *format_name() const;

private:
    /**
     * The FORTH store command to use. Either C! for RAM or EEC! for EEPROM
     */
    std::string store_cmd;

    /**
      * The emit_end_of_file method is used to write the ETX and
      * checksum out to the file.  It is safe top call this method more
      * than once, only one ETX will be written.
      */
    void emit_end_of_file();

    /**
      * The default constructor.  Do not use.
      */
    output_file_forth();

    /**
      * The copy constructor.  Do not use.
      */
    output_file_forth(const output_file_forth &);

    /**
      * The assignment operator.  Do not use.
      */
    output_file_forth &operator=(const output_file_forth &);
};

};

#endif // SRECORD_OUTPUT_FILE_FORTH_H
