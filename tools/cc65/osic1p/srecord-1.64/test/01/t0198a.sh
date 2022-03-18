#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2009 David Kozub <zub@linux.fjfi.cvut.cz>
# Copyright (C) 2009, 2011 Peter Miller
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

TEST_SUBJECT="msbin write"
. test_prelude

# Simple Motorola file
cat > test.in << 'fubar'
S00600004844521B
S31480000000112233445566778899AABBCCDDEEFF73
S30980000040DEADBEEFFE
S3088000100056789AFF
S5030003F9
S7058000000476
fubar
if test $? -ne 0; then no_result; fi

# Target msbin file, encoded as Motorola:
cat > test.ok.src << 'fubar'
S00600004844521B
S12300004230303046460A0000008003100000000000800F000000F8070000112233445554
S123002066778899AABBCCDDEEFF400000800400000038030000DEADBEEF001000800300F9
S118004000006801000056789A00000000040000800000000052
S5030003F9
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

# reconstruct original msbin binary
srec_cat test.ok.src -o test.ok -binary
if test $? -ne 0; then no_result; fi

# encode test.in into MsBin format
srec_cat test.in -o test.out -MsBin
if test $? -ne 0; then fail; fi

# test if the encoded msbin file is identical
cmp test.out test.ok
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
