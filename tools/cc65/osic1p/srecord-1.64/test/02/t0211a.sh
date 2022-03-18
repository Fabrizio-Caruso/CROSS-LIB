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

TEST_SUBJECT="read mips flash"
. test_prelude

cat > test.in << 'fubar'
!R
>00000xxx @00000000 !E
@00000000
>00000000
48656C6C 6F2C2057 6F726C64 210A0000
>#DL_DONE
>FINISHED
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
00000000: 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A 00 00  #Hello, World!...
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -mips-flash-be -o test.out -hexdump
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# ----------  Must Also Cope With Erase  -----------------------------------

cat > test.in << 'fubar'
!R
>1fc00xxx @1fc00000 !C
>1fc00xxx @1fc00000 !E
@1fc00000
>1fc00000
48656C6C 6F2C2057 6F726C64 210A0000
>LOCKFLSH
@1fc00000 !S
@1fc20000 !S
@1fc40000 !S
@1fc60000 !S
@1fc80000 !S
@1fca0000 !S
@1fcc0000 !S
@1fce0000 !S
>#DL_DONE
>FINISHED
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
1FC00000: 48 65 6C 6C 6F 2C 20 57 6F 72 6C 64 21 0A 00 00  #Hello, World!...
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -mips-flash-be -o test.out -hexdump
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
