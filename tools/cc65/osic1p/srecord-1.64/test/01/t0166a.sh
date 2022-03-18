#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2008, 2011, 2012 Peter Miller
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

TEST_SUBJECT="catenate vs parentheses"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S31502001000EDAC0123212223240200100CA723D28651
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat                                                                 \
    \(                                                                   \
        test.in --exclude 0x02001000 0x2001010                           \
        -generate 0x02001000 0x02001004 -repeat-data 0xED 0xAC 0x01 0x23 \
        -generate 0x02001004 0x02001008 -repeat-data 0x21 0x22 0x23 0x24 \
    \)                                                                   \
    --length-b-e 0x02001008 4                                            \
    -checksum-neg_b-e 0x0200100C 4 4                                     \
    -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
