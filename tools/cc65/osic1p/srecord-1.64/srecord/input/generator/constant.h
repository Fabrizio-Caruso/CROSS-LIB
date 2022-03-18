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

#ifndef SRECORD_INPUT_GENERATOR_CONSTANT_H
#define SRECORD_INPUT_GENERATOR_CONSTANT_H

#include <srecord/input/generator.h>

namespace srecord
{

/**
  * The srecord::input_generator_constant class is used to represent the
  * state of a data generator which supplies constant bytes.
  */
class input_generator_constant:
    public input_generator
{
public:
    /**
      * The destructor.
      */
    virtual ~input_generator_constant();

private:
    /**
      * The constructor.
      */
    input_generator_constant(const interval &range, unsigned char datum);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const interval &range, unsigned char datum);

protected:
    // See base class for documentation.
    std::string filename() const;

    // See base class for documentation.
    const char *get_file_format_name() const;

    // See base class for documentation.
    unsigned char generate_data(unsigned long address);

private:
    /**
      * The datum instance variable is used to remember the constant
      * byte value to be generated.
      */
    unsigned char datum;

    /**
      * The default constructor.  Do not use.
      */
    input_generator_constant();

    /**
      * The copy constructor.  Do not use.
      */
    input_generator_constant(const input_generator_constant &);

    /**
      * The assignment operator.  Do not use.
      */
    input_generator_constant &operator=(const input_generator_constant &);
};

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_INPUT_GENERATOR_CONSTANT_H
