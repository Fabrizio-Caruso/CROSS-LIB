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

TEST_SUBJECT="-c-array -output-word"
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
const unsigned short eprom[] =
{
0x48FF, 0x6C65, 0x6F6C, 0x202C, 0x6F57, 0x6C72, 0x2164, 0xFF0A, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x6548, 0x6C6C, 0x2C6F, 0x5720, 0x726F,
0x646C, 0x0A21, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
0xFFFF, 0xFFFF, 0xFFFF, 0x6548, 0x6C6C, 0x2C6F, 0x5720, 0x726F, 0x646C,
0x0A21,
};
const unsigned long eprom_termination = 0x00000007;
const unsigned long eprom_start       = 0x00000006;
const unsigned long eprom_finish      = 0x00000050;
const unsigned long eprom_length      = 0x0000004A;

#define EPROM_TERMINATION 0x00000007
#define EPROM_START       0x00000006
#define EPROM_FINISH      0x00000050
#define EPROM_LENGTH      0x0000004A
fubar
if test $? -ne 0; then no_result; fi

cat > test.h.ok << 'fubar'
#ifndef TEST_H
#define TEST_H

extern const unsigned long eprom_termination;
extern const unsigned long eprom_start;
extern const unsigned long eprom_finish;
extern const unsigned long eprom_length;
extern const unsigned short eprom[];

#endif /* TEST_H */
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding=2 \
    -o test.out -c-array -ow -include
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

diff test.h.ok test.h
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
