#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2009, 2011 Peter Miller
# Copyright (C) 2014 Scott Finneran
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

TEST_SUBJECT="MD5 filter"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S109000068656C6C6F0AD8
S5030001FB
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

# This is reported by md5sum:
# b1946ac92492d2347c6235b4d2611184
cat > test.ok << 'fubar'
S00600004844521B
S109000068656C6C6F0AD8
S1130100B1946AC92492D2347C6235B4D261118428
S5030002FA
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > ok2 << 'fubar'
srec_cat: libgcrypt not available
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -md5 0x100 -o test.out > LOG 2>&1
if test $? -ne 0
then
    # if libgcrypt not available, pass by default
    if diff ok2 LOG > /dev/null 2> /dev/null
    then
        echo
        echo "    SRecord appears to have been compiled without the gcrypt"
        echo "    library, this test is therefore declared to pass by default."
        echo
        pass
    fi

    # some other error
    cat LOG
    fail
fi

diff test.ok test.out
if test $? -ne 0; then fail; fi

#
# The things tested here, worked.
# No other guarantees are made.
#
pass

# vim: set ts=8 sw=4 et :
