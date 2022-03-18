#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2006-2008, 2010 Peter Miller
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

TEST_SUBJECT="ti-tagged format"
. test_prelude

# --------------------------------------------------------------------------
#
# Test writing the format
#
cat > test.in << 'fubar'
S00600004844521B
S10A000048656C6C6F2C20B5
S10A0123576F726C64210A9E
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
K0008HDR90000B4865B6C6CB6F2C*2090123B576FB726CB6421*0A7F3DCF
:
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -ti-tagged
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi


# --------------------------------------------------------------------------
#
# Test reading the format
#
cat > test.ok << 'fubar'
S00A00006F756768696E6704
S10A000048656C6C6F2C20B5
S10A0123576F726C64210A9E
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

cat > test.in << 'fubar'
K000Coughing7FBBAF
B4865B6C6CB6F2C*207FBBDF
90123B576FB726CB6421*0A7FAD2F
:
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -ti-tagged -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
