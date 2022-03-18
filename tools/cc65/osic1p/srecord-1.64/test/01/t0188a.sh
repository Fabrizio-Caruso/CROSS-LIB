#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2009, 2011, 2014 Peter Miller
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

TEST_SUBJECT="l2h-crc-16"
. test_prelude

echo "123456789" | dd bs=9 count=1 > test.in 2>/dev/null
if test $? -ne 0; then no_result; fi

# augmented

cat > test.ok << 'fubar'
0xD1A2
fubar
if test $? -ne 0; then no_result; fi

# This uses the low-to-high flag, reversing the order of the bits within
# each byte as they flow through the algorithm.  This is done *inside*
# the crc16 table and updcrc code.
test_crc16 -r < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# Now we double check.  We use the old high-to-low code, but we bitrev
# each byte before we feed it to the algorithm, and then bitrev the
# resulting CRC.  This should give identical answers to the above.
test_crc16 -h < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# un-augmented

cat > test.ok << 'fubar'
0x1E67
fubar
if test $? -ne 0; then no_result; fi

# This uses the low-to-high flag, reversing the order of the bits within
# each byte as they flow through the algorithm.  This is done *inside*
# the crc16 table and updcrc code.
test_crc16 -a -r < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# Now we double check.  We use the old high-to-low code, but we bitrev
# each byte before we feed it to the algorithm, and then bitrev the
# resulting CRC.  This should give identical answers to the above.
test_crc16 -a -h < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
