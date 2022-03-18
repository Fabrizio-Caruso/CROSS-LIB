#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2006-2008, 2013 Peter Miller
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

TEST_SUBJECT="line length"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S1190000416E642057696E646F7773204D75737420446965210AA2
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1090000416E6420576903
S10900066E646F777320A5
S109000C4D7573742044DD
S10700126965210AED
S5030004F8
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -line-leng 26
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
# vim: set ts=8 sw=4 et :
