#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2003, 2006-2008, 2010 Peter Miller
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

TEST_SUBJECT="INHX16 format"
. test_prelude

# --------------------------------------------------------------------------

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
:010000040000FB
:0700000065486C6C2C6F5720726F646C0A2186
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -inhx16
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# --------------------------------------------------------------------------

cat > test.in << 'fubar'
:010000040000FB
:0700000065486C6C2C6F5720726F646C0A2186
:00000001FF
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S111000048656C6C6F2C20576F726C64210A7B
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -inhx16 -o test.out -data-only
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
