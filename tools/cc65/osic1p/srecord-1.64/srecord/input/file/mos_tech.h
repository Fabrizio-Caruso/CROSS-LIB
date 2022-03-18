//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2006-2008, 2010, 2011, 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_INPUT_FILE_MOS_TECH_H
#define SRECORD_INPUT_FILE_MOS_TECH_H

#include <srecord/input/file.h>

namespace srecord {

/**
  * The mos_tech class is used to parse a MOS Technology format file.
  */
class input_file_mos_tech:
    public input_file
{
public:
    /**
      * The destructor.
      */
    virtual ~input_file_mos_tech();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param file_name
      *     The name of the file to be read.
      * @returns
      *     smart pointer to new instance
      */
    static pointer create(const std::string &file_name);

protected:
    // See base class for documentation.
    bool read(record &record);

    // See base class for documentation.
    const char *get_file_format_name(void) const;

    // See base class for documentation.
    int format_option_number(void) const;

private:
    /**
      * A constructor.  The input is read from the named file (or
      * the standard input if the file anme is "-").
      *
      * @param file_name
      *     The name of the file to be read.
      */
    input_file_mos_tech(const std::string &file_name);

    /**
      * Read one line (record) of input.  Called by the #read method.
      * Returns false at end of file.
      */
    bool read_inner(record &);

    /**
      * The garbage_warning instance variable is used to remember
      * whther or not a warning has been issued about non-format
      * lines in the file.  Only one warning per file is issued.
      */
    bool garbage_warning;

    /**
      * The seen_some_input instance variable is used to
      * remember whether any input has been seen.
      */
    bool seen_some_input;

    /**
      * The data_record_count instance variable is used to remember the
      * number of data records seen to date.
      */
    int data_record_count;

    /**
      * The default constructor.  Do not use.
      */
    input_file_mos_tech();

    /**
      * The copy constructor.  Do not use.
      */
    input_file_mos_tech(const input_file_mos_tech &);

    /**
      * The assignment operator.  Do not use.
      */
    input_file_mos_tech &operator=(const input_file_mos_tech &);
};

};

#endif // SRECORD_INPUT_FILE_MOS_TECH_H
// vim: set ts=8 sw=4 et :
