#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007, 2008, 2010, 2011 Peter Miller
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

TEST_SUBJECT="srec_input minus expr"
. test_prelude

cat > in.srec << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S12301234E6F77206973207468652074696D6520666F7220616C6C20676F6F64206D656E6F
S123014320746F20636F6D6520746F2074686520616964206F66207468656972207061728B
S107016374792E0A6F
S5030003F9
S9030123D8
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "http://srecord.sourceforge.net/"
Execution Start Address: 00000000
Data:   0000 - 0043
fubar
if test $? -ne 0; then no_result; fi

srec_cat in.srec -offset - -minimum-address in.srec -o out.srec
if test $? -ne 0; then fail; fi

srec_info out.srec > test.out
if test $? -ne 0; then no_result; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
