//
// srecord - Manipulate EPROM load files
// Copyright (C) 2010, 2011 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
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

#include <srecord/output/filter.h>


srecord::output_filter::~output_filter()
{
}


srecord::output_filter::output_filter(const output::pointer &a_deeper) :
    deeper(a_deeper)
{
}


void
srecord::output_filter::write(const record &r)
{
    deeper->write(r);
}


void
srecord::output_filter::line_length_set(int n)
{
    deeper->line_length_set(n);
}


void
srecord::output_filter::address_length_set(int n)
{
    deeper->address_length_set(n);
}


int
srecord::output_filter::preferred_block_size_get(void)
    const
{
    return deeper->preferred_block_size_get();
}


bool
srecord::output_filter::preferred_block_size_set(int nbytes)
{
    return deeper->preferred_block_size_set(nbytes);
}


const std::string
srecord::output_filter::filename(void)
    const
{
    return deeper->filename();
}


const char *
srecord::output_filter::format_name()
    const
{
    return deeper->format_name();
}


void
srecord::output_filter::notify_upper_bound(unsigned long addr)
{
    deeper->notify_upper_bound(addr);
}


void
srecord::output_filter::command_line(arglex_tool *cmdln)
{
    deeper->command_line(cmdln);
}


// vim: set ts=8 sw=4 et :
