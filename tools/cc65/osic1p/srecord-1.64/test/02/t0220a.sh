#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2012, 2014 Peter Miller
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

TEST_SUBJECT="crc-16-ansi"
. test_prelude

echo "123456789" | dd bs=9 count=1 > test.in  2>/dev/null
if test $? -ne 0; then no_result; fi

# If anyone has the ANSI stantda test vectors,
# I'd rather use those.
cat > test.ok << 'fubar'
0x9ECF
fubar
if test $? -ne 0; then no_result; fi

test_crc16 -p ansi < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
