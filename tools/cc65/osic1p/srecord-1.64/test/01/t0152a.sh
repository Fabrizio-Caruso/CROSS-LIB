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

TEST_SUBJECT="CRC16"
. test_prelude

#
# Build the CRC16 test vectors
#
cp /dev/null zero.length.file
test $? -eq 0 || no_result
echo A | dd bs=1 count=1 2>/dev/null > single.a.file
test $? -eq 0 || no_result
echo 123456789 | dd bs=9 count=1 2>/dev/null > nine.digits.file
test $? -eq 0 || no_result

echo "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" |
dd bs=64 count=1 2>/dev/null > upper-case-a.64.file
test $? -eq 0 || no_result
cat upper-case-a.64.file upper-case-a.64.file upper-case-a.64.file \
        upper-case-a.64.file > upper-case-a.256.file
test $? -eq 0 || no_result

#
# now we run the test vectors with and without augmentation
#
test_crc16 -a < zero.length.file > test.out
test $? -eq 0 || fail
echo >> test.out
test $? -eq 0 || no_result

test_crc16 -a < single.a.file >> test.out
test $? -eq 0 || fail
echo >> test.out
test $? -eq 0 || no_result

test_crc16 -a < nine.digits.file >> test.out
test $? -eq 0 || fail
echo >> test.out
test $? -eq 0 || no_result

test_crc16 -a < upper-case-a.256.file >> test.out
test $? -eq 0 || fail

cat > test.ok << 'fubar'
0xFFFF

0xE1B1

0xA69D

0xA4CE
fubar
if test $? -ne 0; then no_result; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
