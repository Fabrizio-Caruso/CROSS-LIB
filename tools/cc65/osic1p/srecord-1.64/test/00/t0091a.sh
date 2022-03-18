#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2004, 2006-2008 Peter Miller
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

TEST_SUBJECT="Verilog VMEM format"
. test_prelude

# ---------- Test Writing (64-bit) -----------------------------------------

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
@00000000 48656C6C6F2C2057 6F726C64210AFFFF
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding 8 \
    -o test.out -vmem 64
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Test Writing (128-bit) ----------------------------------------

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/* HDR */
@00000000 48656C6C6F2C20576F726C64210AFFFF
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-pad 16 \
    -o test.out -vmem 128
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
