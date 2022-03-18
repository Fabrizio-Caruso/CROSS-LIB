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

TEST_SUBJECT="Formatted Binary format"
. test_prelude

# ---------- test reading Formatted Binary ---------------------------------

cat > test.in.srec << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1230000081C2A49080000000200FF010204081020408055AA0102030405060708090A0BFC
S11200200C0D0E0F10111213141516000002FB15
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in.srec -o test.in -bin
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1230000010204081020408055AA0102030405060708090A0B0C0D0E0F10111213141516E1
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cmp test.in -fb test.ok
if test $? -ne 0; then fail; fi

# ---------- test writing Formatted Binary ---------------------------------

cat > test.in.srec << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in.srec -o test.in -bin
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S1200000081C2A4908000000000EFF48656C6C6F2C20576F726C64210A0000047349
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.ok.srec -o test.ok -bin
if test $? -ne 0; then no_result; fi

srec_cat test.in -bin -o test.out -fb
if test $? -ne 0; then fail; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
