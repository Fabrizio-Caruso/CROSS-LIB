/* $Id: util.c,v 1.13 2006/04/05 21:21:05 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: utility routines
 * Copyright (C) 2000,2004,2006 by Christian Groessler
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
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "cpgtypes.h"
#include "cpgdefs.h"
#include "pcosdir.h"
#include "fhdr.h"
#include "util.h"
#include "blockop.h"

/* find an empty directory entry */
/* returns 0 for success, -1 for error */
int find_empty_dirent(FILE *floppy,
                      int *filephysflag,
                      int *index,
                      cpg_u32_t *blknum,
                      struct dir_sector *blockbuf)
{
    /* *** modified block copy of find_dir_name() code *** */
    int i;

    *filephysflag = 0;

    /* read all directory blocks until a free filename slot occurs */
    for (i = 0;  i < PCOS_NUM_DIR_BLOCKS; i++) {  /* all directory blocks */
        /* read block */
        if (read_phys_block(floppy,*blknum = 0x202 + i,(void *)blockbuf)) {
            fprintf(stderr,"find_empty_name: cannot read dir block\n");
            return(-1);
        }
        for (*index = 0; *index < PCOS_DIR_ENTRIES_BLOCK; (*index)++) {  /* loop over all entries in this block */
            if (blockbuf->files[*index].name[0] == 0xff) {

                /* ta-da! found an entry! */
                return(0);

            }
        }
    }
    /* if we enter this loop,
     * main directory chunk has been extended to file blocks area,
     * because directory is too big
     */
    *filephysflag = 1;
    while (blockbuf->link != 0xffffffff) {
        if (read_file_block(floppy,*blknum = READBE32(&blockbuf->link),(void *)blockbuf)) {
            fprintf(stderr,"find_emtpy_name: cannot read dir block (2)\n");
            return(-1);
        }
/* --- same as above, simple block copy --- */
        for (*index = 0; *index < PCOS_DIR_ENTRIES_BLOCK; (*index)++) {  /* loop over all entries in this block */
            if (blockbuf->files[*index].name[0] == 0xff) {

                /* ta-da! found an entry! */
                return(0);

            }
        }
/* --- */
    }
#ifdef DEBUG
    fprintf(stderr,"find_empty_name(): leave - no empty entry found\n");
#endif
    return(-1);
}

/* find a directory entry */
/* returns 0 for success, -1 for error */
int find_dir_name(FILE *floppy,
                  struct m20dir_entry *ent,
                  int *filephysflag,
                  int *index,
                  cpg_u32_t *blknum,
                  struct dir_sector *blockbuf)
{
    int i;

    *filephysflag = 0;

    /* read all directory blocks until a filename starting with 0xff 0xff
     * or our filename is encountered
     */
    for (i = 0;  i < PCOS_NUM_DIR_BLOCKS; i++) {  /* all directory blocks */
        /* read block */
        if (read_phys_block(floppy,*blknum = 0x202 + i,(void *)blockbuf)) {
            fprintf(stderr,"find_dir_name: cannot read dir block\n");
            return(-1);
        }
        for (*index = 0; *index < PCOS_DIR_ENTRIES_BLOCK; (*index)++) {  /* loop over all entries in this block */
            if (blockbuf->files[*index].name[0] == 0xff) {
                /* if 2nd char also is 0xff, end of used directory structure has been reached */
                if (blockbuf->files[*index].name[1] == 0xff) {
                    goto notfound;  /* end of used directory sectors */
                }
                /* deleted entry */
                continue;
            }
            if (!strcmp(ent->name,blockbuf->files[*index].name)) {
                goto found;
            }
        }
    }
    /* if we enter this loop,
     * main directory chunk has been extended to file blocks area,
     * because directory is too big
     */
    *filephysflag = 1;
    while (blockbuf->link != 0xffffffff) {
        if (read_file_block(floppy,*blknum = READBE32(&blockbuf->link),(void *)blockbuf)) {
            fprintf(stderr,"find_dir_name: cannot read dir block (2)\n");
            return(-1);
        }
/* --- same as above, simple block copy --- */
        for (*index = 0; *index < PCOS_DIR_ENTRIES_BLOCK; (*index)++) {  /* loop over all entries in this block */
            if (blockbuf->files[*index].name[0] == 0xff) {
                /* if 2nd char also is 0xff, end of used directory structure has been reached */
                if (blockbuf->files[*index].name[1] == 0xff) {
                    goto notfound;  /* end of used directory sectors */
                }
                /* deleted entry */
                continue;
            }
            if (!strcmp(ent->name,blockbuf->files[*index].name)) {
                goto found;
            }
        }
/* --- */
    }
  notfound:
#ifdef DEBUG
    fprintf(stderr,"find_dir_name(): leave - file not found\n");
#endif
    return(-1);

