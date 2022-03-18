//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2004, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_FILL_H
#define SRECORD_INPUT_FILTER_FILL_H

#include <srecord/interval.h>
#include <srecord/input/filter.h>
#include <srecord/record.h>

namespace srecord {

/**
  * The srecord::input_filter_fill class is used to represent a filter
  * which replaces in set data locations with constant data.
  */
class input_filter_fill:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_fill();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param value
      *     The byte value to fill with.
      * @param range
      *     The address range to be filled.
      */
    input_filter_fill(const input::pointer &deeper, int value,
        const interval &range);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param value
      *     The byte value to fill with.
      * @param range
      *     The address range to be filled.
      */
    static pointer create(const input::pointer &deeper, int value,
        const interval &range);

protected:
    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The filler_value instance variable is used to remember the value
      * to assign to fill bytes.
      */
    int filler_value;

    /**
      * The filler_block instance variable is used to remember the base
      * of a dynamically allocated array of bytes, used to construct
      * filler block records.
      */
    unsigned char *filler_block;

    /**
      * The range instance variable is used to remember the range of
      * addresses to be filled.  As fill blocks are produced the range
      * is reduced.
      */
    interval range;

    /**
      * The generate method is used to genetate fill records.
      */
    bool generate(record &record);

    /**
      * The default constructor.  Do not use.
      */
    input_filter_fill();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_fill(const input_filter_fill &);

    /**
      * The assignment.  Do not use.
      */
    input_filter_fill &operator=(const input_filter_fill &);
};

};

#endif // SRECORD_INPUT_FILTER_FILL_H
