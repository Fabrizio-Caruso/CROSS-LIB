#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 1999, 2006-2008 Peter Miller
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

TEST_SUBJECT="crop filter"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S12300005468697320697320612074657374206F66207468652063726F702066696C746589
S1230020722E0A5765206E6565642061206C6172676520656E6F75676820626C6F636B20FD
S1210040746F206578657273697A6520616C6C206F66207468652063617365732E0A16
S5030003F9
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

#
# [===data===]
#      [========crop========]
#
cat > test.ok << 'fubar'
S00600004844521B
S123003220656E6F75676820626C6F636B20746F206578657273697A6520616C6C206F66F9
S10F0052207468652063617365732E0AD6
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crop 50 10000 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
#       [===data===]
# [===crop===]
#
cat > test.ok << 'fubar'
S00600004844521B
S11900805468697320697320612074657374206F662074686520FB
S5030001FB
S90300807C
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -offset 128 -crop 50 150 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# [======data======]
#    [===crop===]
#
cat > test.ok << 'fubar'
S00600004844521B
S117003220656E6F75676820626C6F636B20746F2065786580
S5030001FB
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crop 50 70 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
#    [===data===]
# [======crop======]
#
cat > test.ok << 'fubar'
S00600004844521B
S12300805468697320697320612074657374206F66207468652063726F702066696C746509
S12300A0722E0A5765206E6565642061206C6172676520656E6F75676820626C6F636B207D
S12100C0746F206578657273697A6520616C6C206F66207468652063617365732E0A96
S5030003F9
S90300807C
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -offset 128 -crop 50 10000 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# [===data===]
#              [===crop===]
#
cat > test.ok << 'fubar'
S00600004844521B
S5030000FC
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -crop 10000 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
#              [===data===]
# [===crop===]
#
srec_cat test.in -offset 256 -crop 0 128 -o test.out
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
