#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2001, 2006-2008 Peter Miller
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

TEST_SUBJECT="Atmel Generic write"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S10B00002C019413940E00502E
S10700A00C1295089D
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
000000:2C01
000001:9413
000002:940E
000003:0050
000050:0C12
000051:9508
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -ag
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
