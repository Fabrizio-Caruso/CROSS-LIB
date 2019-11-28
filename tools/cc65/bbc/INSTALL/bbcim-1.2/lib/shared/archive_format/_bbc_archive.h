/* archive read attributes.h
 *
 * This file is public domain, by W.H.Scholten 1997-2001, 2009
 */

#ifndef _BBC_ARCHIVE__archive_h
#define _BBC_ARCHIVE__archive_h



#define BBC_ARCHIVE__FILETYPE__TAPE  1
#define BBC_ARCHIVE__FILETYPE__DFS   2


#define BBC_ARCHIVE__ATTR__LOCKED   1 /* DFS & TAPE */

#define BBC_ARCHIVE__ATTR__R        1 /* ADFS */
#define BBC_ARCHIVE__ATTR__W        2
#define BBC_ARCHIVE__ATTR__E        4
#define BBC_ARCHIVE__ATTR__D        8

struct bbc_archive__file_attr {
	/* BBC filename/attributes */
	int  type; /* BBC_ARCHIVE__FILETYPE__TAPE etc. */
    char *name; /* malloced */
    unsigned int load_address;
    unsigned int exec_address;
    unsigned int length;
    unsigned char attributes;
	int crc; /* 0-65535 for a valid crc, any other value denotes an invalid or no crc found; bbcim returns -1 in such a case */
	char *next_tape_file; /* host filename! (malloced) */
};

struct bbc_archive__file {
	struct bbc_archive__file_attr *attr;
	char *filename;
	char *inf_filename;
};

enum bbc_archive__dfs_bootoption { BBCIM_DFS_BOOTOPTION_NONE, BBCIM_DFS_BOOTOPTION_LOAD, BBCIM_DFS_BOOTOPTION_RUN, BBCIM_DFS_BOOTOPTION_EXEC };

enum bbc_archive__disktype { BBC_ARCHIVE__UNKNOWN_DISK, BBC_ARCHIVE__DFS_DISK, BBC_ARCHIVE__DDOS_DISK, BBC_ARCHIVE__W62_DISK, BBC_ARCHIVE__SOLIDISK_DDFS_DISK, BBC_ARCHIVE__ADFS_DISK, BBC_ARCHIVE__HDFS_DISK };

//OLDstruct bbc_archive__disk_image {
//OLD	int  type; /* one of the above 'Disktype' */
//OLD	int  sides;
//OLD	int  tracks;
//OLD	int  sectors_per_track;
//OLD	int  bytes_per_sector;
//OLD};



struct bbc_archive__diskimage_attr {
    int  type; /* BBC_ARCHIVE__DFS_DISK, BBC_ARCHIVE__DDOS_DISK, BBC_ARCHIVE__W62_DISK, BBC_ARCHIVE__SOLIDISK_DDFS_DISK, BBC_ARCHIVE__ADFS_DISK, BBC_ARCHIVE__HDFS_DISK */
    int  sides;
    int  tracks;
    int  sectors_per_track;
    int  bytes_per_sector;
};

struct bbc_archive__diskimage {
	struct bbc_archive__diskimage_attr *attr;
	char *filename;
	char *dsk_filename;
};


int  bbc_archive__read_file_attributes(char *filename, struct bbc_archive__file_attr *file, int verbose);
void bbc_archive__free_file_attributes(struct bbc_archive__file_attr *file);

int  bbc_archive__read_disk_attributes(char *diskname, struct bbc_archive__diskimage_attr *image, int verbose);
void bbc_archive__free_disk_attributes(struct bbc_archive__diskimage_attr *disk);

char *bbc_archive__new_inf_filename(char *filename, int verbose); /* make .inf filename */
int bbc_archive__is_inf_file(char *filename); /* Is the file a .inf file? */
char *bbc_archive__new_dsk_filename(char *diskname, int verbose); /* make .dsk filename */
int bbc_archive__write_file_attributes(char *filename, struct bbc_archive__file_attr *file, int verbose);
int bbc_archive__write_disk_attributes(char *diskname, struct bbc_archive__diskimage_attr *image, int verbose);
int bbc_archive__write_disk_attributes2(char *diskname, int sides, int tracks, int sectors, int bytes, int type, int verbose);

int bbc_archive__diskinfo_from_string(char *string, struct bbc_archive__diskimage_attr *image); /* Should be internal? But used in bbcim for command line processing */

extern char *bbc_archive__dfs_names[];
extern char *bbc_archive__dfs_sides[];

//extern int bbc_archive__errno;
//extern char *bbc_archive__error;


#endif /* _BBC_ARCHIVE__archive_h */
