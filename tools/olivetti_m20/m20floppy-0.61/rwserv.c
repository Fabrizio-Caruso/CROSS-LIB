/* $Id: rwserv.c,v 1.13 2006/10/17 22:33:52 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: image write services
 * Copyright (C) 2000,2001,2004,2006 by Christian Groessler
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
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include "cpgdefs.h"
#include "cpgtypes.h"
#include "rwprotos.h"
#include "blockop.h"
#include "pcosdir.h"
#include "fhdr.h"
#include "util.h"

extern struct m20dir *diskdir;

/* create a new, empty image */
void do_new(FILE **file,int argc,char **argv)
{
    cpg_u8_t *imgbuf;
    int i,j;

    imgbuf = malloc(286720);  /* total image size: 1120 blocks */
    if (! imgbuf) {
        fprintf(stderr,"do_new: cannot allocate memory: %s\n",strerror(errno));
        return;
    }

    memset(imgbuf,0,286720);
    memset(imgbuf+0x1000,0xff,0x100);
    memset(imgbuf+0x20200,0xff,0x21000-0x20200);
    *(imgbuf+0x2001c) = 7;
    *(imgbuf+0x2001d) = 0x32;
    *(imgbuf+0x20038) = 0xff;
    *(imgbuf+0x20039) = 0xff;
    for (j=3,i=0x202f0; i<0x20ff0; i+=0x100,j++) {
        *(imgbuf + i + 0xc) = 0;
        *(imgbuf + i + 0xd) = 0;
        *(imgbuf + i + 0xe) = 0;
        *(imgbuf + i + 0xf) = j;
    }
    if (fwrite(imgbuf,286720,1,*file) != 1) {
        fprintf(stderr,"do_new: cannot write image file: %s\n",strerror(errno));
        return;
    }
}

