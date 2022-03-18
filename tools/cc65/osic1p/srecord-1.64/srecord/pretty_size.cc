//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008-2010 Peter Miller
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

#include <cstdio>

#include <srecord/pretty_size.h>


std::string
srecord::pretty_size(long long x, int width)
{
    double tmp = x;
    bool negative = false;
    if (x < 0)
    {
        negative = true;
        tmp = -tmp;
    }
    const char *suffix = " kMGTPEZY";
    while (tmp >= 1024)
    {
        tmp /= 1024;
        ++suffix;
    }
    if (*suffix == ' ')
        suffix = "";
    else
        --width;
    if (width < 1)
        width = 1;
    if (negative)
        tmp = -tmp;
    char buffer[20];
    int prec = 0;
    if (tmp >= 100)
        prec = 0;
    else if (tmp >= 10)
        prec = 1;
    else
        prec = 2;
    snprintf(buffer, sizeof(buffer), "%*.*f%.1s", width, prec, tmp, suffix);
    return buffer;
}
