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

TEST_SUBJECT="hexdump input"
. test_prelude

cat > test.in << 'fubar'
00000000:
          22
             49 20 64 6F 6E 27 74 20 63 61 72 65 20 69 66  #"I don't care if
00000010: 20 69 74 20 77 6F 72 6B 73 20 6F 6E 20 79 6F 75  # it works on you
00000020: 72 20 6D 61 63 68 69 6E 65 2C 20 77 65 20 61 72  #r machine, we ar
00000030: 65 6E 27 74 20 73 68 69 70 70 69 6E 67 20 79 6F  #en't shipping yo
00000040: 75 72 0A 6D 61 63 68 69 6E 65 2E 22 20 2D 2D 20  #ur.machine." --
00000050: 56 69 64 69 75 20 50 6C 61 74 6F 6E 0A           #Vidiu Platon.
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S1230000224920646F6E2774206361726520696620697420776F726B73206F6E20796F75FE
S123002072206D616368696E652C207765206172656E2774207368697070696E6720796F42
S120004075720A6D616368696E652E22202D2D20566964697520506C61746F6E0A56
S5030003F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -hexdump -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