/* it's a hack - itzchakh */
static void do_put_one_file(FILE **file,char *srcfile,char *destfile)
{
    cpg_u8_t *databuf;
    struct dir_sector blkbuf;
    cpg_u32_t blknum;
    int found,free_blocks,max_free,max_free_num;
    int i,needed_blocks,extents = 0;
    int filephysflag,index,end = FALSE;
    struct stat statbuf;
    cpg_u8_t xbuf[256];
    struct disk_header *dskhdr = (struct disk_header *)xbuf;
    struct file_header fhdr;
    FILE *infile;
    char *pcos_destfile_p;
    char pcos_destfile[16];
    cpg_u32_t fhdr_blocknum;
    off_t filesize,curoff = 0,last_size = 0;

    if (stat(srcfile,&statbuf)) {
        fprintf(stderr,"cannot stat %s: %s\n",srcfile,strerror(errno));
        return;
    }

    /* make pcos_destfile: strip directories if present */
    memset(pcos_destfile,0,16);
    while (*(destfile + strlen(destfile) - 1) == '/' && *destfile)
        *(destfile + strlen(destfile) - 1) = 0;  /* remove trailing "/" */
    pcos_destfile_p = strrchr(destfile,'/');
    if (pcos_destfile_p) {
        if (*(pcos_destfile_p+1)) {
            strncpy(pcos_destfile,pcos_destfile_p+1,14);
        }
        else {
            fprintf(stderr,"empty destination file '%s'\n",destfile);
            return;
        }
    }
    else {
        strncpy(pcos_destfile,destfile,14);
    }

    /* do a first sanity check on the file size, so we don't need to read the
     * m20 disk to find out the file won't fit
     */
    if (statbuf.st_size > 1088 * 256) {
        fprintf(stderr,"file too big\n");
        return;
    }
    filesize = statbuf.st_size;
    needed_blocks = (statbuf.st_size + 255) >> 8;  /* doesn't account for file description block */

    /* open input file */
    infile = fopen(srcfile,"rb");
    if (! infile) {
        fprintf(stderr,"cannot open input file %s: %s\n",srcfile,strerror(errno));
        return;
    }

    /* get contents of diskette */
    if (get_dir(*file,&diskdir)) {
        fprintf(stderr,"do_put: get_dir error!\n");
        return;
    }

    /* get block map block */
    if (read_phys_block(*file,0x200,(void *)dskhdr)) {
        fprintf(stderr,"do_put: cannot read main disk block\n");
        return;
    }
    /* calc # of free blocks */
    free_blocks = get_free_blocks(dskhdr,NULL,NULL);

    /* allocate buffer for copying file data */
    databuf = malloc(286720);  /* max size: 1120 blocks */
    if (! databuf) {
        fprintf(stderr,"do_put: cannot allocate memory: %s\n",strerror(errno));
        return;
    }

    /* search whether a file with same name is already on disk */
    for (found=-1,i=0; i<diskdir->entries; i++) {
        if (!strncmp(diskdir->files[i].name,pcos_destfile,14)) {
            found = i;
            break;
        }
    }

    /* if yes, check that the # of blocks used by the file and
     * the # of free blocks together are greater than or
     * equal the blocks needed for the new file.
     */
    if (found != -1) {
        if (free_blocks + (diskdir->files[found].alloc_size >> 8) <= needed_blocks) {
            fprintf(stderr,"do_put: not enough free space on disk\n");
            return;
        }
        /* yes, it will fit. delete the old file first... */
        rm_file(*file,&diskdir->files[found],dskhdr,KEEP_DIR_ENTRY);  /* delete file */
    }
    /* if no, the check is only with the free blocks */
    else {
        if (free_blocks <= needed_blocks) {
            fprintf(stderr,"do_put: not enough free space on disk\n");
            return;
        }
    }
    get_free_blocks(dskhdr,&max_free,&max_free_num);
#ifdef DEBUG
    printf("XXXX max_free_num: %d, fhdr = %d\n",max_free_num,max_free);
    printf("XXXX needed_blocks: %d\n",needed_blocks);
#endif
    fhdr_blocknum = max_free_num++;  /* block number of file description block */
    max_free--;
    memset(&fhdr,0,sizeof(fhdr));
    /* occupy file description block in global block list */
    set_blk_bits(dskhdr,fhdr_blocknum,1);
    while (needed_blocks && max_free) {

        if (curoff + max_free * 256 > filesize) {
            end = TRUE;
            max_free = (filesize - curoff + 255) >> 8;
            last_size = filesize - curoff;
        }

        /* enter the extent into the file description block */
        WRITEBE32(&fhdr.extentlist[extents].startsec,max_free_num);
        WRITEBE16(&fhdr.extentlist[extents].numsecs,max_free);
        extents++;
        if (extents > MAX_FILE_EXTENTS) {
            fprintf(stderr,"do_put: too many extents!\n");
            return;
        }

        /* occupy blocks in global block list */
        set_blk_bits(dskhdr,max_free_num,max_free);

#ifdef DEBUG
        printf("XXX reading %d bytes... (max_free_num=%d)\n",max_free << 8,max_free_num);
#endif
        /* write an extent */
        if (end) {
            memset(databuf,0,(last_size + 255) & ~255);
            if (fread(databuf,last_size,1,infile) != 1) {
                fprintf(stderr,"do_put: cannot read end from source file: %s\n",strerror(errno));
                return;
            }
        }
        else {
            if (fread(databuf,max_free << 8,1,infile) != 1) {
                fprintf(stderr,"do_put: cannot read from source file: %s\n",strerror(errno));
                return;
            }
        }
        for (i=0; i<max_free; i++) {
#ifdef DEBUG
            dumpbuf(databuf + (i << 8),max_free_num + i);
#endif
            if (write_file_block(*file,max_free_num + i,(void *)(databuf + (i << 8)))) {
                fprintf(stderr,"do_put: cannot write block %d\n",max_free_num + i);
                return;
            }
        }
        needed_blocks -= max_free;
        curoff += max_free << 8;
        if (end) break;
        get_free_blocks(dskhdr,&max_free,&max_free_num);
#ifdef DEBUG
        printf("XXXX max_free_num: %d\n",max_free_num);
#endif
    }
    if (needed_blocks) {
        fprintf(stderr,"internal error in do_put!!\n");
        return;
    }
    WRITEBE32(&fhdr.filelen,statbuf.st_size);
    WRITEBE16(&fhdr.extents,extents);
    /* write file description block */
    if (write_file_block(*file,fhdr_blocknum,(void *)&fhdr)) {
        fprintf(stderr,"do_put: cannot write file description block\n");
        return;
    }
    /* update directory entry */
    if (found != -1) {
        /* file already existed, only change start block # */
        if (find_dir_name(*file,
                          &diskdir->files[found],
                          &filephysflag,
                          &index,
                          &blknum,
                          &blkbuf) == -1) {
            fprintf(stderr,"do_put: internal error #2\n");
            return;
        }
      finishname:
        WRITEBE16(&blkbuf.files[index].startsec,fhdr_blocknum);
        /* write back the directory sector */
        if (filephysflag) {
            if (write_file_block(*file,blknum,(void *)&blkbuf)) {
                fprintf(stderr,"do_put: (file) cannot update dir block\n");
                return;
            }
        }
        else {
            if (write_phys_block(*file,blknum,(void *)&blkbuf)) {
                fprintf(stderr,"do_put: (phys) cannot update dir block\n");
                return;
            }
        }
    }
    else {
        /* new file */
        if (find_empty_dirent(*file,&filephysflag,&index,&blknum,&blkbuf) == -1) {
            fprintf(stderr,"do_put: directory full\n");
            return;
        }
        memset(blkbuf.files[index].name,0,sizeof(blkbuf.files[index].name));
        strncpy(blkbuf.files[index].name,pcos_destfile,14);
        goto finishname;
    }
    /* write back the modified block */
    if (write_phys_block(*file,0x200,(void *)dskhdr)) {
        fprintf(stderr,"do_put: cannot write main disk block\n");
        return;
    }
    /* close the input file */
    if (fclose(infile)) {
        fprintf(stderr,"cannot close input file %s: %s  (ignored)\n",srcfile,strerror(errno));
    }
}

