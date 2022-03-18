//
// srecord - The "srecord" program.
// Copyright (C) 2007-2010, 2012, 2013 Peter Miller
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
#include <unistd.h>
#include <getopt.h>

#include <srecord/bitrev.h>
#include <srecord/crc16.h>
#include <srecord/progname.h>
#include <srecord/quit.h>
#include <srecord/versn_stamp.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: [ -av ] %s\n", srecord::progname_get());
    exit(1);
}


static const struct option options[] =
{
    { "augment", 0, 0, 'a' },
    { "broken", 0, 0, 'b' },
    { "ccitt", 0, 0, 'c' },
    { "help", 0, 0, 'h' },
    { "polynomial", 1, 0, 'p' },
    { "reverse", 0, 0, 'r' },
    { "table", 0, 0, 't' },
    { "version", 0, 0, 'V' },
    { "xmodem", 0, 0, 'x' },
};


int
main(int argc, char **argv)
{
    srecord::progname_set(argv[0]);
    srecord::crc16::seed_mode_t seed_mode = srecord::crc16::seed_mode_ccitt;
    bool augment = true;
    unsigned short polynomial = srecord::crc16::polynomial_ccitt;
    bool print_table = false;
    srecord::crc16::bit_direction_t bitdir =
        srecord::crc16::bit_direction_most_to_least;
    bool h_flag = false;
    for (;;)
    {
        int c = getopt_long(argc, argv, "abchp:rtVx", options, 0);
        if (c == EOF)
            break;
        switch (c)
        {
        case 'a':
            augment = !augment;
            break;

        case 'b':
            seed_mode = srecord::crc16::seed_mode_broken;
            break;

        case 'c':
            seed_mode = srecord::crc16::seed_mode_ccitt;
            break;

        case 'h':
            h_flag = true;
            break;

        case 'p':
            {
                char *ep = 0;
                polynomial = strtol(optarg, &ep, 0);
                if (ep == optarg || *ep)
                    polynomial = srecord::crc16::polynomial_by_name(optarg);
            }
            break;

        case 'r':
            bitdir = srecord::crc16::bit_direction_least_to_most;
            break;

        case 't':
            print_table = true;
            break;

        case 'x':
            seed_mode = srecord::crc16::seed_mode_xmodem;
            break;

        case 'V':
            srecord::print_version();
            return 0;

        default:
            usage();
            // NOTREACHED
        }
    }
    if (optind != argc)
        usage();

    srecord::crc16 check(seed_mode, augment, polynomial, bitdir);
    if (print_table)
    {
        check.print_table();
        return 0;
    }
    for (;;)
    {
        char buffer[1024];
        int n = read(0, buffer, sizeof(buffer));
        if (n < 0)
            srecord::quit_default.fatal_error_errno("read stdin");
        if (!n)
            break;
        if (h_flag)
        {
            for (int j = 0; j < n; ++j)
                buffer[j] = srecord::bitrev8(buffer[j]);
        }
        check.nextbuf(buffer, n);
    }

    // The h_flags is use to validate the crc16 least-to-most code.
    // Because that code calculate the CRC bit reversed, we bit reverse
    // here so that we can test for identical-ness.
    if (h_flag)
        printf("0x%04X\n", srecord::bitrev16(check.get()));
    else
        printf("0x%04X\n", check.get());
    return 0;
}


// vim: set ts=8 sw=4 et :
