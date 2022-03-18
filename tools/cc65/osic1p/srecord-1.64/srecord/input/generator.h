//
// srecord - The "srecord" program.
// Copyright (C) 2007, 2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_INPUT_GENERATOR_H
#define SRECORD_INPUT_GENERATOR_H

#include <srecord/input.h>
#include <srecord/interval.h>

namespace srecord
{

/**
  * The srecord::input_generator class is used to represent the state of
  * generating data from whole cloth.
  */
class input_generator:
    public input
{
public:
    /**
      * The destructor.
      */
    virtual ~input_generator();

    /**
      * The constructor.
      *
      * @param range
      *     The data range over which data is to be generated.
      */
    input_generator(const interval &range);

    /**
      * The create class method may be used to create new instances of
      * input data generators.
      *
      * @param cmdln
      *     The command line arguments, for deciding what to generate.
      */
    static input::pointer create(arglex_tool *cmdln);

protected:
    // See base class for documentation
    bool read(record &record);

    // See base class for documentation
    void disable_checksum_validation();

    /**
      * The generate_data method is used to manufacture data for a
      * specific address.
      *
      * @param address
      *     The address to generate data for.
      * @returns
      *     one byte of data
      */
    virtual unsigned char generate_data(unsigned long address) = 0;

private:
    /**
      * The range instance variable is used to remember the address
      * range over which we are to generate data.  It shrinks as the
      * data is generated.
      */
    interval range;

    /**
      * The default constructor.
      */
    input_generator();

    /**
      * The copy constructor.
      */
    input_generator(const input_generator &);

    /**
      * The assignment operator.
      */
    input_generator &operator=(const input_generator &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_GENERATOR_H
