//
// srecord - manipulate eprom load files
// Copyright (C) 2000-2002, 2006-2008, 2010 Peter Miller
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

#include <srecord/input/filter/checksum/negative.h>


srecord::input_filter_checksum_negative::~input_filter_checksum_negative()
{
}


srecord::input_filter_checksum_negative::input_filter_checksum_negative(
        const input::pointer &a1, int a2, int a3, endian_t a_end, int a5) :
    input_filter_checksum(a1, a2, a3, a_end, a5)
{
}


srecord::input::pointer
srecord::input_filter_checksum_negative::create(const input::pointer &a_deeper,
    int a2, int a3, endian_t a_end, int a5)
{
    return
        pointer
        (
            new input_filter_checksum_negative(a_deeper, a2, a3, a_end, a5)
        );
}


srecord::input_filter_checksum::sum_t
srecord::input_filter_checksum_negative::calculate()
{
    return -sum;
}
