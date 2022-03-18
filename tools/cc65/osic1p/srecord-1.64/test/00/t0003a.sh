#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 1999, 2002, 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="intel functionality"
. test_prelude

cat > test.in << 'fubar'
:10000000DB00E60F5F1600211100197ED300C3004C
:1000100000000101030307070F0F1F1F3F3F7F7FF2
:01002000FFE0
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1230000DB00E60F5F1600211100197ED300C30000000101030307070F0F1F1F3F3F7F7F4A
S1040020FFDC
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -intel -o test.out -motorola -header HDR > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.in << 'fubar'
S00600004844521B
S1230000DB00E60F5F1600211100197ED300C30000000101030307070F0F1F1F3F3F7F7F4A
S1040020FFDC
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
:020000040000FA
:20000000DB00E60F5F1600211100197ED300C30000000101030307070F0F1F1F3F3F7F7F4E
:01002000FFE0
:0400000500000000F7
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -motorola -o test.out -intel > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.in1 << 'fubar'
:10000000DB00E60F5F1600211100197ED300C3004C
:1000100000000101030307070F0F1F1F3F3F7F7FF2
:01002000FFE0
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

cat > test.in2 << 'fubar'
:20000000DB00E60F5F1600211100197ED300C30000000101030307070F0F1F1F3F3F7F7F4E
:01002000FFE0
:0400000500000000F7
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

srec_cmp test.in1 -intel test.in2 -intel > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
