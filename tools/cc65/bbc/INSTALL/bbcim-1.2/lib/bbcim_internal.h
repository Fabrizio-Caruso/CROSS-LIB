/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * bbcim.h (bbcim; bbc diskbeeld manipulatie): Functies.
 *
 * Copyright (C) W.H. Scholten 1996, 1997, 1999, 2000, 2003
 */

#ifndef _BBCIM__bbcim_internal_h
#define _BBCIM__bbcim_internal_h

#ifdef DEBUG
#  define DEB(x) x
#else
#  define DEB(x)
#endif

/* Multi-byte reading functions: */
int bbcim_read4lsb(unsigned char *data);
int bbcim_read3lsb(unsigned char *data);
int bbcim_read2lsb(unsigned char *data);


/* Byte offsets in disk image for Acorn DFS */
#define OFFSET__DFS_title0_7     0
#define OFFSET__DFS_title8_11    256
#define OFFSET__DFS_accessct    (256+0x04)
#define OFFSET__DFS_entries     (256+5)
#define OFFSET__DFS_bootsec     (256+6)
#define OFFSET__DFS_sectondiskL (256+0x07)

/* The next list of offsets are +n*8 for each file */
#define OFFSET__DFS_filename   8
#define OFFSET__DFS_directory  15
#define OFFSET__DFS_loadaddrL (256+8)
#define OFFSET__DFS_loadaddrM (256+9)
#define OFFSET__DFS_execaddrL (256+0x0a)
#define OFFSET__DFS_execaddrM (256+0x0b)
#define OFFSET__DFS_lengthL   (256+0x0c)
#define OFFSET__DFS_lengthM   (256+0x0d)
#define OFFSET__DFS_eellddss  (256+0x0e)
#define OFFSET__DFS_startsecL (256+0x0f)


#endif /* _BBCIM__bbcim_internal_h */
