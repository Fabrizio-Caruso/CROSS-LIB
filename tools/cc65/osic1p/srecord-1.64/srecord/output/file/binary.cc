//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2001-2003, 2006-2008, 2010, 2011 Peter Miller
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

#include <srecord/output/file/binary.h>
#include <srecord/record.h>


srecord::output_file_binary::~output_file_binary()
{
}


srecord::output_file_binary::output_file_binary(
    const std::string &a_file_name
) :
    srecord::output_file(a_file_name)
{
    if (line_termination == line_termination_native)
        line_termination = line_termination_binary;
}


srecord::output::pointer
srecord::output_file_binary::create(const std::string &a_file_name)
{
    return pointer(new srecord::output_file_binary(a_file_name));
}


void
srecord::output_file_binary::write(const srecord::record &record)
{
    // This format can't do header records or termination records
    if (record.get_type() != srecord::record::type_data)
        return;
    seek_to(record.get_address());
    const unsigned char *data = record.get_data();
    int length = record.get_length();
    while (length-- > 0)
        put_char(*data++);
}


void
srecord::output_file_binary::line_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


void
srecord::output_file_binary::address_length_set(int)
{
    //
    // Irrelevant.  Ignore.
    //
}


bool
srecord::output_file_binary::preferred_block_size_set(int)
{
    // Ignore.
    return true;
}


int
srecord::output_file_binary::preferred_block_size_get()
    const
{
    //
    // Irrelevant.  Use the largest we can get.
    //
    return srecord::record::max_data_length;
}


const char *
srecord::output_file_binary::format_name()
    const
{
    return "Binary";
}


bool
srecord::output_file_binary::is_binary(void)
    const
{
    return true;
}
