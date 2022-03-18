#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2013 Peter Miller
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

TEST_SUBJECT="filter checksum bitnot"
. test_prelude


# ----------  Bitnot Checksum  ----------------------------------------------
#
# Test the checksum of all bytes 0x01
#
srec_cat -gen 0 0x10 --const 0x01 \
        -header=HDR -esa 0 \
        > test.in
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S113000001010101010101010101010101010101DC
S1070400FFFFFFEF08
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -Checksum_BitNot_Big_Endian 0x400 4 > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ----------  Bitnot Checksum  ----------------------------------------------
#
# Test the checksum of all bytes 0xFF
#
srec_cat -gen 0 0x10 --const 0xFF \
        -header=HDR -esa 0 \
        > test.in
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1130000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC
S1070400FFFFF00FF7
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -Checksum_BitNot_Big_Endian 0x400 4 > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
