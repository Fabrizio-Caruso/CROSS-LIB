#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007, 2008, 2011 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
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
const unsigned char eprom[] =
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

srec_cat test.in -o test.out -c-array -section-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with words ----------------------------------------

cat > test.ok << 'fubar'
/* HDR */
const unsigned short eprom[] =
{
0x48FF, 0x6C65, 0x6F6C, 0x202C, 0x6F57, 0x6C72, 0x2164, 0xFF0A, 0x6548,
0x6C6C, 0x2C6F, 0x5720, 0x726F, 0x646C, 0x0A21, 0x6548, 0x6C6C, 0x2C6F,
0x5720, 0x726F, 0x646C, 0x0A21,
};

const unsigned long eprom_address[] =
{
0x00000006, 0x00000020, 0x00000042,
};
const unsigned long eprom_word_address[] =
{
0x00000003, 0x00000010, 0x00000021,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000008, 0x00000007, 0x00000007,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000006;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x0000004A;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000006
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x0000004A
#define EPROM_SECTIONS    0x00000003
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 \
    -o test.out -c-array -section-style -ow
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with decimal words -----------------------------

cat > test.ok << 'fubar'
/* HDR */
const unsigned short eprom[] =
{
18687, 27749, 28524, 8236, 28503, 27762, 8548, 65290, 25928, 27756, 11375,
22304, 29295, 25708, 2593, 25928, 27756, 11375, 22304, 29295, 25708, 2593,
};

const unsigned long eprom_address[] =
{
6, 32, 66,
};
const unsigned long eprom_word_address[] =
{
3, 16, 33,
};
const unsigned long eprom_length_of_sections[] =
{
8, 7, 7,
};
const unsigned long eprom_sections    = 3;
const unsigned long eprom_termination = 7;
const unsigned long eprom_start       = 6;
const unsigned long eprom_finish      = 80;
const unsigned long eprom_length      = 74;

#define EPROM_TERMINATION 7
#define EPROM_START       6
#define EPROM_FINISH      80
#define EPROM_LENGTH      74
#define EPROM_SECTIONS    3
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 \
    -o test.out -c-array -section-style -ow -dec-style
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- one more time, with words and a generated include file -----------

cat > test.ok << 'fubar'
/* HDR */
const unsigned short eprom[] =
{
0x48FF, 0x6C65, 0x6F6C, 0x202C, 0x6F57, 0x6C72, 0x2164, 0xFF0A, 0x6548,
0x6C6C, 0x2C6F, 0x5720, 0x726F, 0x646C, 0x0A21, 0x6548, 0x6C6C, 0x2C6F,
0x5720, 0x726F, 0x646C, 0x0A21,
};

const unsigned long eprom_address[] =
{
0x00000006, 0x00000020, 0x00000042,
};
const unsigned long eprom_word_address[] =
{
0x00000003, 0x00000010, 0x00000021,
};
const unsigned long eprom_length_of_sections[] =
{
0x00000008, 0x00000007, 0x00000007,
};
const unsigned long eprom_sections    = 0x00000003;
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000006;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x0000004A;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000006
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x0000004A
#define EPROM_SECTIONS    0x00000003
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.h << 'fubar'
#ifndef TEST_H
#define TEST_H

extern const unsigned long eprom_termination;
extern const unsigned long eprom_start;
extern const unsigned long eprom_finish;
extern const unsigned long eprom_length;
extern const unsigned long eprom_sections;
extern const unsigned short eprom[];
extern const unsigned long eprom_address[];
extern const unsigned long eprom_word_address[];
extern const unsigned long eprom_length_of_sections[];

#endif /* TEST_H */
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 \
    -o test.out -c-array -section-style -ow -include
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

diff test.ok.h test.h
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
