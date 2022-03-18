#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007, 2008, 2010, 2011 Peter Miller
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

TEST_SUBJECT="ascii-hex"
. test_prelude

cat > test.in << 'fubar'
S321000000007FD243A67FF343A63FC0003F3BDE700C3BE0000193FE00007FFA02A6A8
S3210000001C93FE00047FFB02A693FE00087FD242A67FF342A648001F040000000074
S3210000003800000000000000000000000000000000000000000000000000000000A6
S32100000054000000000000000000000000000000000000000000000000000000008A
fubar
if test $? -ne 0; then no_result; fi

cat > x.awk << 'fubar'
{
    line = $0
    sub(/\^B/, "\2", line)
    sub(/\^C/, "\3", line)
    print line
}
fubar
if test $? -ne 0; then no_result; fi

awk -f x.awk > test.ok << 'fubar'
^B $A0000,
7F D2 43 A6 7F F3 43 A6 3F C0 00 3F 3B DE 70 0C
3B E0 00 01 93 FE 00 00 7F FA 02 A6 93 FE 00 04
7F FB 02 A6 93 FE 00 08 7F D2 42 A6 7F F3 42 A6
48 00 1F 04 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
^C
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -ah -data-only > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
