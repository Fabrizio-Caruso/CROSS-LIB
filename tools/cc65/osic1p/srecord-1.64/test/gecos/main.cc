//
// srecord - Manipulate EPROM load files
// Copyright (C) 2014 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pwd.h>
#include <string>
#include <unistd.h>
#include <vector>

#include <srecord/arglex.h>


typedef std::vector<std::string> string_list_t;


static std::string
trim(const std::string &text)
{
    bool white_space = false;
    std::string result;
    const char *cp = text.c_str();
    for (;;)
    {
        unsigned char c = *cp++;
        if (!c)
            break;
        if (isspace(c))
        {
            white_space = !result.empty();
        }
        else
        {
            if (white_space)
                result += ' ';
            result += c;
            white_space = false;
        }
    }
    return result;
}


static void
split(string_list_t &result, const char *text, const char *sep)
{
    const char *ep = text;
    for (;;)
    {
        if (*ep == '\0')
            return;

        const char *p = strpbrk(ep, sep);
        if (!p)
        {
            std::string name(ep);
            result.push_back(trim(name));
            return;
        }

        std::string name(ep, p - ep);
        result.push_back(trim(name));
        ep = p + 1;
    }
}


int
main(int argc, char **argv)
{
    srecord::arglex cmdline(argc, argv);
    cmdline.token_first();
    for (;;cmdline.token_next())
    {
        switch (cmdline.token_cur())
        {
        case srecord::arglex::token_version:
            return EXIT_SUCCESS;

        case srecord::arglex::token_eoln:
            break;

        default:
            cmdline.default_command_line_processing();
            continue;
        }
        break;
    }

    struct passwd *pw;
    pw = getpwuid(getuid());
    if (!pw)
    {
        fprintf(stderr, "getpwui(%d) failed", getuid());
        return EXIT_FAILURE;
    }

    string_list_t gecos;
    split(gecos, pw->pw_gecos, ",");

    string_list_t words;
    split(words, gecos[0].c_str(), " \t\n");

    if (words.size() >= 1)
        printf(" -una %s\n", words[0].c_str());
    if (words.size() >= 2)
        printf(" -gna %s\n", words[1].c_str());

    return EXIT_SUCCESS;
}


// vim: set ts=8 sw=4 et :
