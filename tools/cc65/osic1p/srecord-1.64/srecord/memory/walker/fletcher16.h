//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2011 Peter Miller
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

#ifndef SRECORD_MEMORY_WALKER_FLETCHER16_H
#define SRECORD_MEMORY_WALKER_FLETCHER16_H

#include <srecord/fletcher16.h>
#include <srecord/memory/walker.h>

namespace srecord
{

/**
  * The srecord::memory_walker_fletcher16 class is used to represent the parse
  * state of a memory walker which calculates a running Fletcher-16 checksum.
  */
class memory_walker_fletcher16:
    public memory_walker
{
public:
    typedef boost::shared_ptr<memory_walker_fletcher16> pointer;

    /**
      * The destructor.
      */
    virtual ~memory_walker_fletcher16();

private:
    /**
      * The constructor.  It is private on purpose, use the
      * #create method instead.
      *
      * @param sum1
      *     The seed value for sum1.  Defaults to zero.
      * @param sum2
      *     The seed value for sum2.  Defaults to zero.
      * @param answer
      *     Set this to -1 to be completely ignored.
      *     If >= 0, this is the desired outcome if the checksum
      *     includes the checksum itself.  The checksum returned will be
      *     calculated to return this desired outcome, when traversed,
      *     rather than a pure Fletcher-16 checksum.
      * @param end
      *     The endian-ness of the checksum.  This is needed to
      *     manipulate the answer.  Ignored if answer is ignored.
      */
    memory_walker_fletcher16(int sum1, int sum2, int answer, endian_t end);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param sum1
      *     The seed value for sum1.  Defaults to zero.
      * @param sum2
      *     The seed value for sum2.  Defaults to zero.
      * @param answer
      *     Set this to -1 to be completely ignored.
      *     If >= 0, this is the desired outcome if the checksum
      *     includes the checksum itself.  The checksum returned will be
      *     calculated to return this desired outcome, when traversed,
      *     rather than a pure Fletcher-16 checksum.
      * @param end
      *     The endian-ness of the checksum.  This is needed to
      *     manipulate the answer.  Ignored if answer is ignored.
      */
    static pointer create(int sum1, int sum2, int answer, endian_t end);

    /**
      * The get method is used to get the Fletcher-16 checksum once all memory
      * chunks have been processed by calls to our #observe method.
      */
    unsigned get(void) const;

protected:
    // See base class for documentation.
    void observe(unsigned long address, const void *data, int data_size);

private:
    /**
      * The checksum instance variable is used to remember the running
      * state of the Fletcher-16 checksum calculation.
      */
    fletcher16 checksum;

    /**
      * The default constructor.  Do not use.
      */
    memory_walker_fletcher16();

    /**
      * The copy constructor.  Do not use.
      */
    memory_walker_fletcher16(const memory_walker_fletcher16 &);

    /**
      * The assignment operator.  Do not use.
      */
    memory_walker_fletcher16 &operator=(const memory_walker_fletcher16 &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_MEMORY_WALKER_FLETCHER16_H
