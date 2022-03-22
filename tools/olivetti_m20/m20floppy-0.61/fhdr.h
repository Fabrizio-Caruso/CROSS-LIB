/* $Id: fhdr.h,v 1.5 2006/04/05 20:40:39 chris Exp $
 *
 * PCOS diskette file header structure
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

#ifndef __FHDR_H__
#define __FHDR_H__

/* all entries are big-endian! */

#include "packstrt.h"

WPACK struct ALIGN_ extent {
    cpg_u32_t startsec;
    cpg_u16_t numsecs;
} ALIGN_2;

#define MAX_FILE_EXTENTS 38

WPACK struct ALIGN_ file_header { /* sizeof: 256 bytes */
    cpg_u32_t     filelen;
    cpg_u16_t     extents;
    cpg_u8_t      unknown[16];
    cpg_u16_t     unknown2;
    struct extent extentlist[MAX_FILE_EXTENTS];
    cpg_u32_t     link;        /* 0xffffffff for last */
} ALIGN_2;

#include "packend.h"
#endif /* #ifndef __FHDR_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
