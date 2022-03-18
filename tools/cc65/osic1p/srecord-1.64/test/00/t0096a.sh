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

TEST_SUBJECT="CRLF"
. test_prelude

cat > test.in << 'fubar'
S109000068656C6C6F0AD8
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S1230000533032323030303036383734373437303341324632463733373236353633364603
S12300203732363432453733364637353732363336353636364637323637363532453645C8
S123004036353734324631440D0A53313039303030303638363536433643364630414438E1
S11100600D0A533530333030303146420D0A2C
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.ok.srec -o test.ok -bin > LOG 2>&1
if test $? -ne 0; then cat LOG; no_result; fi

srec_cat test.in -o test.out -crlf > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
