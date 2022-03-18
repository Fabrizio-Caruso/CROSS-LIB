//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2006-2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <srecord/progname.h>
#include <srecord/quit/normal.h>


srecord::quit_normal::~quit_normal()
{
}


srecord::quit_normal::quit_normal()
{
}


void
srecord::quit_normal::exit(int n)
{
    ::exit(n);
}


void
srecord::quit_normal::message_v(const char *fmt, va_list ap)
{
    char buf[2000];
    snprintf(buf, sizeof(buf), "%s: ", progname_get());
    size_t len = strlen(buf);
    vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);

    std::cout.flush();

    int column = 0;
    char *cp = buf;
    for (;;)
    {
        unsigned char c = *cp++;
        if (!c)
            break;
        if (isspace(c) || !isprint(c))
            continue;

        std::string word;
        for (;;)
        {
            word += c;
            c = *cp;
            if (!c)
                break;
            if (isspace(c) || !isprint(c))
                break;
            ++cp;
        }
        if (!column)
        {
            std::cerr << word;
            column = word.size();
        }
        else if (column + 1 + word.size() > 80)
        {
            std::cerr << std::endl << "    ";
            column = 4;
            std::cerr << word;
            column += word.size();
        }
        else
        {
            std::cerr << ' ';
            ++column;
            std::cerr << word;
            column += word.size();
        }
    }
    std::cerr << std::endl;
    std::cerr.flush();
}


static srecord::quit_normal quit_normal_default;
srecord::quit &srecord::quit_default = quit_normal_default;
