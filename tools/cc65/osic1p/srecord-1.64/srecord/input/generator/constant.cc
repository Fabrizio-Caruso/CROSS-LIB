//
// srecord - The "srecord" program.
// Copyright (C) 2007-2011 Peter Miller
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

#include <cstdio>

#include <srecord/input/generator/constant.h>


srecord::input_generator_constant::~input_generator_constant()
{
}


srecord::input_generator_constant::input_generator_constant(
        const interval &a_range, unsigned char a_datum) :
    srecord::input_generator(a_range),
    datum(a_datum)
{
}


srecord::input::pointer
srecord::input_generator_constant::create(const interval &a_range,
    unsigned char a_datum)
{
    return pointer(new srecord::input_generator_constant(a_range, a_datum));
}


unsigned char
srecord::input_generator_constant::generate_data(unsigned long)
{
    return datum;
}


std::string
srecord::input_generator_constant::filename()
    const
{
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "constant 0x%2.2X", datum);
    return buffer;
}


const char *
srecord::input_generator_constant::get_file_format_name()
    const
{
    return "constant";
}


// vim: set ts=8 sw=4 et :
