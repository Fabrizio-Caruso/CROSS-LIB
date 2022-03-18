#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007, 2008, 2011 Peter Miller
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

TEST_SUBJECT="fill range padding"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000748656C6C6F2C20576F726C64210A74
S111002048656C6C6F2C20576F726C64210A5B
S111004248656C6C6F2C20576F726C64210A39
S5030003F9
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1170004FFFFFF48656C6C6F2C20576F726C64210AFFFFFF77
S113002048656C6C6F2C20576F726C64210AFFFF5B
S1130040FFFF48656C6C6F2C20576F726C64210A3B
S5030003F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-pad 4 \
        -o test.out > LOG 2>&1
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
