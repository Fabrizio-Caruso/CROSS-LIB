#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2012 Peter Miller
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

TEST_SUBJECT="read TRS-80"
. test_prelude

cat > test.in.in << 'fubar'
05 03 48 44 52
01 10 00 00 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A
02 02 00 00
fubar
test $? -eq 0 || no_result

cat > test.ok.1 << 'fubar'
Format: TRS80
Header: "HDR"
Execution Start Address: 00000000
Data:   0000 - 000D
fubar
test $? -eq 0 || no_result

cat > test.ok.2 << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
test $? -eq 0 || no_result

# now build the TRS80 formatted file
srec_cat test.in.in -nh -o test.in -bin
test $? -eq 0 || no_result

srec_info test.in -trs80 > test.out
test $? -eq 0 || fail

diff test.ok.1 test.out
test $? -eq 0 || fail

srec_cat test.in -trs80 -o test.out
test $? -eq 0 || fail

diff test.ok.2 test.out
test $? -eq 0 || fail

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
