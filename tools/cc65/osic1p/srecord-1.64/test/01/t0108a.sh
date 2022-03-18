#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007, 2008, 2011 Peter Miller
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

TEST_SUBJECT="TI-Tagged"
. test_prelude

cat > test.in << 'fubar'
00050        7FDD4F
90000BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F400F
90010BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3FFF
90020BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3FEF
90030BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3FDF
90040BFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFFBFFFF7F3FCF
:
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1230000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC
S1230020FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC
S1130040FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBC
S5030003F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -tit -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
