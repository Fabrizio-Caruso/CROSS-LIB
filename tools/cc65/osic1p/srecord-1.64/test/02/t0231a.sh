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

TEST_SUBJECT="read -tek-ex csum bug"
. test_prelude

cat > test.in << 'fubar'
Hello, World!
fubar
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
%4E6E280000000054495041635953580D5953580600030001000E00050200000300000001595358
%4E664800000020C0168311C0BB000077367735F7317726C4353C055A00BF0746674E5628008001
%0E81E800000000
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Format: Tektronix Extended
Execution Start Address: 00000000
Data:   0000 - 003F
fubar
if test $? -ne 0; then no_result; fi

srec_info test.in -tex > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
