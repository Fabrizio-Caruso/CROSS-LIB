//
// srecord - manipulate eprom load files
// Copyright (C) 1998, 1999, 2002, 2006-2008, 2010 Peter Miller
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

#include <cstring>

#include <srecord/progname.h>


static char *progname;


void
srecord::progname_set(char *s)
{
    for (;;)
    {
        char            *cp1;
        char            *cp2;

        cp1 = strrchr(s, '/');
        if (!cp1)
            cp1 = s;
        else
        {
            if (!cp1[1])
            {
                *cp1 = 0;
                continue;
            }
            ++cp1;
        }

        cp2 = strrchr(s, '\\');
        if (!cp2)
            cp2 = s;
        else
        {
            if (!cp2[1])
            {
                *cp2 = 0;
                continue;
            }
            ++cp2;
        }

        progname = (cp1 > cp2 ? cp1 : cp2);
        // avoid some libtool nonsense
        if (0 == memcmp(progname, "lt-", 3))
            progname += 3;
        size_t len = strlen(progname);
        if
        (
            progname[len - 4] == '.'
        &&
            (progname[len - 3] == 'e' || progname[len - 3] == 'E')
        &&
            (progname[len - 2] == 'x' || progname[len - 2] == 'X')
        &&
            (progname[len - 1] == 'e' || progname[len - 1] == 'E')
        )
            progname[len - 4] = '\0';
        return;
    }
}


const char *
srecord::progname_get()
{
    return (progname ? progname : "???");
}
