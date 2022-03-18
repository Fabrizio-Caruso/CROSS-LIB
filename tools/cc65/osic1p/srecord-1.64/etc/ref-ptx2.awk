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
BEGIN {
        print ".hy 0"
        print ".de TH"
        print ".."
        print ".TS H"
        print "l r lw(2.5i) lw(2.5i)."
        print ".TH"
}
{
        print $10 "\t\\*[Page*Of*" $10 "]\tT{"
        print ".ad r"
        print $4
        print "T}\tT{"
        print ".ad l"
        print $6
        print "T}"
}
END {
        print ".TE"
        print ".ad b"
}
