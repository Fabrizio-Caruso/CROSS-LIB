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

#include <sstream>

#include <srecord/string.h>


static bool
unhex_nybble(char c, int &result)
{
    switch (c)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        result = (c - '0');
        return true;

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
        result = (c - 'A' + 10);
        return true;

    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
        result = (c - 'a' + 10);
        return true;

    default:
        return false;
    }
}


static bool
unhex_byte(const char *text, char &retval)
{
    int c1 = 0;
    if (!unhex_nybble(text[0], c1))
        return false;
    int c2 = 0;
    if (!unhex_nybble(text[1], c2))
        return false;
    retval = ((c1 << 4) | c2);
    return true;
}


std::string
srecord::string_url_decode(const std::string &text)
{
    const char *begin = text.c_str();
    const char *cur = begin;
    const char *end = begin + text.size();
    std::stringstream result;
    while (cur < end)
    {
        unsigned char c = *cur++;
        if (c == '%')
        {
            if (end - cur >= 1 && *cur == '%')
            {
                result << c;
                ++cur;
            }
            else
            {
                if (end - cur >= 2)
                {
                    char whatsit;
                    if (unhex_byte(cur, whatsit))
                    {
                        result << whatsit;
                        cur += 2;
                    }
                }
            }
        }
        else
        {
            result << c;
        }
    }
    return result.str();
}


// vim: set ts=8 sw=4 et :
