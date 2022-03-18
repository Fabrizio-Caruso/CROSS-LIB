#!/bin/sh
#
#       srecord - Manipulate EPROM load files
#       Copyright (C) 2008, 2011 Peter Miller
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

TEST_SUBJECT="motorola --disable=data-count"
. test_prelude

cat > test.in << 'fubar'
Hello, World!
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S10D000048656C6C6F2C20576F727A
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -bin -crop 0 10 -o test.out --disable=data-count --disable=head
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
