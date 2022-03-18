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

TEST_SUBJECT="fletcher16 answer"
. test_prelude

srec_cat - -o test.in -bin << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi


# Note that 0 == 0xFF mod 255

cat > test.ok << 'fubar'
0x6028
0xFFFF
fubar
if test $? -ne 0; then no_result; fi

test_fletcher16 0 0 0x0000 < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi


# try again for different answer

cat > test.ok << 'fubar'
0x91A4
0xADDE
fubar
if test $? -ne 0; then no_result; fi

test_fletcher16 0 0 0xDEAD < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# try again for different answer

cat > test.ok << 'fubar'
0x8CCA
0xCEFA
fubar
if test $? -ne 0; then no_result; fi

test_fletcher16 0 0 0xFACE < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# try again for different answer

cat > test.ok << 'fubar'
0x8227
0x2143
fubar
if test $? -ne 0; then no_result; fi

test_fletcher16 0 0 0x4321 < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
