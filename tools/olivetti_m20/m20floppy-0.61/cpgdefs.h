/* $Id: cpgdefs.h,v 1.7 2006/04/05 18:55:00 chris Exp $
 *
 * CPG's compiler independence and miscellaneous defines
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
 */

#ifndef __CPGDEFS_H__
#define __CPGDEFS_H__

#ifdef SALFORD
#  define ALIGN_ 8
#endif

#ifndef ALIGN_
#  define ALIGN_
#endif

#ifdef __GNUC__
#  if ((__GNUC__==2) && (__GNUC_MINOR__>=7)) || (__GNUC__>2)
#  define ALIGN_2 __attribute__ ((packed))
#  define _NEWGNUC_
#  endif
#  define CPG_INLINE __inline__
#endif

#ifdef __WATCOMC__
#  define WPACK _Packed
#endif

#ifndef ALIGN_2
#  define ALIGN_2
#endif

#ifndef WPACK
#  define WPACK
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 900)
#  define _NEWMSC_
#endif

#ifndef CPG_INLINE
#  define CPG_INLINE
#endif

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#ifdef CPG_LITTLE_ENDIAN

#define READBE16(a) ((*(cpg_u8_t *)(a))<<8)|(*(((cpg_u8_t *)(a))+1))
#define READBE32(a) ((*(cpg_u8_t *)(a))<<24)|(*(((cpg_u8_t *)(a))+1)<<16)|(*(((cpg_u8_t *)(a))+2)<<8)|(*(((cpg_u8_t *)(a))+3))
#define WRITEBE16(a,v) do { \
                     cpg_u8_t *h = (cpg_u8_t *)(a); \
                     *h = (v)>>8; \
                     *(h+1) = v; } while (0)

#define WRITEBE32(a,v) do { \
                     cpg_u8_t *h = (cpg_u8_t *)(a); \
                     *h = (v)>>24; \
                     *(h+1) = (v)>>16; \
                     *(h+2) = (v)>>8; \
                     *(h+3) = v; } while (0)

#endif

#ifdef CPG_BIG_ENDIAN

#define READBE16(a) (*(a))
#define READBE32(a) (*(a))
#define WRITEBE16(a,v) (*(a)=(v))
#define WRITEBE32(a,v) (*(a)=(v))

#endif

#endif /* #ifndef __CPGDEFS_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
