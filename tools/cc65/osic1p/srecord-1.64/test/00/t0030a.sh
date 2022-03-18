#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1999, 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="WindowsNT binary"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S11A0000546869732069730A61206E617374790D0A746573742E0D85
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.int -bin > log 2>&1
if test $? -ne 0; then cat log; fail; fi

srec_cat test.int -bin -o test.out -header HDR > log 2>&1
if test $? -ne 0; then cat log; fail; fi

diff test.in test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
