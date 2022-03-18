#!/bin/sh
#
# srecord - Manipulate EPROM load files
# Copyright (C) 2011 Peter Miller
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

echo "<html>"
echo "<body>"
echo '<h1 align="center">'
echo '<img src="../srecord-64.png" align="left" />'
echo '<img src="../srecord-64.png" align="right" />'
echo "SRecord Manual Pages </h1>"
oldsection=1
echo '<br clear="all" />'
echo "<h2>Commands</h2>"
echo "<ul>"
for f in $*
do
    base=$(basename $f .html)
    section=$(echo $f | sed 's|.*/man\([0-9]\)/.*|\1|' )
    if [ "$section" != "$oldsection" ]
    then
        echo "</ul>"
        case $section in
        1)
            echo "<h2>Commands</h2>"
            ;;
        3)
            echo "<h2>Library</h2>"
            ;;
        5)
            echo "<h2>File Formats</h2>"
            ;;
        *)
            echo "<h2>Section $section</h2>"
            ;;
        esac
        echo "<ul>"
        oldsection=$section
    fi
    echo "<li><a href=\"man$section/$base.html\" ><i>$base</i>($section)</a>"
done
echo "</ul>"
echo "</body>"
echo "</html>"
exit 0

# vim: set ts=8 sw=4 et :
