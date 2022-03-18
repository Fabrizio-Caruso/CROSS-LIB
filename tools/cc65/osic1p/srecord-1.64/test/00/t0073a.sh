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

TEST_SUBJECT="srec_cat -header"
. test_prelude

cat > test.in << 'fubar'
Hello, World!
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S01100005468697320697320612074657374F9
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -bin -o test.out -header "This is a test" > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "This is a test"
Data:   0000 - 000D
fubar
if test $? -ne 0; then no_result; fi

srec_info test.out > test.out2 2> LOG
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out2
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
