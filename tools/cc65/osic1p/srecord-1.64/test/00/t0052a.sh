#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2000, 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="VHDL output"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
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
  0 => eprom_entry(72),
  1 => eprom_entry(101),
  2 => eprom_entry(108),
  3 => eprom_entry(108),
  4 => eprom_entry(111),
  5 => eprom_entry(44),
  6 => eprom_entry(32),
  7 => eprom_entry(87),
  8 => eprom_entry(111),
  9 => eprom_entry(114),
  10 => eprom_entry(108),
  11 => eprom_entry(100),
  12 => eprom_entry(33),
  13 => eprom_entry(10),
  others => eprom_dont_care
 );
end package body eprom_pack;
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -vhdl
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
