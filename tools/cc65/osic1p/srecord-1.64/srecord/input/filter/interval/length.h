//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008-2011 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_INTERVAL_LENGTH_H
#define SRECORD_INPUT_FILTER_INTERVAL_LENGTH_H

#include <srecord/input/filter/interval.h>

namespace srecord {

/**
  * The srecord::input_filter_interval_length class is used to represent a
  * filter which injects the maximum minus minimum addresses of the data
  * into the data.
  */
class input_filter_interval_length:
    public input_filter_interval
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_interval_length();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the length.
      * @param length
      *     The number of bytes of length to be inserted.
      * @param end
      *     The byte order.
      * @param width
      *     The number of bytes per unit of length.
      * @param inclusive
      *     true if the output location is included in the address
      *     range, false if not
      */
    input_filter_interval_length(const input::pointer &deeper,
        long address, int length, endian_t end, int width, bool inclusive);

public:
    /**
      * The create class method is used to create a new dynamically
      * allocated instance of this class.
      *
      * @param deeper
      *     The input source to be filtered.
      * @param address
      *     The address at which to place the length.
      * @param length
      *     The number of bytes of length to be inserted.
      * @param end
      *     The byte order.
      * @param width
      *     The number of bytes per unit of length.
      * @param inclusive
      *     true if the output location is included in the address
      *     range, false if not
      */
    static pointer create(const input::pointer &deeper, long address,
        int length, endian_t end, int width, bool inclusive);

protected:
    // See base class for documentation.
    long calculate_result() const;

private:
    /**
      * The number of bytes per unit of length.
      */
    int width;

    /**
      * The default constructor.
      */
    input_filter_interval_length();

    /**
      * The copy constructor.
      */
    input_filter_interval_length(const input_filter_interval_length &);

    /**
      * The assignment operator.
      */
    input_filter_interval_length &operator=(
        const input_filter_interval_length &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_FILTER_INTERVAL_LENGTH_H
