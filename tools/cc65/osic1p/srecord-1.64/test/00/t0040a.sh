#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="srec_cmp --verbose"
. test_prelude

cat > test.one << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C642E0A6E
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.two << 'fubar'
S00600004844521B
S111000068656C6C6F5F5F686F726C642E0ACB
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok1 << 'fubar'
Different:      (0x1, 0x2, 0x4 - 0xd)
Left only:      0xe
Right only:     0x0
Execution start address 1 not equal to 0.
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok2 << 'fubar'
Different:      (0x1, 0x2, 0x4 - 0xd)
Left only:      0xe
Right only:     0
Execution start address 1 not equal to 0.
fubar
if test $? -ne 0; then no_result; fi

srec_cmp -verbose test.one -offset 1 test.two > test.out 2>&1
if test $? -ne 2; then fail; fi

diff test.ok1 test.out > LOG 2>&1 || diff test.ok2 test.out >> LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