void do_put(FILE **file,int argc,char **argv)
{
    char *srcfile,*destfile;

    if (argc != 2 && argc != 3) {
        fprintf(stderr,"need srcname [destname] argument\n");
        return;
    }
    srcfile = *(argv+1);
    if (argc == 2) destfile = srcfile;
    else destfile = *(argv+2);

    do_put_one_file(file,srcfile,destfile);
}

void do_mput(FILE **file,int argc,char **argv)
{
    int f;
    char *filename;

    if (argc < 2) {
        fprintf(stderr,"no files to put\n");
        return;
    }

    argc--;  /* skip command argument */
    f = 0;   /* files start at argv[1] (start with f=1: see f++ in while() loop) */

    while (f++,argc--) {
        filename = *(argv+f);

#ifdef DEBUG
        printf("mputting %s\n",filename);
#endif

        do_put_one_file(file,filename,filename);
    }
}

void do_rm(FILE **file,int argc,char **argv)
{
    int i,found = -1;
    char *srcfile;
    char scbuf[256];
    struct disk_header *dskhdr = (struct disk_header *)scbuf;

    if (argc-- < 2) {
        fprintf(stderr,"need name argument\n");
        return;
    }
    argv++;

    if (read_phys_block(*file,0x200,(void *)dskhdr)) {
        fprintf(stderr,"do_rm: cannot read main disk block\n");
        return;
    }

    /* get contents of diskette */
    if (get_dir(*file,&diskdir)) {
        fprintf(stderr,"do_rm: get_dir error!\n");
        return;
    }

    while (argc) {
        srcfile = *argv;

        /* search requested file */
        for (i=0; i<diskdir->entries; i++) {
            if (!strcmp(diskdir->files[i].name,srcfile)) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            fprintf(stderr,"%s not found on diskette\n",srcfile);
            /*return;*/
        }
        else rm_file(*file,&diskdir->files[i],dskhdr,RM_DIR_ENTRY);  /* delete file */
        argc--;
        argv++;
    }

    /* write back the modified block */
    if (write_phys_block(*file,0x200,(void *)dskhdr)) {
        fprintf(stderr,"do_rm: cannot write main disk block\n");
        return;
    }
}

/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
