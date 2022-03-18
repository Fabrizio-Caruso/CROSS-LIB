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

#ifndef SRECORD_INPUT_INTERVAL_H
#define SRECORD_INPUT_INTERVAL_H

#include <srecord/interval.h>
#include <srecord/input.h>

namespace srecord {

/**
  * The srecord::input_interval function is used to read an input stream and
  * build an interval based on the addresses which contain data within
  * the input.
  */
interval input_interval(srecord::input::pointer);

};

#endif // SRECORD_INPUT_INTERVAL_H
