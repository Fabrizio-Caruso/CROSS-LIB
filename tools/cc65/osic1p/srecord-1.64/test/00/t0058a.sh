#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2001, 2006-2008, 2014 Peter Miller
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

TEST_SUBJECT="-multiple"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S1140000436F6D706C65746520527562626973680AB9
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -rb=ignore -cb=ignore > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

srec_cmp -dsw test.in test.out -rb=ignore -cb=ignore > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
# vim: set ts=8 sw=4 et :
