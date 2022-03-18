//
// srecord - manipulate eprom load files
// Copyright (C) 2004, 2006-2008, 2010 Peter Miller
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

#include <climits>

#include <srecord/interval.h>
#include <srecord/arglex/tool.h>
#include <srecord/input.h>
#include <srecord/input/interval.h>


srecord::interval
srecord::arglex_tool::get_interval_factor(const char *name)
{
    switch (token_cur())
    {
    case token_paren_begin:
        {
            token_next();
            interval retval = get_interval(name);
            if (token_cur() != token_paren_end)
            {
                fatal_error
                (
                    "closing parentheses expected before %s",
                    token_name(token_cur())
                );
                // NOTREACHED
            }
            token_next();
            return retval;
        }

    default:
        {
            if (!can_get_number())
            {
                fatal_error
                (
                    "the %s range requires two numeric arguments",
                    name
                );
                // NOTREACHED
            }
            unsigned long n1 = get_number("address range minimum");
            unsigned long n2 = 0;
            if (can_get_number())
            {
                n2 = get_number("address range maximum");
            }
            if (n2 && n1 >= n2)
            {
                fatal_error
                (
                    "the %s range %lu..%lu is invalid",
                    name,
                    n1,
                    n2
                );
                // NOTREACHED
            }
            interval result(n1, n2);
            return result;
        }

    case token_within:
        {
            token_next();
            srecord::input::pointer ifp = get_input();
            interval result = srecord::input_interval(ifp);
            return result;
        }

    case token_over:
        {
            token_next();
            srecord::input::pointer ifp = get_input();
            interval result = srecord::input_interval(ifp);
            return result.flatten();
        }
    }
}


srecord::interval
srecord::arglex_tool::get_interval_term(const char *name)
{
    interval result = get_interval_factor(name);
    for (;;)
    {
        switch (token_cur())
        {
        case token_intersection:
            {
                token_next();
                result *= get_interval_factor(name);
            }
            break;

        default:
            return result;
        }
    }
}


srecord::interval
srecord::arglex_tool::get_interval(const char *name)
{
    interval result = get_interval_term(name);
    for (;;)
    {
        switch (token_cur())
        {
        case token_number:
        case token_within:
        case token_over:
        case token_paren_begin:
            result += get_interval_term(name);
            continue;

        case token_union:
            token_next();
            result += get_interval_term(name);
            continue;

        case token_minus:
            token_next();
            result -= get_interval_term(name);
            continue;

        case token_range_padding:
            {
                token_next();

                //
                // Collect the multiple from the command line.
                //
                long mult = get_number("--range-padding", 2, USHRT_MAX);

                //
                // Pad the range so that is contains whole multiples, aligned.
                //
                result = result.pad(mult);
            }
            break;

        default:
            return result;
        }
    }
}
