/* $Id: roserv.c,v 1.13 2006/07/11 22:22:31 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: image readonly services
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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "cpgdefs.h"
#include "cpgtypes.h"
#include "roprotos.h"
#include "blockop.h"
#include "pcosdir.h"
#include "util.h"

static const char spaces[]="                "; /* 16 space characters */
struct m20dir *diskdir;

/* show directory */
void do_ls(FILE **file,int argc,char **argv,int lsl)
{
    int i;
    char nambuf[18];
    char scbuf[256];
    struct disk_header *dskhdr = (struct disk_header *)scbuf;

    if (read_phys_block(*file,0x200,(void *)dskhdr)) {
        fprintf(stderr,"do_ls: cannot read main disk block\n");
        return;
    }
    if (get_dir(*file,&diskdir)) {
        fprintf(stderr,"do_ls: get_dir error!\n");
        return;
    }
    if (lsl) printf("\n");
    if (lsl && dskhdr->volname[0]) {  /* disk has a volume name */
        printf("Volume name: %s\n",dskhdr->volname);
    }
    if (lsl)
        printf("# of files: %d\t\tfree blocks: %d\n",
               diskdir->entries,
               get_free_blocks(dskhdr,NULL,NULL));
    if (lsl && diskdir->entries) {
        printf("\nNAME            \tSIZE\tBLOCKS\tALLBL\tEXTENTS\n");
        printf("--------------  \t----\t------\t-----\t-------\n");
    }
    for (i=0; i<diskdir->entries; i++) {
        if (lsl) {
            strcpy(nambuf,diskdir->files[i].name);
            strncpy(nambuf + strlen(nambuf),spaces,16-strlen(nambuf));
            nambuf[16] = 0;
            printf("%s\t%lu\t%lu\t%lu\t%lu\n",
                   nambuf,(unsigned long)diskdir->files[i].size,
                   (unsigned long)((diskdir->files[i].size + 0xff) >> 8),
                   (unsigned long)((diskdir->files[i].alloc_size) >> 8),
                   (unsigned long)diskdir->files[i].extents);
        }
        else {
            puts(diskdir->files[i].name);
        }
    }
    if (lsl) printf("\n");
}

/* cat a file */
void do_cat(FILE **file,int argc,char **argv)
{
    int i,found = -1;
    char *srcfile;

    if (argc != 2) {
        fprintf(stderr,"need srcname argument\n");
        return;
    }
    srcfile = *(argv+1);

    /* get contents of diskette */
    if (get_dir(*file,&diskdir)) {
        fprintf(stderr,"do_cat: get_dir error!\n");
        return;
    }

    /* search requested file */
    for (i=0; i<diskdir->entries; i++) {
        if (!strcmp(diskdir->files[i].name,srcfile)) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        fprintf(stderr,"%s not found on diskette\n",srcfile);
        return;
    }
    get_file(*file,&diskdir->files[i],stdout);  /* copy file out */
}

/* copy a file to host */
void do_get(FILE **file,int argc,char **argv)
{
    int i,found = -1;
    char *srcfile,*destfile;
    FILE *dest;

    if (argc != 2 && argc != 3) {
        fprintf(stderr,"need srcname [destname] argument\n");
        return;
    }
    srcfile = *(argv+1);
    if (argc == 2) destfile = srcfile;
    else destfile = *(argv+2);

#ifdef DEBUG
    printf("srcfile: %s, destfile: %s\n",srcfile,destfile);
#endif

    /* get contents of diskette */
    if (get_dir(*file,&diskdir)) {
        fprintf(stderr,"do_get: get_dir error!\n");
        return;
    }

    /* search requested file */
    for (i=0; i<diskdir->entries; i++) {
        if (!strcmp(diskdir->files[i].name,srcfile)) {
            found = i;
            break;
        }
    }
    if (found == -1) {
        fprintf(stderr,"%s not found on diskette\n",srcfile);
        return;
    }

    /* open/create destination file */
    dest = fopen(destfile,"wb");
    if (! dest) {
        fprintf(stderr,"cannot open destination file %s: %s\n",destfile,strerror(errno));
        return;
    }

    /* copy file out */
    get_file(*file,&diskdir->files[i],dest);

    /* close the destination file */
    if (fclose(dest)) {
        fprintf(stderr,"cannot close destination file %s: %s\n",destfile,strerror(errno));
    }
}

/* copy many files to host */
void do_mget(FILE **file,int argc,char **argv)
{
    int i,f,found = -1;
    char *filename;
    FILE *dest;

    if (argc < 2) {
        fprintf(stderr,"no files to get\n");
        return;
    }

    argc--;  /* skip command argument */
    f = 0;   /* files start at argv[1] (start with f=1: see f++ in while() loop) */

    /* get contents of diskette */
    if (get_dir(*file,&diskdir)) {
        fprintf(stderr,"do_get: get_dir error!\n");
        return;
    }

    while (f++,argc--) {
        filename = *(argv+f);

#ifdef DEBUG
        printf("mgetting %s\n",filename);
#endif

        /* search requested file */
        for (i=0,found=-1; i<diskdir->entries; i++) {
            if (!strcmp(diskdir->files[i].name,filename)) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            fprintf(stderr,"%s not found on diskette -- ignoring\n",filename);
            continue;
        }

        /* open/create destination file */
        dest = fopen(filename,"wb");
        if (! dest) {
            fprintf(stderr,"cannot open destination file %s: %s\n",filename,strerror(errno));
            return;
        }

        /* copy file out */
        get_file(*file,&diskdir->files[i],dest);

        /* close the destination file */
        if (fclose(dest)) {
            fprintf(stderr,"cannot close destination file %s: %s  (ignored)\n",filename,strerror(errno));
        }
    }

    return;
}

/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
