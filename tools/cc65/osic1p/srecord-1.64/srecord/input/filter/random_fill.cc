//
// srecord - manipulate eprom load files
// Copyright (C) 2004, 2006-2008, 2010 Peter Miller
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
#include <srecord/r250.h>
#include <srecord/input/filter/random_fill.h>
#include <srecord/record.h>


srecord::input_filter_random_fill::~input_filter_random_fill()
{
}


srecord::input_filter_random_fill::input_filter_random_fill(
        const srecord::input::pointer &a1, const interval &a3) :
    srecord::input_filter(a1),
    range(a3)
{
}


srecord::input::pointer
srecord::input_filter_random_fill::create(const input::pointer &a_deeper,
    const interval &a_range)
{
    return pointer(new srecord::input_filter_random_fill(a_deeper, a_range));
}


bool
srecord::input_filter_random_fill::generate(srecord::record &record)
{
    if (range.empty())
        return false;
    unsigned char buffer[srecord::record::max_data_length];
    interval chunk(range.get_lowest(), range.get_lowest() + sizeof(buffer));
    chunk *= range;
    chunk.first_interval_only();
    int nbytes = chunk.get_highest() - chunk.get_lowest();
    for (int j = 0; j < nbytes; ++j)
        buffer[j] = r250();
    record =
        srecord::record
        (
            srecord::record::type_data,
            chunk.get_lowest(),
            buffer,
            nbytes
        );
    range -= chunk;
    return true;
}


bool
srecord::input_filter_random_fill::read(srecord::record &record)
{
    if (!srecord::input_filter::read(record))
        return generate(record);
    if (record.get_type() == srecord::record::type_data)
    {
        range -=
            interval
            (
                record.get_address(),
                record.get_address() + record.get_length()
            );
    }
    return true;
}
