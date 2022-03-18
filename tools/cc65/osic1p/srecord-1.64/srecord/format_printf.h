//
// srecord - manipulate eprom load files
// Copyright (C) 2001, 2002, 2005-2008, 2010 Peter Miller
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

#ifndef SRECORD_FORMAT_PRINTF_H
#define SRECORD_FORMAT_PRINTF_H

#ifdef __GNUC__
#define FORMAT_PRINTF(x, y) __attribute__((__format__(printf, x, y)))
#define NORETURN __attribute__((__noreturn__))
#define DEPRECATED __attribute__((__deprecated__))
#else
#define FORMAT_PRINTF(x, y)
#define DEPRECATED
#define NORETURN
#endif

#endif // SRECORD_FORMAT_PRINTF_H
