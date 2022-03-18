#!/bin/sh
#
# srecord - manipulate eprom load files
# Copyright (C) 1998, 1999, 2006-2008, 2012 Peter Miller
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

TEST_SUBJECT="length filter"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S1230000436F70797269676874202843292031393938205065746572204D696C6C65723B94
S11900200A416C6C207269676874732072657365727665642E0A3A
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S107010000000236BF
S1230300436F70797269676874202843292031393938205065746572204D696C6C65723B91
S11903200A416C6C207269676874732072657365727665642E0A37
S5030003F9
S9030300F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat - -offset 0x300 -length-be 0x100 < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S107010036020000BF
S1230300436F70797269676874202843292031393938205065746572204D696C6C65723B91
S11903200A416C6C207269676874732072657365727665642E0A37
S5030003F9
S9030300F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat - -offset 0x300 -length-le 0x100 < test.in > test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
# vim: set ts=8 sw=4 et :
