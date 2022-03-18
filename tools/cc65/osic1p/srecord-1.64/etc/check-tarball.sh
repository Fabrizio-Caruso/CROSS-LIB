#!/bin/sh
#
#       srecord - The "srecord" program.
#       Copyright (C) 2007, 2011 Peter Miller
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
set -e
set -x

base=`basename $1 .tar.gz`

tar xzf $1
cd $base
./configure
make
make sure
cd ..
rm -rf $base
exit 0

# vim: set ts=8 sw=4 et :
