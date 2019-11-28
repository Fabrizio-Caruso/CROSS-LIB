/* BBCIM: Acorn BBC computer diskimage manipulation
 *
 * bbcim.h (bbcim; bbc diskbeeld manipulatie): Functies.
 *
 * Copyright (C) W.H. Scholten 1996, 1997, 1999, 2000, 2001, 2003, 2009, 2014, 2016
 */



#ifndef _BBCIM__bbcim_h
#define _BBCIM__bbcim_h

#define BBCIM_VERSION "1.2"
#define BBCIM_COPYRIGHT "(C) W.H.Scholten 1996-2016"

#include <_bbc_archive.h>



extern int bbcim_debug;
extern int bbcim_lib_version;


/* Used in the frontend: */
#define BBCIM__SD_SECTORSIZE       (256)
#define BBCIM__DD_SECTORSIZE       (256)
#define BBCIM__ADFS_SECTORSIZE     (256)

#define BBCIM__SD_SECTORSPERTRACK   (10)
#define BBCIM__DD_SECTORSPERTRACK   (18)
#define BBCIM__ADFS_SECTORSPERTRACK (16)


/* Foutmeldingen (error messages) */
#define BBCIM__OK                             0
#define BBCIM_ERROR__GENERIC                 -1
#define BBCIM_ERROR__OUT_OF_MEMORY           -2
#define BBCIM_ERROR__OPEN_FAILED             -3

#define BBCIM_ERROR__NO_ROOT_DIR             -10
#define BBCIM_ERROR__BAD_DISK_TYPE_ARGUMENT  -11
#define BBCIM_ERROR__BAD_DISK_SIDE           -12

#define BBCIM_ERROR__DISK_TRUNCATED         -20

#define BBCIM_ERROR__NO_INF_FOUND           -30
#define BBCIM_ERROR__BAD_DSK_FORMAT         -31
#define BBCIM_ERROR__BAD_INF_FORMAT         -32


/* BBC like errors: */
#define BBCIM_ERROR__BAD_DISK               -50
#define BBCIM_ERROR__DISK_FULL              -51
#define BBCIM_ERROR__LOCKED_FILE            -52
#define BBCIM_ERROR__FILE_NOT_FOUND         -53
#define BBCIM_ERROR__DIR_NOT_FOUND          -54

#define BBCIM_ERROR__NOT_IMPLEMENTED       -128


/*__________________VARIABELEN___________________*/
extern char *bbcim_error; /* points to the last error */
extern int  bbcim_errno;


//struct bbcim_diskimage_attr {
//    int  type; /* DFS_DISK, W62_DISK, DDOS_DISK, ADFS_DISK, HDFS_DISK */
//    int  sides;
//    int  tracks;
//    int  sectors_per_track;
//    int  bytes_per_sector;
//};
//struct bbcim_diskimage {
//	struct bbcim_diskimage_attr *attr;
//	char *filename;
//	char *dsk_filename;
//};

struct bbcim_tapeimage_attr {
	int  type; /* RAW, BLOCKS */
	int  nblocks;
};
struct bbcim_tapeimage {
	struct bbcim_tapeimage_attr *attr;
	char *filename;
	char *dsk_filename;
};

//#define BBCIM_ATTR__LOCKED   1 /* DFS & TAPE */
//
//#define BBCIM_ATTR__R        1 /* ADFS */
//#define BBCIM_ATTR__W        2
//#define BBCIM_ATTR__E        4
//#define BBCIM_ATTR__D        8
//
//#define BBCIM_FILETYPE__TAPE  1
//#define BBCIM_FILETYPE__DISK  2

//struct bbcim_file_attr {
//	/* BBC filename/attributes */
//	int  type; /* BBCCIM_FILETYPE_TAPE etc. */
//    char *name; /* malloced */
//    int loadaddr;
//    int execaddr;
//    int length;
//    unsigned char attributes;
//	int crc; /* 0-65535 for a valid crc, any other value denotes an invalid or no crc found; bbcim returns -1 in such a case */
//	char *next_tape_file; /* host filename! (malloced) */
//};

//struct bbcim_file {
//	struct bbcim_file_attr *attr;
//	char *filename;
//	char *inf_filename;
//};



struct bbcim_disk_data {
/* should this be in diskimage struct ? */
/* NB only file address and cat address really needed. All else can be calculated from the data in the diskimage struct
   tracksize=image->sectors*image->bytes
   track=addr /(tracksize*sides); side =(addr /tracksize)&(sides-1);
   */
    int file_address;
    int cat_address;
    int file_no; /* index into catalogue at 'cat_address' */

#if 0 /* Not needed: */
    int side;
    int file_track; /* file address */
    int file_sector;
    int file_byte;
    int cat_track;  /* catalogue entry address */
    int cat_sector;
    int cat_byte;
#endif
#if 0
    /* might be useful: */
    int  bootoption;
    char *diskname;
    char dirname[50]; /* current dir */
    int fileptr;
    char filename[50];
    int  status; /* bad image, error? */
#endif
	unsigned char *data;
};

struct bbcim_file_data {
    char *filename;
    unsigned char *data;
};


