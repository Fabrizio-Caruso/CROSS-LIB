//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_OFFSET_H
#define SRECORD_INPUT_FILTER_OFFSET_H

#include <srecord/input/filter.h>

namespace srecord
{

/**
  * The srecord::input_filter_offset class is used to represent a filetr
  * where the addresses of the the deeper input source are offset by a
  * number of bytes.
  */
class input_filter_offset:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_offset();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input stream.
      * @param nbytes
      *     The number of bytes to offset the addresses by.
      *     The value may be negative.
      */
    input_filter_offset(const input::pointer &deeper, long nbytes);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param nbytes
      *     The number of bytes to offset the addresses by.
      *     The value may be negative.
      */
    static pointer create(const input::pointer &deeper, long nbytes);

protected:
    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The nbytes instance variable is used to remember the number of
      * bytes to affset the data by.
      */
    long nbytes;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_offset();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_offset(const input_filter_offset &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_offset &operator=(const input_filter_offset &);
};

};

#endif // SRECORD_INPUT_FILTER_OFFSET_H
