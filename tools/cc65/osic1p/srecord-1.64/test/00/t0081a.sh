#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="BASIC output format"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S12300004E6F77206973207468652074696D6520666F7220616C6C20676F6F64206D656E93
S123002020746F20636F6D6520746F2074686520616964206F6620746865697220706172AF
S107004074792E0A93
S5030003F9
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
REM HDR
DATA 78,111,119,32,105,115,32,116,104,101,32,116,105,109,101,32,102,111,114
DATA 32,97,108,108,32,103,111,111,100,32,109,101,110,32,116,111,32,99,111
DATA 109,101,32,116,111,32,116,104,101,32,97,105,100,32,111,102,32,116,104
DATA 101,105,114,32,112,97,114,116,121,46,10
REM termination = 0
REM start = 0
REM finish = 68
REM length = 68
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -basic
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
