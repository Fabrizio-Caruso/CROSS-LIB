#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2001, 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="EMON52 format"
. test_prelude

# --------------------------------------------------------------------------
#
# Test reading the format.
#
cat > test.in << 'fubar'
10 0000:57 6F 77 21 20 44 69 64 20 79 6F 75 20 72 65 61 0564
10 0010:6C 6C 79 20 67 6F 20 74 68 72 6F 75 67 68 20 61 05E9
10 0020:6C 6C 20 74 68 69 73 20 74 72 6F 75 62 6C 65 20 05ED
10 0030:74 6F 20 72 65 61 64 20 74 68 69 73 20 73 74 72 05F0
04 0040:69 6E 67 21 015F
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1230000576F77212044696420796F75207265616C6C7920676F207468726F75676820618F
S12300206C6C20746869732074726F75626C6520746F2072656164207468697320737472DF
S1070040696E672159
S5030003F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -emon52 -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# --------------------------------------------------------------------------
#
# Test writing the format.
#
cat > test.ok << 'fubar'
10 0000:57 6F 77 21 20 44 69 64 20 79 6F 75 20 72 65 61 0564
10 0010:6C 6C 79 20 67 6F 20 74 68 72 6F 75 67 68 20 61 05E9
10 0020:6C 6C 20 74 68 69 73 20 74 72 6F 75 62 6C 65 20 05ED
10 0030:74 6F 20 72 65 61 64 20 74 68 69 73 20 73 74 72 05F0
04 0040:69 6E 67 21 015F
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -emon52 -o test.out -emon52
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

srec_cmp test.in -emon52 test.out -emon52
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
