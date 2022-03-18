dnl
dnl  srecord - manipulate eprom load files
dnl  Copyright (C) 1998, 1999, 2002, 2006-2012 Peter Miller
dnl  Copyright (C) 2014 Scott Finneran
dnl
dnl  This program is free software; you can redistribute it and/or modify
dnl  it under the terms of the GNU General Public License as published by
dnl  the Free Software Foundation; either version 3 of the License, or
dnl  (at your option) any later version.
dnl
dnl  This program is distributed in the hope that it will be useful,
dnl  but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl  General Public License for more details.
dnl
dnl  You should have received a copy of the GNU General Public License
dnl  along with this program. If not, see <http://www.gnu.org/licenses/>.
dnl
AC_INIT(install-sh)
AC_CONFIG_HEADER(srecord/config.h)
AC_PROG_CXX
AC_PROG_CPP
AC_LANG(C++)
AC_GNU_SOURCE
AC_PROG_INSTALL
AC_PROG_YACC
AC_PROG_RANLIB
AC_CHECK_PROGS(GROFF, groff roff)
AC_CHECK_PROGS(SOELIM, gsoelim soelim roffpp)
AC_CHECK_FUNCS(snprintf vsnprintf)

dnl! This is needed for MingGW build, but not for Unix or Linux, etc.
AC_CHECK_LIB(gpg-error, main)

dnl! @synopsis AC_ADD_CFLAGS
dnl!
dnl! Add the given option to CFLAGS, if it doesn't break the compiler
AC_DEFUN([AC_ADD_CFLAGS],
[AC_MSG_CHECKING([if $CC accepts $1])
  ac_add_cflags__old_cflags="$CFLAGS"
  CFLAGS="$CFLAGS $1"
  AC_TRY_LINK([#include <stdio.h>],
    [printf("Hello, World!\n"); return 0;],
    AC_MSG_RESULT([yes]),
    AC_MSG_RESULT([no])
    CFLAGS="$ac_add_cflags__old_cflags")
])

AC_ADD_CFLAGS(-Wall)
AC_ADD_CFLAGS(-Wextra)
AC_ADD_CFLAGS([-Wl,--as-needed])

dnl
dnl Make sure Boost is installed.
dnl
AC_CHECK_HEADERS(boost/shared_ptr.hpp)
if test "$ac_cv_header_boost_shared_ptr_hpp" != "yes"
then
    AC_MSG_RESULT([
        You need to install the Boost C++ library to be able to compile SRecord.
        http://boost.org/])
    OK=no
    if apt-get --version > /dev/null 2> /dev/null; then
      AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install libboost-dev
      ])
      OK=yes
    fi
    if yum --version > /dev/null 2> /dev/null; then
      AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install libboost-devel
      ])
      OK=yes
    fi
    if test "$OK" != "yes"; then
      AC_MSG_RESULT([
        If you have a package based system, the package you need to
        install will be called libboost-devel or something similar.
      ])
    fi
    exit 1
fi

dnl
dnl     Test to find a Bourne shell which understands functions
dnl
AC_MSG_CHECKING([for a Bourne shell which understands functions])
if test "z$SH" = "z"; then
    if test -f /bin/sh5; then
        SH=/bin/sh5
    else
        SH=/bin/sh
    fi
fi
AC_SUBST(SH)
AC_DEFINE_UNQUOTED(CONF_SHELL, ["$SH"],
[Set this to be the absolute path of a Bourne shell
which understands functions.])
AC_MSG_RESULT($SH)

dnl
dnl     Test to see if this operating system has the ZFS sparse file support.
dnl
AC_MSG_CHECKING([for sparse file lseek support])
AC_TRY_COMPILE([#include <unistd.h>], [fpathconf(0, _PC_MIN_HOLE_SIZE);
lseek(0, 0L, SEEK_DATA); lseek(0, 0L, SEEK_HOLE); ],
AC_DEFINE(HAVE_SPARSE_LSEEK, 1, [Define this symbol if your
operating system has support for sparse file seeking.])
AC_MSG_RESULT(yes),AC_MSG_RESULT(no))dnl

