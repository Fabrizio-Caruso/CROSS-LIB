#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2006-2008 Peter Miller
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

TEST_SUBJECT="command line @file arguments"
. test_prelude

cat > test.in << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S111010048656C6C6F2C20576F726C64210A7A
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

cat > args << 'fubar'
test.in
--offset=0x100
-o test.out
fubar
if test $? -ne 0; then no_result; fi

srec_cat @args > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
