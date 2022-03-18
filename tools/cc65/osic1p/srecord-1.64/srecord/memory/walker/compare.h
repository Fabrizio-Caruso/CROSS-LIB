//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_MEMORY_WALKER_COMPARE_H
#define SRECORD_MEMORY_WALKER_COMPARE_H

#include <srecord/interval.h>
#include <srecord/memory/walker.h>

namespace srecord
{

class memory; // forward

/**
  * The srecord::memory_walker_compare class is used to represent a memory
  * walker which compares memory data with another memory instance.
  *
  * Note that this is strictly an improper subset comparison.  I.e. that
  * all bytes passed to the observe method exists in the other memory
  * instance.
  *
  * For a complete equality test, you need to use this comparison both
  * ways round.
  */
class memory_walker_compare:
    public memory_walker
{
public:
    typedef boost::shared_ptr<memory_walker_compare> pointer;

    /**
      * The destructor.
      */
    virtual ~memory_walker_compare();

private:
    /**
      * The constructor.  It is private on purpose, use thr #create
      * class method instead.
      *
      * @param other
      *     The other memory instance to be checked against this one.
      * @param check_wrong
      *     Whether or not to check that the data agrees as well as the
      *     address ranges.
      */
    memory_walker_compare(const memory &other, bool check_wrong);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param other
      *     The other memory instance to be checked against this one.
      * @param check_wrong
      *     Whether or not to check that the data agrees as well as the
      *     address ranges.
      */
    static pointer create(const memory &other, bool check_wrong);

    // See base class for documentation.
    virtual void observe(unsigned long, const void *, int);

    /**
      * The print method is used to print the results of the comparison
      * on the standard output.
      *
      * @param caption
      *     The text to place at the start of the listing.
      */
    void print(const char *caption) const;

    /**
      * The same method is used to discover whether the result of the
      * comparison indicate that the two memory instances are the same.
      */
    bool same() const;

private:
    /**
      * The other instance variable is used to remember the other memory
      * instance to be checked against this one.
      */
    const memory &other;

    /**
      * The check_wrong instance variable is used to remember whether or
      * not to check that the data agrees as well as the address ranges.
      */
    bool check_wrong;

    /**
      * The unset instance variable is used to remember which bytes of
      * data given to the observe method are NOT present in the other
      * memory instance.
      */
    interval unset;

    /**
      * The wrong instance variable is used to remember which bytes of
      * data given to the observer method were different than the ones
      * in the other memory instance.
      */
    interval wrong;

    /**
      * The default constructor.  Do not use.
      */
    memory_walker_compare();

    /**
      * The copy constructor.  Do not use.
      */
    memory_walker_compare(const memory_walker_compare &);

    /**
      * The assignment operator.  Do not use.
      */
    memory_walker_compare &operator=(const memory_walker_compare &);
};

};

#endif // SRECORD_MEMORY_WALKER_COMPARE_H
