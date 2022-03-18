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

#include <srecord/interval.h>
#include <srecord/arglex/tool.h>
#include <srecord/input.h>
#include <srecord/input/interval.h>


unsigned long
srecord::arglex_tool::get_number(const char *caption)
{
    unsigned long value = 0;
    unsigned long multiple;
    interval over;

    switch (token_cur())
    {
    case token_paren_begin:
        token_next();
        value = get_number(caption);
        if (token_cur() != token_paren_end)
        {
            fatal_error
            (
                "closing parenthesis expected before %s",
                token_name(token_cur())
            );
            // NOTREACHED
        }
        token_next();
        return value;

    case token_number:
        value = value_number();
        token_next();
        return value;

    case token_minimum_address:
        {
            token_next();
            input::pointer ifp = get_input();
            over = input_interval(ifp);
            value = over.get_lowest();
        }
        break;

    case token_maximum_address:
        {
            token_next();
            input::pointer ifp = get_input();
            over = input_interval(ifp);
            value = over.get_highest();
        }
        break;

    case token_length:
        {
            token_next();
            input::pointer ifp = get_input();
            over = input_interval(ifp);
            value = (over.get_highest() - over.get_lowest());
        }
        break;

    case token_stdio:
        token_next();
        value = -get_number(caption);
        break;

    default:
        fatal_error
        (
            "number expected for %s before %s",
            caption,
            token_name(token_cur())
        );
        // NOTREACHED
    }
    switch (token_cur())
    {
    case token_round_down:
        token_next();
        multiple = get_number("-round-down");
        value /= multiple;
        value *= multiple;
        break;

    case token_round_up:
        token_next();
        multiple = get_number("-round-up");
        value = (value + multiple - 1) / multiple;
        value *= multiple;
        break;

    case token_round_nearest:
        token_next();
        multiple = get_number("-round-nearest");
        value = (value + multiple / 2) / multiple;
        value *= multiple;
        break;
    }
    return value;
}


unsigned long
srecord::arglex_tool::get_number(const char *caption, long minimum,
    long maximum)
{
    long value = get_number(caption);
    if (value < minimum || value > maximum)
    {
        fatal_error
        (
            "%s value %ld out of range (%ld, %ld)",
            caption,
            value,
            minimum,
            maximum
        );
        // NOTREACHED
    }
    return value;
}
