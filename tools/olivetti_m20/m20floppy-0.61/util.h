/* $Id: util.h,v 1.7 2006/04/05 18:55:05 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: utility routines declarations
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

#ifndef __UTIL_H__
#define __UTIL_H__

/* flags for rm_file */
#define RM_DIR_ENTRY   0
#define KEEP_DIR_ENTRY 1

extern int get_dir(FILE *,struct m20dir **);
extern int get_file(FILE *,struct m20dir_entry *,FILE *);
extern int get_free_blocks(struct disk_header *,int *,int *);
extern int rm_file(FILE *,struct m20dir_entry *,struct disk_header *,int);
extern int find_dir_name(FILE *,struct m20dir_entry *,int *,int *,cpg_u32_t *,struct dir_sector *);
extern int find_empty_dirent(FILE *,int *,int *,cpg_u32_t *,struct dir_sector *);
extern void CPG_INLINE set_blk_bits(struct disk_header *,cpg_u32_t,cpg_u16_t);
#ifdef DEBUG
extern void dumpbuf(cpg_u8_t *,int);
#endif

#endif /* #ifndef __UTIL_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
