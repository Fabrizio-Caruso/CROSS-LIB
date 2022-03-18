//
// srecord - Manipulate EPROM load files
// Copyright (C) 2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see
// <http://www.gnu.org/licenses/>.
//

#include <srecord/arglex.h>
#include <srecord/sizeof.h>
#include <srecord/output/file.h>


srecord::output_file::line_termination_t
  srecord::output_file::line_termination =
    srecord::output_file::line_termination_native;


srecord::output_file::line_termination_t
srecord::output_file::line_termination_guess()
{
#if __dos__ || __windows__ || __WINDOWS__ || __CYGWIN__ | _WIN32
    // Perform this test first, so that Cygwin thinks it's on Windows,
    // rather than Unix.  This is because most Cygwin users are actually
    // using the mingw executables, and are not actually using the
    // Cygwin shell.
    return line_termination_crlf;
#elif __unix__ || UNIX
    return line_termination_nl;
#elif __macosx__
    return line_termination_cr;
#elif __primos__ || PRIMOS
    return line_termination_primos;
#else
    return line_termination_binary;
#endif
}


bool
srecord::output_file::line_termination_by_name(const std::string &name)
{
    struct table_t
    {
        const char *name;
        line_termination_t end;
    };
    static const table_t table[] =
    {
        { "Apple", line_termination_cr },
        { "Binary", line_termination_binary },
        { "Carriage_Return_Line_Feed", line_termination_crlf },
        { "Carriage_Return", line_termination_cr },
        { "DOS", line_termination_crlf },
        { "Line_Feed", line_termination_nl },
        { "LINUx", line_termination_nl },
        { "Macintosh", line_termination_cr },
        { "Macintosh_Operating_System_X", line_termination_cr },
        { "NewLine", line_termination_nl },
        { "Pr1mos", line_termination_primos },
        { "Primos", line_termination_primos },
        { "Prime", line_termination_primos },
        { "Unix", line_termination_nl },
        { "Windows", line_termination_crlf },
    };

    for (const table_t *tp = table; tp < ENDOF(table); ++tp)
    {
        if (arglex::compare(tp->name, name.c_str()))
        {
            line_termination = tp->end;
            return true;
        }
    }
    return false;
}
