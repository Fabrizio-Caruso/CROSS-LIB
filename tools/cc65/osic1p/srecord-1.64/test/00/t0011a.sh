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

TEST_SUBJECT="-crop -within"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S1230000436F70797269676874202843292031393938205065746572204D696C6C65723B94
S119002020416C6C207269676874732072657365727665642E0A24
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > mask << 'fubar'
S00600004844521B
S10700005858585898
S10700105858585888
S10700205858585878
S10700305858585868
S5030004F8
S9030030CC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1070000436F70795D
S10700103938205007
S107002020416C6C9F
S10700307276656417
S5030004F8
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crop -within mask -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
