//
// srecord - Manipulate EPROM load files
// Copyright (C) 2010, 2013 Peter Miller
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
#include <getopt.h>
#include <string>

#include <srecord/progname.h>
#include <srecord/quit.h>
#include <srecord/versn_stamp.h>


static int number_of_errors;


static bool
read_one_line(const char *filename, FILE *fp, std::string &result)
{
    for (;;)
    {
        int c = getc(fp);
        if (c == EOF)
        {
            if (ferror(fp))
            {
                srecord::quit_default.fatal_error_errno("read %s", filename);
            }
            return !result.empty();
        }
        if (c == '\n')
            return true;
        result += (unsigned char)c;
    }
}


static void
check(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
        srecord::quit_default.fatal_error_errno("open %s", filename);
    int line_number = 0;
    for (;;)
    {
        std::string line;
        if (!read_one_line(filename, fp, line))
            break;
        ++line_number;
        switch (line.c_str()[0])
        {
        case '.':
        case '\0':
        case '\'':
            continue;

        default:
            {
                int n = 0;
                const char *cp = line.c_str();
                for (;;)
                {
                    unsigned char c = *cp++;
                    switch (c)
                    {
                    case '\0':
                        break;

                    case '-':
                        if (cp == line.c_str() + 1 || cp[-2] != '\\')
                        {
                            ++n;
                        }
                        continue;

                    default:
                        continue;
                    }
                    break;
                }
                switch (n)
                {
                case 0:
                    break;

                case 1:
                    fprintf
                    (
                        stderr,
                        "%s: %d: unescaped minus character\n",
                        filename,
                        line_number
                    );
                    ++number_of_errors;
                    break;

                default:
                    fprintf
                    (
                        stderr,
                        "%s: %d: %d unescaped minus characters\n",
                        filename,
                        line_number,
                        n
                    );
                    ++number_of_errors;
                    break;
                }
            }
            continue;
        }
        break;
    }
    fclose(fp);
}


static void
usage(void)
{
    const char *prog = srecord::progname_get();
    fprintf(stderr, "Usage: %s <filename>...\n", prog);
    fprintf(stderr, "       %s --version\n", prog);
    exit(1);
}


static struct option options[] =
{
    { "version", 0, 0, 'V' },
    { 0, 0, 0, 0 }
};


int
main(int argc, char **argv)
{
    srecord::progname_set(argv[0]);
    for (;;)
    {
        int c = getopt_long(argc, argv, "V", options, 0);
        switch (c)
        {
        case EOF:
            break;

        case 'V':
            srecord::print_version();
            return 0;

        default:
            usage();
        }
        break;
    }
    if (optind >= argc)
        usage();
    for (;;)
    {
        check(argv[optind]);
        ++optind;
        if (optind >= argc)
            break;
    }
    if (number_of_errors > 0)
    {
        srecord::quit_default.fatal_error
        (
            "found %d error%s",
            number_of_errors,
            (number_of_errors == 1 ? "" : "s")
        );
    }
    return 0;
}


// vim: set ts=8 sw=4 et :
