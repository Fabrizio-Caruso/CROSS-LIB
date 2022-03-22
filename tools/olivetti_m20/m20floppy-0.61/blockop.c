/* $Id: blockop.c,v 1.7 2006/10/30 20:12:04 chris Exp $
 *
 * Olivetti M20 PCOS diskette access utility: disk block operations
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
#include "blockop.h"

int read_file_blocks(FILE *disk,int blockno,int numblks,cpg_u8_t *buffer)
{
    int err;
    while(numblks--) {
        if ((err = read_file_block(disk,blockno,buffer))) {
            fprintf(stderr,"read_file_blocks: error %d\n",err);
            exit(55);
        }
        blockno++;
        buffer += 256;
    }
    return(0);
}

int read_phys_blocks(FILE *disk,int blockno,int numblks,cpg_u8_t *buffer)
{
    int err;
    while(numblks--) {
        if ((err = read_phys_block(disk,blockno,buffer))) {
            fprintf(stderr,"read_phys_blocks: error %d\n",err);
            exit(55);
        }
        blockno++;
        buffer += 256;
    }
    return(0);
}

int write_file_blocks(FILE *disk,int blockno,int numblks,cpg_u8_t *buffer)
{
    int err;
    while(numblks--) {
        if ((err = write_file_block(disk,blockno,buffer))) {
            fprintf(stderr,"write_file_blocks: error %d\n",err);
            exit(65);
        }
        blockno++;
        buffer += 256;
    }
    return(0);
}

int write_phys_blocks(FILE *disk,int blockno,int numblks,cpg_u8_t *buffer)
{
    int err;
    while(numblks--) {
        if ((err = write_phys_block(disk,blockno,buffer))) {
            fprintf(stderr,"write_phys_blocks: error %d\n",err);
            exit(65);
        }
        blockno++;
        buffer += 256;
    }
    return(0);
}

int read_file_block(FILE *disk,int blockno,cpg_u8_t *buffer)
{
    if (blockno >= 0x200) blockno += 0x10;
    return(read_phys_block(disk,blockno,buffer));
}

int write_file_block(FILE *disk,int blockno,cpg_u8_t *buffer)
{
    if (blockno >= 0x200) blockno += 0x10;
    return(write_phys_block(disk,blockno,buffer));
}

#ifdef DOS

#include <dpmi.h>
#include <go32.h>

static int firstcall = 1;
static __dpmi_raddr oldint1e;

void cleanup_int1e(void)
{
    __dpmi_set_real_mode_interrupt_vector(0x1e, &oldint1e);
    /* don't free dosdpbseg since it isn't needed */
}

static void setup_int1e(void)
{
    int dosdpbseg, scratch;
    __dpmi_raddr newint1e;
    unsigned char xferbuf;

    firstcall = 0;

    /* get old Int 1EH vector */
    __dpmi_get_real_mode_interrupt_vector(0x1e, &oldint1e);
#ifdef DEBUG
    printf("old INT 1EH vector: %04X:%04X\n", oldint1e.segment, oldint1e.offset16);
#endif

    /* allocate own disk parameter block in real memory */
    dosdpbseg = __dpmi_allocate_dos_memory(1, &scratch);

    /* copy the contents (11 bytes, we copy 16 bytes) to our parameter block */
    movedata(_go32_conventional_mem_selector(), oldint1e.segment * 16 + oldint1e.offset16,
             _go32_conventional_mem_selector(), dosdpbseg * 16,
             16);

    /* modify our parameter block for 256 byte sectors and 16 sectors per track */
    xferbuf = 1;   /* 256 byte sector */
    dosmemput(&xferbuf, 1, dosdpbseg * 16 + 3);
    xferbuf = 16;   /* sectors per track */
    dosmemput(&xferbuf, 1, dosdpbseg * 16 + 4);

    /* point Int 1EH vector to our disk parameter block */
    newint1e.segment = dosdpbseg;
    newint1e.offset16 = 0;
    __dpmi_set_real_mode_interrupt_vector(0x1e, &newint1e);
#ifdef DEBUG
    printf("new INT 1EH vector: %04X:%04X\n", newint1e.segment, newint1e.offset16);
#endif

    /* restore Int 1EH vector at program exit */
    atexit(cleanup_int1e);
}

#define blkno_to_chs(blkno,cyl,head,sec) do { \
                                             cyl = blkno / 32; \
                                             head = (blkno / 16) & 1; \
                                             sec = (blkno % 16) + 1; \
                                         } \
                                         while (0)

