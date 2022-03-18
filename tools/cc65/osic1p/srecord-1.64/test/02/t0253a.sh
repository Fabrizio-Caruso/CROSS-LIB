#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2014 Scott Finneran
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="-c-array -section-style"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000748656C6C6F2C20576F726C64210A74
S111002048656C6C6F2C20576F726C64210A5B
S111004248656C6C6F2C20576F726C64210A39
S5030003F9
S9030007F5
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
__root __no_init const unsigned char eprom[] @0x20000000 =
{
0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
0x21, 0x0A, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57, 0x6F, 0x72,
0x6C, 0x64, 0x21, 0x0A, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57,
0x6F, 0x72, 0x6C, 0x64, 0x21, 0x0A,
};

const unsigned long eprom_address[] =
{
0x00000007, 0x00000020, 0x00000042,
};
const unsigned long eprom_length_of_sections[] =
{
0x0000000E, 0x0000000E, 0x0000000E,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000007;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x00000049;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000007
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x00000049
#define EPROM_SECTIONS    0x00000003
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -c-array -section-style \
    -prefix="__root __no_init" -postfix="@0x20000000"
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with only prefix -------------------------

cat > test.in << 'fubar'
S00600004844521B
S111000748656C6C6F2C20576F726C64210A74
S5030001FB
S9030007F5
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
__root __no_init const unsigned char eprom[] =
{
0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
0x21, 0x0A,
};
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000007;
const unsigned long eprom_finish      = 0x00000015;
const unsigned long eprom_length      = 0x0000000E;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000007
#define EPROM_FINISH      0x00000015
#define EPROM_LENGTH      0x0000000E
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -c-array -prefix="__root __no_init"
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with only postfix -------------------------

cat > test.in << 'fubar'
S00600004844521B
S111000748656C6C6F2C20576F726C64210A74
S5030001FB
S9030007F5
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
const unsigned char eprom[] @0x20000000 =
{
0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64,
0x21, 0x0A,
};
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000007;
const unsigned long eprom_finish      = 0x00000015;
const unsigned long eprom_length      = 0x0000000E;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000007
#define EPROM_FINISH      0x00000015
#define EPROM_LENGTH      0x0000000E
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -c-array -postfix="@0x20000000"
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
