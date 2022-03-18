#!/bin/sh
#
#       srecord - manipulate eprom load files
#       Copyright (C) 1998, 2003, 2006, 2007 Peter Miller
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
2)
        ;;
*)
        echo "Usage: $0 tmp-dir tarball" 1>&2
        exit 1
        ;;
esac

tmp=$1
tarball=$2

mkdir -p $tmp/BUILD $tmp/BUILD_ROOT $tmp/RPMS/i386 \
        $tmp/SOURCES $tmp/SPECS $tmp/SRPMS

mac=`rpm --showrc | awk '/^macrofile/{print $3}'`
cat > $tmp/rpmrc << fubar
macrofiles: ${mac}:$tmp/macros
fubar

here=`pwd`/$tmp
cat > $tmp/macros << fubar
%_builddir      $here/BUILD
%_buildroot     $here/BUILD_ROOT
%_rpmdir        $here/RPMS
%_sourcedir     $here/SOURCES
%_specdir       $here/SPECS
%_srcrpmdir     $here/SRPMS
fubar

rpmbuild -ta --target i386-pc-linux --rcfile $tmp/rpmrc -vv $2
test $? -eq 0 || exit 1

exit 0
