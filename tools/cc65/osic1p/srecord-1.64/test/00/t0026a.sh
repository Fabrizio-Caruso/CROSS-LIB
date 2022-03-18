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

TEST_SUBJECT="unsplit filter"
. test_prelude

cat > test.in << 'fubar'
Hello, World!
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
Hello, World!
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -bin -split 2 0 -o test.even > log 2>&1
if test $? -ne 0; then cat log; fail; fi

srec_cat test.in -bin -split 2 1 -o test.odd > log 2>&1
if test $? -ne 0; then cat log; fail; fi

srec_cat test.even -unsplit 2 0 test.odd -unsplit 2 1 \
        -o test.out -bin > log 2>&1
if test $? -ne 0; then cat log; fail; fi

diff test.out test.ok

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
