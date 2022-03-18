//
// srecord - manipulate eprom load files
// Copyright (C) 2006-2008, 2010 Peter Miller
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

#include <srecord/input/filter/sequence.h>
#include <srecord/record.h>


srecord::input_filter_sequence::~input_filter_sequence()
{
}


srecord::input_filter_sequence::input_filter_sequence(
        srecord::input::pointer arg) :
    srecord::input_filter(arg),
    last_address(0),
    warned(false)
{
}


srecord::input::pointer
srecord::input_filter_sequence::create(srecord::input::pointer arg)
{
    return pointer(new srecord::input_filter_sequence(arg));
}


bool
srecord::input_filter_sequence::read(srecord::record &record)
{
    if (!srecord::input_filter::read(record))
        return false;
    if (record.get_type() == srecord::record::type_data)
    {
#if 0
        fprintf(stderr, "%s: %d: address = %08lX, length = %08X\n",
            __FILE__, __LINE__,
            record.get_address(),
            record.get_length()
           );
#endif
        if (!warned)
        {
            //
            // Issue a warning if the data records are not in strictly
            // ascending address order.
            //
            if (record.get_address() < last_address)
            {
                warning
                (
                    "data records not in strictly ascending order "
                        "(expected >= 0x%04lX, got 0x%04lX)",
                    last_address,
                    long(record.get_address())
                );
                warned = true;
            }
        }
        last_address = record.get_address() + record.get_length();
    }
    return true;
}
