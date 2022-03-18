//
// srecord - The "srecord" program.
// Copyright (C) 2007, 2010, 2011 Peter Miller
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

#include <srecord/arglex/tool.h>


std::string
srecord::arglex_tool::get_string(const char *caption)
{
    switch (token_cur())
    {
    case token_string:
    case token_number:
        {
            std::string result = value_string();
            token_next();
            return result;
        }

    default:
        fatal_error
        (
            "string expected for %s before %s",
            caption,
            token_name(token_cur())
        );
        // NOTREACHED
        return std::string();
    }
}
