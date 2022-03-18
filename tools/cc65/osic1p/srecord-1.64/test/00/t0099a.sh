#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2005-2008 Peter Miller
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

TEST_SUBJECT="motorola 2 (word addresses)"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S12300004A8E641A273D44AD3588C7BF3F860EA96BB6035EED9CEB5F4637D105E34FD8229E
S123002044A3AB427AF583A2AC951B0C61ECAA63F074A4BC8E6367BCD2F1BC0DD548D42815
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S12300004A8E641A273D44AD3588C7BF3F860EA96BB6035EED9CEB5F4637D105E34FD8229E
S123001044A3AB427AF583A2AC951B0C61ECAA63F074A4BC8E6367BCD2F1BC0DD548D42825
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -mot 2
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

srec_cat test.out 2 -o test.back
if test $? -ne 0; then fail; fi

diff test.in test.back
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
