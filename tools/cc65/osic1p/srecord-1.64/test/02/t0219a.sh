#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2011 Peter Miller
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

TEST_SUBJECT="option --guess"
. test_prelude

cat > test.in << 'fubar'
*
0000 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A $S0473
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
srec_info: test.in: the file format can be accessed directly with the
    -Prom_Pogrammer_heXadecimal command line option (-ppx)
Format: Stag Prom Programmer hexadecimal (PPX)
Data:   0000 - 000D
fubar
if test $? -ne 0; then no_result; fi

srec_info test.in --guess > test.out 2>&1
if test $? -ne 0
then
    cat test.out
    fail
fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
