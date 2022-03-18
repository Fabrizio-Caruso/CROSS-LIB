#!/bin/sh
#
# srecord - The "srecord" program.
# Copyright (C) 2008, 2011, 2012 Peter Miller
# Copyright (C) 2014 Scott Finneran
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

TEST_SUBJECT="CRC16"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S10C000031323334353637383916
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S10C000031323334353637383916
S1050100A69DB6
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16-b-e 0x100 -ccitt -no-augment -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S10C000031323334353637383916
S1050100E5CC48
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16-b-e 0x100 -ccitt -augment -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S10C000031323334353637383916
S1050100BEEF4C
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16-b-e 0x100 -xmodem -no-augment -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S10C000031323334353637383916
S105010031C305
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16-b-e 0x100 -xmodem -augment -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S10C000031323334353637383916
S105010029B11F
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16-b-e 0x100 -broken -augment -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S10E0000313233343536373839E5CC63
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16-b-e -max-address test.in -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
S00600004844521B
S10E0000313233343536373839CCE563
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crc16-l-e -max-address test.in -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
