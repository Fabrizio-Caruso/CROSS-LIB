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

#include <srecord/interval.h>
#include <srecord/pretty_size.h>
#include <srecord/arglex/tool.h>


srecord::interval
srecord::arglex_tool::get_interval_small(const char *caption)
{
    interval result = get_interval(caption);
    bool big_range_ok = false;
    if (token_cur() == token_big)
    {
        big_range_ok = true;
        token_next();
    }
    if (!big_range_ok)
    {
        long long c = result.coverage();
        if (c > (1LL << 30))
        {
            fatal_error
            (
                "the %s range %s is %sB and is probably larger than you "
                    "intended (use --big to override this error message)",
                caption,
                result.representation().c_str(),
                pretty_size(c).c_str()
            );
        }
    }
    return result;
}
