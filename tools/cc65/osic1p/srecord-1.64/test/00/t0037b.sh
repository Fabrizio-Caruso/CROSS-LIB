#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2003, 2006-2008, 2013 Peter Miller
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

TEST_SUBJECT="tekext format, read"
. test_prelude

#
# Test reading this format
#
cat > test.in << 'fubar'
%1A626810000000202020202020
%0E81F810000000
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S30B1000000020202020202024
S5030001FB
S70510000000EA
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -tekext -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
