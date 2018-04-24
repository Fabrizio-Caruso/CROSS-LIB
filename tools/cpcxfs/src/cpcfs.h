
/*				<<<<Last Modified: Thu Feb 08 15:07:30 1996>>>>
------------------------------------------------------------------------------

        =====
        CPCfs  --  c p c f s . h	Variable, Structures, Prototypes
        =====

	Version 0.85                    (c) February '96 by Derik van Zuetphen
------------------------------------------------------------------------------
*/

#ifndef CPCFS_H_INCLUDED
#define CPCFS_H_INCLUDED

#include <limits.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <signal.h>
#include <time.h>
#include "match.h"



/****** Configure here! (or in Makefile) ******/

/* One out of the constants LINUX, HPUX or DOS must be set to 1.
This is normally done in the Makefile, in the project file, or with the
Options menu. */


/* for Win32 we want dos code */
#if defined(WIN32) || defined(WIN64)
#define DOS		1
#endif

/*
#ifndef DOS
#define DOS		1
#endif
#ifndef LINUX
#define LINUX		0
#endif
*/
#ifndef HPUX
#define HPUX		0
#endif
#ifndef SUNOS
#define SUNOS		0
#endif


//#ifndef USE_READLINE
//#define USE_READLINE	1	/* either GNU "readline" or ACTlib17 "inputs" */
//#endif

/****** End of configuration ******/

#define MAJORVERSION	0
#define MINORVERSION	85
#define PATCHLEVEL	0
extern char stamp[];
extern char copyright[];


/****** Operating System ******/

#define SYSV	LINUX | HPUX
#define BSD	SUNOS
#define UNIX	SYSV | BSD
#if UNIX
#include "unix.h"
#elif DOS
#include "dos.h"
#endif


#include <string.h>

typedef unsigned char	uchar;
typedef unsigned short	ushort;
/* <ushort> defined in types.h, must be in INTEL byte-order! (low, high) */
#if defined(DOS)
typedef unsigned short ushort;
#endif

typedef char bool;
#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif



/***********
  Variables
 ***********/

/****** CPC Filesystem ******/
/* size of block index in bytes - as stored in directory */
extern int	BLKNR_SIZE;
/* number of block index's that will fit into a directory entry */
extern int	BLKNR;

/* KT - removed */
#if 0
typedef enum {	SYSTEMFORMAT=0x41,
		DATAFORMAT=0xC1,
		IBMFORMAT=0x01,
		VORTEXFORMAT=0x80	/* A flag, the sector offset is 01 too*/
} Format_ID;
#endif

#define RECORDSIZE	128	/* 1/8 kByte, constant for CP/M */
#define EXTENTSIZE	16384	/* 16 kByte, constant for CP/M */
#define CPM_EOF		26	/* Ctrl-Z */

extern	unsigned char filler;	/* copied from track header */

/* KT - added this #def so we can check where full_imagename is used */
#if defined(WIN32) || defined(WIN64)
#define FULL_IMAGENAME_LENGTH _MAX_PATH
#else
#define FULL_IMAGENAME_LENGTH	65
#endif

extern	char	full_imagename[FULL_IMAGENAME_LENGTH];	/* full pathname, ... */
extern	char	*imagename;		/* ... only name portion, and ... */
extern	int	imagefile;		/* ... handle of imagefile */

extern	int	cur_user;
/* KT - removed */
/*extern	int	cur_format; */

extern	uchar	*track;		/* 0x100 header + track data */

extern uchar	*block_buffer;	/* 1024 or more bytes for one block */

extern	uchar	*blk_alloc;	/* Block-Allocation-Table, points to entry, */
				/* FF = free block */
extern	int	allocated_blks;
extern	int	free_blks;
extern	int	total_blks;	/* = allocated_blks + free_blks */
extern	float	percentage;	/* of allocated blocks */



