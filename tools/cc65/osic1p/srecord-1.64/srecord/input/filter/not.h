//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_NOT_H
#define SRECORD_INPUT_FILTER_NOT_H

#include <srecord/input/filter.h>

namespace srecord
{

/**
  * The srecord::input_filter_not class is used to represent an input stream
  * which bit-wise NOTs the data.
  */
class input_filter_not:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_not();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      */
    input_filter_not(const input::pointer &deeper);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source
      */
    static pointer create(const input::pointer &deeper);

protected:
    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The default constructor.  Do not use.
      */
    input_filter_not();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_not(const input_filter_not &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_not &operator=(const input_filter_not &);
};

};

#endif // SRECORD_INPUT_FILTER_NOT_H
