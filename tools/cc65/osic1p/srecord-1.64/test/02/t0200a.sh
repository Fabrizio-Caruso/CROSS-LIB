#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2010, 2011 Peter Miller
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

TEST_SUBJECT="srecord::output_filter_reblock"
. test_prelude

# Note: the position of this data depends on srecord::memory_chunk::size
#        defined in srecord/memory/chunk.h
# This test of of data alignment spanning a chunk boundary.
# If you change that value, you will have to change this test.
cat > test.in << 'fubar'
S00600004844521B
S12306E0000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C1D1E1F06
S1230700202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3FE5
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S322000006E0000102030405060708090A0B0C0D0E0F101112131415161718191A1B1C61
S322000006FD1D1E1F202122232425262728292A2B2C2D2E2F30313233343536373839FB
S30B0000071A3A3B3C3D3E3F68
S5030003F9
S70500000000FA
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out --address-length=4 \
        --output-block-size=29  --output-block-packing
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
