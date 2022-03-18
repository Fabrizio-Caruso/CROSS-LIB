//
// srecord - Manipulate EPROM load files
// Copyright (C) 2010, 2011, 2013 Peter Miller
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

#include <srecord/fletcher16.h>


static void
usage(void)
{
    fprintf(stderr, "Usage: test_fletcher16 [ <sum1> <sum2> [ <answer> ]]\n");
    exit(1);
}


int
main(int argc, char **argv)
{
    unsigned char sum1 = 0xFF;
    unsigned char sum2 = 0xFF;
    int answer = -1;
    if (argc == 2 && argv[1][0] == '-')
        return 0;
    switch (argc)
    {
    case 4:
        answer = strtol(argv[3], 0, 0);
        // Fall through...

    case 3:
        sum2 = strtol(argv[2], 0, 0);
        sum1 = strtol(argv[1], 0, 0);
        break;

    case 1:
        break;

    default:
        usage();
    }

    srecord::fletcher16 checksum(sum1, sum2, answer, srecord::endian_little);
    srecord::fletcher16 checksum1(sum1, sum2, -1);
    for (;;)
    {
        char buffer[1024];
        int n = read(0, buffer, sizeof(buffer));
        if (n <= 0)
            break;
        checksum.nextbuf(buffer, n);
        checksum1.nextbuf(buffer, n);
    }
    printf("0x%04X\n", checksum.get());
    if (answer >= 0)
    {
        fflush(stdout);
        unsigned n = checksum.get();

        // big endian ?!?
        checksum1.next(n >> 8);
        checksum1.next(n & 0xFF);
        printf("0x%04X\n", checksum1.get());
    }
    return 0;
}


// vim: set ts=8 sw=4 et :
