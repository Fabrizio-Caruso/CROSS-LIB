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

TEST_SUBJECT="basic srecord functionality"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S1130000285F245F2212226A000424290008237C2A
S11300100002000800082629001853812341001813
S113002041E900084E42234300182342000824A952
S107003000144ED492
S5030004F8
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1230000285F245F2212226A000424290008237C0002000800082629001853812341001851
S117002041E900084E42234300182342000824A900144ED418
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
