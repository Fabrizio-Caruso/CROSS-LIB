//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2005-2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_BYTE_SWAP_H
#define SRECORD_INPUT_FILTER_BYTE_SWAP_H

#include <srecord/input/filter.h>
#include <srecord/record.h>

namespace srecord {

/**
  * The srecord::input_filter_byte_swap class is used to reverse the
  * even-addressed and odd-addressed bytes in an input source.
  */
class input_filter_byte_swap:
    public input_filter
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_byte_swap();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The incoming data sourec to be byte-swapped.
      */
    input_filter_byte_swap(const input::pointer &deeper);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data sourec to be byte-swapped.
      */
    static pointer create(const input::pointer &deeper);

protected:
    // See base class for documentation.
    bool read(record &record);

    // See base class for documentation.
    void command_line(arglex_tool *cmdln);

private:
    /**
      * The buffer instance variable is used to remember the data
      * fetched from the deeper data source.
      */
    record buffer;

    /**
      * The buffer_pos instance variable is used to remeber the byte
      * position within the buffer instance variable.
      */
    size_t buffer_pos;

    /**
      * The mask instance variable is used to remember the bit mask to
      * be xor-ed with the address to form the byte-swapped address.
      * It defaults to 1, but can be altered by command line option.
      */
    record::address_t mask;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_byte_swap();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_byte_swap(const input_filter_byte_swap &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_byte_swap &operator=(const input_filter_byte_swap &);
};

};

#endif // SRECORD_INPUT_FILTER_BYTE_SWAP_H
