#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2011 Peter Miller
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

TEST_SUBJECT="ppx hex read"
. test_prelude

cat > test.in << 'fubar'
*
0000 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0010 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0020 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0030 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0040 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0050 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0060 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0070 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78
0080 12 34 56 78 12 34 56 78 12 34 56 78 12 34 56 78 $S26D0
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123000012345678123456781234567812345678123456781234567812345678123456783C
S123002012345678123456781234567812345678123456781234567812345678123456781C
S12300401234567812345678123456781234567812345678123456781234567812345678FC
S12300601234567812345678123456781234567812345678123456781234567812345678DC
S1130080123456781234567812345678123456781C
S5030005F7
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -ppx -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
