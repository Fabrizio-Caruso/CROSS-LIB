#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2008 Peter Miller
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

TEST_SUBJECT="crc32 -xmodem"
. test_prelude

cat > test.in << 'fubar'
S00A0000746573742E696E30
S1230000D46EBDDE679162F728C97423D8B8ACD103E3E275D6383B02A05DBFA2F9F475B021
S1230020421C58D8C68B45162F288CBFF07FEB22E184550747074F93FB1C32DE00169D3F5A
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00A0000746573742E696E30
S1230000D46EBDDE679162F728C97423D8B8ACD103E3E275D6383B02A05DBFA2F9F475B021
S1230020421C58D8C68B45162F288CBFF07FEB22E184550747074F93FB1C32DE00169D3F5A
S10700407B63EE3DAF
S5030003F9
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -becrc32 0x40 -xmodem -o test.out > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
