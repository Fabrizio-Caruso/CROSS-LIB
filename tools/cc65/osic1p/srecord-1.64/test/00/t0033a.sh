#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="wilson format"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S123000016BCE78D82BA86FC3BB52876C7509572DE88A48326862D938186AFDE8C4C65A2C0
S1230020094C308B06B687416CAFB833004DA5DED54962FCD08F8F51153E2FA18B9444947D
S1230040E0741FE62AA7289656E0C9562D6E3503B897FF88268ED93BCC08DD579D21EB7DC0
S10E0060950B64BFB28C56086C1F5F48
S5030004F8
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.uue << 'fubar'
S00600004844521B
S1230000236540404040563B3CE7CDC23B3AC6FC7B3B3568B63C3790D5B23D3EC83A34C3D3
S123002066C66DD3C1C63A3F3D3ECC8CA53A323B3E0A236540404060498C70CB463B36C7B3
S123004081AC3A3F3B3873408D3A353D3E3D3589A2FC3D30CFCF91557E6F3A31CBD484D480
S1230060BB0A236540404080E0B45FE66A3A3768D696E03C39966DAE75433B38D7FFC86632
S1230080CE3D397B3C3C483D3D97DD61EBBD3B3E0A2350404040A0D54BA43B3F3B32CC9683
S11100A048AC5F9F860A274540404040FA0A5C
S5030006F6
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.ok.uue -o test.ok -bin
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -wilson
if test $? -ne 0; then fail; fi

cmp test.ok test.out
if test $? -ne 0; then fail; fi

#
# Test input
#
mv test.out test.in
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123000016BCE78D82BA86FC3BB52876C7509572DE88A48326862D938186AFDE8C4C65A2C0
S1230020094C308B06B687416CAFB833004DA5DED54962FCD08F8F51153E2FA18B9444947D
S1230040E0741FE62AA7289656E0C9562D6E3503B897FF88268ED93BCC08DD579D21EB7DC0
S10E0060950B64BFB28C56086C1F5F48
S5030004F8
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -wilson -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
