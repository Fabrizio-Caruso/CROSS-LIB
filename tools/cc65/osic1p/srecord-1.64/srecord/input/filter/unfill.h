//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2005-2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_UNFILL_H
#define SRECORD_INPUT_FILTER_UNFILL_H

#include <srecord/input/filter.h>
#include <srecord/record.h>

namespace srecord
{

/**
  * The srecord::input_filter_unfill class is used to represent a filter
  * which makes holes in the data wherever a specified data byte value
  * appears.
  *
  * This is the inverse of the srecord::input_filter_fill class.
  *
  * Usually this is used to find the actual limits of data from an
  * extracted EPROM image; you can specify a minimum run length of the
  * same byte, so that you don't simply get a 1/256 reduction in density
  * in the middle of "real" data.
  */
class input_filter_unfill:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_unfill();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input source to be filtered.
      * @param value
      *     The value of the bytes to be turned into holes.
      * @param minimum
      *     The minimum run length to be considered a hole.
      */
    input_filter_unfill(const input::pointer &deeper, int value, int minimum);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param value
      *     The value of the bytes to be turned into holes.
      * @param minimum
      *     The minimum run length to be considered a hole.
      */
    static pointer create(const input::pointer &deeper, int value, int minimum);

protected:
    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The fill_value instance variable is used to remember the value
      * of the bytes to be turned into holes.
      */
    record::data_t fill_value;

    /**
      * The fill_minimum instance variable is used to remember the
      * minimum run length to be considered a hole.
      */
    record::address_t fill_minimum;

    /**
      * The buffer instance variable is used to remember the data
      * that has been read from the deeper input source and is being
      * processed to unfill certain valued bytes.
      */
    record buffer;

    /**
      * The fill_value instance variable is used to remember where we
      * are up to in the "buffer" instance variable.
      */
    size_t buffer_pos;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_unfill();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_unfill(const input_filter_unfill &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_unfill &operator=(const input_filter_unfill &);
};

};

#endif // SRECORD_INPUT_FILTER_UNFILL_H
