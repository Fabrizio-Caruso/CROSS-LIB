//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2003, 2005-2008, 2010, 2012 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_H
#define SRECORD_INPUT_FILTER_H


#include <srecord/input.h>

namespace srecord {

/**
  * The srecord::input_filter class is an abstract interface for all of the
  * various filters that can be applied to an incoming EPROM file.
  *
  * Each filter is-a input, but each also has-a deeper input, from which
  * is obtained the data to be filtered.
  */
class input_filter:
    public input
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter();

    // See base class for documentation.
    bool read(class record &record);

    // See base class for documentation.
    std::string filename() const;

    // See base class for documentation.
    std::string filename_and_line() const;

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    void disable_checksum_validation();

protected:
    /**
      * The constructor.  Only derived classes may call.
      *
      * @param deeper
      *     The deeper input to be filtered.
      */
    input_filter(input::pointer deeper);

    //
    // I'd like this to be private, but srecord::input_filter_crc16 and
    // srecord::input_filter_crc32 need to access it directly, rather than
    // going through the srecord::input_filter::read method.
    //
    pointer ifp;

private:
    /**
      * The default constructor.  Do not use.
      */
    input_filter();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter(const input_filter &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter &operator=(const input_filter &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILTER_H
