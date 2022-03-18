//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2006-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/quit/exception.h>


srecord::quit_exception::~quit_exception()
{
}


srecord::quit_exception::quit_exception()
{
}


void
srecord::quit_exception::exit(int)
{
    throw vomit();
}


void
srecord::quit_exception::message_v(const char *, va_list)
{
    // don't say anything
}
