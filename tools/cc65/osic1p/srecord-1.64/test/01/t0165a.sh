#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2008, 2011 Peter Miller
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

TEST_SUBJECT="class srec_output_file_forth"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
HEX
48 0000 C!
65 0001 C!
6C 0002 C!
6C 0003 C!
6F 0004 C!
2C 0005 C!
20 0006 C!
57 0007 C!
6F 0008 C!
72 0009 C!
6C 000A C!
64 000B C!
21 000C C!
0A 000D C!

( THE END )
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -forth
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
