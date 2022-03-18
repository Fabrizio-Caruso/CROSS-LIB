#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2001, 2006-2008 Peter Miller
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

TEST_SUBJECT="tektronix format write"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S12300007035569B64D86309D19FA14439839DD06C3768B1E17406A4BEC3720F13BCB3F0F1
S12300202C3F4EC78A52260314DEA57281952564FADA8110A2C0A098E8929EF401780AAE53
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
/000020027035569B64D86309D19FA14439839DD06C3768B1E17406A4BEC3720F13BCB3F0AE
/002020042C3F4EC78A52260314DEA57281952564FADA8110A2C0A098E8929EF401780AAEB3
/00000000
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -tek
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
