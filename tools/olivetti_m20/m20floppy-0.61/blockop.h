/* $Id: blockop.h,v 1.3 2006/04/05 18:55:00 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: disk block public functions
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

#ifndef __BLOCKOP_H__
#define __BLOCKOP_H__

extern int read_file_block(FILE *,int,cpg_u8_t *);
extern int read_phys_block(FILE *,int,cpg_u8_t *);
extern int write_file_block(FILE *,int,cpg_u8_t *);
extern int write_phys_block(FILE *,int,cpg_u8_t *);
extern int read_file_blocks(FILE *,int,int,cpg_u8_t *);
extern int read_phys_blocks(FILE *,int,int,cpg_u8_t *);
extern int write_file_blocks(FILE *,int,int,cpg_u8_t *);
extern int write_phys_blocks(FILE *,int,int,cpg_u8_t *);

#endif /* #ifndef __BLOCKOP_H__ */
/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
