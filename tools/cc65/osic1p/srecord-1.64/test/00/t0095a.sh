#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2004, 2006-2008 Peter Miller
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

TEST_SUBJECT="stewie format"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S110000048656C6C6F2C20576F726C640A9D
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S0030000FC
S11C000053303033533110000048656C6C6F2C20576F726C640A9D5338EF
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.ok.srec -o test.ok -bin
if test $? -ne 0; then fail; fi

#
# Read Motorola format and write Stewie format.
#
srec_cat test.in -o test.out -stewie
if test $? -ne 0; then fail; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

#
# Read Stewie format and write Motorola format.
#
srec_cat test.out -stewie -o test.out2 -header HDR
if test $? -ne 0; then fail; fi

diff test.in test.out2
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
