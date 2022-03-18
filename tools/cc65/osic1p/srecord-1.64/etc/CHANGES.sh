#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 2006, 2007 Peter Miller
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
case $# in
1)
        project=$1
        ;;

*)
        echo "usage: $0 <project>" 1>&2
        exit 1
        ;;
esac
set -e
aegis -l changes -p $project -v -pl 66 -pw 72 < /dev/null
changes=`aegis -list changes -terse -project $project`
for n in $changes
do
        echo '' | tr '\12' '\14'
        aegis -l change_details -p $project -c $n -v -pl 66 -pw 72 < /dev/null
done
exit 0
