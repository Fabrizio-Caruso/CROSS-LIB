//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2006-2010, 2013 Peter Miller
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

#include <srecord/interval.h>
#include <srecord/input/filter/checksum.h>
#include <srecord/record.h>


srecord::input_filter_checksum::input_filter_checksum(
    input::pointer a_input_filter,
    int a_checksum_address,
    int a_length,
    endian_t a_end,
    int a_width
) :
    input_filter(a_input_filter),
    checksum_address(a_checksum_address),
    length(a_length),
    end(a_end),
    sum(0),
    width(a_width)
{
    if (length < 0)
        length = 0;
    else if (length > (int)sizeof(sum_t))
        length = sizeof(sum_t);
    if (width < 1)
        width = 1;
    else if (width > length)
        width = length;
}


srecord::input_filter_checksum::~input_filter_checksum()
{
}


bool
srecord::input_filter_checksum::generate(record &result)
{
    if (length <= 0)
        return false;
    unsigned char chunk[sizeof(sum_t)];
    sum_t value = calculate();
    record::encode(chunk, value, length, end);
    result = record(record::type_data, checksum_address, chunk, length);
    length = 0;
    return true;
}


bool
srecord::input_filter_checksum::read(record &record)
{
    if (!input_filter::read(record))
        return generate(record);
    if (record.get_type() == record::type_data)
    {
        if (width <= 1)
        {
            for (size_t j = 0; j < record.get_length(); ++j)
            {
                sum += record.get_data(j);
            }
        }
        else if (end == endian_little)
        {
            // Little endian
            for (size_t j = 0; j < record.get_length(); ++j)
            {
                sum += (sum_t)record.get_data(j) << (8 *
                    ((record.get_address() + j) % width));
            }
        }
        else
        {
            // Big endian
            for (size_t j = 0; j < record.get_length(); ++j)
            {
                sum += (sum_t)record.get_data(j) << (8 *
                    (width - 1 - ((record.get_address() + j)
                    % width)));
            }
        }
    }
    return true;
}


// vim: set ts=8 sw=4 et :
