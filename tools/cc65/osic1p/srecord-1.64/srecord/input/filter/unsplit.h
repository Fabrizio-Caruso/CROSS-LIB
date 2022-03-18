//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2005-2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_UNSPLIT_H
#define SRECORD_INPUT_FILTER_UNSPLIT_H

#include <srecord/input/filter.h>
#include <srecord/record.h>

namespace srecord
{

/**
  * The srecord::input_filter_unsplit class is used to represent a filter
  * whoch explosed the deeper input source.  This is the inverse of the
  * srecord::input_filter_split class.  This can be used to takes the images
  * of thwo parallel EPROM images and create a unified image when they
  * are interleaved rather than consecutive.
  *
  * @sa
  *    srecord::input_filter_split
  */
class input_filter_unsplit:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_unsplit();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The deeper input source to be filtered.
      * @param modulus
      *     The number of bytes wide each swathe is.
      * @param offset
      *     The offset within the swathe.
      * @param width
      *     The width of each stripe within the swathe.
      */
    input_filter_unsplit(const input::pointer &deeper, int modulus, int offset,
        int width);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param modulus
      *     The number of bytes wide each swathe is.
      * @param offset
      *     The offset within the swathe.
      * @param width
      *     The width of each stripe within the swathe.
      */
    static pointer create(const input::pointer &deeper, int modulus, int offset,
        int width);

protected:
    // See base class for documentation.
    bool read(record &record);

private:
    /**
      * The modulus instance variable is used to remember the number of
      * bytes wide each swathe is.
      */
    record::address_t modulus;

    /**
      * The offset instance variable is used to remember the offset
      * within the swathe.
      */
    record::address_t offset;

    /**
      * The width instance variable is used to remember the width of
      * each stripe within the swathe.
      */
    record::address_t width;

    /**
      * The buffer instance variable is used to remember the last lot
      * of data read from the deeper inputs source, and currently being
      * processed.
      */
    record buffer;

    /**
      * The buffer_pos instance variable is used to remember where we
      * are up to in the "buffer" instance varaible.
      */
    size_t buffer_pos;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_unsplit();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_unsplit(const input_filter_unsplit &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_unsplit &operator=(const input_filter_unsplit &);
};

};

#endif // SRECORD_INPUT_FILTER_UNSPLIT_H
