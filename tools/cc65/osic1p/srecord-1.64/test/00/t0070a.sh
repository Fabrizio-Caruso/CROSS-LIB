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

TEST_SUBJECT="FourPackedCode format"
. test_prelude

#---------------------------------------------------------------------------
#
# Test reading the format
#
cat > test.in << 'fubar'
$kL&@h%%,:,B.\?00EPuX0K3rO0JI))
$;UPR'%%,:<Hn&FCG:at<GVF(;G9wIw
$7FD1p%%,:LHmy:>GTV%/KJ7@GE[kYz
$B[6\;%%,:\KIn?GFWY/qKI1G5:;-_e
$%%%%%
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S123B000576F77212044696420796F75207265616C6C7920676F207468726F7567682061DF
S120B0206C6C20746861742074726F75626C6520746F207265616420746869733F73
S5030002FA
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fpc -o test.out -header HDR
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#---------------------------------------------------------------------------
#
# Test writing the format
#
cat > test.ok << 'fubar'
$mbw6)%%,:,B.\?00EPuX0K3rO0JI))Hn&FCG:at<GVF(;G9wIw
$K%6Re%%,:LHmy:>GTV%/KJ7@GE[kYzKIn?GFWY/qKI1G5:;-_e
$%%%%%
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -fpc -o test.out -fpc
if test $? -ne 0; then fail; fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass
