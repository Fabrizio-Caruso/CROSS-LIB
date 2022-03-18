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

#ifndef SRECORD_INPUT_FILTER_CROP_H
#define SRECORD_INPUT_FILTER_CROP_H

#include <srecord/interval.h>
#include <srecord/input/filter.h>
#include <srecord/record.h>

namespace srecord {

/**
  * The srecord::input_filter_crop filter is used to crop the data by
  * selecting portions of the address range to be passed through.
  */
class input_filter_crop:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_crop();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The input source to be cropped.
      * @param range
      *     The address range to be preserved.  The rest will be
      *     ignored.
      */
    input_filter_crop(const input::pointer &deeper, const interval &range);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param range
      *     The address range to be preserved.  The rest will be
      *     ignored.
      */
    static pointer create(const input::pointer &deeper, const interval &range);

protected:
    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The range instance variable is used to remember the address
      * range to be preserved.
      */
    interval range;

    /**
      * The dat instance variable is used to remember the current input
      * data record being filtered.
      */
    record data;

    /**
      * The data_range instance variable is used to remember the address
      * range in the incoming data.
      */
    interval data_range;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_crop();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_crop(const input_filter_crop &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_crop &operator=(const input_filter_crop &);
};

};

#endif // SRECORD_INPUT_FILTER_CROP_H
