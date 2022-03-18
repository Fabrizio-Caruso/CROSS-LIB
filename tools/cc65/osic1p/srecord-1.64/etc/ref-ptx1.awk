#
# srecord - manipulate eprom load files
# Copyright (C) 1998, 2006, 2007, 2009 Peter Miller
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
function getc()
{
    getc_tmp = substr(getc_buf, getc_pos, 1)
    if (getc_tmp != "")
        ++getc_pos
    return getc_tmp
}

BEGIN {
    th_name = "none"
    th_section = "none"
    active = 0
}
/^\.TH/ {
    # watch out for the quotes
    getc_buf = $0
    getc_pos = 4
    argc = 0
    for (;;)
    {
        c = getc()
        while (c == " " || c == "\t")
            c = getc()
        if (c == "")
            break
        quoted = 0
        arg = ""
        for (;;)
        {
            if (c == "\"")
                quoted = !quoted
            else if (c == "")
                break
            else if (!quoted && (c == " " || c == "\t"))
                break
            else
                arg = arg c
            c = getc()
        }
        argv[++argc] = arg
    }

    # th_name = argv[1]
    # gsub(/\\\*\(n\)/, "SRecord", th_name)
    # th_section = argv[2]
    th_name = FILENAME
    sub(/^.*\//, "", th_name)
    sub(/\.[0-9]$/, "", th_name)
    th_section = FILENAME
    sub(/^.*\./, "", th_section)
    active = 0
    next
}
/^\.SH/ {
    active = ($2 == "NAME")
    next
}
/^['.]\\"/ {
    # ignore comments
    next
}
/^['.]XX/ {
    # ignore indexing
    next
}
/^['.]if/ {
    # ignore conditional (around indexing)
    next
}
/^['.]\\}/ {
    # ignore conditional (around indexing)
    next
}
/^['.]/ {
    if (active)
    {
        sub(/^.[a-zA-Z][a-zA-Z]*[       ]*/, "")
        print th_name "(" th_section ") " $0
    }
    next
}
{
    if (active)
    {
        gsub(/  /, " ")              # Translate tabs to spaces
        gsub(/__*/, " ")             # Collapse underscores
        gsub(/  +/, " ")             # Collapse spaces
        gsub(/ *, */, ", ")          # Fix comma spacings
        sub(/^ /, "")                # Kill initial spaces
        sub(/ $/, "")                # Kill trailing spaces
        gsub(/\\f\(../, "")          # Kill font changes
        gsub(/\\f./, "")             # Kill font changes
        gsub(/\\s[-+0-9]*/, "")      # Kill size changes
        gsub(/\\&/, "")              # Kill \&
        gsub(/\\\((ru|ul)/, "_")     # Translate
        gsub(/\\\[(mi|hy|em)]/, "-") # Translate
        gsub(/\\\((mi|hy|em)/, "-")  # Translate
        gsub(/\\\*\(n\)/, "SRecord")
        gsub(/\\\*\(../, "")         # Kill troff strings
        gsub(/\\/, "")               # Kill all backslashes
        print th_name "(" th_section ") " $0
    }
}
