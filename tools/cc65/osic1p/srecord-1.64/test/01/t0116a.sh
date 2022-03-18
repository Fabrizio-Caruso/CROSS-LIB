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

TEST_SUBJECT="ti-txt"
. test_prelude

cat > test.in << 'fubar'
@DEAD
99 F2 B9 CB 79 D3 EE F8 8F C6 A9 40 AB CD 71 A6
F0 52 5E 0C 13 9D 0A 2C 26 1F 62 25 53 F0 3D ED
E2 F6 B8 5B CA A6 53 59 6C FD 99 17 CA 0B BE BB
5D 1C C7 70 B9 D2 9C DF F1 FF 05 45 EF 42 32 D1
39 EA 2D 03 91 80 5C FD 7D F5 15 48 00 D3 03 5E
EF CA CE A8 9C 6B 88 8E 6A 8D D3 59 CF 05 2A 08
EF 57 0B 80 D8 67 7E 55 5D 93 9D 5D 66 A0 BB 55
6B 8A FD 07 F5 85 95 5F 12 68 B8 E2 6D E2 EA 5D
3A F6 DD 12 5D 5B 67 BA EE 05 18 54 A5 D3 A9 10
5D A7 18 52 2C AD B1 3F 16 69 21 83 4C 0B E0 86
01 BE 98 5F 19 FF 19 08 04 31 5C AA 05 06 BA 62
AD D2 B5 D9 80 66 18 96 D0 39 19 1C 45 FA A2 46
B8 3A A5 D1 39 BF D9 3E F0 36 E8 F5 3C A2 58 E9
75 0D C2 F5 73 DB 8B 43 14 A4 5F 59 9E 01 A0 56
3B 45 28 75 04 01 B3 F5 37 9B EA 73 3D 42 5C B2
4F 1F A7 C3 FA 32 06 0E D7 66 68 75 67 08 CC A3
4D F4 18 52 F5 CB 47 2D 66 31 A0 A3 74 FD 56 C3
1C FD 86 16 30 8D 24 07 F3 8C 7C 5A 94 48 FD E2
q
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123DEAD99F2B9CB79D3EEF88FC6A940ABCD71A6F0525E0C139D0A2C261F622553F03DED78
S123DECDE2F6B85BCAA653596CFD9917CA0BBEBB5D1CC770B9D29CDFF1FF0545EF4232D19F
S123DEED39EA2D0391805CFD7DF5154800D3035EEFCACEA89C6B888E6A8DD359CF052A08DC
S123DF0DEF570B80D8677E555D939D5D66A0BB556B8AFD07F585955F1268B8E26DE2EA5DFC
S123DF2D3AF6DD125D5B67BAEE051854A5D3A9105DA718522CADB13F166921834C0BE08631
S123DF4D01BE985F19FF190804315CAA0506BA62ADD2B5D980661896D039191C45FAA24659
S123DF6DB83AA5D139BFD93EF036E8F53CA258E9750DC2F573DB8B4314A45F599E01A0569D
S123DF8D3B4528750401B3F5379BEA733D425CB24F1FA7C3FA32060ED76668756708CCA3DA
S123DFAD4DF41852F5CB472D6631A0A374FD56C31CFD8616308D2407F38C7C5A9448FDE260
S5030009F3
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -ti-txt -o test.out -header HDR > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