    /* we found what */
  found:
#ifdef DEBUG
    fprintf(stderr,"find_dir_name(): found\n");
#endif
    return(0);
}

/* disable a directory entry */
/* returns 0 for success, -1 for error */
static int rm_dir_name(FILE *floppy,struct m20dir_entry *ent)
{
    int j;
    struct dir_sector blockbuf;
    int filephysflag = 0;
    cpg_u32_t blknum;

#ifdef DEBUG
    fprintf(stderr,"*** rm_dir_name for %s\n",ent->name);
#endif

    if (find_dir_name(floppy,ent,&filephysflag,&j,&blknum,&blockbuf) == -1)
        return(-1);

    /* save first char at end of name */
    blockbuf.files[j].name[13] = blockbuf.files[j].name[0];
    blockbuf.files[j].name[0] = 0xff;  /* disable entry */
    ent->name[0] = 0xff;  /* !!! better recreate directory table */
    /* write back the modified sector */
    if (filephysflag) {
        if (write_file_block(floppy,blknum,(void *)&blockbuf)) {
            fprintf(stderr,"rm_dir_name: (file) cannot update dir block\n");
            return(-1);
        }
    }
    else {
        if (write_phys_block(floppy,blknum,(void *)&blockbuf)) {
            fprintf(stderr,"rm_dir_name: (phys) cannot update dir block\n");
            return(-1);
        }
    }
    return(0);
}

static int get_file_data(FILE *floppy,struct m20dir_entry *ent)
{
    int i;
    cpg_u32_t alloc_size,ext_blocks;
    struct file_header blockbuf;

    if (read_file_block(floppy,ent->startsec,(void *)&blockbuf)) {
        fprintf(stderr,"get_file_data: cannot read file description block\n");
        return(-1);
    }
    /* get values */
    ent->size = READBE32(&blockbuf.filelen);
    ent->extents = READBE16(&blockbuf.extents);
    ent->flags = 0;
    for (i=0, ext_blocks=0; i<ent->extents && i<38; i++)
        ext_blocks += READBE16(&blockbuf.extentlist[i].numsecs);
    ext_blocks++;  /* account for file description block */
    alloc_size = ext_blocks << 8;
    ent->alloc_size = alloc_size;
    return(0);
}

