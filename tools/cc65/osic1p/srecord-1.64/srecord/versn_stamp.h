//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2006, 2007, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General
// Public License along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_COMMON_VERSN_STAMP_H
#define SRECORD_COMMON_VERSN_STAMP_H

namespace srecord
{

/**
  * The version_stamp function is used to obtain the version number,
  * as a string.  This reflects Aegis' idea of the version, so you can
  * re-create that Aegis version easily.
  */
const char *version_stamp(void);

/**
  * The copyright_years function is used to obtain the range of
  * copyright years for the copyright notice.  This reflects Aegis'
  * tracking of the project, and updated automatically.
  */
const char *copyright_years(void);

/**
  * The print_version function is used to print the program version, and
  * brief copyright notice, to the standard output.
  */
void print_version(void);

};

#endif // SRECORD_COMMON_VERSN_STAMP_H
