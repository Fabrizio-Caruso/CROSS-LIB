#!/bin/sh
#
#       srecord - Manipulate EPROM load files
#       Copyright (C) 2008, 2011 Peter Miller
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
groff "$@" 2>groff.log
es=$?
cat groff.log 1>&2
test $es = 0 || exit $es

if grep "can't open" groff.log 2>/dev/null
then
    exit 1
fi

exit 0

# vim: set ts=8 sw=4 et :