/* returns 0 for success, -1 for error */
int get_dir(FILE *floppy,struct m20dir **m20dir)
{
    cpg_u32_t ce;
    int i,j;
    int dirsize = M20DIR_SIZE;
    struct dir_sector blockbuf;

#ifdef DEBUG
    fprintf(stderr,"get_dir(): entered\n");
#endif

    *m20dir = malloc(dirsize);   /* allocate space for directory table */
    if (! *m20dir) {
        fprintf(stderr,"get_dir: malloc failed!\n");
        return(-1);
    }
    memset(*m20dir,0,M20DIR_SIZE);

    /* read all directory blocks until a filename starting with 0xff 0xff is encountered */
    for (i = 0;  i < PCOS_NUM_DIR_BLOCKS; i++) {  /* all directory blocks */
        /* read block */
        if (read_phys_block(floppy,0x202 + i,(void *)&blockbuf)) {
            fprintf(stderr,"get_dir: cannot read dir block\n");
            return(-1);
        }
        for (j = 0; j < PCOS_DIR_ENTRIES_BLOCK; j++) {  /* all files in 1 block */
            if (blockbuf.files[j].name[0] == 0xff) {
                /* if 2nd char also is 0xff, end of used directory structure has been reached */
                if (blockbuf.files[j].name[1] == 0xff) {
                    goto done;  /* end of used directory sectors */
                }
                /* deleted entry */
                continue;
            }
            ce = (*m20dir)->entries;
            strcpy((*m20dir)->files[ce].name,blockbuf.files[j].name);
            (*m20dir)->files[ce].startsec = READBE16(&blockbuf.files[j].startsec);
            if (get_file_data(floppy,&((*m20dir)->files[ce]))) {
                fprintf(stderr,"get_dir: get_file_data failed\n");
                return(-1);
            }
            (*m20dir)->entries++;
        }
    }
    /* if we enter this loop,
     * main directory chunk has been extended to file blocks area,
     * because directory is too big
     */
    while (blockbuf.link != 0xffffffff) {
        if (read_file_block(floppy,READBE32(&blockbuf.link),(void *)&blockbuf)) {
            fprintf(stderr,"get_dir: cannot read dir block (2)\n");
            return(-1);
        }
        *m20dir = realloc(*m20dir,dirsize + PCOS_DIR_ENTRIES_BLOCK * sizeof(struct m20dir_entry));
        if (! *m20dir) {
            fprintf(stderr,"cannot realloc memory: %s\n",strerror(errno));
            return(-1);
        }
        dirsize += PCOS_DIR_ENTRIES_BLOCK * sizeof(struct m20dir_entry);
/* --- same as above, simple block copy --- */
        for (j = 0; j < PCOS_DIR_ENTRIES_BLOCK; j++) {  /* all files in 1 block */
            if (blockbuf.files[j].name[0] == 0xff) {
                /* if 2nd char also is 0xff, end of used directory structure has been reached */
                if (blockbuf.files[j].name[1] == 0xff) {
                    goto done;  /* end of used directory sectors */
                }
                /* deleted entry */
                continue;
            }
            ce = (*m20dir)->entries;
            strcpy((*m20dir)->files[ce].name,blockbuf.files[j].name);
            (*m20dir)->files[ce].startsec = READBE16(&blockbuf.files[j].startsec);
            if (get_file_data(floppy,&((*m20dir)->files[ce]))) {
                fprintf(stderr,"get_dir: get_file_data failed\n");
                return(-1);
            }
            (*m20dir)->entries++;
        }
/* --- */
    }
  done:
#ifdef DEBUG
    fprintf(stderr,"get_dir(): leave\n");
#endif
    return(0);
}

/* returns 0 for success, -1 for error */
int get_file(FILE *floppy,struct m20dir_entry *srcfile,FILE *destfile)
{
    cpg_u32_t size,extents,startblk,writesize,curpos;
    struct file_header hdrbuf;
    cpg_u16_t extblocks;
    cpg_u8_t *transbuf;
    int i;

    transbuf = malloc(300000);
    if (! transbuf) {
        fprintf(stderr,"get_file: cannot allocate memory: %s\n",strerror(errno));
        return(-1);
    }
    if (read_file_block(floppy,srcfile->startsec,(void *)&hdrbuf)) {
        fprintf(stderr,"get_file: cannot read file description block\n");
        return(-1);
    }
    /* get values */
    size = READBE32(&hdrbuf.filelen);
    extents = READBE16(&hdrbuf.extents);

#ifdef DEBUG
    printf("start sector: %lu, file size %lu, extents: %lu\n",
           (unsigned long)srcfile->startsec,(unsigned long)size,(unsigned long)extents);
#endif

    for (i=0,curpos=0; i<extents && curpos<size; i++) {
        /* read extent */
        extblocks = READBE16(&hdrbuf.extentlist[i].numsecs);
        startblk = READBE32(&hdrbuf.extentlist[i].startsec);
#ifdef DEBUG
        printf("extent: start block: %lu, # of blocks = %lu\n",(unsigned long)startblk,(unsigned long)extblocks);
#endif
        if ((extblocks << 8) > 300000) {
            fprintf(stderr,"file too big: %lu\n",(unsigned long)(extblocks << 8));
            return(-1);
        }
        if (read_file_blocks(floppy,startblk,extblocks,transbuf)) {
            fprintf(stderr,"get_file: cannot read file contents\n");
            return(-1);
        }
        /* if it's the last extent, adjust write size to real file len */
        writesize = extblocks << 8;
        if (curpos + writesize > size) {
#ifdef DEBUG
            fprintf(stderr,"get_file: length from extents: %u, length from header: %u\n",
                    curpos + writesize,size);
#endif
            writesize = size - curpos;
        }
#ifdef DEBUG
        printf("writesize: %d\n",writesize);
#endif
        if (fwrite(transbuf,writesize,1,destfile) != 1) {
            fprintf(stderr,"get_file: cannot write: %s\n",strerror(errno));
            return(-1);
        }
        curpos += writesize;
    }
    return(0);
}

