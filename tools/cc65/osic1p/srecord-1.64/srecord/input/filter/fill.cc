//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2004, 2006-2010 Peter Miller
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

#include <cstring>

#include <srecord/interval.h>
#include <srecord/input/filter/fill.h>
#include <srecord/record.h>


srecord::input_filter_fill::~input_filter_fill()
{
    delete [] filler_block;
}


srecord::input_filter_fill::input_filter_fill(const input::pointer &a1,
        int a2, const interval &a3) :
    input_filter(a1),
    filler_value(a2),
    filler_block(0),
    range(a3)
{
}


srecord::input::pointer
srecord::input_filter_fill::create(const input::pointer &a_deeper,
    int a_value, const interval &a_range)
{
    return pointer(new input_filter_fill(a_deeper, a_value, a_range));
}


bool
srecord::input_filter_fill::generate(record &result)
{
    if (range.empty())
        return false;
    interval::data_t lo = range.get_lowest();
    size_t rec_len = record::maximum_data_length(lo);
    interval::data_t hi = lo + rec_len;
    interval chunk(lo, hi);
    chunk *= range;
    chunk.first_interval_only();
    size_t fill_block_size = 256;
    if (!filler_block)
    {
        filler_block = new unsigned char [fill_block_size];
        memset(filler_block, filler_value, fill_block_size);
    }
    rec_len = chunk.get_highest() - chunk.get_lowest();
    assert(rec_len <= fill_block_size);
    result = record(record::type_data, lo, filler_block, rec_len);
    range -= chunk;
    return true;
}


bool
srecord::input_filter_fill::read(record &result)
{
    if (!input_filter::read(result))
        return generate(result);
    if (result.get_type() == record::type_data)
    {
        range -=
            interval
            (
                result.get_address(),
                result.get_address() + result.get_length()
            );
    }
    return true;
}