/****** User Interface *****/
#define	CONFIGNAME	"cpcfs.cfg"
#define HELPFILE	"cpcfs.hlp"
#define INPUTLEN	256
extern	int	nbof_args;
extern	char	*arg[INPUTLEN];
extern	char	prompt[INPUTLEN];
extern	int	pagelen, lineno;
extern	bool	Break_Wish;
extern	jmp_buf break_entry;
extern	bool	Interactive;
extern	char	installpath[];
extern	int	mode;		/* Mode for GET-commands */
#define M_TEXT	1
#define M_BIN	2
#define M_AUTO	3

extern	bool	force;
extern int	Verb;


/****** Utility Macros ******/
/* Answer a plural suffix, if nr is not 1 */
#define plural(nr)	((nr)==1?"":"s")	
#define plural_y(nr)	((nr)==1?"y":"ies")
#define atoxi(CP)	(int)strtol((CP),NULL,0)
			/* same as atoi, but recognizes 0x and 0 prefixes */

#ifndef max
#define max(A,B)	((A)>=(B)?(A):(B))
#endif
#ifndef min
#define min(A,B)	((A)<=(B)?(A):(B))
#endif


/****** Bitmasks for attributes and dir command */
#define DIR_DOUBLE	0x0	/* \				*/
#define DIR_WIDE	0x1	/*  \ bit 0 and 1 => 4 choices	*/
#define DIR_AMSHEAD	0x2	/*  /				*/
#define DIR_LONG	0x3	/* /				*/
#define DIR_SORT	0x4	/*    bit 2 => 1 choice		*/

#define ATTR_1		0x400
#define ATTR_2		0x200
#define ATTR_3		0x100
#define ATTR_4		0x080
#define ATTR_5		0x040
#define ATTR_6		0x020
#define ATTR_7		0x010
#define ATTR_8		0x008
#define ATTR_R		0x004
#define ATTR_S		0x002
#define ATTR_A		0x001


/************
  Structures
 ************/

struct d_header {	/* disk header of size 0x100 */
	uchar	tag[0x30];
		/*00-21	MV - CPC ... */
		/*22-2F	unused (0) */
	uchar	nbof_tracks;
		/*30	number of tracks (40) */
	uchar	nbof_heads;
		/*31	number of heads (1) 2 not yet supported by cpcemu */
	short	tracksize;	/* short must be 16bit integer */
		/*32-33	tracksize (including 0x100 bytes header)
			9 sectors * 0x200 bytes each + header = 0x1300 */
	uchar	unused[0xcc];
		/*34-FF	unused (0)*/
};

extern struct d_header disk_header;



struct s_info {		/* sector info, used in track header */
/*Sector-Information (for each sector):*/
	uchar	track;
		/*18+i	tracknumber	\				*/
	uchar	head;
		/*19+i	headnumber	 | Sector-ID-Information	*/
	uchar	sector;
		/*1A+i	sectornumber	 |				*/
	uchar	BPS;
		/*1B+i	BPS          	/				*/
	uchar	status1;
		/*1C+i	status 1 errorcode (0)*/
	uchar	status2;
		/*1D+i	status 2 errorcode (0)*/
	uchar	unused[2];
		/*1E+i,1F+i unused (0)*/
} ;

struct t_header  {	/* track header of size 0x100 */
	uchar	tag[0x10];
		/*00-0C	Track-Info\r\n*/
		/*0D-0F	unused (0)*/
	uchar	track;
		/*10	tracknumber (0 to number-of-tracks - 1)*/
	uchar	head;
		/*11	headnumber (0)*/
	uchar	unused[2];
		/*12-13	unused (0)*/
		/*Format-Track-Parameter:*/
	uchar	BPS;
		/*14	BPS (bytes per sector) (2 for 0x200 Bytes)*/
	uchar	SPT;
		/*15	SPT (sectors per track) (9, max. 18 possible)*/
	uchar	GAP3;
		/*16	GAP#3 Format (gap for formatting: 0x4E)*/
	uchar	filler;
		/*17	Filling-Byte (filler for formatting: 0xE5)*/
	struct s_info sector[29];
} ;


