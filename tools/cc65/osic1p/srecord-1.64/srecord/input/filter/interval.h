//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008-2011 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_INTERVAL_H
#define SRECORD_INPUT_FILTER_INTERVAL_H

#include <srecord/endian.h>
#include <srecord/interval.h>
#include <srecord/input/filter.h>

namespace srecord {

/**
  * The srecord::input_filter_interval class is used to represent a filter
  * which works with the interval representing the data's coverage,
  * <i>exclusive</i> of where the result is to be placed.
  */
class input_filter_interval:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_interval();

protected:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the interval.
      * @param length
      *     The number of bytes of interval to be inserted.
      * @param end
      *     The byte order.
      * @param inclusive
      *     true if the output location is included in the address
      *     range, false if not
      */
    input_filter_interval(const input::pointer &deeper, long address,
        int length, endian_t end, bool inclusive);

    /**
      * The calculate_result method is used to calculate the final value
      * to be placed into the output.
      */
    virtual long calculate_result() const = 0;

    /**
      * The get_range method may be used to derived classes to get
      * access to the address range covered by the input data.
      */
    const interval &get_range() const { return range; }

    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The address instance variable is used to remember where the
      * final result is to be placed.
      */
    long address;

    /**
      * The length instance variable is used to remember how many bytes
      * are to be placed at the above address.
      *
      * 0 < length && length <= 8
      * (a length of zero means we have already emitted the results)
      */
    int length;

    /**
      * The end instance variable is used to remember whether to use
      * big endian order or little endian byte order.
      */
    endian_t end;

    /**
      * The range instance variable is used to remember the address
      * range covered by the input data.
      */
    interval range;

    /**
      * The generate method is used to generate the final record with
      * the interval data in it.
      *
      * @param record
      *     where to put the result
      */
    bool generate(record &record);

    /**
      * The default constructor.  Do not use.
      */
    input_filter_interval();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_interval(const input_filter_interval &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_interval &operator=(const input_filter_interval &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILTER_INTERVAL_H
