//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_STRING_H
#define SRECORD_STRING_H

#include <string>

namespace srecord {

/**
  * The string_url_encode function build a new string by adding url
  * quoting ("$FF")
  */
std::string string_url_encode(const std::string &text);

/**
  * The string_url_decode function build a new string by removing url
  * quoting ("$FF")
  */
std::string string_url_decode(const std::string &text);

std::string string_quote_c(const std::string &text);

};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_STRING_H
