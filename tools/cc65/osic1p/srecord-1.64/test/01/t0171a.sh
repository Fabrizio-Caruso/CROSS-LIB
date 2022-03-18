#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2009, 2011 Peter Miller
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

TEST_SUBJECT="read MIF"
. test_prelude

cat > test.in << 'fubar'
DEPTH = 14;
WIDTH = 8;
ADDRESS_RADIX = HEX;
DATA_RADIX = HEX;
CONTENT BEGIN
[0..77]: 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A;
78: 01 02 03 04;
[80..ff]: 00;
END;
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
00000000: 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A 48 65  #Hello, World!.He
00000010: 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A 48 65 6C 6C  #llo, World!.Hell
00000020: 6F 2C 20 57 6F 72 6C 64 21 0A 48 65 6C 6C 6F 2C  #o, World!.Hello,
00000030: 20 57 6F 72 6C 64 21 0A 48 65 6C 6C 6F 2C 20 57  # World!.Hello, W
00000040: 6F 72 6C 64 21 0A 48 65 6C 6C 6F 2C 20 57 6F 72  #orld!.Hello, Wor
00000050: 6C 64 21 0A 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64  #ld!.Hello, World
00000060: 21 0A 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A  #!.Hello, World!.
00000070: 48 65 6C 6C 6F 2C 20 57 01 02 03 04              #Hello, W....
00000080: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
00000090: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
000000A0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
000000B0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
000000C0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
000000D0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
000000E0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
000000F0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  #................
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -mif -o test.out -hexdump
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