static int dos_bios_read(FILE *disk,int blockno,cpg_u8_t *buffer)
{
    __dpmi_regs regs;
    unsigned int drive, cyl, head, sec;

    if (firstcall) setup_int1e();

    /* setup registers and call BIOS Int 13H function 2 (read sectors) */

    drive = (unsigned int)disk;
    blkno_to_chs(blockno,cyl,head,sec);

#ifdef DEBUG
    printf("reading sector %u (cyl %u, head %u, sec %u)\n",
           blockno,
           cyl, head, sec);
#endif

    regs.x.ax = 0x0201;    /* AH: read sectors, AL: # of sectors */
    regs.h.ch = cyl;       /* CL: low eight bits of cylinder # */
    regs.h.cl = sec;       /* CH: sector # (bits 0-5), high 2 bits of cylinder # */
    regs.h.dh = head;      /* DH: head # */
    regs.h.dl = drive;     /* DL: drive # */
    regs.x.es = __tb >> 4;
    regs.x.bx = __tb & 0x0f;

    __dpmi_int(0x13, &regs);  /* call BIOS */
    if (regs.x.flags & 1) {   /* if carry flag set -> error */
        fprintf(stderr,"dos_bios_read: BIOS call failed: status code: 0x%02x\n",regs.h.ah);
        exit(51);
    }

    /* copy sector data to return buffer */
    dosmemget((__tb >> 4) * 16 + (__tb & 0x0f), 256, buffer);

    return(0);
}

static int dos_bios_write(FILE *disk,int blockno,cpg_u8_t *buffer)
{
    __dpmi_regs regs;
    unsigned int drive, cyl, head, sec;

    if (firstcall) setup_int1e();

    /* setup registers and call BIOS Int 13H function 3 (write sectors) */

    drive = (unsigned int)disk;
    blkno_to_chs(blockno,cyl,head,sec);

    /* copy sector data to realmode buffer */
    dosmemput(buffer, 256, (__tb >> 4) * 16 + (__tb & 0x0f));

#ifdef DEBUG
    printf("writing sector %u (cyl %u, head %u, sec %u)\n",
           blockno,
           cyl, head, sec);
#endif

    regs.x.ax = 0x0301;    /* AH: write sectors, AL: # of sectors */
    regs.h.ch = cyl;       /* CL: low eight bits of cylinder # */
    regs.h.cl = sec;       /* CH: sector # (bits 0-5), high 2 bits of cylinder # */
    regs.h.dh = head;      /* DH: head # */
    regs.h.dl = drive;     /* DL: drive # */
    regs.x.es = __tb >> 4;
    regs.x.bx = __tb & 0x0f;

    __dpmi_int(0x13, &regs);  /* call BIOS */
    if (regs.x.flags & 1) {   /* if carry flag set -> error */
        fprintf(stderr,"dos_bios_write: BIOS call failed: status code: 0x%02x\n",regs.h.ah);
        exit(61);
    }

    return(0);
}

#endif

int read_phys_block(FILE *disk,int blockno,cpg_u8_t *buffer)
{
    if (blockno > 0x460) {
        fprintf(stderr,"read_phys_block: access outa bounds - block %d\n",blockno);
        exit(50);
    }
#ifdef DOS
    if ((unsigned int)disk < 'Z' - 'A')
        return(dos_bios_read(disk,blockno,buffer));
#endif
    if (fseek(disk,blockno * 256,SEEK_SET)) {
        fprintf(stderr,"read_phys_block: fseek failed: %s\n",strerror(errno));
        exit(51);
    }
    if (fread(buffer,256,1,disk) != 1) {
        fprintf(stderr,"read_phys_block: fread failed %s\n",strerror(errno));
        exit(52);
    }
    return(0);
}

int write_phys_block(FILE *disk,int blockno,cpg_u8_t *buffer)
{
    if (blockno > 0x460) {
        fprintf(stderr,"write_phys_block: access outa bounds - block %d\n",blockno);
        exit(60);
    }
#ifdef DOS
    if ((unsigned int)disk < 'Z' - 'A')
        return(dos_bios_write(disk,blockno,buffer));
#endif
    if (fseek(disk,blockno * 256,SEEK_SET)) {
        fprintf(stderr,"write_phys_block: fseek failed: %s\n",strerror(errno));
        exit(61);
    }
    if (fwrite(buffer,256,1,disk) != 1) {
        fprintf(stderr,"write_phys_block: fwrite failed %s\n",strerror(errno));
        exit(62);
    }
    return(0);
}

/* Local Variables: */
/* c-file-style: "cpg" */
/* c-basic-offset: 4 */
/* End: */
