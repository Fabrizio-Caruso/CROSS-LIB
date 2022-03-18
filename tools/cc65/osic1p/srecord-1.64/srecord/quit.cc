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

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>

#include <srecord/quit.h>


srecord::quit::~quit()
{
}


srecord::quit::quit()
{
}


srecord::quit::quit(const quit &)
{
}


srecord::quit &
srecord::quit::operator=(const quit &)
{
    return *this;
}


void
srecord::quit::fatal_error(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fatal_error_v(fmt, ap);
    va_end(ap);
}


void
srecord::quit::fatal_error_v(const char *fmt, va_list ap)
{
    message_v(fmt, ap);
    exit(1);
}


void
srecord::quit::fatal_error_errno(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fatal_error_errno_v(fmt, ap);
    va_end(ap);
}


void
srecord::quit::fatal_error_errno_v(const char *fmt, va_list ap)
{
    int n = errno;
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    fatal_error("%s: %s", buf, strerror(n));
}


void
srecord::quit::warning(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    warning_v(fmt, ap);
    va_end(ap);
}


void
srecord::quit::warning_v(const char *fmt, va_list ap)
{
    char buf[1024];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    message("warning: %s", buf);
}


void
srecord::quit::message(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    message_v(fmt, ap);
    va_end(ap);
}
