//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2003, 2005-2010 Peter Miller
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

#ifndef SRECORD_INPUT_FILTER_MESSAGE_CRC32_H
#define SRECORD_INPUT_FILTER_MESSAGE_CRC32_H

#include <srecord/crc32.h>
#include <srecord/endian.h>
#include <srecord/input/filter/message.h>

namespace srecord
{

/**
  * The srecord::input_filter_message_crc32 class is used to represent the
  * state of a checksum filter that inserts a CRC32 into the data.
  */
class input_filter_message_crc32:
    public input_filter_message
{
public:
    /**
      * The destructor.
      */
    virtual ~input_filter_message_crc32();

private:
    /**
      * The constructor.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param address
      *     where to place the checksum
      * @param end
      *     The byte order.
      */
    input_filter_message_crc32(const input::pointer &deeper,
        unsigned long address, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param deeper
      *     The incoming data source to be filtered
      * @param address
      *     where to place the checksum
      * @param end
      *     The byte order.
      */
    static pointer create(const input::pointer &deeper, unsigned long address,
        endian_t end);

protected:
    // See base class for documentation.
    void command_line(arglex_tool *cmdln);

    // See base class for documentation.
    void process(const memory &input, record &output);

    // See base class for documentation.
    const char *get_algorithm_name() const;

private:
    /**
      * The address instance variable is used to remember where to place
      * the CRC in memory.
      */
    unsigned long address;

    /**
      * The end instance variable is used to remember whether the byte
      * order is big-endian or little-endian.
      */
    endian_t end;

    /**
      * The seed_mode instance variable is used to remember what the
      * user wants for the initial seed.  The default is to use the
      * standard CCITT seed.
      */
    crc32::seed_mode_t seed_mode;

    /**
      * The default constructor.  Do not use.
      */
    input_filter_message_crc32();

    /**
      * The copy constructor.  Do not use.
      */
    input_filter_message_crc32(const input_filter_message_crc32 &);

    /**
      * The assignment operator.  Do not use.
      */
    input_filter_message_crc32 &operator=(const input_filter_message_crc32 &);
};

};

#endif // SRECORD_INPUT_FILTER_MESSAGE_CRC32_H
