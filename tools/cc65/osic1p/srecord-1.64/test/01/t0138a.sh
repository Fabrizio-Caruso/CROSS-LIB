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

TEST_SUBJECT="vhdl alignment"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S10800050001020304E8
S5030001FB
S9030005F7
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
-- HDR
--
-- Generated automatically by srec_cat -VHDL - do not edit
--
library IEEE;
use IEEE.numeric_std.all;
use work.eprom_defs_pack.all;

package eprom_pack is
 constant eprom_rom : eprom_rom_array;
end package eprom_pack;

package body eprom_pack is
 constant eprom_rom : eprom_rom_array := eprom_rom_array'(
  1 => eprom_entry(258),
  2 => eprom_entry(50593792),
  others => eprom_dont_care
 );
end package body eprom_pack;
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0 -within test.in -range-padding 4 \
        -o test.out -vhdl 4
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

cat > test.ok << 'fubar'
-- HDR
--
-- Generated automatically by srec_cat -VHDL - do not edit
--
library IEEE;
use IEEE.numeric_std.all;
use work.eprom_defs_pack.all;

package eprom_pack is
 constant eprom_rom : eprom_rom_array;
end package eprom_pack;

package body eprom_pack is
 constant eprom_rom : eprom_rom_array := eprom_rom_array'(
  2 => eprom_entry(65280),
  3 => eprom_entry(258),
  4 => eprom_entry(772),
  others => eprom_dont_care
 );
end package body eprom_pack;
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fill 0xFF -within test.in -range-padding 2 \
        -o test.out -vhdl 2
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
