//
// srecord - Manipulate EPROM load files
// Copyright (C) 2013 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#include <srecord/arglex/tool.h>


int
main(int argc, char **argv)
{
    srecord::arglex_tool cmdline(argc, argv);
    cmdline.token_first();

    //
    // Consume the command line.  It should be empty.
    //
    while (cmdline.token_cur() != srecord::arglex::token_eoln)
    {
        cmdline.default_command_line_processing();
    }

    //
    // Check the options, to ensure they are all mutually exclusive.
    //
    cmdline.test_ambiguous();

    //
    // success
    //
    return 0;
}


// vim: set ts=8 sw=4 et :
