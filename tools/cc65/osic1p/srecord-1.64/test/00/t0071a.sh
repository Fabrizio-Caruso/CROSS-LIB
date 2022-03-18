#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 2001, 2003, 2006-2008 Peter Miller
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

TEST_SUBJECT="Signetics format"
. test_prelude

# --------------------------------------------------------------------------
#
# Test reading the format
#
cat > test.in << 'fubar'
:B00010A5576F77212044696420796F75207265617B
:B01010E56C6C7920676F207468726F756768206136
:B02010256C6C20746861742074726F75626C652068
:B0300D5F746F207265616420746869733FD1
:B03D00
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123B000576F77212044696420796F75207265616C6C7920676F207468726F7567682061DF
S120B0206C6C20746861742074726F75626C6520746F207265616420746869733F73
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -signetics -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

# --------------------------------------------------------------------------
#
# Test writing the format
#
cat > test.ok << 'fubar'
:B00020C5576F77212044696420796F75207265616C6C7920676F207468726F75676820614D
:B0201D3F6C6C20746861742074726F75626C6520746F207265616420746869733FDC
:B03D00
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -signetics -o test.out -signetics
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

srec_cmp test.in -sig test.out -sig
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
