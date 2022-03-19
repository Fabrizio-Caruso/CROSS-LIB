/*  Lecture des disquettes Thomson sur PC
 *  Version 2.1
 *  Copyright (C) 2000-2003 Eric Botcazou
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef FLOPPY_H
#define FLOPPY_H

struct floppy_info {
   int num_drives;
   int drive_type[4];
   int fm_support;
   int write_support;
};

/* drive type: 1 (5"25 - 360 kb)
 *             2 (5"25 - 1.2 Mb)
 *             3 (3"5  - 720 kb)
 *             4 (3"5 - 1.44 Mb)
 *             5 (3"5 - 2.88 Mb)
 *             6 (3"5 - 2.88 Mb)
 */

extern int FloppyInit(struct floppy_info *fi, int enable_write_support);
extern void FloppyExit(void);

extern int FloppyReadSector(int drive, int density, int track, int sector, int nsects, unsigned char data[]);
extern int FloppyWriteSector(int drive, int density, int track, int sector, int nsects, const unsigned char data[]);
extern int FloppyFormatTrack(int drive, int density, int track, const unsigned char header_table[]);

#endif

