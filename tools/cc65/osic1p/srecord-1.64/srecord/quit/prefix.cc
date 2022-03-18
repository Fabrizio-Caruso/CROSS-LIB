//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2006-2008, 2010 Peter Miller
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

#include <cstdio>
#include <cstdarg>

#include <srecord/quit/prefix.h>


srecord::quit_prefix::~quit_prefix()
{
}


srecord::quit_prefix::quit_prefix(quit &a1, const char *a2) :
    prefix(a2),
    deeper(a1)
{
}


srecord::quit_prefix::quit_prefix(quit &a1, const std::string &a2) :
    prefix(a2),
    deeper(a1)
{
}


void
srecord::quit_prefix::exit(int n)
{
    deeper.exit(n);
}


void
srecord::quit_prefix::message_v(const char *fmt, va_list ap)
{
    if (prefix != "")
    {
        char buf[1024];
        vsnprintf(buf, sizeof(buf), fmt, ap);
        deeper.message
        (
            "%.*s: %s",
            (int)prefix.length(),
            prefix.data(),
            buf
        );
    }
    else
        deeper.message_v(fmt, ap);
}
