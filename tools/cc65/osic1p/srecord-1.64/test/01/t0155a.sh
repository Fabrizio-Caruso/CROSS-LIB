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

TEST_SUBJECT="hexdump"
. test_prelude

cat > test.in << 'fubar'
Now is the time for all good men to come to the aid of their party.
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
00000000: 4E 6F 77 20 69 73 20 74                6D 65 20  #Now is t     me
00000010: 66 6F 72 20 61 6C 6C 20 67 6F 6F 64 20 6D 65 6E  #for all good men
00000020: 20 74 6F 20 63 6F 6D 65 20 74                    # to come t
00000030:                      74 68 65 69 72 20 70 61 72  #       their par
00000040: 74 79 2E 0A                                      #ty..
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -bin -exclude 0x08 0x0D 0x2a 0x37 -o test.out -hexdump
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
