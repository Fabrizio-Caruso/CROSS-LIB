#!/bin/sh
#
# srecord - The "srecord" program.
# Copyright (C) 2007, 2008, 2011, 2012 Peter Miller
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

TEST_SUBJECT="CRC16 -no-aug"
. test_prelude

# ---------- First Test Vector: Zero Length Data ---------------------------

cat > test.in << 'fubar'
S01300007A65726F206C656E6774682064617461D0
S5030000FC
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S01300007A65726F206C656E6774682064617461D0
S10501001D0FCD
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -o test.out 2> LOG
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S01300007A65726F206C656E6774682064617461D0
S1050100FFFFFB
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -no-augment -o test.out 2> LOG
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Second Test Vector: Single 'A' character -----------------------

cat > test.in << 'fubar'
S015000073696E676C652041206368617261637465723A
S104000041BA
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S015000073696E676C652041206368617261637465723A
S104000041BA
S10501009479EC
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S015000073696E676C652041206368617261637465723A
S104000041BA
S1050100E1B167
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -no-augment -o test.out 2>/dev/null
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Third Test Vector: Single nine "123456789" digits -------------

cat > test.in << 'fubar'
S00E00006E696E6520646967697473A3
S10C000031323334353637383916
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00E00006E696E6520646967697473A3
S10C000031323334353637383916
S1050100E5CC48
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00E00006E696E6520646967697473A3
S10C000031323334353637383916
S1050100A69DB6
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -no-augment -o test.out 2>/dev/null
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ---------- Fourth Test Vector: 256 'A' characters ------------------------

cat > test.in << 'fubar'
S0150000323536206F662041206368617261637465722A
S12300004141414141414141414141414141414141414141414141414141414141414141BC
S123002041414141414141414141414141414141414141414141414141414141414141419C
S123004041414141414141414141414141414141414141414141414141414141414141417C
S123006041414141414141414141414141414141414141414141414141414141414141415C
S123008041414141414141414141414141414141414141414141414141414141414141413C
S12300A041414141414141414141414141414141414141414141414141414141414141411C
S12300C04141414141414141414141414141414141414141414141414141414141414141FC
S12300E04141414141414141414141414141414141414141414141414141414141414141DC
S5030008F4
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S0150000323536206F662041206368617261637465722A
S12300004141414141414141414141414141414141414141414141414141414141414141BC
S123002041414141414141414141414141414141414141414141414141414141414141419C
S123004041414141414141414141414141414141414141414141414141414141414141417C
S123006041414141414141414141414141414141414141414141414141414141414141415C
S123008041414141414141414141414141414141414141414141414141414141414141413C
S12300A041414141414141414141414141414141414141414141414141414141414141411C
S12300C04141414141414141414141414141414141414141414141414141414141414141FC
S12300E04141414141414141414141414141414141414141414141414141414141414141DC
S1050100E938D8
S5030009F3
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S0150000323536206F662041206368617261637465722A
S12300004141414141414141414141414141414141414141414141414141414141414141BC
S123002041414141414141414141414141414141414141414141414141414141414141419C
S123004041414141414141414141414141414141414141414141414141414141414141417C
S123006041414141414141414141414141414141414141414141414141414141414141415C
S123008041414141414141414141414141414141414141414141414141414141414141413C
S12300A041414141414141414141414141414141414141414141414141414141414141411C
S12300C04141414141414141414141414141414141414141414141414141414141414141FC
S12300E04141414141414141414141414141414141414141414141414141414141414141DC
S1050100A4CE87
S5030009F3
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16be 0x100 -no-augment -o test.out 2>/dev/null
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
