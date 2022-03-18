//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2006-2010 Peter Miller
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

#ifndef SRECORD_MEMORY_WALKER_CRC16_H
#define SRECORD_MEMORY_WALKER_CRC16_H

#include <srecord/crc16.h>
#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The srecord::memory_walker_crc16 class is used to represent the parse
  * state of a memory walker which calculates a running CRC16 checksum.
  */
class memory_walker_crc16:
    public memory_walker
{
public:
    typedef boost::shared_ptr<memory_walker_crc16> pointer;

    /**
      * The destructror.
      */
    virtual ~memory_walker_crc16();

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * #create class method instead.
      *
      * @param seed_mode
      *     The selector for the initial seed for the calculation
      * @param augment_flag
      *     Whether or not to augment the calculation
      * @param polynomial
      *     The CRC polynomial to be used.
      * @param bitdir
      *     the bit direction of the CRC.
      */
    memory_walker_crc16(crc16::seed_mode_t seed_mode, bool augment_flag,
        unsigned short polynomial, crc16::bit_direction_t bitdir);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param seed_mode
      *     The selector for the initial seed for the calculation
      * @param augment_flag
      *     Whether or not to augment the calculation
      * @param polynomial
      *     The CRC polynomial to be used.
      * @param bitdir
      *     the bit direction of the CRC
      */
    static pointer create(crc16::seed_mode_t seed_mode, bool augment_flag,
        unsigned short polynomial, crc16::bit_direction_t bitdir);

    /**
      * The get method is used to get the CRC16 checksum once all memory
      * chunks have been processed by calls to our observe method.
      */
    unsigned get() const;

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int);

private:
    /**
      * The checksum instance variable is used to remember the running
      * state of the CRC16 checksum calculation.
      */
    crc16 *checksum;

    /**
      * The copy constructor.  No not use.
      */
    memory_walker_crc16(const memory_walker_crc16 &);

    /**
      * The assignment operator.  No not use.
      */
    memory_walker_crc16 &operator=(const memory_walker_crc16 &);
};

};

#endif // SRECORD_MEMORY_WALKER_CRC16_H
