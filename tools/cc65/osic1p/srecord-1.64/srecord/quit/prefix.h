//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2006-2008, 2010 Peter Miller
//
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
// General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SRECORD_QUIT_PREFIX_H
#define SRECORD_QUIT_PREFIX_H

#include <string>
#include <srecord/quit.h>

namespace srecord
{

/**
  * The quit_prefix class is used to filter the messages issued to include
  * a line prefix.  The message is then passed ot a deeper quit handler.
  */
class quit_prefix:
    public quit
{
public:
    /**
      * The destructor.
      */
    virtual ~quit_prefix();

    /**
      * A constructor.  The given handler is used to process the
      * messages, once prefixed.  The given string is used as the
      * line prefix.
      */
    quit_prefix(quit &deeper, const char *prefix);

    /**
      * A constructor.  The given handler is used to process the
      * messages, once prefixed.  The given string is used as the
      * line prefix.
      */
    quit_prefix(quit &deeper, const std::string &prefix);

    // See base class for documentation.
    virtual void exit(int);

    // See base class for documentation.
    virtual void message_v(const char *, va_list);

private:
    /**
      * The prefix instance variable is used to remember the line
      * prefix to use.
      */
    std::string prefix;

    /**
      * The deepr instance variable is used to remember which handler
      * is to be used to process the error messages once the refix
      * has been added.
      */
    quit &deeper;

    /**
      * The default constructor.  Do not use.
      */
    quit_prefix();

    /**
      * The copy constructor.  Do not use.
      */
    quit_prefix(const quit_prefix &);

    /**
      * The assignment operator.  Do not use.
      */
    quit_prefix &operator=(const quit_prefix &);
};

};

#endif // SRECORD_QUIT_PREFIX_H
