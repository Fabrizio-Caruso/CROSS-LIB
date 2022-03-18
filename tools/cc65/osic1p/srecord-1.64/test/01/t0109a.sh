#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2007, 2008, 2010 Peter Miller
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

TEST_SUBJECT="ti-tagged-16 format"
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
K0008HDR90000B4865B6C6CB6F2CB20FF90091BFF57B6F72B6C64B210A7F290F
:
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding 2 \
        -o test.out -ti-tagged-16
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi


# --------------------------------------------------------------------------
#
# Test reading the format
#
cat > test.in << 'fubar'
00028        7FDCFF
K000Coughing7FBBAF
90000BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F400F
90008BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3F8F
90010BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3FFF
90018BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3F7F
90020BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3FEF
:
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00A00006F756768696E6704
S1230000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC
S1230020FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC
S1130040FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBC
S5030003F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -ti-tagged-16 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
