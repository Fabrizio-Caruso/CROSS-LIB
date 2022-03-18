//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2003, 2006-2008, 2010 Peter Miller
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

#ifndef SRECORD_PROGNAME_H
#define SRECORD_PROGNAME_H

namespace srecord
{

/**
  * The progname_set function is used by main() to set the name of the
  * currently executing programme.
  */
void progname_set(char *);

/**
  * The progname_get function is used to retrieve the name of the
  * currently executing programme.  Used by error messages.
  */
const char *progname_get();

};

#endif // SRECORD_PROGNAME_H
