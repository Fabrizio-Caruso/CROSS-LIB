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

TEST_SUBJECT="-asm -cl430"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S111011148656C6C6F2C20576F726C64210A69
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
; HDR
        .global eprom
        .text
eprom
        .byte   0x48,0x65,0x6C,0x6C,0x6F,0x2C,0x20,0x57,0x6F,0x72,0x6C,0x64
        .byte   0x21,0x0A,0x48,0x65,0x6C,0x6C,0x6F,0x2C,0x20,0x57,0x6F,0x72
        .byte   0x6C,0x64,0x21,0x0A

        .global eprom_address
eprom_address
        .long   0x00000000, 0x00000111

        .global eprom_length_of_sections
eprom_length_of_sections
        .long   0x0000000E, 0x0000000E

        .global eprom_sections
eprom_sections
        .long   0x0002
; upper bound = 0x011F
; lower bound = 0x0000
; length =      0x011F
        .end
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -asm -cl430 > LOG 2>&1
if test $? -ne 0; then cat LOG; fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
