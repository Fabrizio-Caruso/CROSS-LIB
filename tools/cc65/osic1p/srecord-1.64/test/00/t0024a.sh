#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 1999, 2006-2008 Peter Miller
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

TEST_SUBJECT="split filter"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S123000054686520717569636B2062726F776E20666F78206A756D7073206F76657220740A
S11000206865206C617A7920646F672E0A90
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S11A0000546571696B626F6E66786A6D736F65206820617964670A55
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -split 2 0 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S121000068657175636B6272776E666F206A6D70206F65727468206C7A79646F2E0A9C
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -split 3 1 2 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
