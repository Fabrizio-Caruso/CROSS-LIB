/* $Id: cpgtypes.h,v 1.4 2006/04/05 18:55:01 chris Exp $
 *
 * Generic type defines
 * Copyright (C) 2000,2006 by Christian Groessler
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write
 * to the Free Software Foundation, 51 Franklin Street - Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * The types are:
 * --------------
 * cpg_u8_t  - 8bit  value, unsigned
 * cpg_s8_t  - 8bit  value, signed
 * cpg_u16_t - 16bit value, unsigned
 * cpg_s16_t - 16bit value, signed
 * cpg_u32_t - 32bit value, unsigned
 * cpg_s32_t - 32bit value, signed
 * cpg_u64_t - 64bit value, unsigned   (not all compilers support this)
 * cpg_s64_t - 64bit value, signed     (not all compilers support this)
 * cpg_ptr_t - a pointer is guaranteed to fit into this
 */

#ifndef _CPGTYPES_H_
#define _CPGTYPES_H_

/* some common default word sizes */
#if (defined(WINNT) || defined(WIN95) || defined(WIN32) || defined(SOLARIS) || \
    defined(SCO) || defined(DJGPP)) && !defined(__32BIT__) && !defined(WIN64)
#define __32BIT__
#endif
#if defined(WIN64) && !defined(__64BIT__)
#define __64BIT__
#endif
#if defined(LINUX)
#  if defined(__alpha__) && !defined(__64BIT__)
#    define __64BIT__
#    define __LP64__
#  endif
#  if defined(__ia64__) && !defined(__64BIT__)
#    define __64BIT__
#  endif
#  if defined(__i386__) && !defined(__32BIT__)
#    define __32BIT__
#  endif
#endif

#if !defined(__16BIT__) && !defined(__32BIT__) && !defined(__64BIT__)
#error "Word size unknown! (need __16BIT__ or __32BIT__ or __64BIT__)"
#endif

typedef unsigned char  cpg_u8_t;
typedef   signed char  cpg_s8_t;
typedef unsigned short cpg_u16_t;
typedef   signed short cpg_s16_t;
#ifndef WIN64
typedef unsigned long  cpg_ptr_t;    /* a pointer will always fit into a long */
#else
typedef unsigned __int64 cpg_ptr_t;
#endif

#ifdef __16BIT__
typedef unsigned long  cpg_u32_t;
typedef   signed long  cpg_s32_t;
typedef unsigned long  cpg_u64_t[2];
typedef   signed long  cpg_s64_t[2];
#else
#if defined(WIN32) || defined(WINNT) || defined(WIN95) /* DWORD kack */
typedef unsigned long  cpg_u32_t;
typedef   signed long  cpg_s32_t;
#else
typedef unsigned int   cpg_u32_t;
typedef   signed int   cpg_s32_t;
#endif

#ifdef __32BIT__
#ifdef __GNUC__
typedef unsigned long long cpg_u64_t;
typedef   signed long long cpg_s64_t;
#else
#if (defined(_MSC_VER) && (_MSC_VER >= 900)) || (defined(__WATCOMC__) && (__WATCOMC__+1 >= 1100+1))
typedef unsigned __int64 cpg_u64_t;
typedef   signed __int64 cpg_s64_t;
#else
typedef unsigned long  cpg_u64_t[2];
typedef   signed long  cpg_s64_t[2];
#endif /* not defined _MSC_VER and not defined __GNUC__ and not defined __WATCOMC__ */
#endif /* not defined __GNUC__ */
#endif /* #ifdef __32BIT__ */

#ifdef __64BIT__
#ifndef WIN64
typedef unsigned long  cpg_u64_t;
typedef   signed long  cpg_s64_t;
#else
typedef unsigned __int64 cpg_u64_t;
typedef   signed __int64 cpg_s64_t;
#endif /* defined WIN64 */
#endif /* defined __64BIT__ */
#endif /* not defined __16BIT__ */

#endif /* #ifndef _CPGTYPES_H_ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
