#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2011 Peter Miller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or (at
# your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

TEST_SUBJECT="ancient Intel end-of-file records"
. test_prelude

cat > test.in << 'fubar'
:1000000012C02CC02BC02AC029C028C027C026C0BF
:1000100025C0FBC123C080C245C2EBC11FC01EC0AA
:1017F000000000000201010053454C20200034BBD2
:00FF0F01F1
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
:020000040000FA
:2000000012C02CC02BC02AC029C028C027C026C025C0FBC123C080C245C2EBC11FC01EC079
:1017F000000000000201010053454C20200034BBD2
:040000050000FF0FE9
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -intel -o test.out -intel > log 2>&1
if test $? -ne 0
then
    cat log
    fail
fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
