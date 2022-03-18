//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_MEMORY_WALKER_CRC32_H
#define SRECORD_MEMORY_WALKER_CRC32_H

#include <srecord/crc32.h>
#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The srecord::memory_walker_crc32 class is used to represent the parse
  * state of a memory walker which calculates a running CRC32 checksum.
  */
class memory_walker_crc32:
    public memory_walker
{
public:
    typedef boost::shared_ptr<memory_walker_crc32> pointer;

    /**
      * The destructor.
      */
    virtual ~memory_walker_crc32();

private:
    /**
      * The default constructor.  It is private on putpose, use the
      * #create method instead.
      *
      * @param seed_mode
      *     How to calculate the initial seed.
      */
    memory_walker_crc32(crc32::seed_mode_t seed_mode);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param seed_mode
      *     How to calculate the initial seed.
      */
    static pointer create(crc32::seed_mode_t seed_mode);

    /**
      * The get method is used to get the CRC32 checksum once all memory
      * chunks have been processed by calls to our observe method.
      */
    unsigned get() const;

protected:
    // See base class for documentation.
    void observe(unsigned long, const void *, int);

private:
    /**
      * The checksum instance variable is used to remember the running
      * state of the CRC32 checksum calculation.
      */
    crc32 checksum;

    /**
      * The copy constructor.  Do not use.
      */
    memory_walker_crc32(const memory_walker_crc32 &);

    /**
      * The assignment operator.  Do not use.
      */
    memory_walker_crc32 &operator=(const memory_walker_crc32 &);
};

};

#endif // SRECORD_MEMORY_WALKER_CRC32_H
