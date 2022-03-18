//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008, 2010, 2011 Peter Miller
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

#include <srecord/input/filter/interval.h>
#include <srecord/record.h>


srecord::input_filter_interval::~input_filter_interval()
{
}


srecord::input_filter_interval::input_filter_interval(
    const input::pointer &a_deeper,
    long a_address,
    int a_length,
    endian_t a_end,
    bool inclusive
) :
    input_filter(a_deeper),
    address(a_address),
    length(a_length <= 1 ? 1 : a_length >= 8 ? 8 : a_length),
    end(a_end)
{
    if (inclusive)
        range = interval(address, address + length);
}


bool
srecord::input_filter_interval::generate(record &result)
{
    if (length <= 0)
        return false;
    long value = calculate_result();
    unsigned char chunk[8];
    record::encode(chunk, value, length, end);
    result = record(record::type_data, address, chunk, length);
    length = 0;
    return true;
}


bool
srecord::input_filter_interval::read(record &record)
{
    if (!input_filter::read(record))
        return generate(record);
    if (record.get_type() == record::type_data)
    {
        interval i(record.get_address(), record.get_address_end());
        range += i;
    }
    return true;
}

// vim: set ts=8 sw=4 et :