dnl!
dnl! Test to see if libgcrypt is sufficiently modern.
dnl!
AC_MSG_CHECKING([for gcrypt SHA224 support])
AC_TRY_COMPILE([#include <gcrypt.h>],
[gcry_md_hd_t handle; return 0;],
AC_DEFINE(HAVE_GCRY_MD_HD_T, 1, [Define this symbol if your
operating system has a libgcrypt that defines the gcry_md_hd_t symbol.])
AC_MSG_RESULT(yes),AC_MSG_RESULT(no))dnl

AC_DEFUN(AC_GCRYPT_SETUP,
[
    dnl!
    dnl! Test to see if libgcrypt is sufficiently modern.
    dnl!
    AC_MSG_CHECKING([for gcrypt SHA224 support])
    AC_TRY_COMPILE([#include <gcrypt.h>],
        [return GCRY_MD_SHA224;],
        AC_DEFINE(HAVE_LIBGCRYPT_SHA224, 1, [Define this symbol if your
        operating system has a libgcrypt that supports SHA224.])
    AC_MSG_RESULT(yes),AC_MSG_RESULT(no))dnl

    dnl!
    dnl! Test to see if libgcrypt is sufficiently modern.
    dnl!
    AC_MSG_CHECKING([for gcrypt WHIRLPOOL support])
    AC_TRY_COMPILE([#include <gcrypt.h>],
    [return GCRY_MD_WHIRLPOOL;],
    AC_DEFINE(HAVE_LIBGCRYPT_WHIRLPOOL, 1, [Define this symbol if your
    operating system has a libgcrypt that supports WHIRLPOOL.])
    AC_MSG_RESULT(yes),AC_MSG_RESULT(no))dnl

    AC_CHECK_LIB(gcrypt, gcry_md_open,,
        [AC_MSG_FAILURE([gcrypt test failed (--without-gcrypt to disable)])])
])

dnl!
dnl! Test to see if libgcrypt is enabled(default) or disabled.
dnl!
AC_ARG_WITH([gcrypt],
            [AS_HELP_STRING([--without-gcrypt],
                            [disable support for libgcrypt])],
            [],
            [with_gcrypt=yes])

LIBGCRYPT=
AS_IF([test "x$with_gcrypt" != xno], AC_GCRYPT_SETUP())

dnl!
dnl!  Now look for libtool.
dnl!
AC_CHECK_PROGS(LIBTOOL, libtool)
if test -z "$LIBTOOL"
then
  AC_MSG_RESULT([
        You must have GNU Libtool installed to build srecord.
        Homepage: http://www.gnu.org/software/libtool/])
  OK=no
  if apt-get --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo apt-get install libtool
    ])
    OK=yes
  fi
  if yum --version > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo yum install libtool
    ])
    OK=yes
  fi
  if pkg_info -a > /dev/null 2> /dev/null; then
    AC_MSG_RESULT([
        The following command may be used to install it:
        sudo pkg_add -r libtool
    ])
    OK=yes
  fi
  if test "$OK" != "yes"; then
    AC_MSG_RESULT([
        If you are using a package based install, you will need the
        libtool package.
    ])
  fi
  exit 1
fi

dnl
dnl     If the soelim program exists, and understands the -I option,
dnl     arrange for the Makefile to install the .ps files from the
dnl     documentation source files.
dnl
if test -n "$SOELIM"
then
  if $SOELIM -I. /dev/null > /dev/null 2>&1
  then
    : nothing
  else
    AC_MSG_RESULT([])
    AC_MSG_RESULT([     The $SOELIM program does not understand the -I])
    AC_MSG_RESULT([     option.  There is a patch available for GNU Groff])
    AC_MSG_RESULT([     which makes this work correctly.  See the BUILDING])
    AC_MSG_RESULT([     file for details.])
    GROFF=
  fi
fi
if test -n "$GROFF"
then
  HAVE_GROFF=yes
 else
  HAVE_GROFF=no
  AC_MSG_RESULT([])
  AC_MSG_RESULT([       The SRecord documentation set and manual pages will])
  AC_MSG_RESULT([       be formatted and installed if you first install])
  AC_MSG_RESULT([       GNU Groff and then re-run this ./configure script.])
  AC_MSG_RESULT([       Remember to use \`\`rm config.cache'' before you do.])
  AC_MSG_RESULT([])
fi
AC_SUBST(HAVE_GROFF)

#
# Evaluate some of the variables, to remove ${prefix} references.
# This way, they can be used in C programs and Roff input.
# Make sure that srecord is mentioned in the datadir path;
# add it if it is not already there.
#
test "x$prefix" = xNONE && prefix="${ac_default_prefix-/usr/local}"
test "x$exec_prefix" = xNONE && exec_prefix="$prefix"
eval "exec_prefix=$exec_prefix"
eval "bindir=$bindir"
eval "datarootdir=$datarootdir"
eval "mandir=$mandir"
eval "libdir=$libdir"
eval "datadir=$datadir"


AH_TOP([/*
 * srecord - manipulate eprom load files
 * Copyright (C) 1998-2002, 2006-2014 Peter Miller
 * Copyright (C) 2014 Scott Finneran
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIB_CONFIG_H
#define LIB_CONFIG_H
])

AH_BOTTOM([
/*
 * There is more to do, but we need to insulate it from config.status,
 * because it screws up the #undef lines.  They are all implications of
 * the above information, so there is no need for you to edit the file,
 * if you are configuring Aegis manually.
 */
#include <srecord/config.messy.h>

#endif /* LIB_CONFIG_H */])

AC_OUTPUT(Makefile)
dnl! vim: set ts=8 sw=4 et :
