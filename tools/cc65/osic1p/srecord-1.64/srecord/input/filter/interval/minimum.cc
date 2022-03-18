//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008, 2010 Peter Miller
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

#include <srecord/input/filter/interval/minimum.h>


srecord::input_filter_interval_minimum::~input_filter_interval_minimum()
{
}


srecord::input_filter_interval_minimum::input_filter_interval_minimum(
        const pointer &a_deeper, long a_address, int a_length, endian_t a_end,
        bool inclusive) :
    input_filter_interval(a_deeper, a_address, a_length, a_end, inclusive)
{
}


srecord::input::pointer
srecord::input_filter_interval_minimum::create(const pointer &a_deeper,
    long a_address, int a_length, endian_t a_end, bool inclusive)
{
    return
        pointer
        (
            new input_filter_interval_minimum
            (
                a_deeper,
                a_address,
                a_length,
                a_end,
                inclusive
            )
        );
}


long
srecord::input_filter_interval_minimum::calculate_result()
    const
{
    return get_range().get_lowest();
}