#define is_blk_free(x,y) \
    (!(*(((cpg_u8_t *)(x))+((y)>>3)) & (0x80>>((y)&7))))

#define clr_blk(x,y) \
    do { *(((cpg_u8_t *)(x))+((y)>>3)) &= ~(0x80>>((y)&7)); } while (0)

#define set_blk(x,y) \
    do { *(((cpg_u8_t *)(x))+((y)>>3)) |= (0x80>>((y)&7)); } while (0)

static void CPG_INLINE clr_blk_bits(struct disk_header *dskhdr,cpg_u32_t sect,cpg_u16_t numsec)
{
    int i;
    cpg_u8_t *bfl = dskhdr->bfl;
    for (i=sect; i<sect+numsec; i++) clr_blk(bfl,i);
}

void CPG_INLINE set_blk_bits(struct disk_header *dskhdr,cpg_u32_t sect,cpg_u16_t numsec)
{
    int i;
    cpg_u8_t *bfl = dskhdr->bfl;
    for (i=sect; i<sect+numsec; i++) set_blk(bfl,i);
}

/* return # of free blocks and opt. biggest chunk of free blocks */
int get_free_blocks(struct disk_header *dskhdr,int *max_free,int *max_free_num)
{
    int free_blocks = 0;
    int max_free_blocks = 0;
    int max_free_blk_num = 0;
    int cur_free_blocks = 0;
    int cur_free_blk_num = 0;
    int last_free = FALSE;
    int prev_free = FALSE;
    int i;

#ifdef DEBUG
    printf("Block bitmap:\n");
    for (i=0; i<NUM_FILE_BLOCKS; i++) {
        printf("0x%02x ",dskhdr->bfl[i]);
        if ((i & 15) == 15) printf("\n");
    }
    printf("\n");
#endif

    for (i=0; i<NUM_FILE_BLOCKS*8; i++) {
        if (is_blk_free(dskhdr->bfl,i)) {
            free_blocks++;
            cur_free_blocks++;
            if (! prev_free) {
                prev_free = TRUE;
                cur_free_blk_num = i;
            }
            last_free = TRUE;
        }
        else {
            if (prev_free) {
                if (cur_free_blocks > max_free_blocks) {
                    max_free_blocks = cur_free_blocks;
                    max_free_blk_num = cur_free_blk_num;
                }
            }
            cur_free_blocks = cur_free_blk_num = 0;
            prev_free = FALSE;
            last_free = FALSE;
        }
    }
    if (last_free && prev_free) {
        if (cur_free_blocks > max_free_blocks) {
            max_free_blocks = cur_free_blocks;
            max_free_blk_num = cur_free_blk_num;
        }
    }
    if (max_free) *max_free = max_free_blocks;
    if (max_free_num) *max_free_num = max_free_blk_num;
    return(free_blocks);
}

/* returns 0 for success, -1 for error */
int rm_file(FILE *floppy,struct m20dir_entry *ent,struct disk_header *dskhdr,int flag)
{
    cpg_u32_t sect;
    cpg_u16_t numsec;
    struct file_header blockbuf;
    int i;

    /* read file descrition block */
    if (read_file_block(floppy,ent->startsec,(void *)&blockbuf)) {
        fprintf(stderr,"rm file: cannot read file description block\n");
        return(-1);
    }

    /* clear blocks from block free bitmap */
    for (i=0; i<ent->extents; i++) {  /* for all extents */
        sect = READBE32(&blockbuf.extentlist[i].startsec);
        numsec = READBE16(&blockbuf.extentlist[i].numsecs);
        clr_blk_bits(dskhdr,sect,numsec);  /* clear the bits */
    }
    /* clear file description block from block free bitmap */
    clr_blk_bits(dskhdr,ent->startsec,1);

    if (flag == RM_DIR_ENTRY) {
        /* disable entry in the directory */
        if (rm_dir_name(floppy,ent)) {
            fprintf(stderr,"rm_file: rm_dir_name failed!\n");
            return(1);
        }
    }

    return(0);
}

#ifdef DEBUG

void dumpbuf(cpg_u8_t *data,int bufnum)
{
    int i;
    printf("\nDUMP OF BUFFER %d (0x%x)\n",bufnum,bufnum);
    for (i=0; i<256; i++) {
        if (!(i&15)) printf("\n%02X - ",i);
        printf("%02X ",*(data+i));
    }
    printf("\n");
}

#endif

/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
