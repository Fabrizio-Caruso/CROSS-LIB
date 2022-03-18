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

TEST_SUBJECT="msbin round-trip"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111FFF848656C6C6F2C20576F726C64210A84
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.middle -MsBin
if test $? -ne 0; then fail; fi

# make sure that round-trip data is identical
srec_cat test.middle -MsBin -o test.out --header HDR
if test $? -ne 0; then fail; fi

diff test.in test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
