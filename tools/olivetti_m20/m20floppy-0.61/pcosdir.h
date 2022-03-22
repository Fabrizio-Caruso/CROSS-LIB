/* $Id: pcosdir.h,v 1.9 2006/04/05 18:55:03 chris Exp $
 *
 * PCOS diskette directory structure
 * Copyright (C) 2000,2001,2006 by Christian Groessler
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

#ifndef __PCOSDIR_H__
#define __PCOSDIR_H__

/* internal directory structure used by m20floppy */

struct m20dir_entry {
    cpg_u8_t  name[16];
    cpg_u32_t size;
    cpg_u32_t alloc_size;
    cpg_u16_t extents;
    cpg_u16_t flags;
    cpg_u16_t startsec;
};

struct m20dir {
    cpg_u32_t           entries;    /* # of entries in the following array */
    struct m20dir_entry files[1];
};

#define m20dir_hdr_size (sizeof(struct m20dir) - sizeof(struct m20dir_entry))

#define PCOS_DIR_ENTRIES_BLOCK 14  /* # of dir entries in one diskette block */
#define PCOS_NUM_DIR_BLOCKS    14  /* # of blocks containing the directory */
#define PCOS_MAX_DIR_ENTRIES   (PCOS_DIR_ENTRIES_BLOCK * PCOS_NUM_DIR_BLOCKS)
                    /* size of memory needed to hold full directory */
#define M20DIR_SIZE (PCOS_MAX_DIR_ENTRIES * \
                     sizeof(struct m20dir_entry) + m20dir_hdr_size)


/* directory structures used by PCOS on disk */
/* all entries are big-endian! */

#include "packstrt.h"

WPACK struct ALIGN_ direntry {
    cpg_u8_t  name[16];  /* in fact 14 only, don't know what the other 2 bytes are... */
                         /* in my tests they are always 0 */
    cpg_u16_t startsec;
} ALIGN_2;

WPACK struct ALIGN_ dir_sector {
    struct direntry files[PCOS_DIR_ENTRIES_BLOCK];
    cpg_u32_t       link;
}  ALIGN_2;

/* block free bitmap + etc (sector 0x200) */

#define NUM_FILE_BLOCKS 0x8a    /* available blocks for files (-16, directory blocks not considered) */

WPACK struct ALIGN_ disk_header {
    cpg_u8_t  volname[15];           /* volume name (max. 14 chars w/terminating zero) */
    cpg_u8_t  unknown1[13];          /* purpose unknown at this time */
    cpg_u16_t sernum_or_unknown;     /* purpose unknown at this time */
    cpg_u8_t  unknown2[0x1a];        /* purpose unknown at this time */
    cpg_u8_t  bfl[NUM_FILE_BLOCKS];  /* block free list */
}  ALIGN_2;

#include "packend.h"
#endif /* #ifndef __PCOSDIR_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
