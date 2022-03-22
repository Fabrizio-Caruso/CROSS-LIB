/* $Id: imgopen.c,v 1.6 2006/10/19 23:38:50 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: image open routines
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
 * These routines open the image file.
 * On DOS, if the filename is "[A-Z]:", it's the disk drive and no
 * open is performed. The functions will encode the drive in the
 * returned FILE *. 0 - A:, 1 - B:, etc.
 */

#include <stdio.h>
#include "imgopen.h"

#ifdef DOS
#include <ctype.h>  /* toupper() */

/*
 * checks whether file name parameter refers to a DOS drive
 * if yes, update file * to reflect this drive
 * returns 0/1 for drive/no drive
 */
static int is_dos_drive(char *name,FILE **file)
{
    char c;
    int drvnum;
    if (strlen(name) == 2 && *(name + 1) == ':') {
        c = toupper(*name);
        drvnum = c - 'A';
        if (drvnum > 'Z' - 'A') return(1);  /* invalid drive letter */
        *file = (FILE *)(drvnum);
        return(0);
    }
    else return(1);
}
#endif

int open_img_rw(char *name,FILE **file)
{
#ifdef DOS
    int ret;
    if (!(ret = is_dos_drive(name, file))) return(ret);
#endif
    *file = fopen(name,"rb+");
    if (!*file) return(1);
    return(0);
}

int open_img_ro(char *name,FILE **file)
{
#ifdef DOS
    int ret;
    if (!(ret = is_dos_drive(name, file))) return(ret);
#endif
    *file = fopen(name,"rb");
    if (!*file) return(1);
    return(0);
}

int open_img_new(char *name,FILE **file)
{
#ifdef DOS
    int ret;
    if (!(ret = is_dos_drive(name, file))) return(ret);
#endif
    *file = fopen(name,"wb");
    if (!*file) return(1);
    return(0);
}


/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
