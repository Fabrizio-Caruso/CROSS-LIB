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

#include <srecord/r250.h>
#include <srecord/input/generator/random.h>


srecord::input_generator_random::~input_generator_random()
{
}


srecord::input_generator_random::input_generator_random(
        const interval &a_range) :
    srecord::input_generator(a_range)
{
}


srecord::input::pointer
srecord::input_generator_random::create(const interval &a_range)
{
    return pointer(new srecord::input_generator_random(a_range));
}


unsigned char
srecord::input_generator_random::generate_data(unsigned long)
{
    return r250();
}


std::string
srecord::input_generator_random::filename()
    const
{
    return "random";
}


const char *
srecord::input_generator_random::get_file_format_name()
    const
{
    return "random";
}

// vim: set ts=8 sw=4 et :