/* vlaggen voor cat-extract-crc-add */
//#define BBCIM_VERBOSE             1
#define BBCIM_EXTRACT_FILES       2
#define BBCIM_EXTRACT_DIR         4
#define BBCIM_SHOW_FILES          8
#define BBCIM_CALC_CRC           16
#define BBCIM__NAME__KEEP_NONROOT_DIR    32
#define BBCIM__NAME__KEEP_ROOT_DIR        1
//#define BBCIM_REMOVE_DISK        64
#define BBCIM_SHORT_NAME        128
#define BBCIM_DISK_INFO         256
#define BBCIM_REMOVE_FILE       512
#define BBCIM_FILE_INFO        1024

/* Low level reading */
#define BBCIM_GET_ATTRIBUTES   2048
#define BBCIM_LOAD_FILE        4096
#define BBCIM_SAVE_FILE        8192
#define BBCIM_NEW_DISK        16384
#define BBCIM_ADD_FILES       32768


/*__________________FUNCTIES___________________*/
int  bbcim_disk_cat_extract_crc (struct bbc_archive__diskimage *image, int side, struct bbc_archive__file* file, int flags, int verbose);

int  bbcim_disk_add_file (struct bbc_archive__diskimage *image, int side, char *filename, struct bbc_archive__file *file, int flags, int verbose);

int  bbcim_disk_del_file (struct bbc_archive__diskimage *image, int side, char *del_bbcfilename, int flags, int verbose);

int  bbcim_disk_minimise (struct bbc_archive__diskimage *image, int verbose);
int  bbcim_disk_expand (struct bbc_archive__diskimage *image, int expand, int verbose);
int  bbcim_disk_new (struct bbc_archive__diskimage *image, int verbose);
int  bbcim_disk_set_boot (struct bbc_archive__diskimage *image, int side, int bootoption, int verbose);
//int  bbcim_disk_identify (struct bbc_archive__diskimage *image, int verbose);
int  bbcim_disk_identify (char *filename, int *possible_disktypes, int verbose);

int  bbcim_interleave_singlesided (struct bbc_archive__diskimage *image0, struct bbc_archive__diskimage *image1, char *interleaved, int verbose);
int  bbcim_split_doublesided (struct bbc_archive__diskimage *image, int verbose);


int  bbcim_ddos_to_dfs (struct bbc_archive__diskimage *image, int verbose);
int  bbcim_w62_to_dfs (struct bbc_archive__diskimage *image, int verbose);


/* Reading from/writing to diskimages taking interleaved images into acount: */
int bbcim_disk_offset(struct bbc_archive__diskimage *image, int side_offset, int side);
int bbcim_disk_write(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *byte);
int bbcim_disk_read(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *byte);
int bbcim_disk_write_multi(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *bytes, int nbytes);
int bbcim_disk_read_multi(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *bytes, int nbytes);


/* ================= Internal functions: ===================== */
/* Utility functions: */
void bbcim_reduce_bbcname__no_dir (char *unixnaam, char *bbcnaam);
void bbcim_reduce_bbcname__no_rootdir (char *unixnaam, char *bbcnaam);



/* Printing/setting error: */
int  bbcim_print_error_unsupported_disktype(char *function, struct bbc_archive__diskimage *image, int verbose);
int  bbcim_print_error_bad_side(char *function, struct bbc_archive__diskimage *image, int side, int verbose);
int  bbcim_print_error_diskimage_truncated(char *function, struct bbc_archive__diskimage *image, char *file, int verbose);

int  bbcim_set_error(int errno, int verbose);


/* Other */
int  bbcim_rename_diskimage(char *from, char *to, int verbose);
int  bbcim_file_sanitycheck(char *filename, int verbose);
int  bbcim_diskimage_sanitycheck(struct bbc_archive__diskimage *image, int verbose);


/* Should go to the main program: */
int  bbcim_overschrijf_vraag (char *oude_naam, char *nieuwe_naam);



/* ========= Low level, to be added: loading files in memory, getting file attributes etc. ========= */
/*
int  bbcim_d_read_first_catalogue_entry (struct bbc_archive__diskimage *image, struct bbc_archive__file *file);

int  bbcim_d_read_catalogue_entry (struct bbc_archive__diskimage *image, struct bbc_archive__file *file);
int  bbcim_d_write_catalogue_entry (struct bbc_archive__diskimage *image, struct bbc_archive__file *file);

int  bbcim_d_write_cat_entry (struct bbc_archive__diskimage *image, struct bbc_archive__file *file, struct bbcim_disk_data *disk_data);

int  bbcim_d_read_first_file (struct bbc_archive__diskimage *image, struct bbc_archive__file *file, byte *data);
int  bbcim_d_read_file (struct bbc_archive__diskimage *image, struct bbc_archive__file *file, byte *data);
int  bbcim_d_read_next_file (struct bbc_archive__diskimage *image, struct bbc_archive__file *file, byte *data);
int  bbcim_d_write_file (struct bbc_archive__diskimage *image, struct bbc_archive__file *file);
*/

#endif /* _BBCIM__bbcim_h */
