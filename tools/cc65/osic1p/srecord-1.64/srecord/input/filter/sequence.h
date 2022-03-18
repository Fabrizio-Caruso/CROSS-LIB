//
// srecord - manipulate eprom load files
// Copyright (C) 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_SEQUENCE_H
#define SRECORD_INPUT_FILTER_SEQUENCE_H

#include <srecord/input/filter.h>

namespace srecord
{

/**
  * The srecord::input_filter_sequence class is used to represent an input
  * filter which does not change the data in any way, but issues
  * warnings if the data is not instrictly ascending address order.
  */
class input_filter_sequence:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_sequence();

private:
    /**
      * The constructor.
      */
    input_filter_sequence(input::pointer deeper);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(input::pointer deeper);

protected:
    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The last_address instance variable is used to remember the high
      * water mark for data record addresses to date.  Records with data
      * addresses less than this cause an "out of sequence" warning.
      * It is check and updated by the read() method.
      */
    unsigned long last_address;

    /**
      * The warned instance variable is used to remember whether or not
      * an "out of sequence" warning has already been issued for this
      * file.
      *
      * We only issue a single warning, because the linker in some
      * embedded too chains emit zillions of these as they walk across
      * object files updating segemnts in parallel.
      */
    bool warned;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_sequence();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_sequence(const input_filter_sequence &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_sequence &operator=(const input_filter_sequence &);
};

};

#endif // SRECORD_INPUT_FILTER_SEQUENCE_H
