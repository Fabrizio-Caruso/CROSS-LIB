#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2004, 2006-2008 Peter Miller
#       Copyright (C) 2014 Scott Finneran
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

TEST_SUBJECT="AOMF format"
. test_prelude

# --------------------------------------------------------------------------
#
# Check writing AOMF
#

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S00500004F4B60
S12300000207000348445200001606120000214348656C6C6F2C20576F726C64210A1104D8
S10E00200900034844520000000012D5
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -offset 0x4321 -header HDR -o test.out -aomf
if test $? -ne 0; then fail; fi

srec_cat test.ok.srec -o test.ok -bin
if test $? -ne 0; then no_result; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

# --------------------------------------------------------------------------
#
# check reading AOMF
#

mv test.out test.in
if test $? -ne 0; then no_result; fi

cat > ok << 'fubar'
Format: Intel Absolute Object Module Format (AOMF)
Header: "HDR"
Data:   4321 - 432E
fubar
if test $? -ne 0; then no_result; fi

srec_info test.in -aomf > test.out
if test $? -ne 0; then fail; fi

diff ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
