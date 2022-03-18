//
// srecord - Manipulate EPROM load files
// Copyright (C) 2014 Peter Miller
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

#ifndef SRECORD_DEFCON_H
#define SRECORD_DEFCON_H

namespace srecord {


enum defcon_t
{
    defcon_ignore,
    defcon_warning,
    defcon_fatal_error
};


const char *text_from_defcon(defcon_t x);
int defcon_from_text(const char *text);


};

// vim: set ts=8 sw=4 et :
#endif // SRECORD_DEFCON_H