/****** Directory ******/

typedef struct {
	uchar	user;		/* actually a byte */
	uchar	root[8];	/* padded with space */
	uchar	ext[3];		/* ditto	     */
	uchar	name[13];	/* <root 8>+"."+<ext 3>+"\0" (for globbing) */
	uchar	rec;		/* size in 128 Byte records */

	int	attr;		/* bit array of size 11 (87654321rsa) */
	int	blk[16];	/* 16 or 8 indices of 1 or 2 byte */
	uchar	extent;		/* aka sorting criterion for dir entires */

/* only stored to write it back */
	uchar	unused[2];	/* used for internal CP/M purposes */

/* organisational */
	bool	first;		/* this entry is first */
	long	size;		/* length in Bytes, only set if <first> */
	int	next;		/* next entry for this file, -1 if last */
} DirEntry;

extern DirEntry *directory;


typedef enum
{
	ORDER_SIDES,
	ORDER_CYLINDERS,
	ORDER_EAGLE
} BLOCK_ORDER;

/****** Disk Parameter Block ******/

typedef struct {
/* extended DPB info, needed for format, in DPB_store */
	/* KT - removed */
	/*uchar  ID;*/  /* Identifier */

	ushort SEC_side1[32];
	ushort SEC_side2[32];
	ushort SECS;/* number of sectors per track (8, >9) */
	ushort TRKS;/* number of tracks per side (>40, 80) */
	ushort HDS; /* number of heads per disk (>1, 2) */	
	ushort BPS; /* Bytes Per Sector (128, 256, >512, 1024) */
	
/* original Disk Parameter Block (> marks CPC defaults) */
	ushort SPT; /* records Per Track (18, 20, 30, 32, 34, >36, 40) */
	uchar  BSH; /* Block SHift ...      2^BSH = BLM+1 = Rec/Block */
	uchar  BLM; /* BLock Mask (>3/7, 4/15) */
	uchar  EXM; /* EXtent Mask (0, 1)   Number of Extents/Entry - 1 */
	ushort DSM; /* max. blocknumber = number of blocks - 1 */
	ushort DRM; /* DiRectory size - 1 (31, >63, 127) */
	uchar  AL0; /* \ DRM in binary (80h/0, >C0h/0, F0h/0) */
	uchar  AL1; /* / (1 bit=1 block, 11000000/00000000 = 2 blocks) */
	ushort CKS; /* ChecK recordS,nb of rec in dir (8, >16, 32) */
	ushort OFS; /* OFfSet, reserved tracks (1, >2, 3, 4) */

/* extended DPB info, automatically calculated */
	ushort BLS; /* BLock Size in bytes (>1024, 2048)*/
	bool   SYS; /* CP/M present in system tracks */
	ushort DBL; /* Directory BLocks = CKS/8 */

	BLOCK_ORDER	order;
	uchar	side0_hd;	/* if accessing side 0, use this head value */
	uchar	side1_hd;	/* if accessing side 1, use this head value */

	/* sector id skew */
	int		skew;

	int		num_extents; 	/* calculated from EXM*/

	unsigned char *label_data;	/* NULL = no label file, !=NULL = label file */
	unsigned long label_data_length;

} DPB_type;

/* KT - added link list of DPB's */
typedef struct DPB_list_entry
{
	/* ident */
	char	*ident;
	/* format description */
	char	*description;
	/* dpb for this format */
	DPB_type dpb;
	/* pointer to next entry in list */
	struct DPB_list_entry *next;
} DPB_list_entry;

typedef struct DPB_list
{
	/* pointer to first entry in list */
	DPB_list_entry *first;
} DPB_list;

/* KT - removed */
#if 0
/* DPB templates for SYSTEM, DATA, IBM, VORTEX, and user defined */
extern const int DPB_store_size;
typedef enum {	SYSTEM_DPB=0,
		DATA_DPB=1,
		IBM_DPB=2,
		VORTEX_DPB=3,
		USER_DPB=4
} DPB_Index_Type;
extern DPB_type DPB_store[];
#endif

