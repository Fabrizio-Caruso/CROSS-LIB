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

TEST_SUBJECT="-generate -repeat-string"
. test_prelude

cat > test.ok << 'fubar'
S00600004844521B
S224012345436F707972696768742028432920313831322054636861696B6F76736B792E201B
S22401236520436F707972696768742028432920313831322054636861696B6F76736B792EFB
S2190123852020436F707972696768742028432920313831322084
S5030003F9
fubar
if test $? -ne 0; then no_result; fi

srec_cat -generate 0x12345 0x1239A \
        -repeat-string 'Copyright (C) 1812 Tchaikovsky.  ' \
        -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
