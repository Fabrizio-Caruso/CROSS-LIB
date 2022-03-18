#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2009, 2011, 2012 Peter Miller
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

TEST_SUBJECT="adler16"
. test_prelude

# This test vector is from the Wikipedia page...
cat > test.in << 'fubar'
S00600004844521B
S10C000057696B6970656469615C
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

# ...as is this test result.
cat > test.ok << 'fubar'
S00600004844521B
S10C000057696B6970656469615C
S105010040A712
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -adler16-b-e 0x100 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
