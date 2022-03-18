#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2008, 2011 Peter Miller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="number from --minimum"
. test_prelude

cat > x1.srec << 'fubar'
S00600004844521B
S10D000A01010101010101010101DE
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > x2.srec << 'fubar'
S00600004844521B
S10D002802020202020202020202B6
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123000A0101010101010101010100000000000000000000000000000000000000000202C4
S10B002A0202020202020202BA
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat \
        x1.srec \
        x2.srec -fill 0 -maximum-address x1.srec -minimum-address x2.srec \
        -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
