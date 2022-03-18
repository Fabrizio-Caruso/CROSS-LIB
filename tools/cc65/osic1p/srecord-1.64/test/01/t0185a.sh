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

TEST_SUBJECT="SHA384"
. test_prelude

cat > test.in << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > test.ok << 'fubar'
S00600004844521B
S111000048656C6C6F2C20576F726C64210A7B
S1230100D195483C9B554356BA50A855A605AAEE134612DCFDD05988FC605181D93603F2F7
S113012015A0D07812A0B333FC2CCC75025736F549
S5030003F9
S9030000FC
fubar
if test $? -ne 0; then no_result; fi

cat > ok2 << 'fubar'
srec_cat: libgcrypt not available
fubar
if test $? -ne 0; then no_result; fi

srec_cat test.in -sha384 0x100 -o test.out > LOG 2>&1
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
