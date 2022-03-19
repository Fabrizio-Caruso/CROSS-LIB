/*  LibSAP
 *  Version 0.9.3
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


#ifndef LIBSAP_H
#define LIBSAP_H

#include <stdio.h>
#include <time.h>


/* constants for the SAP physical file formats */
#define SAP_NSECTS      16

#define SAP_NTRACKS1    80
#define SAP_SECTSIZE1   256
#define SAP_TRACKSIZE1  (SAP_NSECTS*SAP_SECTSIZE1)

#define SAP_NTRACKS2    40
#define SAP_SECTSIZE2   128
#define SAP_TRACKSIZE2  (SAP_NSECTS*SAP_SECTSIZE2)


/* types used by the library */
typedef unsigned int sapID;

typedef struct {
   unsigned char format;
   unsigned char protection;
   unsigned char track;
   unsigned char sector;
   unsigned char data[258];
} sapsector_t;  /* type of a SAP sector */

typedef struct {
   int size;
   int file_type;
   int data_type;
   time_t date;
   char comment[9];
   int nblocks;
   int *block;
} sapfileinfo_t;

typedef int (*sapfilecb_t)(sapID id, const char filename[], int n, unsigned char trk20_data[]);


/* return values */
#define SAP_OK          0
#define SAP_NO_STD_FMT  (1<<0)
#define SAP_PROTECTED   (1<<1)
#define SAP_BAD_SECTOR  (1<<2)
#define SAP_CRC_ERROR   (1<<3)
#define SAP_ERROR       (1<<7)


/* errno values */
extern int sap_errno;

#define SAP_EBADF     1
#define SAP_EBUSY     2
#define SAP_EEMPTY    3
#define SAP_EFBIG     4
#define SAP_EINVAL    5
#define SAP_ENFILE    6
#define SAP_ENOENT    7
#define SAP_ENOSPC    8
#define SAP_EPERM     9
#define SAP_ETOOMANY  10


/* low-level functions */
extern int _ExtractDir(char buffer[], int buffer_size, int drive, int density, const unsigned char trk20_data[]);
extern int _ForEachFile(sapID id, const char pattern[], sapfilecb_t callback, int save_back);


/* physical format API functions */
#define SAP_FORMAT1  1
#define SAP_FORMAT2  2

extern sapID sap_OpenArchive(const char filename[], int *format);
extern sapID sap_CreateArchive(const char filename[], int format);
extern int   sap_CloseArchive(sapID id);
extern int   sap_FillArchive(sapID id, sapsector_t *sector);
extern int   sap_ReadSector(sapID id, int track, int sect, sapsector_t *sector);
extern int   sap_ReadSectorEx(sapID id, int track, int sect, int nsects, unsigned char data[]);
extern int   sap_WriteSector(sapID id, int track, int sect, sapsector_t *sector);
extern int   sap_WriteSectorEx(sapID id, int track, int sect, int nsects, const unsigned char data[]);


/* logical format API functions */
#define SAP_TRK40   1
#define SAP_TRK80   2

extern int   sap_FormatArchive(sapID id, int capacity);
extern int   sap_ListArchive(sapID id, char buffer[], int buffer_size);
extern int   sap_AddFile(sapID id, const char filename[]);
extern int   sap_DeleteFile(sapID id, const char pattern[]);
extern int   sap_ExtractFile(sapID id, const char pattern[]);
extern int   sap_GetFileInfo(sapID id, const char filename[], sapfileinfo_t *info);

#endif

