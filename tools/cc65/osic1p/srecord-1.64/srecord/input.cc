//
// srecord - manipulate eprom load files
// Copyright (C) 1998-2000, 2002, 2003, 2005-2008, 2010 Peter Miller
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

#include <errno.h>
#include <iostream>
#include <cstring>

#include <srecord/input.h>
#include <srecord/record.h>
#include <srecord/quit/prefix.h>


srecord::input::input() :
    quitter(&quit_default)
{
}


srecord::input::~input()
{
}


void
srecord::input::fatal_error(const char *fmt, ...)
    const
{
    va_list ap;
    va_start(ap, fmt);
    quit_prefix blab(*quitter, filename_and_line());
    blab.fatal_error_v(fmt, ap);
    va_end(ap);
}


void
srecord::input::fatal_error_errno(const char *fmt, ...)
    const
{
    va_list ap;
    va_start(ap, fmt);
    quit_prefix blab(*quitter, filename_and_line());
    blab.fatal_error_errno_v(fmt, ap);
    va_end(ap);
}


void
srecord::input::warning(const char *fmt, ...)
    const
{
    va_list ap;
    va_start(ap, fmt);
    quit_prefix blab(*quitter, filename_and_line());
    blab.warning_v(fmt, ap);
    va_end(ap);
}


std::string
srecord::input::filename_and_line()
    const
{
    return filename();
}


void
srecord::input::set_quit(quit &arg)
{
    quitter = &arg;
}


void
srecord::input::reset_quit()
{
    quitter = &quit_default;
}


void
srecord::input::command_line(srecord::arglex_tool *)
{
    // Do nothing.
}
