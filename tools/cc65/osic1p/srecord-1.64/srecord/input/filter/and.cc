//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2006-2010 Peter Miller
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

#include <srecord/input/filter/and.h>
#include <srecord/record.h>


srecord::input_filter_and::~input_filter_and()
{
}


srecord::input_filter_and::input_filter_and(const input::pointer &a1,
        int a2) :
    input_filter(a1),
    value(a2)
{
}


srecord::input::pointer
srecord::input_filter_and::create(const input::pointer &a_deeper, int a_mask)
{
    return pointer(new input_filter_and(a_deeper, a_mask));
}


bool
srecord::input_filter_and::read(record &result)
{
    if (!input_filter::read(result))
        return false;
    if (result.get_type() == record::type_data)
    {
        for (size_t j = 0; j < result.get_length(); ++j)
            result.set_data(j, result.get_data(j) & value);
    }
    return true;
}
