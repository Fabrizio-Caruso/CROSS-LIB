//
// srecord - manipulate eprom load files
// Copyright (C) 2002, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_CONFIG_MESSY_H
#define SRECORD_CONFIG_MESSY_H

//
// If gcrypt is too old, there is no point trying to make it work.
//
#if defined(HAVE_LIBGCRYPT) && !defined(HAVE_GCRY_MD_HD_T)
#undef HAVE_LIBGCRYPT
#endif

//
// Make sure Solaris includes POSIX extensions.
//
#if (defined(__sun) || defined(__sun__) || defined(sun)) && \
        (defined(__svr4__) || defined(svr4))

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 1
#endif

#ifndef __EXTENSIONS__
#define __EXTENSIONS__ 1
#endif

//
// fix a glitch in Solaris's <sys/time.h>
// which only show's up when you turn __EXTENSIONS__ on
//
#define _timespec timespec      // fix 2.4
#define _tv_sec tv_sec          // fix 2.5.1

#endif // sun

//
// Cygwin's mingw mode...
//
#if defined(__MSVCRT__)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#endif

#endif // SRECORD_CONFIG_MESSY_H
