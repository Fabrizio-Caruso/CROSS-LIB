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

#ifndef SRECORD_INPUT_GENERATOR_REPEAT_H
#define SRECORD_INPUT_GENERATOR_REPEAT_H

#include <srecord/input/generator.h>

namespace srecord
{

/**
  * The srecord::input_generator_repeat class is used to represent
  * generating data which cycles over a fixed set of byte values.
  */
class input_generator_repeat:
    public input_generator
{
public:
    /**
      * The destructor.
      */
    virtual ~input_generator_repeat();

private:
    /**
      * The constructor.
      *
      * @param range
      *     The address range over which to generate data.
      * @param data
      *     Pointer to the base of an array of data to be repeated.
      * @param length
      *     The length of the array of data to be repeated.
      */
    input_generator_repeat(const interval &range, unsigned char *data,
        size_t length);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param range
      *     The address range over which to generate data.
      * @param data
      *     Pointer to the base of an array of data to be repeated.
      * @param length
      *     The length of the array of data to be repeated.
      */
    static pointer create(const interval &range, unsigned char *data,
        size_t length);

protected:
    // See base class for documentation.
    unsigned char generate_data(unsigned long address);

    // See base class for documentation.
    std::string filename() const;

    // See base class for documentation.
    const char *get_file_format_name() const;

private:
    /**
      * The address instance variable is used to remember the start of
      * the generated data, so that modulo arithmentic will align the
      * data repeats.
      */
    unsigned long address;

    /**
      * The data instance variable is used to remember the base of a
      * dynamically allocated array of data to be repeated.
      */
    unsigned char *data;

    /**
      * The length instance variable is used to remember the lenth of
      * the dynamically allocated array of data to be repeated.
      */
    size_t length;

    /**
      * The default constructor.  Do not use.
      */
    input_generator_repeat();

    /**
      * The copy constructor.  Do not use.
      */
    input_generator_repeat(const input_generator_repeat &);

    /**
      * The assignment operator.  Do not use.
      */
    input_generator_repeat &operator=(const input_generator_repeat &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_GENERATOR_REPEAT_H
