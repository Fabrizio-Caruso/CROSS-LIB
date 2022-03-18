#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2006-2008 Peter Miller
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

TEST_SUBJECT="C-array"
. test_prelude

mkdir foo
test $? -eq 0 || no_result

cat > test.in << 'fubar'
S00600004844521B
S10A000048656C6C6F2C2DA8
S10A0010576F726C64210AB2
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
const unsigned char eprom[] =
{
0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x2C, 0x2D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x0A,
};
const unsigned long eprom_termination = 0x00000000;
const unsigned long eprom_start       = 0x00000000;
const unsigned long eprom_finish      = 0x00000017;
const unsigned long eprom_length      = 0x00000017;

#define EPROM_TERMINATION 0x00000000
#define EPROM_START       0x00000000
#define EPROM_FINISH      0x00000017
#define EPROM_LENGTH      0x00000017
fubar
test $? -eq 0 || no_result

srec_cat test.in -o foo/test.out -ca -incl
test $? -eq 0 || fail

diff test.ok foo/test.out
test $? -eq 0 || fail

cat > test.h.ok << 'fubar'
#ifndef FOO_TEST_H
#define FOO_TEST_H

extern const unsigned long eprom_termination;
extern const unsigned long eprom_start;
extern const unsigned long eprom_finish;
extern const unsigned long eprom_length;
extern const unsigned char eprom[];

#endif /* FOO_TEST_H */
fubar
test $? -eq 0 || no_result

diff test.h.ok foo/test.h
test $? -eq 0 || fail

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
