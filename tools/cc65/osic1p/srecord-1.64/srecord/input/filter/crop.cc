//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001, 2002, 2006-2008, 2010 Peter Miller
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
#include <srecord/input/filter/crop.h>
#include <srecord/record.h>


srecord::input_filter_crop::~input_filter_crop()
{
}


srecord::input_filter_crop::input_filter_crop(
        const srecord::input::pointer &a_deeper, const interval &a_range) :
    srecord::input_filter(a_deeper),
    range(a_range),
    data(),
    data_range()
{
}


srecord::input::pointer
srecord::input_filter_crop::create(const srecord::input::pointer &a_deeper,
    const interval &a_range)
{
    return pointer(new srecord::input_filter_crop(a_deeper, a_range));
}


bool
srecord::input_filter_crop::read(srecord::record &record)
{
    for (;;)
    {
        //
        // If we are not holding any current data,
        // fetch another record from out input.
        //
        if (data_range.empty())
        {
            //
            // If the input is exhausted, we are done.
            //
            if (!srecord::input_filter::read(data))
            {
                return false;
            }
            switch (data.get_type())
            {
            default:
                //
                // If the input is of a type we don't
                // care about, pass it through.
                //
                record = data;
                return true;

            case srecord::record::type_data:
                //
                // Data records are remembered, and
                // doled out peicmeal, as they mask
                // against the clip region.
                //
                data_range =
                    interval
                    (
                        data.get_address(),
                        data.get_address() + data.get_length()
                    );
                break;

            case srecord::record::type_execution_start_address:
                //
                // Discard execution start address records which do not
                // fall into the clip region.
                //
                if (!range.member(data.get_address()))
                    continue;
                record = data;
                return true;
            }
        }

        //
        // Intersect the data left unprocessed in this record
        // with the clip region.  There could be more than one
        // region, but we only want the first one.  We will make
        // another pass if there is more than one.
        //
        interval fragment = range * data_range;
        if (fragment.empty())
        {
            data_range = interval();
            continue;
        }
        fragment.first_interval_only();

        //
        // Construct the return record from the data held.
        //
        unsigned long lo = fragment.get_lowest();
        unsigned long hi = fragment.get_highest();
        record =
            srecord::record
            (
                srecord::record::type_data,
                lo,
                data.get_data() + lo - data.get_address(),
                hi - lo
            );

        //
        // Subtract this segment from the data held.  If there
        // is anything left, another pass will construct another
        // data record for it.
        //
        data_range -= fragment;

        //
        // Success
        //
        return true;
    }
}
