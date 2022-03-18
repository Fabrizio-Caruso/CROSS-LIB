#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2006-2008 Peter Miller
#
#       This program is free software; you can redistribute it and/or modify
#       it under the terms of the GNU General Public License as published by
#       the Free Software Foundation; either version 3 of the License, or
#       (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public License
#       along with this program. If not, see
#       <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="format guessing"
. test_prelude

# --------------------------------------------------------------------------
#
cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Format: Motorola S-Record
Header: "HDR"
Execution Start Address: 00000000
Data:   0000 - 000D
fubar
if test $? -ne 0; then no_result; fi

srec_info test.in -guess > test.out 2>test.err
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# --------------------------------------------------------------------------
#
cat > test.in << 'fubar'
:0E00000048656C6C6F2C20576F726C64210A7F
:0400000500000000F7
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Format: Intel Hexadecimal (MCS-86)
Execution Start Address: 00000000
Data:   0000 - 000D
fubar
if test $? -ne 0; then no_result; fi

srec_info test.in -guess > test.out 2>test.err
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# --------------------------------------------------------------------------
#
cat > test.in << 'fubar'
K0005B4865B6C6CB6F2CB2057B6F72B6C64B210A7F6C7F
:
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Format: Texas Instruments Tagged (SDSMAC)
Data:   0000 - 000D
fubar
if test $? -ne 0; then no_result; fi

srec_info test.in -guess > test.out 2>test.err
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
