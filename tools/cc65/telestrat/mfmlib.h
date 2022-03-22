/**********************************************************************************/
/*                                                                                */
/* MFM disk library routines v1.0                                                 */
/*                                                                                */
/* F.Frances 2/12/01                                                              */
/*                                                                                */
/**********************************************************************************/


int open_disk(char *filename); /* parameter: disk image name (eg. "sedoric.dsk") */
int close_disk();	/* this is mandatory before leaving your program */

int read_sector(char *sector_buf, int cylinder, int head, int sector);
int write_sector(char *sector_buf, int cylinder, int head, int sector);


/* All routines return the following error values */

#define OK 0
#define NO_OPENED_DISK 1
#define DISK_ALREADY_OPENED 2
#define CANNOT_OPEN_DISK 3
#define INVALID_DISK 4
#define TRACK_NOT_FOUND 5
#define SECTOR_ID_NOT_FOUND 6
#define SECTOR_DATA_NOT_FOUND 7
