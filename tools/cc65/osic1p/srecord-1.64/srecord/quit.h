//
// srecord - manipulate eprom load files
// Copyright (C) 2000, 2002, 2003, 2006-2008, 2010, 2011 Peter Miller
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

#ifndef SRECORD_QUIT_H
#define SRECORD_QUIT_H

#include <stdarg.h>
#include <srecord/format_printf.h>

namespace srecord
{

/**
  * The quit class is an abstract class for reporting error messages,
  * both fatal and non-fatal.
  */
class quit
{
public:
    /**
      * The destructor.
      */
    virtual ~quit();

    /**
      * The default constructor.
      */
    quit();

    /**
      * The copy constructor.
      */
    quit(const quit &);

    /**
      * The assignment operator.
      */
    quit &operator=(const quit &);

    /**
      * The fatal_error method is used to report fatal errors.
      * The `fmt' string is in the same style a standard C printf
      * function.  It calls the fatal_error_v method.  This method
      * does not return.
      */
    virtual void fatal_error(const char *fmt, ...)
                                                        FORMAT_PRINTF(2, 3);
    /**
      * The fatal_error_v method is used to report fatal errors.
      * The `fmt' string is in the same style a standard C vprintf
      * function.  It calls the message_v and exit methods.
      * This method does not return.
      */
    virtual void fatal_error_v(const char *, va_list);

    /**
      * The fatal_error_errno method is used to report fatal errors,
      * and append the string equivalent of errno.      The `fmt' string
      * is in the same style a standard C printf function.      It calls
      * the fatal_error_errno_v method.  This method does not return.
      */
    virtual void fatal_error_errno(const char *fmt, ...)
                                                        FORMAT_PRINTF(2, 3);
    /**
      * The fatal_error_errno_v method is used to report fatal
      * errors.  The `fmt' string is in the same style a standard C
      * vprintf function.  It calls the message_v and exit methods.
      * This method does not return.
      */
    virtual void fatal_error_errno_v(const char *, va_list);

    /**
      * The warning method is used to likely but non-fatal errors.
      * The `fmt' string is in the same style a standard C printf
      * function.  It calls the warning_v method.
      */
    virtual void warning(const char *fmt, ...)
                                                        FORMAT_PRINTF(2, 3);
    /**
      * The warning_v method is used to report likely but non-fatal
      * errors.  The `fmt' string is in the same style a standard
      * C vprintf function.  It calls the message_v method.
      */
    virtual void warning_v(const char *, va_list);

    /**
      * The message method is used to send an error message to a
      * suitable destination.  The `fmt' string is in the same style
      * a standard C printf function.  The message_v method is called.
      */
    virtual void message(const char *fmt, ...)
                                                        FORMAT_PRINTF(2, 3);
    /**
      * The message_v method is used to send an error message to
      * a suitable destination.  The `fmt' string is in the same
      * style a standard C vprintf function.  Derived classes are
      * required to supply this method.
      */
    virtual void message_v(const char *, va_list) = 0;

    /**
      * The exit method is used to terminate execution.  It could
      * call the global exit function, or it could throw an exception, or
      * some other action, but it shall NOT return.  Derived classes
      * are required to supply this method.
      */
    virtual void exit(int) = 0;
};

/**
  * The quit_default variable is a reference to a quit implementation.
  * It should be used unless there is a good reason not to.
  */
extern quit &quit_default;

};

#endif // SRECORD_QUIT_H
