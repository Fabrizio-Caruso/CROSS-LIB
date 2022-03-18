//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/string.h>


std::string
srecord::string_quote_c(const std::string &arg)
{
    std::string result;
    result += '"';
    const char *cp = arg.c_str();
    for (;;)
    {
        unsigned char c = *cp++;
        switch (c)
        {
        case '\0':
            result += '"';
            return result;

        case '\t':
            result += "\\t";
            break;

        case '\n':
            result += "\\n";
            break;

        case '\r':
            result += "\\r";
            break;

        case '\f':
            result += "\\f";
            break;

        case '\\':
            result += "\\\\";
            break;

        default:
            if (isprint((unsigned char)c))
                result += c;
            else
            {
                result += '\\';
                result += '0' + ((c >> 6) & 3);
                result += '0' + ((c >> 3) & 7);
                result += '0' + (c & 7);
            }
            break;
        }
    }
}


// vim: set ts=8 sw=4 et :