extern DPB_type *dpb;	/* pointer to current DPB */

/* KT - added */
/* current dpb list entry */
extern DPB_list_entry *dpb_list_entry;
/* first dpb list entry in list */
extern DPB_list dpb_list;

DPB_list_entry *get_dpb_entry_from_format_name(const char *);

/*******
  Tools
 *******/

void	initialise(void);
void putcharm(int,char);
void printm(int,char*,...);
char *lower(char*);
char *upper(char*);
char *append_suffix (char*,char*);
int  errorf (bool,const char*,...);
const char *show_attr(int attr, int mask, bool always);
const char* show_all_attr(int att, bool always);
void do_break();
void newpage(char* keys);
char nextline();
char *repstr(char c,int times);
const char *show_format (struct DPB_list_entry *);
char *show_mode (int m);
void reparse (int argnb);
void expand_percent(char *from, char *to, int max);
void echom (int v, char *p);
bool confirmed() ;
bool tag_ok ();
void alloc_block(int blk);
void free_block(int blk);
bool is_free_block(int blk);
void calc_allocation ();
bool inactive ();
int  trk_calc (int blk);
int  sec_calc (int blk);
void abandonimage();
int  parse_cpm_filename(char *name, int *user, char *root, char *ext);
int  parse_filename(char *name, int *drive, char *path, char *root, char *ext);
int  pager(char *filename);
bool has_wildcards(char os_tag, char *filename);
void build_cpm_name   (char *buf, int user, char *root, char *ext);
void build_cpm_name_32(char *buf, int user, char *root, char *ext);
void str2mem(char *mem, char *str, int spc);
char *show_hex(int nr, uchar *buf, int size);
void *Malloc(int bytes);
void LoadLabelFile(char *LabelFilename, unsigned char **ppData, unsigned long *ppLength);


/********
  Tracks
 ********/

int  read_track (int hd, int trk);
int  write_track();
bool next_sector(int*,int*,int*);

/***********
  Directory
 ***********/

void get_directory();
void put_directory();
void update_directory(int set_dirty);

extern int glob_env;
int glob_cpm_next();
int glob_cpm_file(char *pat);

/*******
  Image
 *******/

void close_image();
int  open_image(char *name,int);
/* KT - modified */
int format(char* name, DPB_list_entry *entry, int extended);
/* int  format(char* name,DPB_type *dpb); */
/* KT - removed */
/* int  sysgen(char* name);*/
int  comment_image(const char *text);
int	write_label(DPB_type *dpb,char *label_data, unsigned long label_data_length);
	
/********
  Blocks
 ********/

int  get_free_block();


/*****************
  FS Maintenance
 *****************/

long delete (bool silent, char *pattern) ;
int  parse_attr(char *str, int *mask, bool *set);
int  change_attrib(char* pattern, int set, int reset);
long	nuke(bool silent, char *pattern) ;
void	clean(void);

/**********
  Transfer
 **********/

int  detectmode (char *buf, int size) ;
long get (char *src,char *target);
long put (char *src, char *trg);
int  dir(char *pat, int mask);
int  ren_file(char *from, char *to);
int  ren_wild(char *pat, int user);
int  copy_file(char *from, char *to);
int  copy_wild(char *pat, int user);

/*********
  Dumping
 *********/

int  dump(FILE *file, int block, int head, int track, int sector);
int  dumpdir (FILE *file);
int  map (FILE *file);


/****************
  User Interface
 ****************/

int  execute_file (char *name);
int  execute_cmd (char *cmd);
/*int  execute_one_cmd (char *cmd);*/

#if defined(WIN32) || defined(WIN64)
#define vert	'|'
#define hori	'-'
#define cross	'+'
#else
#if DOS
#define vert	0xB3
#define hori	0xC4
#define cross	0xC5
#else
#define vert	'|'
#define hori	'-'
#define cross	'+'
#endif
#endif

#endif
