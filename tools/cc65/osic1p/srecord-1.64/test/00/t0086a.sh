#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="Spectrum format"
. test_prelude

cat > test.in << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S111000048656C6C6F2C20576F726C64210A7B
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok.srec << 'fubar'
S0220000687474703A2F2F737265636F72642E736F75726365666F7267652E6E65742F1D
S123000002303030302030313030313030300A303030312030313130303130310A3030306F
S1230020322030313130313130300A303030332030313130313130300A3030303420303126
S12300403130313131310A303030352030303130313130300A3030303620303031303030F4
S123006030300A303030372030313031303131310A303030382030313130313131310A30F4
S12300803030392030313131303031300A303031302030313130313130300A3030313120C4
S12300A030313130303130300A303031322030303130303030310A3030313320303030309C
S10900C0313031300A0367
S5030007F5
S9030000FC
fubar
if test $? -ne 0; then no_result; fi
srec_cat test.ok.srec -o test.ok -bin
if test $? -ne 0; then no_result; fi

srec_cat test.in -o test.out -spectrum
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

srec_cmp test.out -spectrum test.in
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
