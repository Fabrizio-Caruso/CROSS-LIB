//
// srecord - The "srecord" program.
// Copyright (C) 2007-2010, 2012 Peter Miller
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

#include <srecord/output/file/brecord.h>
#include <srecord/record.h>


srecord::output_file_brecord::~output_file_brecord()
{
}


srecord::output_file_brecord::output_file_brecord(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name),
    block_size(BUFFER_MAXIMUM_MAXIMUM)
{
}


srecord::output::pointer
srecord::output_file_brecord::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_brecord(a_file_name));
}


void
srecord::output_file_brecord::write(const srecord::record &record)
{
    switch (record.get_type())
    {
    case srecord::record::type_execution_start_address:
        put_4bytes_be(record.get_address());
        put_byte(0);
        put_char('\n');
        break;

    default:
        // This format can't do any other record types
        break;

    case srecord::record::type_data:
        put_4bytes_be(record.get_address());
        assert(record.get_length() <= BUFFER_MAXIMUM_MAXIMUM);
        put_byte(record.get_length());
        for (unsigned j = 0; j < record.get_length(); ++j)
            put_byte(record.get_data(j));
        put_char('\n');
        break;
    }
}


void
srecord::output_file_brecord::line_length_set(int w)
{
    int x = (w - 10) / 2;
    if (x < 2)
        x = 2;
    else if (x > BUFFER_MAXIMUM_MAXIMUM)
        x = BUFFER_MAXIMUM_MAXIMUM;
    block_size = x;
}


void
srecord::output_file_brecord::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


bool
srecord::output_file_brecord::preferred_block_size_set(int nbytes)
{
    if (nbytes < 2 || nbytes > BUFFER_MAXIMUM_MAXIMUM)
        return false;
    block_size = nbytes;
    return true;
}


int
srecord::output_file_brecord::preferred_block_size_get()
    const
{
    return block_size;
}


const char *
srecord::output_file_brecord::format_name()
    const
{
    return "B-Record";
}


// vim: set ts=8 sw=4 et :
