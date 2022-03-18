//
// srecord - manipulate eprom load files
// Copyright (C) 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
//
//      From: Hendrik Beijeman <hbeyeman@gmail.com>
//      To: pmiller@opensource.org.au
//      Subject: Re: patch for srecord 1.59
//      Date: Sat, 05 May 2012 06:26:02 +0200 (05/05/12 14:26:02)
//
//      I hereby formally assign all copyright to the author of srecord,
//      Peter Miller.
//

#ifndef SRECORD_MEMORY_WALKER_STM32_H
#define SRECORD_MEMORY_WALKER_STM32_H

#include <srecord/stm32.h>
#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The memory_walker_stm32 class is a vistor for calculating the CRC used by
  * the hardware CRC unit on the STM32 series of ARM MPUs. The algo used by the
  * STM32 hardware unit is just a CRC with a different polynomial and word-fed
  * instead of byte-fed.  The MPU is little endian but srecord will compute the
  * correct checksum regardless of the endianness of the generating platform.
  * http://www.st.com/
  */
class memory_walker_stm32:
    public memory_walker
{
public:
    typedef boost::shared_ptr<memory_walker_stm32> pointer;

    /**
      * The destructor.
      */
    virtual ~memory_walker_stm32();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    /**
      * The get method is used to get the CRC32 (STM32) checksum once
      * all memory chunks have been processed by calls to our observe
      * method.
      */
    unsigned get(void) const;

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int);

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    memory_walker_stm32();

    /**
      * The checksum instance variable is used to remember the running
      * state of the CRC32 (STM32) checksum calculation.
      */
    stm32 checksum;

    /**
      * The copy constructor.  Do not use.
      */
    memory_walker_stm32(const memory_walker_stm32 &);

    /**
      * The assignment operator.  Do not use.
      */
    memory_walker_stm32 &operator=(const memory_walker_stm32 &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_MEMORY_WALKER_STM32_H
