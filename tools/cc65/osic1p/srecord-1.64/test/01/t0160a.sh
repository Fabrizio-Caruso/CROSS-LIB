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

TEST_SUBJECT="interval subtraction"
. test_prelude

cat > test.in << 'fubar'
S005000068741E
S104000010EB
S1040002F108
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cp test.in test.ok
if test $? -ne 0; then no_result; fi

#
# strictly speaking, the braces are redundant, but they are there (a) to
# help the human reader, and (b) if we change the presedence.
#
srec_cat test.in -fill 0xff -over { test.in -exclude -within test.in } \
    -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
