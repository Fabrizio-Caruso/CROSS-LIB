//
// srecord - Manipulate EPROM load files
// Copyright (C) 2009-2012, 2014 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/input/filter/message.h>
#include <srecord/record.h>


srecord::input_filter_message::~input_filter_message()
{
}


srecord::input_filter_message::input_filter_message(
    const input::pointer &a_deeper,
    bool a_naked
) :
    input_filter(a_deeper),
    naked(a_naked),
    buffer_pos(0),
    have_forwarded_header(false),
    have_given_result(false),
    have_forwarded_start_address(false)
{
}


unsigned
srecord::input_filter_message::get_minimum_alignment(void)
    const
{
    return 0;
}


bool
srecord::input_filter_message::read(record &result)
{
    //
    // If we haven't read the deeper input yet, read all of it into
    // a memory buffer, then message the bytes.
    //
    if (buffer.empty())
    {
        buffer.reader(ifp, defcon_ignore, defcon_warning);

        unsigned multiple = get_minimum_alignment();
        if (multiple >= 2 && !buffer.is_well_aligned(multiple))
        {
            warning
            (
                "The %s filter uses %u-byte alignment, but unaligned "
                "data is present.  Use a \"--fill 0xNN --within <input> "
                "--range-padding %u\" filter *before* the %s filter to fix "
                "this problem.  "
                "See srec_info(1) for how to see the data ranges.",
                get_algorithm_name(),
                multiple,
                multiple,
                get_algorithm_name()
            );
        }

        if (buffer.has_holes())
        {
            warning
            (
                "The data presented for %s calculation has at least one hole "
                "in it.  This is bad.  It means that the in-memory "
                "calculation performed by your embedded system will be "
                "different than the calculation performed here.  You are "
                "strongly advised to use the \"--fill 0xFF --over <inoput>\" "
                "filter *before* the %s filter to ensure both calculations are "
                "using the same byte values.  "
                "See srec_info(1) for how to see the holes.",
                get_algorithm_name(),
                get_algorithm_name()
            );
        }
    }

    //
    // Pass on the header of the deeper file.
    //
    if (!have_forwarded_header)
    {
        have_forwarded_header = true;
        record *rp = buffer.get_header();
        if (rp)
        {
            result = *rp;
            return true;
        }
    }

    //
    // Calculate the result.
    //
    if (!have_given_result)
    {
        have_given_result = true;
        process(buffer, result);
        return true;
    }

    //
    // Now forward the rest of the data.
    //
    if (!naked)
    {
        unsigned long ret_address = buffer_pos;
        unsigned char data[64];
        size_t nbytes = sizeof(data);
        if (buffer.find_next_data(ret_address, data, nbytes))
        {
            result = record(record::type_data, ret_address, data, nbytes);
            buffer_pos = ret_address + nbytes;
            return true;
        }
    }

    //
    // Now forward on the start address of the deeper file.  (We could
    // have done it any time after reading the data, but we do last
    // because that is where "real" format usually put it.)
    //
    if (!have_forwarded_start_address)
    {
        have_forwarded_start_address = true;
        record *rp = buffer.get_execution_start_address();
        if (rp)
        {
            result = *rp;
            return true;
        }
    }

    //
    // All done.
    //
    return false;
}


// vim: set ts=8 sw=4 et :
