/*******************************************************************************
* Copyright (c) 2008-2022 JackAsser, Krill, Claus, Björn Esser
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*******************************************************************************/

#define VERSION "3.4"

#define _CRT_SECURE_NO_WARNINGS /* avoid security warnings for MSVC */

#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wchar.h>

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <stdint.h>
#endif

#define min(a, b) (((a) < (b)) ? (a) : (b))

#ifdef _WIN32
#include <windows.h>
#define FILESEPARATOR '\\'
#else
#define FILESEPARATOR '/'
#endif

#define DIRENTRIESPERBLOCK     8
#define DIRTRACK_D41_D71       18
#define DIRTRACK_D81           40
#define SECTORSPERTRACK_D81    40
#define MAXNUMFILES_D81        ((SECTORSPERTRACK_D81 - 3) * DIRENTRIESPERBLOCK)
#define DIRENTRYSIZE           32
#define BLOCKSIZE              256
#define BLOCKOVERHEAD          2
#define TRACKLINKOFFSET        0
#define SECTORLINKOFFSET       1
#define FILETYPEOFFSET         2
#define FILETYPEDEL            0
#define FILETYPESEQ            1
#define FILETYPEPRG            2
#define FILETYPEUSR            3
#define FILETYPEREL            4
#define FILETYPETRANSWARPMASK  0x100
#define FILETRACKOFFSET        3
#define FILESECTOROFFSET       4
#define FILENAMEOFFSET         5
#define FILENAMEMAXSIZE        16
#define FILENAMEEMPTYCHAR      (' ' | 0x80)
#define TRANSWARPSIGNATROFFSLO 21
#define TRANSWARPSIGNATURELO   'T'
#define TRANSWARPSIGNATROFFSHI 22
#define TRANSWARPSIGNATUREHI   'W'
#define DIRDATACHECKSUMOFFSET  23
#define TRANSWARPTRACKOFFSET   24
#define FILECHECKSUMOFFSET     25
#define LOADADDRESSLOOFFSET    26
#define LOADADDRESSHIOFFSET    27
#define ENDADDRESSLOOFFSET     28
#define ENDADDRESSHIOFFSET     29
#define FILEBLOCKSLOOFFSET     30
#define FILEBLOCKSHIOFFSET     31
#define D64NUMBLOCKS           (664 + 19)
#define D64SIZE                (D64NUMBLOCKS * BLOCKSIZE)
#define D64SIZE_EXTENDED       (D64SIZE + 5 * 17 * BLOCKSIZE)
#define D71SIZE                (D64SIZE * 2)
#define D81SIZE                (D81NUMTRACKS * SECTORSPERTRACK_D81 * BLOCKSIZE)
#define D64NUMTRACKS           35
#define D64NUMTRACKS_EXTENDED  (D64NUMTRACKS + 5)
#define D71NUMTRACKS           (D64NUMTRACKS * 2)
#define D81NUMTRACKS           80
#define BAM_OFFSET_SPEED_DOS   0xac
#define BAM_OFFSET_DOLPHIN_DOS 0xc0
#define DIRSLOTEXISTS          0
#define DIRSLOTFOUND           1
#define DIRSLOTNOTFOUND        2
/* for sector chain analysis */
#define UNALLOCATED            0 /* unused as of now */
#define ALLOCATED              1 /* part of a valid sector chain */
#define FILESTART              2 /* analysed to be the start of a sector chain */
#define FILESTART_TRUNCATED    3 /* analysed to be the start of a sector chain, was truncated */
#define POTENTIALLYALLOCATED   4 /* currently being analysed */
/* error codes for sector chain validation */
#define VALID                  0 /* valid chain */
#define ILLEGAL_TRACK          1 /* ends with illegal track pointer */
#define ILLEGAL_SECTOR         2 /* ends with illegal sector pointer */
#define LOOP                   3 /* loop in current chain */
#define COLLISION              4 /* collision with other file */
#define CHAINED                5 /* ends at another file start */
#define CHAINED_TRUNCATED      6 /* ends at start of a truncated file */
#define FIRST_BROKEN           7 /* issue already in first sector */
/* undelete levels */
#define RESTORE_DIR_ONLY        0 /* Only restore all dir entries without touching any t/s links */
#define RESTORE_VALID_FILES     1 /* Fix dir entries for files with valid t/s chains */
#define RESTORE_VALID_CHAINS    2 /* Also add wild sector chains with valid t/s chains */
#define RESTORE_INVALID_FILES   3 /* Also fix dir entries with invalid t/s chains */
#define RESTORE_INVALID_CHAINS  4 /* Also add and fix wild invalid t/s chains */
#define RESTORE_INVALID_SINGLES 5 /* Also include single block files */
/* error codes for directory */
#define DIR_OK                 0
#define DIR_ILLEGAL_TS         1
#define DIR_CYCLIC_TS          2

#define TRANSWARP                "TRANSWARP"
#define TRANSWARPBASEBLOCKSIZE   0xc0
#define TRANSWARPBUFFERBLOCKSIZE 0x1f
#define TRANSWARPBLOCKSIZE       (TRANSWARPBASEBLOCKSIZE + TRANSWARPBUFFERBLOCKSIZE)
#define TRANSWARPKEYSIZE         29 /* 232 bits */
#define TRANSWARPKEYHASHROUNDS   33

/* Table for conversion of uppercase PETSCII to Unicode */
static unsigned int p2u_uppercase_tab[256] = {
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    ' ', '!', '\"', '#', '$', '%', '&', 0x2019, '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', 0xa3, ']', 0x2191, 0x2190,
    0x1fb79, 0x2660, 0x1fb72, 0x1fb78, 0x1fb77, 0x1fb76, 0x1fb7a, 0x1fb71, 0x1fb74, 0x256e, 0x2570, 0x256f, 0x1fb7c, 0x2572, 0x2571, 0x1fb7d,
    0x1fb7e, 0x25cf, 0x1fb7b, 0x2665, 0x1fb70, 0x256d, 0x2573, 0x25cb, 0x2663, 0x1fb75, 0x2666, 0x253c, 0x1fb8c, 0x2502, 0x3c0, 0x25e5,
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    ' ', 0x258c, 0x2584, 0x2594, 0x2581, 0x258e, 0x1fb95, 0x1fb75, 0x1fb8f, 0x25e4, 0x1fb75, 0x251c, 0x2597, 0x2514, 0x2510, 0x2581,
    0x250c, 0x2534, 0x252c, 0x2524, 0x258e, 0x258d, 0x2590, 0x2594, 0x2580, 0x2583, 0x1fb7f, 0x2596, 0x259d, 0x2518, 0x2598, 0x259a,
    0x2500, 0x2660, 0x1fb72, 0x2500, 0x1fb77, 0x1fb76, 0x1fb7a, 0x1fb71, 0x1fb74, 0x256e, 0x2570, 0x256f, 0x1fb7c, 0x2572, 0x2571, 0x1fb7d,
    0x1fb7e, 0x25cf, 0x1fb7b, 0x2665, 0x1fb70, 0x256d, 0x2573, 0x25cb, 0x2663, 0x1fb75, 0x2666, 0x253c, 0x1fb8c, 0x2502, 0x3c0, 0x25e5,
    ' ', 0x258c, 0x2584, 0x2594, 0x2581, 0x258e, 0x1fb95, 0x1fb75, 0x1fb8f, 0x25e4, 0x1fb75, 0x251c, 0x2597, 0x2514, 0x2510, 0x2581,
    0x250c, 0x2534, 0x252c, 0x2524, 0x258e, 0x258d, 0x1fb88, 0x1fb82, 0x1fb83, 0x2583, 0x1fb7f, 0x2596, 0x259d, 0x2518, 0x2598, 0x3c0
};

/* Table for conversion of lowercase PETSCII to Unicode */
static unsigned int p2u_lowercase_tab[256] = {
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    ' ', '!', '\"', '#', '$', '%', '&', 0x2019, '(', ')', '*', '+', ',', '-', '.', '/',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?',
    '@', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '[', 0xa3, ']', 0x2191, 0x2190,
    0x1fb79, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0x253c, 0x1fb8c, 0x2502, 0x1fb95, 0x1fb98,
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.',
    ' ', 0x258c, 0x2584, 0x2594, 0x2581, 0x258e, 0x1fb95, 0x1fb75, 0x1fb8f, 0x1fb99, 0x1fb75, 0x251c, 0x2597, 0x2514, 0x2510, 0x2581,
    0x250c, 0x2534, 0x252c, 0x2524, 0x258e, 0x258d, 0x2590, 0x2594, 0x2580, 0x2583, 0x1fb7f, 0x2713, 0x259d, 0x2518, 0x2598, 0x259a,
    0x2500, 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0x253c, 0x1fb8c, 0x2502, 0x1fb95, 0x1fb98,
    ' ', 0x258c, 0x2584, 0x2594, 0x2581, 0x258e, 0x1fb95, 0x1fb75, 0x1fb8f, 0x1fb99, 0x1fb75, 0x251c, 0x2597, 0x2514, 0x2510, 0x2581,
    0x250c, 0x2534, 0x252c, 0x2524, 0x258e, 0x258d, 0x1fb88, 0x1fb82, 0x1fb83, 0x2583, 0x2713, 0x2596, 0x259d, 0x2518, 0x2598, 0x1fb96
};

typedef struct {
    const unsigned char* alocalname;                  /* local file name or name of loop file in ASCII */
    unsigned char        plocalname[FILENAMEMAXSIZE]; /* loop file in PETSCII */
    unsigned char        pfilename[FILENAMEMAXSIZE];  /* disk file name in PETSCII */
    int                  direntryindex;
    int                  direntrysector;
    int                  direntryoffset;
    int                  sectorInterleave;
    int                  first_sector_new_track;
    int                  track;
    int                  sector;
    int                  nrSectors;
    int                  nrSectorsShown;
    int                  filetype;
    int                  mode;
    int                  force_new;
    int                  size;
    int                  last_track;
    bool                 have_key;
    unsigned char        key[TRANSWARPKEYSIZE];
} imagefile;

enum mode {
    MODE_BEGINNING_SECTOR_MASK   = 0x003f, /* 6 bits */
    MODE_MIN_TRACK_MASK          = 0x0fc0, /* 6 bits */
    MODE_MIN_TRACK_SHIFT         = 6,
    MODE_SAVETOEMPTYTRACKS       = 0x1000,
    MODE_FITONSINGLETRACK        = 0x2000,
    MODE_SAVECLUSTEROPTIMIZED    = 0x4000,
    MODE_LOOPFILE                = 0x8000,
    MODE_TRANSWARPBOOTFILE       = 0x10000,
    MODE_NOFILE                  = 0x20000
};

typedef enum {
    IMAGE_D64,
    IMAGE_D64_EXTENDED_SPEED_DOS,
    IMAGE_D64_EXTENDED_DOLPHIN_DOS,
    IMAGE_D71,
    IMAGE_D81
} image_type;

static const char *filetypename_uc[] = {
    "DEL", "SEQ", "PRG", "USR", "REL", "CBM", "???", "???",
    "???", "???", "???", "???", "???", "???", "???", "???"
};

static const char *filetypename_lc[] = {
    "del", "seq", "prg", "usr", "rel", "cbm", "???", "???",
    "???", "???", "???", "???", "???", "???", "???", "???"
};

static const char *error_name[] = {
    "no error", "illegal track", "illegal sector", "loop", "collision", "chained", "chained", "first block broken"
};

static const char *dir_error_string[] = {
    "no error", "illegal track or sector in directory sector chain", "cycle in directory sector chain"
};

static const int
sectors_per_track[] = {
    /*  1-17 */ 21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
    /* 18-24 */ 19,19,19,19,19,19,19,
    /* 25-30 */ 18,18,18,18,18,18,
    /* 31-35 */ 17,17,17,17,17,
    /* 36-52 */ 21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
    /* 53-59 */ 19,19,19,19,19,19,19,
    /* 60-65 */ 18,18,18,18,18,18,
    /* 66-70 */ 17,17,17,17,17
};

static const int
sectors_per_track_extended[] = {
    /*  1-17 */ 21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
    /* 18-24 */ 19,19,19,19,19,19,19,
    /* 25-30 */ 18,18,18,18,18,18,
    /* 31-35 */ 17,17,17,17,17,
    /* 36-40 */ 17,17,17,17,17,
    /* 41-57 */ 21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,21,
    /* 58-64 */ 19,19,19,19,19,19,19,
    /* 65-70 */ 18,18,18,18,18,18,
    /* 71-75 */ 17,17,17,17,17,
    /* 76-80 */ 17,17,17,17,17
};

static int quiet           = 0;  /* global quiet flag */
static int verbose         = 0;      /* global verbose flag */
static int num_files       = 0;      /* number of files to be written provided by the user */
static int max_hash_length = 16;     /* number of bytes of the filenames to calculate the hash over */
static int unicode         = 0;      /* which unicode mapping to use: 0 = none, 1 = upper case, 2 = lower case */
static int modified        = 0;      /* image needs to be written */
static int dir_error       = DIR_OK; /* directory has an error */

/* Prints the command line help */
static void
usage()
{
    printf("\n*** This is cc1541 version " VERSION " built on " __DATE__ " ***\n\n");
    printf("Usage: cc1541 [options] image.[d64|d71|d81]\n\n");
    printf("-n diskname   Disk name, default='cc1541'.\n");
    printf("-i id         Disk ID, default='00 2a'.\n");
    printf("-w localname  Write local file to disk, if filename is not set then the\n");
    printf("              local name is used. After file written, the filename is unset.\n");
    printf("-W localname  Like -w, but encode file in Transwarp format.\n");
    printf("              Provide Transwarp bootfile as last file using\n");
    printf("              \"-f 'transwarp vX.YZ' -w 'transwarp vX.YZ.prg'\"\n");
    printf("-K key        Set an encryption key for Transwarp files, a string of up to 29\n");
    printf("              characters.\n");
    printf("-f filename   Use filename as name when writing next file, use prefix # to\n");
    printf("              include arbitrary PETSCII characters (e.g. -f \"start#a0,8,1\").\n");
    printf("-o            Do not overwrite if file with same name exists already.\n");
    printf("-V            Do not modify image unless it is in valid CBM DOS format.\n");
    printf("-T filetype   Filetype for next file, allowed parameters are PRG, SEQ, USR, REL\n");
    printf("              and DEL, or a decimal number between 0 and 255. Default is PRG.\n");
    printf("-P            Set write protect flag for next file.\n");
    printf("-O            Set open flag for next file.\n");
    printf("-N            Force creation of a new directory entry, even if a file with the\n");
    printf("              same name exists already.\n");
    printf("-l filename   Write loop file (an additional dir entry) to existing file to\n");
    printf("              disk, set filename with -f.\n");
    printf("-L            Add dir entry without writing file (track and sector will be 0),\n");
    printf("              requires a filename given with -f.\n");
    printf("-B numblocks  Write the given value as file size in blocks to the directory for\n");
    printf("              the next file.\n");
    printf("-M numchars   Hash computation maximum filename length, this must\n");
    printf("              match loader option FILENAME_MAXLENGTH in Krill's Loader.\n");
    printf("              Default is 16.\n");
    printf("-m            Ignore filename hash collisions, without this switch a collision\n");
    printf("              results in an error.\n");
    printf("-d track      Maintain a shadow directory (copy of the actual directory without\n");
    printf("              a valid BAM).\n");
    printf("-t            Use directory track to also store files (makes -x useless)\n");
    printf("              (default no).\n");
    printf("-u numblocks  When using -t, amount of dir blocks to leave free (default=2).\n");
    printf("-x            Don't split files over directory track hole (default split files).\n");
    printf("-F            Next file first sector on a new track (default=0).\n");
    printf("              Any negative value assumes aligned tracks and uses current\n");
    printf("              sector + interleave. After each file, the value falls back to the\n");
    printf("              default. Not applicable for D81.\n");
    printf("-S value      Default sector interleave, default=10.\n");
    printf("              At track end, reduces this by 1 to accommodate large tail gap.\n");
    printf("              If negative, no special treatment of tail gap. Not applicable for\n");
    printf("              D81.\n");
    printf("-s value      Next file sector interleave, valid after each file.\n");
    printf("              At track end, reduces this by 1 to accommodate large tail gap.\n");
    printf("              If negative, no special treatment of tail gap.\n");
    printf("              The interleave value falls back to the default value set by -S\n");
    printf("              after the first sector of the next file. Not applicable for D81.\n");
    printf("-e            Start next file on an empty track (default start sector is\n");
    printf("              current sector plus interleave).\n");
    printf("-E            Try to fit file on a single track.\n");
    printf("-r track      Restrict next file blocks to the specified track or higher.\n");
    printf("-b sector     Set next file beginning sector to the specified value.\n");
    printf("              Not applicable for D81.\n");
    printf("-c            Save next file cluster-optimized (d71 only).\n");
    printf("-4            Use tracks 35-40 with SPEED DOS BAM formatting.\n");
    printf("-5            Use tracks 35-40 with DOLPHIN DOS BAM formatting.\n");
    printf("-R level      Try to restore deleted and formatted files.\n");
    printf("              level 0: Only restore dir entries without touching any t/s links.\n");
    printf("              level 1: Fix dir entries for files with valid t/s chains.\n");
    printf("              level 2: Also add wild sector chains with valid t/s chains.\n");
    printf("              level 3: Also fix dir entries with invalid t/s chains.\n");
    printf("              level 4: Also add and fix wild invalid t/s chains.\n");
    printf("              level 5: Also add reasonable wild single blocks.\n");
    printf("-g filename   Write additional g64 output file with given name.\n");
    printf("-a            Print command line options that would create the same directory as\n");
    printf("              the one in the given image (for directory art import).\n");
    printf("-U mapping    Print PETSCII as Unicode (requires Unicode 13.0 font, e.g.\n");
    printf("              UNSCII). Use mapping 0 for ASCII output, 1 for upper case, 2 for\n");
    printf("              lower case, default is 0.\n");
    printf("-q            Be quiet.\n");
    printf("-v            Be verbose.\n");
    printf("-h            Print this command line help.\n");
    printf("\n");

    exit(-1);
}

/* Returns a pointer to the filename in a path */
static const unsigned char*
basename(const unsigned char* path)
{
    const unsigned char* name;
    (name = (unsigned char*)strrchr((char *)path, FILESEPARATOR)) ? ++name : (name = path);
    return name;
}

/* Calculates a hash from a filename to be used by Krill's Loader */
static unsigned int
filenamehash(const unsigned char *filename)
{
    int pos = min(max_hash_length, (int)strlen((char *)filename));
    while ((pos > 0) && (((unsigned char) filename[pos - 1]) == FILENAMEEMPTYCHAR)) {
        --pos;
    }

    unsigned char hashlo = pos;
    unsigned char hashhi = pos;
    int carry = 1;

    for (int i = pos - 1; i >= 0; --i) {
        unsigned int sum = hashlo + filename[i] + carry;
        carry = (sum >= 256) ? 1 : 0;
        sum &= 0xff;
        hashlo = sum;
        sum += hashhi + carry;
        carry = (sum >= 256) ? 1 : 0;
        hashhi = sum;
    }

    return (hashhi << 8) | hashlo;
}

/* Returns the size of an image in bytes */
static unsigned int
image_size(image_type type)
{
    switch (type) {
    case IMAGE_D64:
        return D64SIZE;

    case IMAGE_D64_EXTENDED_SPEED_DOS:
    /* fall through */
    case IMAGE_D64_EXTENDED_DOLPHIN_DOS:
        return D64SIZE_EXTENDED;

    case IMAGE_D71:
        return D71SIZE;

    case IMAGE_D81:
        return D81SIZE;

    default:
        return 0;
    }
}

/* Returns the number of tracks in an image */
static unsigned int
image_num_tracks(image_type type)
{
    switch (type) {
    case IMAGE_D64:
        return D64NUMTRACKS;

    case IMAGE_D64_EXTENDED_SPEED_DOS:
    /* fall through */
    case IMAGE_D64_EXTENDED_DOLPHIN_DOS:
        return D64NUMTRACKS_EXTENDED;

    case IMAGE_D71:
        return D71NUMTRACKS;

    case IMAGE_D81:
        return D81NUMTRACKS;

    default:
        return 0;
    }
}

/* Returns the number of sectors for a given track */
static int
num_sectors(image_type type, int track)
{
    return (type == IMAGE_D81) ? SECTORSPERTRACK_D81
           : (((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) ? sectors_per_track_extended[track - 1]
              : sectors_per_track[track - 1]);
}

/* Returns the number of blocks in an image */
static unsigned int
image_num_blocks(image_type type)
{
    int num_blocks = 0;
    for (unsigned int t = 1; t <= image_num_tracks(type); t++) {
        num_blocks += num_sectors(type, t);
    }
    return num_blocks;
}

/* Returns the directory track of an image */
static int
dirtrack(image_type type)
{
    return (type == IMAGE_D81) ? DIRTRACK_D81 : DIRTRACK_D41_D71;
}

/* Converts an ASCII character to PETSCII */
static unsigned char
a2p(unsigned char a)
{
    switch (a) {
    case '\n':
        return 0x0d;
    case '_':
        return 0xa4;
    case 0x7e:
        return 0xff;
    default:
        if ((a >= 0x5b) && (a < 0x5f)) {
            return a;
        }
        if ((a >= 0x60) && (a <= 0x7e)) {
            return a ^ 0x20;
        }
        if ((a >= 'A') && (a <= 'Z')) {
            return a | 0x80;
        }
        return a;
    }
}

/* Converts a PETSCII character to ASCII */
static unsigned char
p2a(unsigned char p)
{
    switch (p) {
    case 0x0a:
    case 0x0d:
        return '\n';
    case 0x40:
    case 0x60:
        return p;
    case 0xa0:
    case 0xe0:
        return ' ';
    case 0xa4:
    case 0xe4:
        return '_';
    default:
        switch (p & 0xe0) {
        case 0x40:
        case 0x60:
            p ^= 0x20;
            break;
        case 0xc0:
            p ^= 0x80;
            break;
        }
    }
    return ((isprint(p) ? p : '.'));
}

/* Converts an ASCII string to PETSCII filled up with Shift-Space to length */
static void
ascii2petscii(const unsigned char* ascii, unsigned char* petscii, int len)
{
    int pos = 0;
    while (ascii[pos] != '\0' && pos < len) {
        petscii[pos] = a2p(ascii[pos]);
        ++pos;
    }
    while (pos < len) {
        petscii[pos] = FILENAMEEMPTYCHAR;
        ++pos;
    }
}

/* Prints a PETSCII filename as ASCII with escapes */
static void
print_filename_with_escapes(const unsigned char* petscii, int len)
{
    /* find end of petscii string */
    while(len > 1 && petscii[len-1] == 0xa0) {
        len--;
    }
    for(int ppos = 0; ppos < len; ppos++) {
        unsigned char c = petscii[ppos];
        if((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) || c == 32) {
            switch (c & 0xe0) {
            case 0x40:
            case 0x60:
                putchar(c ^ 0x20);
                break;
            default:
                putchar(c);
            }
        } else {
            printf("#%02x", c);
        }
    }
}

/* Determines length of 0xa0 terminated PETSCII string */
static int
pstrlen(const unsigned char* string)
{
    int len = 0;
    while(string[len] != 0xa0 && len < FILENAMEMAXSIZE) {
        len++;
    }
    return len;
}

/* Writes digit decimal number into PETSCII string */
#define PPUTNUM_BUF_LEN 17
static void
pputnum(unsigned char* string, unsigned int num)
{
    char buffer[PPUTNUM_BUF_LEN];
    snprintf(buffer, PPUTNUM_BUF_LEN, "%d", num);
    for(unsigned int len = 0; len < strlen(buffer); len++) {
        string[len] = a2p(buffer[len]);
    }
}

/* Writes 2 digit decimal number into PETSCII string */
#define PPUTNUM2_BUF_LEN 3
static void
pputnum2(unsigned char* string, unsigned int num)
{
    char buffer[PPUTNUM2_BUF_LEN];
    snprintf(buffer, PPUTNUM2_BUF_LEN, "%02d", num);
    for(unsigned int len = 0; len < strlen(buffer); len++) {
        string[len] = a2p(buffer[len]);
    }
}

/* Writes 4 digit hex number into PETSCII string */
#define PPUTHEX_BUF_LEN 5
static void
pputhex(unsigned char* string, unsigned int num)
{
    char buffer[PPUTHEX_BUF_LEN];
    snprintf(buffer, PPUTHEX_BUF_LEN, "%04x", num);
    for(int len = 0; len < 4; len++) {
        string[len] = a2p(buffer[len]);
    }
}

/* Converts a two digit hex string to an int */
static unsigned int
hex2int(char hex)
{
    if ((hex < '0' || hex > '9') && (hex < 'a' || hex > 'f')) {
        fprintf(stderr, "ERROR: Invalid hex string in filename\n");

        exit(-1);
    }
    if (hex <= '9') {
        hex -= '0';
    } else {
        hex -= 'a' - 10;
    }
    return (unsigned int)hex;
}

/* Converts an ASCII string to PETSCII with escape evaluation filled up with emptychar to length */
static void
evalhexescape(const unsigned char* ascii, unsigned char* petscii, int len, unsigned char emptychar)
{
    int read = 0, write = 0;

    while (ascii[read] != '\0' && write < len) {
        if (ascii[read] == '#') {
            unsigned int hi = hex2int(ascii[++read]);
            unsigned int lo = hex2int(ascii[++read]);
            petscii[write] = (unsigned char)(16 * hi + lo);
        } else {
            petscii[write] = a2p(ascii[read]);
        }
        read++;
        write++;
    }
    while (write < len) {
        petscii[write] = emptychar;
        ++write;
    }
}

/* Converts a unicode character to utf8 */
char* utf8_encode(int c, char* out)
{
    if (c < 0x80)
        *out++ =(c & 0xff);
    else if (c < 0x800) {
        *out++ = (0xC0 | ((c >> 6) & 0x1f));
        *out++ = (0x80 | (c & 0x3f));
    } else if (c < 0x10000) {
        *out++ = (0xE0 | ((c >> 12) & 0xf));
        *out++ = (0x80 | ((c >> 6) & 0x3f));
        *out++ = (0x80 | (c & 0x3f));
    } else {
        *out++ = (0xF0 | ((c >> 18) & 0x07));
        *out++ = (0x80 | ((c >> 12) & 0x3f));
        *out++ = (0x80 | ((c >> 6) & 0x3f));
        *out++ = (0x80 | (c & 0x3f));
    }
    return out;
}

#ifdef _WIN32
/* Enables console formatting under Windows if possible */
static bool
EnableVTMode()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return false;
    }
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return false;
    }
    dwMode |= 0x0004; /* ENABLE_VIRTUAL_TERMINAL_PROCESSING not defined for older SDKs */
    if (!SetConsoleMode(hOut, dwMode)) {
        return false;
    }
    return true;
}
#endif

/* Enables reverse printing to console */
void reverse_print_on()
{
#ifdef _WIN32
    /* Avoid escape values for inverse printing under Windows if they are not supported by the console */
    if (EnableVTMode()) {
        printf("\033[7m");
    }
#else
    printf("\033[7m");
#endif
}

/* Disables reverse printing to console */
void reverse_print_off()
{
#ifdef _WIN32
    /* Avoid escape values for inverse printing under Windows if they are not supported by the console */
    if (EnableVTMode()) {
        printf("\033[m");
    }
#else
    printf("\033[m");
#endif
}

/* Prints a PETSCII character */
static void
putp(unsigned char petscii, FILE *file)
{
    if (unicode) {
        int u;
        int reverse = 0;

        if (petscii < 0x20 || (petscii >= 0x80 && petscii <= 0x9f)) {
            reverse = 1;
            reverse_print_on();
            petscii += 0x40;
        }

        if (unicode == 1) {
            u = p2u_uppercase_tab[petscii];
        } else {
            u = p2u_lowercase_tab[petscii];
        }

#ifdef _WIN32
        _setmode(_fileno(file), _O_U16TEXT);
        putwc(u, file);
        _setmode(_fileno(file), _O_TEXT);
#else
        {
            char temp[5];
            *utf8_encode(u, temp) = 0; /* fancy way to zero-terminate temp after conversion */
            fputs(temp, file);
        }
#endif

        if(reverse) {
            reverse_print_off();
        }
    } else {
        putc(p2a(petscii), file);
    }
}

/* Prints a PETSCII string */
static void
print_petscii(unsigned char *petscii, int len)
{
    for(int i = 0; i < len; i++) {
        putp(petscii[i], stdout);
    }
}

/* Prints the given PETSCII filename like the C64 when listing the directory */
static void
print_dirfilename(unsigned char* pfilename)
{
    int ended = 0;
    putc('\"', stdout);
    for (int pos = 0; pos < FILENAMEMAXSIZE; pos++) {
        unsigned char c = pfilename[pos];
        if (c == FILENAMEEMPTYCHAR) {
            if (!ended) {
                putc('\"', stdout);
                ended = 1;
            } else {
                putc(' ', stdout);
            }
        } else {
            putp(c, stdout);
        }
    }
    if (!ended) {
        putc('\"', stdout);
    } else {
        putc(' ', stdout);
    }
}

/* Prints the given PETSCII filename */
static void
print_filename(FILE *file, unsigned char* pfilename)
{
    putc('\"', file);
    for (int pos = 0; pos < FILENAMEMAXSIZE; pos++) {
        if (pfilename[pos] == FILENAMEEMPTYCHAR) {
            break;
        }
        putp(pfilename[pos], file);
    }
    putc('\"', file);
}

/* Calculates the overall sector index from a given track and sector, returns -1 for invalid track/sector */
static int
linear_sector(image_type type, int track, int sector)
{
    if ((track < 1) || (track > ((type == IMAGE_D81) ? D81NUMTRACKS : ((type == IMAGE_D64) ? D64NUMTRACKS : (type == IMAGE_D71 ? D71NUMTRACKS : D64NUMTRACKS_EXTENDED))))) {
        return -1;
    }

    int numsectors = num_sectors(type, track);
    if ((sector < 0) || (sector >= numsectors)) {
        return -1;
    }

    int linear_sector = 0;
    for (int i = 0; i < track - 1; i++) {
        linear_sector += num_sectors(type, i + 1);
    }
    linear_sector += sector;

    return linear_sector;
}

/* Returns the image offset of the bam entry for a given track */
static int
get_bam_offset(image_type type, unsigned int track)
{
    int bam;
    unsigned int offset;

    if (type == IMAGE_D81) {
        if (track <= 40) {
            bam = linear_sector(type, dirtrack(type), 1 /* sector */) * BLOCKSIZE;
            offset = bam + (track * 6) + 11;
        } else {
            bam = linear_sector(type, dirtrack(type), 2 /* sector */) * BLOCKSIZE;
            offset = bam + ((track - 40) * 6) + 11;
        }
    } else if ((type == IMAGE_D71) && (track > D64NUMTRACKS)) {
        /* access second side bam */
        bam = linear_sector(type, dirtrack(type) + D64NUMTRACKS, 0) * BLOCKSIZE;
        offset = bam + (track - D64NUMTRACKS - 1) * 3;
    } else {
        if (((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) && (track > D64NUMTRACKS)) {
            track -= D64NUMTRACKS;
            bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE + ((type == IMAGE_D64_EXTENDED_SPEED_DOS) ? BAM_OFFSET_SPEED_DOS : BAM_OFFSET_DOLPHIN_DOS);
        } else {
            bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE;
        }
        offset = bam + track * 4 + 1;
    }
    return offset;
}

/* Checks if a given sector is marked as free in the BAM and also not used by directory */
static int
is_sector_free(image_type type, const unsigned char* image, int track, int sector, int numdirblocks, int dir_sector_interleave)
{
    int bam;
    const unsigned char* bitmap;

    if (sector < 0) {
        fprintf(stderr, "ERROR: Illegal sector %d for track %d\n", sector, track);

        exit(-1);
    }

    if (type == IMAGE_D81) {
        if (track <= 40) {
            bam = linear_sector(type, dirtrack(type), 1 /* sector */) * BLOCKSIZE;
            bitmap = image + bam + (track * 6) + 11;
        } else {
            bam = linear_sector(type, dirtrack(type), 2 /* sector */) * BLOCKSIZE;
            bitmap = image + bam + ((track - 40) * 6) + 11;
        }
    } else if ((type == IMAGE_D71) && (track > D64NUMTRACKS)) {
        /* access second side bam */
        bam = linear_sector(type, dirtrack(type) + D64NUMTRACKS, 0) * BLOCKSIZE;
        bitmap = image + bam + (track - D64NUMTRACKS - 1) * 3;
    } else {
        if (((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) && (track > D64NUMTRACKS)) {
            track -= D64NUMTRACKS;
            bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE + ((type == IMAGE_D64_EXTENDED_SPEED_DOS) ? BAM_OFFSET_SPEED_DOS : BAM_OFFSET_DOLPHIN_DOS);
        } else {
            bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE;
        }
        bitmap = image + bam + (track * 4) + 1;
    }

    int byte = sector >> 3;
    int bit = sector & 7;

    int is_not_dir_block = 1;
    if ((track == dirtrack(type)) && (numdirblocks > 0)) {
        int dirsector = 0;
        int s = 2;
        for (int i = 0; is_not_dir_block && (i < numdirblocks); i++) {
            switch (i) {
            case 0:
                dirsector = 0;
                break;

            case 1:
                dirsector = 1;
                break;

            default:
                dirsector += dir_sector_interleave;
                if (dirsector >= num_sectors(type, track)) {
                    dirsector = s;
                    s++;
                }
                break;
            }
            is_not_dir_block = (sector != dirsector);
        }
    }

    return is_not_dir_block && ((bitmap[byte] & (1 << bit)) != 0);
}

/* Marks given sector in BAM */
static void
mark_sector(image_type type, unsigned char* image, int track, int sector, int free)
{
    if (free != is_sector_free(type, image, track, sector, 0, 0)) {
        int bam;
        unsigned char* bitmap;
        if (type == IMAGE_D81) {
            if (track <= 40) {
                bam = linear_sector(type, dirtrack(type), 1 /* sector */) * BLOCKSIZE;
                bitmap = image + bam + (track * 6) + 11;
            } else {
                bam = linear_sector(type, dirtrack(type), 2 /* sector */) * BLOCKSIZE;
                bitmap = image + bam + ((track - 40) * 6) + 11;
            }

            /* update number of free sectors on track */
            if (free) {
                ++bitmap[-1];
            } else {
                --bitmap[-1];
            }
        } else if ((type == IMAGE_D71) && (track > D64NUMTRACKS)) {
            /* access second side bam */
            bam = linear_sector(type, dirtrack(type) + D64NUMTRACKS, 0) * BLOCKSIZE;
            bitmap = image + bam + (track - D64NUMTRACKS - 1) * 3;

            /* update number of free sectors on track */
            if (free) {
                image[bam + 0xdd + track - D64NUMTRACKS - 1]++;
            } else {
                image[bam + 0xdd + track - D64NUMTRACKS - 1]--;
            }
        } else {
            if (((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) && (track > D64NUMTRACKS)) {
                track -= D64NUMTRACKS;
                bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE + ((type == IMAGE_D64_EXTENDED_SPEED_DOS) ? BAM_OFFSET_SPEED_DOS : BAM_OFFSET_DOLPHIN_DOS);
            } else {
                bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE;
            }
            bitmap = image + bam + (track * 4) + 1;

            /* update number of free sectors on track */
            if (free) {
                ++image[bam + (track * 4)];
            } else {
                --image[bam + (track * 4)];
            }
        }

        /* update bitmap */
        int byte = sector >> 3;
        int bit = sector & 7;

        if (free) {
            bitmap[byte] |= 1 << bit;
        } else {
            bitmap[byte] &= ~(1 << bit);
        }
    }
}

/* Returns offset for header on directory track */
static int
get_header_offset(image_type type)
{
    int offset;

    if (type == IMAGE_D81) {
        offset = 4;
    } else {
        offset = 0x90;
    }
    return offset;
}

/* Returns offset for id on directory track */
static int
get_id_offset(image_type type)
{
    int offset;

    if (type == IMAGE_D81) {
        offset = 0x16;
    } else {
        offset = 0xa2;
    }
    return offset;
}

/* Updates the directory with the given header and id */
static void
update_directory(image_type type, unsigned char* image, unsigned char* header, unsigned char* id, int shadowdirtrack)
{
    unsigned int bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE;

    if (type != IMAGE_D81) {
        image[bam + 0x03] = (type == IMAGE_D71) ? 0x80 : 0x00;
    }

    /* Set header and ID */
    unsigned char pheader[FILENAMEMAXSIZE];
    unsigned char pid[5];
    evalhexescape(header, pheader, FILENAMEMAXSIZE, FILENAMEEMPTYCHAR);
    evalhexescape(id, pid, 5, FILENAMEEMPTYCHAR);
    memcpy(image + bam + get_header_offset(type), pheader, FILENAMEMAXSIZE);
    memcpy(image + bam + get_id_offset(type), pid, 5);

    if (type == IMAGE_D81) {
        unsigned int bam = linear_sector(type, dirtrack(type), 1 /* sector */) * BLOCKSIZE;
        image[bam + 0x04] = id[0];
        image[bam + 0x05] = id[1];

        bam = linear_sector(type, dirtrack(type), 2 /* sector */) * BLOCKSIZE;
        image[bam + 0x04] = id[0];
        image[bam + 0x05] = id[1];
    }

    if (shadowdirtrack > 0) {
        unsigned int shadowbam = linear_sector(type, shadowdirtrack, 0 /* sector */) * BLOCKSIZE;
        memcpy(image + shadowbam, image + bam, BLOCKSIZE);

        image[shadowbam + 0x00] = shadowdirtrack;
    }
}

/* Writes an empty directory and BAM */
static void
initialize_directory(image_type type, unsigned char* image, unsigned char* header, unsigned char* id, int shadowdirtrack)
{
    unsigned int dir = linear_sector(type, dirtrack(type), 0 /* sector */) * BLOCKSIZE;

    /* Clear image */
    memset(image, 0, image_size(type));

    /* Write initial BAM */
    if (type == IMAGE_D81) {
        image[dir + 0x00] = dirtrack(type);
        image[dir + 0x01] = 3;
        image[dir + 0x02] = 0x44;

        image[dir + 0x14] = FILENAMEEMPTYCHAR;
        image[dir + 0x15] = FILENAMEEMPTYCHAR;

        image[dir + 0x1b] = FILENAMEEMPTYCHAR;
        image[dir + 0x1c] = FILENAMEEMPTYCHAR;

        unsigned int bam = linear_sector(type, dirtrack(type), 1 /* sector */) * BLOCKSIZE;
        image[bam + 0x00] = dirtrack(type);
        image[bam + 0x01] = 2;
        image[bam + 0x02] = 0x44;
        image[bam + 0x03] = 0xbb;
        image[bam + 0x06] = 0xc0;

        bam = linear_sector(type, dirtrack(type), 2 /* sector */) * BLOCKSIZE;
        image[bam + 0x00] = 0;
        image[bam + 0x01] = 255;
        image[bam + 0x02] = 0x44;
        image[bam + 0x03] = 0xbb;
        image[bam + 0x06] = 0xc0;
    } else {
        image[dir + 0x00] = dirtrack(type);
        image[dir + 0x01] = 1;
        image[dir + 0x02] = 0x41;
        image[dir + 0x03] = (type == IMAGE_D71) ? 0x80 : 0x00;

        image[dir + 0xa0] = FILENAMEEMPTYCHAR;
        image[dir + 0xa1] = FILENAMEEMPTYCHAR;

        image[dir + 0xa7] = FILENAMEEMPTYCHAR;
        image[dir + 0xa8] = FILENAMEEMPTYCHAR;
        image[dir + 0xa9] = FILENAMEEMPTYCHAR;
        image[dir + 0xaa] = FILENAMEEMPTYCHAR;
    }

    /* Mark all sectors unused */
    for (unsigned int t = 1; t <= image_num_tracks(type); t++) {
        for (int s = 0; s < num_sectors(type, t); s++) {
            mark_sector(type, image, t, s, 1 /* free */);
        }
    }

    /* Reserve space for BAM */
    mark_sector(type, image, dirtrack(type), 0 /* sector */, 0 /* not free */);
    if (type == IMAGE_D71) {
        mark_sector(type, image, dirtrack(type) + D64NUMTRACKS, 0 /* sector */, 0 /* not free */);
    } else if (type == IMAGE_D81) {
        mark_sector(type, image, dirtrack(type), 1 /* sector */, 0 /* not free */);
        mark_sector(type, image, dirtrack(type), 2 /* sector */, 0 /* not free */);
    }

    /* first dir block */
    unsigned int dirblock = linear_sector(type, dirtrack(type), (type == IMAGE_D81) ? 3 : 1) * BLOCKSIZE;
    image[dirblock + SECTORLINKOFFSET] = 255;
    mark_sector(type, image, dirtrack(type), (type == IMAGE_D81) ? 3 : 1 /* sector */, 0 /* not free */);

    if (shadowdirtrack > 0) {
        dirblock = linear_sector(type, shadowdirtrack, (type == IMAGE_D81) ? 3 : 1 /* sector */) * BLOCKSIZE;
        image[dirblock + SECTORLINKOFFSET] = 255;

        mark_sector(type, image, shadowdirtrack, 0 /* sector */, 0 /* not free */);
        mark_sector(type, image, shadowdirtrack, (type == IMAGE_D81) ? 3 : 1 /* sector */, 0 /* not free */);
    }

    update_directory(type, image, header, id, shadowdirtrack);
}

/* Computes Transwarp dirdata checksum */
static unsigned char
transwarp_dirdata_checksum(const unsigned char *image, int dir_entry_offset)
{
    int dirdata_checksum = 0;
    int carry = 1;
    for (int offset = DIRDATACHECKSUMOFFSET; offset <= FILEBLOCKSLOOFFSET; ++offset) {
        dirdata_checksum += (image[dir_entry_offset + offset] + carry);
        carry = (dirdata_checksum >= 0x0100) ? 1 : 0;
        dirdata_checksum &= 0xff;
    }

    return dirdata_checksum;
}

/* Checks if a given dir entry points to a Transwarp file */
static bool
is_transwarp_file(const unsigned char *image, int dir_entry_offset)
{
    return (image[dir_entry_offset + TRANSWARPSIGNATROFFSLO] == TRANSWARPSIGNATURELO)
           && (image[dir_entry_offset + TRANSWARPSIGNATROFFSHI] == TRANSWARPSIGNATUREHI)
           && (transwarp_dirdata_checksum(image, dir_entry_offset) == 0);
}

/* Checks if a given dir entry points to the Transwarp bootfile */
static bool
is_transwarp_bootfile(const unsigned char *image, int dir_entry_offset)
{
    return memcmp(image + dir_entry_offset + FILENAMEOFFSET, TRANSWARP, strlen(TRANSWARP)) == 0;
}

/* Return Transwarp file stat */
static int
transwarp_stat(image_type type, const unsigned char *image, int dir_entry_offset, int *start_track, int *end_track, int *low_track, int *high_track)
{
    *start_track = 0;
    *end_track = 0;
    *low_track = 0;
    *high_track = 0;

    int filesize = (image[dir_entry_offset + ENDADDRESSLOOFFSET]  | (image[dir_entry_offset + ENDADDRESSHIOFFSET] << 8))
                   - (image[dir_entry_offset + LOADADDRESSLOOFFSET] | (image[dir_entry_offset + LOADADDRESSHIOFFSET] << 8));
    if (filesize <= 0) {
        return 0;
    }

    *start_track = image[dir_entry_offset + TRANSWARPTRACKOFFSET];
    *end_track = *start_track;

    int size = filesize;

    while (filesize > 0) {
        filesize -= (TRANSWARPBLOCKSIZE * num_sectors(type, *end_track));
        if (filesize > 0) {
            *end_track = (*end_track < DIRTRACK_D41_D71) ? (*end_track - 1) : (*end_track + 1);
        }
    }

    *low_track = (*start_track < *end_track) ? *start_track : *end_track;
    *high_track = (*start_track > *end_track) ? *start_track : *end_track;

    return size;
}

/* Return Transwarp file stat */
static int
transwarp_size(image_type type, int start_track, int end_track, int filesize,
               int *transwarp_blocks, int *nonredundant_blocks, int *redundant_blocks, int *nonredundant_blocks_on_last_track)
{
    *transwarp_blocks = 0;

    int last_track_sectors;
    if (start_track <= end_track) {
        for (int track = start_track; track <= end_track; ++track) {
            last_track_sectors = num_sectors(type, track);
            *transwarp_blocks += last_track_sectors;
        }
    } else {
        for (int track = start_track; track >= end_track; --track) {
            last_track_sectors = num_sectors(type, track);
            *transwarp_blocks += last_track_sectors;
        }
    }

    int spare_bytes = TRANSWARPBLOCKSIZE - (filesize % TRANSWARPBLOCKSIZE);
    if (spare_bytes == TRANSWARPBLOCKSIZE) {
        spare_bytes = 0;
    }

    *nonredundant_blocks = (filesize / TRANSWARPBLOCKSIZE) + ((spare_bytes == 0) ? 0 : 1);
    *redundant_blocks = *transwarp_blocks - *nonredundant_blocks;
    *nonredundant_blocks_on_last_track = last_track_sectors - *redundant_blocks;

    return spare_bytes;
}

/* Deletes a file from disk and BAM, but leaves the directory entry */
static void
wipe_file(image_type type, unsigned char* image, imagefile* file)
{
    int b = linear_sector(type, dirtrack(type), file->direntrysector) * BLOCKSIZE + file->direntryoffset;

    if (is_transwarp_file(image, b)) {
        int start_track;
        int end_track;
        int low_track;
        int high_track;
        int filesize = transwarp_stat(type, image, b, &start_track, &end_track, &low_track, &high_track);
        if (filesize <= 0) {
            fprintf(stderr, "ERROR: Cannot overwrite Transwarp file ");
            print_filename(stderr, file->pfilename);
            fprintf(stderr, "\n");

            exit(-1);
        }

        for (int track = low_track; track <= high_track; ++track) {
            for (int sector = 0; sector < num_sectors(type, track); ++sector) {
                int block_offset = linear_sector(type, track, sector) * BLOCKSIZE;
                memset(image + block_offset, 0, BLOCKSIZE);
                mark_sector(type, image, track, sector, 1 /* free */);
            }
        }

        return;
    }

    unsigned int track = image[b + FILETRACKOFFSET];
    unsigned int sector = image[b + FILESECTOROFFSET];

    if (sector >= 0x80) {
        return; /* loop file */
    }

    while (track != 0) {
        int block_offset = linear_sector(type, track, sector) * BLOCKSIZE;
        int next_track = image[block_offset + TRACKLINKOFFSET];
        int next_sector = image[block_offset + SECTORLINKOFFSET];
        memset(image + block_offset, 0, BLOCKSIZE); /* this also fixes any cyclic t/s chain */
        mark_sector(type, image, track, sector, 1 /* free */);
        track = next_track;
        sector = next_sector;
    }
}

/* Sets image offset to the next DIR entry, returns false when the DIR end was reached */
static bool
next_dir_entry(image_type type, const unsigned char* image, int *track, int *sector, int *offset, char *blockmap)
{
    int b = linear_sector(type, *track, *sector); /* assuming here that the given t/s is valid */
    blockmap[b] = 1;
    if (*offset % BLOCKSIZE == 7 * DIRENTRYSIZE) {
        /* last entry in sector */
        int next_track = image[b * BLOCKSIZE + TRACKLINKOFFSET];
        if (next_track == 0) {
            /* this was the last DIR sector */
            return false;
        }
        int next_sector = image[b * BLOCKSIZE + SECTORLINKOFFSET];
        b = linear_sector(type, next_track, next_sector);
        if(b < 0) {
            dir_error = DIR_ILLEGAL_TS;
            return false;
        }
        if(blockmap[b]) {
            dir_error = DIR_CYCLIC_TS;
            return false;
        }
        *track = next_track;
        *sector = next_sector;
        *offset = 0;
    } else {
        *offset += DIRENTRYSIZE;
    }
    return true;
}

/* Finds all filenames with the given hash */
static int
count_hashes(image_type type, const unsigned char* image, unsigned int hash, bool print)
{
    int num = 0;
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }

    int ds = (type == IMAGE_D81) ? 3 : 1;
    int dt = dirtrack(type);
    int offset = 0;
    do {
        int dirblock = linear_sector(type, dt, ds) * BLOCKSIZE + offset;
        int filetype = image[dirblock + FILETYPEOFFSET];

        if (filetype != FILETYPEDEL) {
            unsigned char *filename = (unsigned char *) image + dirblock + FILENAMEOFFSET;
            if (hash == filenamehash(filename)) {
                ++num;

                if (print) {
                    printf(" [$%04x] ", filenamehash(filename));
                    print_filename(stdout, filename);
                    printf("\n");
                }
            }
        }
    } while (next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);

    return num;
}

/* Checks if multiple filenames have the same hash */
static bool
check_hashes(image_type type, const unsigned char* image)
{
    bool collision = false;
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }
    printf("\n");

    int ds = (type == IMAGE_D81) ? 3 : 1;
    int dt = dirtrack(type);
    int offset = 0;
    do {
        int dirblock = linear_sector(type, dt, ds) * BLOCKSIZE + offset;
        int filetype = image[dirblock + FILETYPEOFFSET];

        if (filetype != FILETYPEDEL) {
            unsigned char *filename = (unsigned char *) image + dirblock + FILENAMEOFFSET;
            if (count_hashes(type, image, filenamehash(filename), false /* print */) > 1) {
                collision = 1;
                fprintf(stderr, "Hash of filename ");
                print_filename(stderr, filename);
                fprintf(stderr, " [$%04x] is not unique\n", filenamehash(filename));
                count_hashes(type, image, filenamehash(filename), true /* print */);
            }
        }
    } while (next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);
    return collision;
}

/* Searches for an existing DIR entry with the given name, returns false if it does not exist */
static bool
find_existing_file(image_type type, unsigned char* image, unsigned char* filename, int *index, int *track, int *sector, int *offset)
{
    *track = dirtrack(type);
    *sector = (type == IMAGE_D81) ? 3 : 1;
    *offset = 0;
    *index = 0;
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }

    do {
        int b = linear_sector(type, *track, *sector) * BLOCKSIZE + *offset;
        int filetype = image[b + FILETYPEOFFSET];
        if(filetype != 0 && memcmp(image + b + FILENAMEOFFSET, filename, FILENAMEMAXSIZE) == 0) {
            return true;
        }
        ++(*index);
    } while (next_dir_entry(type, image, track, sector, offset, blockmap));

    free(blockmap);
    return false;
}

/* Returns an empty DIR slot, allocates a new DIR sector if required */
static void
new_dir_slot(image_type type, unsigned char* image, int dir_sector_interleave, int shadowdirtrack, int *index, int *dirsector,  int *entry_offset, imagefile files[])
{
    int track = dirtrack(type);
    *dirsector = (type == IMAGE_D81) ? 3 : 1;
    *entry_offset = 0;
    int lindex = 0;

    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }

    do {
        int b = linear_sector(type, track, *dirsector) * BLOCKSIZE + *entry_offset;
        if (image[b + FILETYPEOFFSET] == FILETYPEDEL) {
            /* Check if the apparently free slot is used by a new filetype 0 file */
            bool used = false;
            for(int f = 0; f < num_files; f++) {
                if(files[f].direntryindex == lindex) {
                    used = true;
                    break;
                }
            }
            if(!used) {
                *index = lindex;
                free(blockmap);
                return; /* found an empty slot */
            }
        }
        ++lindex;
    } while (next_dir_entry(type, image, &track, dirsector, entry_offset, blockmap));
    free(blockmap);

    /* allocate new dir block */
    int last_sector = *dirsector;
    int next_sector = -1;
    for (int s = 1; s < num_sectors(type, dirtrack(type)); s++) {
        int sector = (last_sector + s * dir_sector_interleave) % num_sectors(type, dirtrack(type));
        if (is_sector_free(type, image, dirtrack(type), sector, 0, 0)) {
            next_sector = sector;
            break;
        }
    }
    if (next_sector == -1) {
        fprintf(stderr, "ERROR: Dir track full\n");
        exit(-1);
    }
    int b = linear_sector(type, dirtrack(type), last_sector) * BLOCKSIZE;
    image[b + TRACKLINKOFFSET] = dirtrack(type);
    image[b + SECTORLINKOFFSET] = next_sector;

    mark_sector(type, image, dirtrack(type), next_sector, 0 /* not free */);
    b = linear_sector(type, dirtrack(type), next_sector) * BLOCKSIZE;
    memset(image + b, 0, BLOCKSIZE);
    image[b + SECTORLINKOFFSET] = 255;
    *dirsector = next_sector;
    *entry_offset = 0;

    if (shadowdirtrack > 0) {
        b = linear_sector(type, shadowdirtrack, last_sector) * BLOCKSIZE;
        image[b + TRACKLINKOFFSET] = shadowdirtrack;
        image[b + SECTORLINKOFFSET] = next_sector;
        mark_sector(type, image, shadowdirtrack, next_sector, 0 /* not free */);

        b = linear_sector(type, shadowdirtrack, next_sector) * BLOCKSIZE;
        memset(image + b, 0, BLOCKSIZE);
        image[b + SECTORLINKOFFSET] = 255;
    }
    *index = lindex;
}

/* Returns suitable index and offset for given filename (either existing slot when overwriting, first free slot or slot in newly allocated segment) */
static bool
find_dir_slot(image_type type, unsigned char* image, unsigned char* filename, int dir_sector_interleave, int shadowdirtrack, int *index, int *dirsector,  int *entry_offset, imagefile files[])
{
    int track;
    if(find_existing_file(type, image, filename, index, &track, dirsector, entry_offset)) {
        return true;
    }
    new_dir_slot(type, image, dir_sector_interleave, shadowdirtrack, index, dirsector, entry_offset, files);
    return false;
}

/* Adds the specified new entries to the directory */
static void
create_dir_entries(image_type type, unsigned char* image, imagefile* files, int num_files, int dir_sector_interleave, unsigned int shadowdirtrack, int nooverwrite)
{
    /* this does not check for uniqueness of filenames */

    int num_overwritten_files = 0;

    if (verbose && num_files > 0) {
        printf("\nCreating dir entries:\n");
    }

    for (int i = 0; i < num_files; i++) {
        /* find or create slot */
        imagefile *file = files + i;

        if (verbose) {
            printf("  ");
            print_dirfilename(file->pfilename);
        }

        if(file->force_new) {
            new_dir_slot(type, image, dir_sector_interleave, shadowdirtrack, &file->direntryindex, &file->direntrysector, &file->direntryoffset, files);
        } else if (find_dir_slot(type, image, file->pfilename, dir_sector_interleave, shadowdirtrack, &file->direntryindex, &file->direntrysector, &file->direntryoffset, files)) {
            if (nooverwrite) {
                fprintf(stderr, "ERROR: Filename exists on disk image already and -o was set\n");
                exit(-1);
            }

            wipe_file(type, image, file);
            num_overwritten_files++;
        }

        int file_entry_offset = linear_sector(type, dirtrack(type), file->direntrysector) * BLOCKSIZE + file->direntryoffset;
        image[file_entry_offset + FILETYPEOFFSET] = file->filetype & 0xff;
        if (verbose && (file->filetype & FILETYPETRANSWARPMASK)) {
            printf(" [Transwarp]");
        }
        memcpy(image + file_entry_offset + FILENAMEOFFSET, file->pfilename, FILENAMEMAXSIZE);

        if (is_transwarp_bootfile(image, file_entry_offset)) {
            if (file->filetype & FILETYPETRANSWARPMASK) {
                if (verbose) {
                    printf("\n");
                }

                fprintf(stderr, "ERROR: Attempt to write Transwarp bootfile as Transwarp file\n");

                exit(-1);
            }

            file->mode |= MODE_TRANSWARPBOOTFILE;
            if (verbose) {
                printf(" [Transwarp bootfile]");
            }

            if (i != 0) {
                // allocate Transwarp bootfile first
                if ((files[0].mode & MODE_TRANSWARPBOOTFILE) != 0) {
                    if (verbose) {
                        printf("\n");
                    }

                    fprintf(stderr, "ERROR: Multiple Transwarp bootfiles\n");
                    exit(-1);
                }

                imagefile transwarp_bootfile = *file;
                for (int j = i; j > 0; --j) {
                    files[j] = files[j - 1];
                }
                files[0] = transwarp_bootfile;
                file = files;
            }
        }

        if (shadowdirtrack > 0) {
            file_entry_offset = linear_sector(type, shadowdirtrack, file->direntrysector) * BLOCKSIZE + file->direntryoffset;
            image[file_entry_offset + FILETYPEOFFSET] = file->filetype;
            memcpy(image + file_entry_offset + FILENAMEOFFSET, file->pfilename, FILENAMEMAXSIZE);
        }

        if (verbose) {
            printf("\n");
        }
    }

    if (!quiet && (num_overwritten_files > 0)) {
        printf("%d out of %d files exist and will be overwritten\n", num_overwritten_files, num_files);
    }
}

/* Prints the allocated tracks and sectors for every file */
static void
print_file_allocation(image_type type, const unsigned char* image, imagefile* files, int num_files)
{
    if (num_files <= 0) {
        imagefile existing_files[144];
        memset(existing_files, 0, sizeof existing_files);

        int t = dirtrack(type);
        int s = (type == IMAGE_D81) ? 3 : 1;
        int o = 0;
        char *blockmap = calloc(image_num_blocks(type), sizeof(char));
        if(blockmap == NULL) {
            fprintf(stderr, "ERROR: Memory allocation error\n");
            exit(-1);
        }

        do {
            int b = linear_sector(type, t, s) * BLOCKSIZE + o;
            int filetype = image[b + FILETYPEOFFSET] & 0xf;
            switch (filetype) {
            case FILETYPEPRG: {
                int track = image[b + FILETRACKOFFSET];
                int sector = image[b + FILESECTOROFFSET];
                int b = linear_sector(type, track, sector);
                if(b >= 0) {
                    unsigned char *filename = (unsigned char *) image + b + FILENAMEOFFSET;
                    memcpy(existing_files[num_files].pfilename, filename, FILENAMEMAXSIZE);
                    existing_files[num_files].track = track;
                    existing_files[num_files].sector = sector;
                    existing_files[num_files].direntryindex = num_files;
                    existing_files[num_files].direntrysector = s;
                    existing_files[num_files].direntryoffset = o;
                    existing_files[num_files].nrSectors = image[b + FILEBLOCKSLOOFFSET] + 256 * image[b + FILEBLOCKSHIOFFSET];

                    if (is_transwarp_file(image, b)) {
                        existing_files[num_files].filetype = filetype | FILETYPETRANSWARPMASK;
                        existing_files[num_files].sectorInterleave = 1;

                        int start_track;
                        int end_track;
                        int low_track;
                        int high_track;
                        int filesize = transwarp_stat(type, image, b, &start_track, &end_track, &low_track, &high_track);

                        existing_files[num_files].size = filesize;

                        existing_files[num_files].track = start_track;
                        existing_files[num_files].last_track = end_track;

                        ++num_files;

                        break;
                    }

                    while (true) {
                        b = linear_sector(type, track, sector);
                        if(b < 0) {
                            break; // TODO: print info about illegal t/s
                        }
                        int offset = b * BLOCKSIZE;
                        int next_track = image[offset + 0];
                        int next_sector = image[offset + 1];
                        if ((track == 0) || (next_track == 0)) {
                            break;
                        }
                        if ((track == next_track) && (next_sector > sector)) {
                            existing_files[num_files].sectorInterleave = next_sector - sector;
                            break;
                        }
                        track = next_track;
                        sector = next_sector;
                    }

                    ++num_files;

                    break;
                }
            }

            default:
                break;
            }
        } while (next_dir_entry(type, image, &t, &s, &o, blockmap));
        free(blockmap);
        files = existing_files;
    }

    if(num_files > 0) {
        printf("\nFile allocation:\n");
    }

    for (int i = 0; i < num_files; i++) {
        printf("%02d/%02d %3d (0x%02x 0x%02x:0x%02x) ", files[i].track, files[i].sector, files[i].nrSectors & 0xffff,
               files[i].direntryindex, files[i].direntrysector, files[i].direntryoffset);
        if (files[i].alocalname) {
            printf("\"%s\" => ", files[i].alocalname);
        }
        print_filename(stdout, files[i].pfilename);
        printf(" (SL: %d)", files[i].sectorInterleave);

        if ((files[i].mode & MODE_LOOPFILE) && (files[i].sectorInterleave != 0)) {
            printf("\n");

            continue;
        }

        if (files[i].filetype & FILETYPETRANSWARPMASK) {
            int transwarp_blocks;
            int nonredundant_blocks;
            int redundant_blocks;
            int nonredundant_blocks_on_last_track;
            int spare_bytes = transwarp_size(type, files[i].track, files[i].last_track, files[i].size, &transwarp_blocks, &nonredundant_blocks, &redundant_blocks, &nonredundant_blocks_on_last_track);

            int num_blocks = 0;
            int filesize = files[i].size + 2;
            while (filesize > 0) {
                ++num_blocks;
                filesize -= 254;
            }

            printf("\n          Transwarp: %d total/%d actual (%d standard) blocks, tracks %d-%d, %d used and %d redundant block%s on last track, 0x%x spare bytes in last block, size 0x%x\n",
                   transwarp_blocks, nonredundant_blocks, num_blocks,
                   (files[i].track < files[i].last_track) ? files[i].track : files[i].last_track,
                   (files[i].track >= files[i].last_track) ? files[i].track : files[i].last_track,
                   nonredundant_blocks_on_last_track, redundant_blocks, (redundant_blocks == 1) ? "" : "s",
                   spare_bytes, files[i].size);

            continue;
        }

        int track = files[i].track;
        int sector = files[i].sector;

        bool firsttrack = true;
        int firstsector = sector;
        bool fileblocks[SECTORSPERTRACK_D81];
        memset(fileblocks, 0, sizeof fileblocks);
        fileblocks[sector] = true;

        int j = 0;
        while (track != 0) {
            if (j == 0) {
                printf("\n          ");
            }
            printf("%02d/%02d", track, sector);
            int b = linear_sector(type, track, sector);
            if(b < 0) {
                break; // TODO: print info about illegal t/s link?
            }
            int offset = b * BLOCKSIZE;
            int next_track = image[offset + 0];
            int next_sector = image[offset + 1];
            if ((track != next_track) && (next_track != 0)) {
                /* track change */
                if (next_sector != 0) {
                    /* interleave violation */
                    printf("!-");
                } else {
                    printf(" -");
                }
            } else if ((next_sector < sector) && (next_track != 0)) {
                /* sector wrap */
                int expected_next_sector = ((sector + abs(files[i].sectorInterleave)) % num_sectors(type, track));
                if (expected_next_sector > 0) {
                    --expected_next_sector;
                }
                bool on_nonempty_firsttrack = (expected_next_sector < next_sector) && firsttrack && (firstsector != 0);
                if ((expected_next_sector != next_sector) && (!on_nonempty_firsttrack)) {
                    while ((expected_next_sector < next_sector) && fileblocks[expected_next_sector]) {
                        ++expected_next_sector;
                    }
                    if (expected_next_sector != next_sector) {
                        /* interleave violation */
                        printf("!.");
                    } else {
                        printf(" .");
                    }
                } else {
                    printf(" .");
                }
            } else if (((next_sector - sector) != abs(files[i].sectorInterleave)) && (next_track != 0)) {
                /* interleave violation */
                printf(" !");
            } else {
                printf("  ");
            }

            if (track != next_track) {
                memset(fileblocks, 0, sizeof fileblocks);
                firsttrack = false;
            }
            track = next_track;
            sector = next_sector;
            if (next_track != 0) {
                fileblocks[sector] = true;
            }

            j++;
            if (j == 10) {
                j = 0;
            }
        }
        printf("\n");
    }
    printf("\n");
}


static void
assign_blocktags(image_type type, const unsigned char *image, int(*blocktags)[SECTORSPERTRACK_D81])
{
    char c = '@';

    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }
    int ds = (type == IMAGE_D81) ? 3 : 1;
    int dt = dirtrack(type);
    int offset = 0;
    do {
        int dirblock = linear_sector(type, dt, ds) * BLOCKSIZE + offset;
        int filetype = image[dirblock + FILETYPEOFFSET] & 0xf;
        if (filetype != 0) {
            int filetrack = image[dirblock + FILETRACKOFFSET];
            int filesector = image[dirblock + FILESECTOROFFSET];

            if (is_transwarp_file(image, dirblock)) {
                int start_track;
                int end_track;
                int low_track;
                int high_track;
                int filesize = transwarp_stat(type, image, dirblock, &start_track, &end_track, &low_track, &high_track);
                if (filesize <= 0) {
                    continue;
                }

                int transwarp_blocks;
                int nonredundant_blocks;
                int redundant_blocks;
                int nonredundant_blocks_on_last_track;
                transwarp_size(type, start_track, end_track, filesize, &transwarp_blocks, &nonredundant_blocks, &redundant_blocks, &nonredundant_blocks_on_last_track);

                for (int track = low_track; track <= high_track; ++track) {
                    for (int sector = 0; sector < SECTORSPERTRACK_D81; ++sector) {
                        blocktags[track][sector] = c + (((track == end_track) && (sector >= nonredundant_blocks_on_last_track)) ? 256 : 0);
                    }
                }
            } else {
                bool new_track = true;
                while (filetrack != 0) {
                    int b = linear_sector(type, filetrack, filesector);
                    if(b < 0) {
                        break;
                    }
                    int block_offset = b * BLOCKSIZE;
                    int next_track = image[block_offset + TRACKLINKOFFSET];
                    int next_sector = image[block_offset + SECTORLINKOFFSET];

                    blocktags[filetrack][filesector] = c + (new_track ? 256 : 0);
                    new_track = (filetrack != next_track);

                    filetrack = next_track;
                    filesector = next_sector;
                }
            }

            switch (c) {
            default:
                ++c;
                break;
            case 'Z':
                c = '0';
                break;
            case '9':
                c = 'a';
                break;
            case 'z':
                c = '@';
                break;
            }
        }

    } while (next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);
}

/* Returns true if the file starting on the given filetrack and filesector uses the given track */
static bool
fileontrack(image_type type, const unsigned char *image, int track, int filetrack, int filesector)
{
    while (filetrack != 0) {
        if (filetrack == track) {
            return true;
        }

        int b = linear_sector(type, filetrack, filesector);
        if(b < 0) {
            return false;
        }
        int block_offset = b * BLOCKSIZE;
        int next_track = image[block_offset + TRACKLINKOFFSET];
        int next_sector = image[block_offset + SECTORLINKOFFSET];
        filetrack = next_track;
        filesector = next_sector;
    }

    return false;
}

/* Prints all filenames of files that use the given track */
static void
print_track_usage(image_type type, const unsigned char *image, int(*blocktags)[SECTORSPERTRACK_D81], int track)
{

    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }
    int ds = (type == IMAGE_D81) ? 3 : 1;
    int dt = dirtrack(type);
    int offset = 0;
    do {
        int dirblock = linear_sector(type, dt, ds) * BLOCKSIZE + offset;
        int filetype = image[dirblock + FILETYPEOFFSET] & 0xf;
        if (filetype != 0) {
            int filetrack = image[dirblock + FILETRACKOFFSET];
            int filesector = image[dirblock + FILESECTOROFFSET];
            bool ontrack = (type == IMAGE_D71) ? fileontrack(type, image, track, (filetrack > D64NUMTRACKS) ? filetrack - D64NUMTRACKS : filetrack + D64NUMTRACKS, filesector) : false;

            if (is_transwarp_file(image, dirblock)) {
                int start_track;
                int end_track;
                int low_track;
                int high_track;
                int filesize = transwarp_stat(type, image, dirblock, &start_track, &end_track, &low_track, &high_track);
                if (filesize <= 0) {
                    continue;
                }

                ontrack = (low_track <= track) && (track <= high_track);
                if (ontrack == false) {
                    continue;
                }

                filetrack = start_track;
            }

            if (ontrack || fileontrack(type, image, track, filetrack, filesector)) {
                unsigned char *filename = (unsigned char *) image + dirblock + FILENAMEOFFSET;
                if (track == filetrack) {
                    reverse_print_on();
                }
                printf("%c", blocktags[filetrack][filesector]);
                if (track == filetrack) {
                    reverse_print_off();
                }
                printf(": ");
                print_filename(stdout, filename);
                printf(" ");
            }
        }
    } while (next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);
}

/* Prints the BAM allocation map and returns the number of free blocks */
static int
check_bam(image_type type, const unsigned char* image)
{
    int sectorsFree = 0;
    int sectorsFreeOnDirTrack = 0;
    int sectorsOccupied = 0;
    int sectorsOccupiedOnDirTrack = 0;

    int blocktags[D81NUMTRACKS][SECTORSPERTRACK_D81];

    if (verbose) {
        memset(blocktags, 0, sizeof blocktags);
        assign_blocktags(type, image, blocktags);

        printf("Block allocation:\n");
    }

    int max_track = (type == IMAGE_D81) ? D81NUMTRACKS
                    : (((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) ? D64NUMTRACKS_EXTENDED
                       : D64NUMTRACKS);
    for (int t = 1; t <= max_track; t++) {

        if (verbose) {
            printf("  %2d: ", t);
        }
        for (int s = 0; s < num_sectors(type, t); s++) {
            if (is_sector_free(type, image, t, s, 0, 0)) {
                if (verbose) {
                    printf(".");
                }
                if (t != dirtrack(type)) {
                    sectorsFree++;
                } else {
                    sectorsFreeOnDirTrack++;
                }
            } else {
                if (verbose) {
                    int blocktag = blocktags[t][s];
                    if (blocktag == 0) {
                        blocktag = '#';
                    }
                    if (blocktag >= 256) {
                        reverse_print_on();
                    }
                    printf("%c", blocktag);
                    if (blocktag >= 256) {
                        reverse_print_off();
                    }
                }
                if (t != dirtrack(type)) {
                    sectorsOccupied++;
                } else {
                    sectorsOccupiedOnDirTrack++;
                }
            }
        }

        if (type == IMAGE_D71) {
            for (int i = num_sectors(type, t); i < 23; i++) {
                if (verbose) {
                    printf(" ");
                }
            }
            int t2 = t + D64NUMTRACKS;

            if (verbose) {
                printf("%2d: ", t2);
            }
            for (int s = 0; s < num_sectors(type, t2); s++) {
                if (is_sector_free(type, image, t2, s, 0, 0)) {
                    if (verbose) {
                        printf(".");
                    }
                    if (t2 != dirtrack(type)) {
                        sectorsFree++;
                    } else {
                        /* track 53 is usually empty except the extra BAM block */
                        sectorsFreeOnDirTrack++;
                    }
                } else {
                    if (verbose) {
                        printf("#");
                    }
                    sectorsOccupied++;
                }
            }
        }

        for (int i = ((type == IMAGE_D81) ? 42 : 23) - num_sectors(type, t); i > 0; --i) {
            if (verbose) {
                printf(" ");
            }
        }
        if (verbose) {
            print_track_usage(type, image, blocktags, t);
            printf("\n");
        }
    }
    if (verbose) {
        printf("%3d/%3d blocks free (%d/%d including dir track)\n", sectorsFree, sectorsFree + sectorsOccupied,
               sectorsFree + sectorsFreeOnDirTrack, sectorsFree + sectorsFreeOnDirTrack + sectorsOccupied + sectorsOccupiedOnDirTrack);
    }

    return sectorsFree;
}

/* Prints the filetype like the C64 when listing the directory */
static void
print_filetype(int filetype)
{
    if ((filetype & 0x80) == 0) {
        printf("*");
    } else {
        printf(" ");
    }
    if(unicode == 1) {
        printf("%s", filetypename_uc[filetype & 0xf]);
    } else {
        printf("%s", filetypename_lc[filetype & 0xf]);
    }
    if ((filetype & 0x40) != 0) {
        printf("<");
    } else {
        printf(" ");
    }
}

/* Prints the directory like the C64 when listing the directory */
static void
print_directory(image_type type, unsigned char* image, int blocks_free)
{
    unsigned char* bam = image + linear_sector(type, dirtrack(type), 0) * BLOCKSIZE;
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }

    printf("\n0 ");
    reverse_print_on();
    printf("\"");
    print_petscii(bam + get_header_offset(type), 16);
    printf("\" ");
    print_petscii(bam + get_id_offset(type), 5);
    reverse_print_off();

    if (verbose) {
        printf("   fn hash");
    }
    printf("\n");

    int ds = (type == IMAGE_D81) ? 3 : 1;
    int dt = dirtrack(type);
    int offset = 0;
    do {
        int dirblock = linear_sector(type, dt, ds) * BLOCKSIZE + offset;
        int filetype = image[dirblock + FILETYPEOFFSET];
        int blocks = image[dirblock + FILEBLOCKSLOOFFSET] + 256 * image[dirblock + FILEBLOCKSHIOFFSET];

        if (filetype != FILETYPEDEL) {
            unsigned char* filename = (unsigned char*)image + dirblock + FILENAMEOFFSET;
            printf("%-3d  ", blocks);
            print_dirfilename(filename);
            print_filetype(filetype);
            if (verbose) {
                printf(" [$%04x]", filenamehash(filename));
            }
            printf("\n");
        }
    } while (next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);
    if(unicode == 1) {
        printf("%d BLOCKS FREE.\n", blocks_free);
    } else {
        printf("%d blocks free.\n", blocks_free);
    }
}

/* Performs GCR encoding on 32 bit value */

static const unsigned char NIBBLE_TO_GCR[] = {
    0x0a, 0x0b, 0x12, 0x13,
    0x0e, 0x0f, 0x16, 0x17,
    0x09, 0x19, 0x1a, 0x1b,
    0x0d, 0x1d, 0x1e, 0x15
};

static void
encode_4_bytes_gcr(char* in, char* out)
{
    out[0] = (NIBBLE_TO_GCR[(in[0] >> 4) & 0xf] << 3) | (NIBBLE_TO_GCR[ in[0]       & 0xf] >> 2); /* 11111222 */
    out[1] = (NIBBLE_TO_GCR[ in[0]       & 0xf] << 6) | (NIBBLE_TO_GCR[(in[1] >> 4) & 0xf] << 1) | (NIBBLE_TO_GCR[ in[1]       & 0xf] >> 4); /* 22333334 */
    out[2] = (NIBBLE_TO_GCR[ in[1]       & 0xf] << 4) | (NIBBLE_TO_GCR[(in[2] >> 4) & 0xf] >> 1); /* 44445555 */
    out[3] = (NIBBLE_TO_GCR[(in[2] >> 4) & 0xf] << 7) | (NIBBLE_TO_GCR[ in[2]       & 0xf] << 2) | (NIBBLE_TO_GCR[(in[3] >> 4) & 0xf] >> 3); /* 56666677 */
    out[4] = (NIBBLE_TO_GCR[(in[3] >> 4) & 0xf] << 5) |  NIBBLE_TO_GCR[ in[3]       & 0xf]; /* 77788888 */
}

/* Transwarp encoding utility functions */

static void
generate_gcr_decoding_table(const unsigned char nibble_to_gcr[], int8_t gcr_to_nibble[])
{
    for (int i = 0; i < 32; ++i) {
        gcr_to_nibble[i] = -(i + 1);
    }

    for (int i = 0; i < 16; ++i) {
        gcr_to_nibble[nibble_to_gcr[i]] = i;
    }
}

static unsigned char
even_bits(unsigned char value)
{
    return (((value >> 6) & 1) << 3)
           | (((value >> 4) & 1) << 2)
           | (((value >> 2) & 1) << 1)
           | (((value >> 0) & 1) << 0);
}

static unsigned char
odd_bits(unsigned char value)
{
    return (((value >> 7) & 1) << 3)
           | (((value >> 5) & 1) << 2)
           | (((value >> 3) & 1) << 1)
           | (((value >> 1) & 1) << 0);
}

typedef struct transwarp_encode_context {
    unsigned int  version;
    unsigned char previous;
    unsigned char previous1;
    unsigned char previous2;
    unsigned char accu;
    unsigned char carry;
    unsigned char recvcarry;
    unsigned char carry2;
    unsigned char sendaccu;
    unsigned char sendcarry;
} transwarp_encode_context;

static const int ENCODE[5][64] = {
    {
        0xf6, 0xee, 0xf5, 0xed, 0x9a, 0xde, 0x96, 0xda, 0xf3, 0xea, 0xf2, 0x9e, 0x93, 0xd6, 0x92, 0xd3,
        0xd2, 0xca, 0xce, 0xbe, 0xb3, 0x7e, 0xb2, 0x7d, 0xcd, 0xba, 0xcb, 0xb6, 0xae, 0x7b, 0xaa, 0x7a,
        0x76, 0x6e, 0x75, 0x6d, 0x5e, 0x5b, 0x5d, 0x5a, 0x73, 0x6b, 0x72, 0x6a, 0xdb, 0x9d, 0xeb, 0xd5,
        0x56, 0x4e, 0x55, 0x4d, 0xbd, 0xb5, 0xbb, 0xad, 0x53, 0x4b, 0x52, 0xdd, 0xab, 0x4a, 0x9b, 0x95
    }, {
        0xf6, 0xee, 0xf5, 0xed, 0x9a, 0xde, 0x96, 0xda, 0xf3, 0xea, 0xf2, 0x9e, 0x93, 0xd6, 0x92, 0xef,
        0xe7, 0x7c, 0x9f, 0x74, 0xe6, 0x6c, 0xdf, 0xa6, 0x9c, 0xba, 0x94, 0xb6, 0xae, 0x7b, 0xaa, 0x7a,
        0x76, 0x6e, 0x75, 0x6d, 0x5e, 0x5b, 0x5d, 0x5a, 0x73, 0x6b, 0x72, 0x6a, 0xdb, 0xd7, 0xeb, 0xe5,
        0x56, 0x64, 0x55, 0x5c, 0xbd, 0xb5, 0xbb, 0xad, 0x65, 0x54, 0x5f, 0xdd, 0xab, 0xa7, 0x9b, 0xa5
    }, {
        0xa5, 0xa7, 0xa9, 0xab, 0xd5, 0xd7, 0xd9, 0xdb, 0x95, 0x99, 0x9b, 0x97, 0xe5, 0x9d, 0xeb, 0xe9
    }, {
        0xf6, 0xee, 0xf5, 0xed, 0x69, 0xde, 0x59, 0xda, 0xb9, 0xea, 0xb7, 0x6f, 0x57, 0xd6, 0x4f, 0xef,
        0xe7, 0xca, 0xce, 0xbe, 0xe6, 0xbf, 0xdf, 0xa6, 0xcd, 0xba, 0xcb, 0xb6, 0xae, 0x7b, 0xaa, 0x7a,
        0x76, 0x6e, 0x75, 0x6d, 0x5e, 0x5b, 0x5d, 0x5a, 0x67, 0x6b, 0x66, 0x6a, 0xe9, 0xd7, 0xeb, 0xe5,
        0x56, 0x4e, 0x55, 0x4d, 0xbd, 0xb5, 0xbb, 0xad, 0x65, 0x4b, 0x5f, 0xdd, 0xab, 0xa7, 0xa9, 0xa5
    }, {
        0xcf, 0xaf, 0xc9, 0x79, 0x69, 0xde, 0x59, 0xda, 0xb9, 0x77, 0xb7, 0x6f, 0x57, 0xd6, 0x4f, 0xd3,
        0xd2, 0xca, 0xce, 0xbe, 0xb3, 0x7e, 0xb2, 0x7d, 0xcd, 0xba, 0xcb, 0xb6, 0xae, 0x7b, 0xaa, 0x7a,
        0x76, 0x6e, 0x75, 0x6d, 0x5e, 0x5b, 0x5d, 0x5a, 0x73, 0x6b, 0x72, 0x6a, 0xdb, 0xd7, 0xd9, 0xd5,
        0x56, 0x4e, 0x55, 0x4d, 0xbd, 0xb5, 0xbb, 0xad, 0x53, 0x4b, 0x52, 0xdd, 0xab, 0x4a, 0xa9, 0x49
    }
};

static const int DECODE[256] = {
    -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x00, 0x02, 0x12,   -1, 0x14, 0x16, 0x68,
        -1,   -1, 0x18, 0x1a, 0x12, 0x1c, 0x1e, 0x6a,   -1, 0x6c, 0x24, 0x26, 0x14, 0x2c, 0x2e, 0x18,
        -1,   -1,   -1,   -1, 0x16, 0x1a, 0x38, 0x3a,   -1, 0x6e, 0x30, 0x32, 0x46, 0x34, 0x36, 0x70,
        -1,   -1, 0x38, 0x3a, 0x54, 0x3c, 0x3e, 0x72,   -1, 0x74, 0x40, 0x42, 0x56, 0x44, 0x46,   -1,
        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
        -1,   -1, 0x68, 0x6a, 0x58, 0x80, 0x6c, 0x8a,   -1, 0x82, 0x6e, 0x88, 0x5a, 0xa2, 0x70, 0x5c,
        -1,   -1,   -1,   -1,   -1, 0x00, 0x44, 0x02,   -1, 0x08, 0x48, 0x0a,   -1, 0x04, 0x4a, 0x76,
        -1,   -1, 0x4c, 0x4e,   -1, 0x06, 0x50, 0x78,   -1, 0x7a, 0x52, 0x0c,   -1, 0x0e, 0x54, 0x46,
        -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, 0x7c, 0x56, 0x58,   -1, 0x5a, 0x5c, 0x7e,
        -1,   -1, 0x5e, 0x60,   -1, 0x20, 0x62, 0x22,   -1, 0x28, 0x64, 0x2a,   -1, 0x10, 0x66, 0x4c,
        -1,   -1,   -1,   -1,   -1, 0xa0, 0x4e, 0x5e,   -1, 0xaa, 0x72, 0xa8,   -1, 0x74, 0x76, 0x60,
        -1,   -1, 0x78, 0x7a,   -1, 0x7c, 0x7e,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1
    };

static unsigned char
encode_read_diff(const int encode[64], unsigned char *accu, unsigned char *carry, unsigned char value)
{
    unsigned char value_to_encode;

    unsigned char target = DECODE[encode[value]] & 0x7e;
    for (value_to_encode = 0; value_to_encode < 64; ++value_to_encode) {
        unsigned char val = DECODE[encode[value_to_encode]] + *accu + *carry;
        if ((val & 0x7e) == target) {
            break;
        }
    }
    if (value_to_encode >= 64) {
        printf("Encoding error, 0x%x = 0x%x + %d + ?\n", target, *accu, *carry);
        for (value_to_encode = 0; value_to_encode < 64; ++value_to_encode) {
            unsigned char val = DECODE[encode[value_to_encode]] + *accu + *carry;
            printf("%d: 0x%x <- 0x%x\n", value_to_encode, val & 0x7e, DECODE[encode[value_to_encode]]);
        }

        fprintf(stderr, "ERROR: Transwarp encoding error\n");
        exit(-2);
    }

    int sum = DECODE[encode[value_to_encode]] + *accu + *carry;
    *accu = sum;
    *carry = sum >= 256;

    unsigned char check = DECODE[encode[value]];
    if ((*accu & 0x7e) != (check & 0x7e)) {
        fprintf(stderr, "ERROR: Transwarp encoding error, 0x%x: actual 0x%x != 0x%x expected\n", DECODE[encode[check & 0x3f]], *accu, check);
        exit(-3);
    }

    unsigned char temp = (*carry << 7) | (*accu >> 1);
    *carry = *accu & 1;
    *accu = (*carry << 7) | ((temp & 0xfb) >> 1);
    *carry = (*accu >> 6) & 1;

    return encode[value_to_encode];
}

unsigned char
encode_send_diff(unsigned char value, unsigned char *accu, unsigned char *carry)
{
    value = (value & ~((1 << 3) | (1 << 0)))
            | (((value >> 3) & 1) << 0)
            | (((value >> 0) & 1) << 3);
    value ^= 0xff;

    int diff = value - *accu - *carry;
    *carry = (diff < 0);

    *accu = value;

    *accu = (((*accu >> 7) & 1) << 7)
            | (((*accu >> 1) & 1) << 6)
            | (((*accu >> 7) & 1) << 5)
            | (*carry << 4)
            | (((*accu >> 6) & 1) << 2)
            | (((*accu >> 5) & 1) << 1);
    *carry = (*accu >> 6) & 1;

    return diff;
}

static unsigned char
encode_receive_diff(const transwarp_encode_context *ctx, unsigned char in, unsigned char *previous, unsigned char *carry)
{
    int offset = (ctx->version <= 84) ? 0 : 2;

    int out = in - *carry - offset;
    int diff = out - ((*previous & 0xc0) | (out & 0x3f));
    *carry = (diff < 0);
    out = ((out ^ *previous) & 0x3f) | diff;
    *previous = in;

    return out;
}

static unsigned char
encode_buffer_byte(const int encode[][64], unsigned char previous, unsigned char *carry, unsigned char in, unsigned char *out)
{
    unsigned char even = (previous >> 1) ^ in;
    unsigned char odd = ((*carry << 7) | (previous >> 1)) ^ in;

    *carry = previous & 1;

    out[(31 * 5) + 4] = encode[2][even_bits(even)];
    out[4] = encode[2][odd_bits(odd)];

    return in;
}

static void
encode_base_bytes(const unsigned char scramble[][256],
                  transwarp_encode_context *ctx, const unsigned char in[3], unsigned char *out)
{
    unsigned char in0 = encode_receive_diff(ctx, in[0], &(ctx->previous), &(ctx->recvcarry));
    unsigned char in1 = encode_receive_diff(ctx, in[1], &(ctx->previous), &(ctx->recvcarry));
    unsigned char in2 = encode_receive_diff(ctx, in[2], &(ctx->previous), &(ctx->recvcarry));

    in0 = scramble[0][in0];
    in1 = scramble[1][in1];
    in2 = scramble[2][in2];

    unsigned char val3 = in0 & 0x3f;
    out[0] = encode_read_diff(ENCODE[3], &(ctx->accu), &(ctx->carry), val3);

    unsigned char val4 = ((in0 >> 6)
                          | (in1 << 2)) & 0x3f;
    out[1] = encode_read_diff(ENCODE[4], &(ctx->accu), &(ctx->carry), val4);

    unsigned char val0 = ((in1 >> 4)
                          | (in2 << 4)) & 0x3f;
    out[2] = encode_read_diff(ENCODE[0], &(ctx->accu), &(ctx->carry), val0);

    unsigned char val1 = in2 >> 2;
    out[3] = encode_read_diff(ENCODE[1], &(ctx->accu), &(ctx->carry), val1);
}

static unsigned char
crc8(unsigned char value)
{
    for (int i = 0; i < 8; ++i) {
        value = (value & 0x80) ? ((value << 1) ^ 0x31) : (value << 1);
    }

    return value;
}

static bool
decode_5_bytes_gcr(const int8_t decoding_map[], const unsigned char *in, unsigned char *out)
{
    unsigned char gcr_hi;
    unsigned char gcr_lo;

    gcr_hi = in[0] >> 3;
    gcr_lo = ((in[0] & 0x7) << 2) | (in[1] >> 6);
    int out0 = (decoding_map[gcr_hi] << 4) | decoding_map[gcr_lo];

    gcr_hi = (in[1] & 0x3e) >> 1;
    gcr_lo = ((in[1] & 0x1) << 4) | (in[2] >> 4);
    int out1 = (decoding_map[gcr_hi] << 4) | decoding_map[gcr_lo];

    gcr_hi = ((in[2] & 0xf) << 1) | (in[3] >> 7);
    gcr_lo = (in[3] & 0x7c) >> 2;
    int out2 = (decoding_map[gcr_hi] << 4) | decoding_map[gcr_lo];

    gcr_hi = ((in[3] & 0x3) << 3) | (in[4] >> 5);
    gcr_lo = in[4] & 0x1f;
    int out3 = (decoding_map[gcr_hi] << 4) | decoding_map[gcr_lo];

    out[0] = out0;
    out[1] = out1;
    out[2] = out2;
    out[3] = out3;

    return (out0 | out1 | out2 | out3) >= 0;
}

static int
decode_gcr_block(const int8_t decoding_map[], const unsigned char *encoded, unsigned char decoded[])
{
    bool ok = decode_5_bytes_gcr(decoding_map, encoded, decoded);
    decoded[0] = decoded[1];
    decoded[1] = decoded[2];
    decoded[2] = decoded[3];

    int computed_checksum = decoded[0] ^ decoded[1] ^ decoded[2];

    int i = 5;
    for (int j = 3; i < 320; i += 5, j += 4) {
        ok &= decode_5_bytes_gcr(decoding_map, encoded + i, decoded + j);
        computed_checksum ^= decoded[j] ^ decoded[j + 1] ^ decoded[j + 2] ^ decoded[j + 3];
    }

    unsigned char last_byte[4];
    ok &= decode_5_bytes_gcr(decoding_map, encoded + i, last_byte);
    decoded[255] = last_byte[0];

    computed_checksum ^= last_byte[0];
    computed_checksum = ok ? computed_checksum : -1;

    return ok ? computed_checksum : -1;
}

static int
encode_transwarp_block(const unsigned char scramble[][256], const int8_t gcr_to_nibble[32],
                       transwarp_encode_context* ctx, const unsigned char *indata, int filepos, unsigned char encoded[325])
{
    const unsigned char *unencoded = indata + filepos;

    unsigned char data[TRANSWARPBLOCKSIZE];
    if (filepos < 2) {
        for (int i = filepos, j = 0; j < TRANSWARPBLOCKSIZE; ++i, ++j) {
            data[j] = (i < 0) ? ' ' : ((i < 2) ? 0 : indata[i]);
        }

        unencoded = data;
    }

    unsigned char semiencoded[TRANSWARPBUFFERBLOCKSIZE];

    for (int i = TRANSWARPBUFFERBLOCKSIZE - 1; i >= 0; --i) {
        unsigned char value = encode_receive_diff(ctx, unencoded[TRANSWARPBASEBLOCKSIZE + i], &(ctx->previous2), &(ctx->carry2));

        value = scramble[3][value];

        semiencoded[i] = encode_send_diff(value, &(ctx->sendaccu), &(ctx->sendcarry));
    }

    unsigned char buffer_previous = ctx->previous1;
    unsigned char buffer_carry = 0;
    for (int i = 0; i < TRANSWARPBUFFERBLOCKSIZE; ++i) {
        int shuffle = (TRANSWARPBUFFERBLOCKSIZE - 1) - (i / 2) - ((i & 1) ? ((TRANSWARPBUFFERBLOCKSIZE / 2) + 1) : 0);
        unsigned char value = semiencoded[shuffle];
        buffer_previous = encode_buffer_byte(ENCODE, buffer_previous, &buffer_carry, value, encoded + 3 + (5 * (TRANSWARPBUFFERBLOCKSIZE - 1 - shuffle)));
    }

    unsigned char previous = unencoded[TRANSWARPBASEBLOCKSIZE - 1];

    const int CRCSTEP = 8;

    for (int i = 0; i < TRANSWARPBASEBLOCKSIZE; i += CRCSTEP) {
        previous = crc8(previous);
        previous ^= unencoded[i];
    }
    previous = crc8(previous);

    if (filepos > ((21 * TRANSWARPBLOCKSIZE) + 2)) {
        const int BACKCHECKOFFS = (21 * TRANSWARPBLOCKSIZE) + TRANSWARPBUFFERBLOCKSIZE;
        previous ^= indata[filepos - BACKCHECKOFFS];
        previous = crc8(previous);
        previous ^= indata[filepos - BACKCHECKOFFS + TRANSWARPBUFFERBLOCKSIZE - 1];
        previous = crc8(previous);
    }

    char head_data[4] = { 7, 0, 0, 0 };
    encode_4_bytes_gcr(head_data, (char *) encoded);
    char tail_data[4] = { 0, 0, 0, 0 };
    encode_4_bytes_gcr(tail_data, (char *) encoded + 320);

    unsigned char accu = ctx->previous;
    unsigned char carry = 0;
    for (int i = 0; i < TRANSWARPBASEBLOCKSIZE; ++i) {
        encode_receive_diff(ctx, unencoded[i], &accu, &carry);
    }

    unsigned char receive_checksum = (-previous - carry);

    unsigned char checksum = receive_checksum ^ (buffer_previous >> 1);

    unsigned char odd = odd_bits(checksum);

    odd ^= (buffer_carry << 3);

    static const unsigned char ENCODE_TOP[] = {
        0x05,
        0x07,
        0x0d,
        0x0b
    };
    encoded[2] = (encoded[2] & 0xf0) | ENCODE_TOP[odd & 0x3];

    encoded[317] = ENCODE[2][even_bits(checksum)];

    encoded[322] = ENCODE[2][odd & 0xc];

    unsigned char top_2_bits = encoded[2];
    unsigned char middle_4_bits = encoded[317];
    unsigned char bottom_2_bits = encoded[322];

    ctx->recvcarry = 0;

    ctx->accu = 0;
    ctx->carry = 0;
    for (int i = 0, j = 0; i < TRANSWARPBASEBLOCKSIZE; i += 3, j += 5) {
        encode_base_bytes(scramble, ctx, unencoded + i, encoded + 3 + j);

        ctx->accu = 8;
        ctx->carry = 0;

        unsigned char target = encoded[3 + j + 4];
        unsigned char target_accu = DECODE[target];
        unsigned char target_check = ENCODE[2][odd_bits(target_accu)];

        if (target != target_check) {
            fprintf(stderr, "ERROR: Transwarp encoding error, [%d] 0x%x != 0x%x <- 0x%x\n", i, target, target_check, DECODE[target]);

            exit(-4);
        }

        unsigned char store = ctx->accu ^ target_accu;
        encoded[3 + j + 4] = ENCODE[2][odd_bits(store)];

        unsigned char stored = DECODE[encoded[3 + j + 4]];
        ctx->accu ^= stored;

        if (ctx->accu != target_accu) {
            fprintf(stderr, "ERROR: Transwarp encoding error, [%d] actual 0x%x != 0x%x expected <- 0x%x\n", i, ctx->accu, target_accu, stored);

            exit(-5);
        }
    }
    if (carry != ctx->recvcarry) {
        fprintf(stderr, "ERROR: Transwarp encoding error, carry %d != %d recvcarry\n", carry, ctx->recvcarry);

        exit(-6);
    }

    unsigned char top_fix = encoded[2] ^ (DECODE[encoded[322]] & 0xf);
    top_fix = ((top_fix >> 2) & 2) | ((top_fix >> 1) & 1);
    encoded[2] = (encoded[2] & 0xf0) | ENCODE_TOP[top_fix];
    encoded[322] = (encoded[322] & 0xf0) | 0x5;

    unsigned char block_checksum;
    block_checksum = DECODE[middle_4_bits];
    block_checksum = (block_checksum >> 1) | (buffer_carry << 7);
    block_checksum ^= DECODE[bottom_2_bits];
    block_checksum ^= (top_2_bits & 0xa);

    if (block_checksum != checksum) {
        fprintf(stderr, "ERROR: Transwarp encoding error, actual 0x%x != 0x%x expected, 0x%x -> [0x%x] -> 0x%x -> 0x%x -> [0x%x]\n", block_checksum, checksum, checksum & 0xaa, ((checksum & 0xaa) >> 1) | (checksum & 0xaa), odd, ENCODE[2][odd], DECODE[encoded[317]]);

        exit(-1);
    }

    unsigned char gcr_decoded[4];
    bool ok = decode_5_bytes_gcr(gcr_to_nibble, encoded + 320, gcr_decoded);
    int gcr_checksum = ok ? gcr_decoded[1] : -1;

    unsigned char decoded[256];
    int computed_checksum = decode_gcr_block(gcr_to_nibble, encoded, decoded);

    ok &= decode_5_bytes_gcr(gcr_to_nibble, encoded, gcr_decoded);
    gcr_decoded[1] ^= gcr_checksum ^ computed_checksum;

    encode_4_bytes_gcr((char *) gcr_decoded, (char *) encoded);

    return ok == 0;
}

static void
permute(unsigned char *key, int len, int *set)
{
    for (int i = 0; i < (len - 1); ++i) {
        int divisor = len - i;

        int remainder = 0;
        for (int i = TRANSWARPKEYSIZE - 12; i >= 0; --i) {
            int dividend = (remainder << 8) | key[i];
            remainder = dividend % divisor;
            key[i] = dividend / divisor;
        }

        int n = set[remainder];
        set[remainder] = set[divisor - 1];
        set[divisor - 1] = n;
    }
}

/* Write file to disk using Transwarp encoding */
static unsigned long long
write_transwarp_file(image_type type, unsigned char *image, imagefile *file, unsigned char *filedata, int *filesize, unsigned int version, bool transwarp_bootfile_fits_on_dir_track)
{
    file->size = *filesize - 2;

    unsigned int track = DIRTRACK_D41_D71 - 1;

    if ((file->mode & MODE_MIN_TRACK_MASK) > 0) {
        /* for Transwarp files, a set minimum track is the file's starting track */
        track = (file->mode & MODE_MIN_TRACK_MASK) >> MODE_MIN_TRACK_SHIFT;
    } else {
        /* allocate */
        bool free_tracks[40];
        for (unsigned int t = 1; t <= image_num_tracks(type); ++t) {
            bool track_free = true;
            for (int sector = 0; sector < num_sectors(type, t); ++sector) {
                if (is_sector_free(type, image, t, sector, 0 /* numdirblocks */, 0 /* dir_sector_interleave */) == false) {
                    track_free = false;
                    break;
                }
            }
            free_tracks[t - 1] = track_free;
        }

        /* below dir track */
        while (track > 0) {
            if (free_tracks[track - 1]) {
                int filesize = file->size;
                int t = track;
                while ((filesize > 0)
                        && (t > 0)) {
                    filesize -= (TRANSWARPBLOCKSIZE * num_sectors(type, t));
                    if (filesize >= 0) {
                        if (free_tracks[t - 1] == false) {
                            break;
                        }
                        --t;
                    }
                }

                if (filesize <= 0) {
                    break;
                }
            }
            --track;
        }

        if (track <= 0) {
            /* above dir track */
            track = DIRTRACK_D41_D71 + (transwarp_bootfile_fits_on_dir_track ? 1 : 2);
            while (track <= image_num_tracks(type)) {
                if (free_tracks[track - 1]) {
                    int filesize = file->size;
                    int t = track;
                    while (filesize > 0) {
                        filesize -= (TRANSWARPBLOCKSIZE * num_sectors(type, t));
                        if (filesize >= 0) {
                            if (free_tracks[t - 1] == false) {
                                break;
                            }
                            ++t;
                        }
                    }

                    if (filesize <= 0) {
                        break;
                    }
                }
                ++track;
            }
        }
    }

    file->track = track;
    file->sector = 0;

    int8_t gcr_to_nibble[32];
    generate_gcr_decoding_table(NIBBLE_TO_GCR, gcr_to_nibble);

    unsigned char key[TRANSWARPKEYSIZE];
    memset(key, 0, sizeof key);

    if (file->have_key != 0) {
        if ((filedata[0] == 0x01)
                && (filedata[1] == 0x08)) {
            srand((unsigned int) time(NULL));

            unsigned int linelink = ((filedata[3] << 8) | filedata[2]) - 0x0801 + 2;
            if ((linelink > 0)
                    && ((linelink - 2) < (unsigned int) file->size)
                    && ((filedata[linelink - 1] | filedata[linelink] | filedata[linelink + 1]) == 0)) {
                while (filedata[linelink] == 0) {
                    filedata[linelink] = rand();
                }
            }

            filedata[2] = rand();
            while ((filedata[3] == 0)
                    || (filedata[3] == 8)) {
                filedata[3] = rand();
            }

            int file_size = file->size;
            int filetrack = track;
            while (file_size > 0) {
                file_size -= (TRANSWARPBLOCKSIZE * num_sectors(type, filetrack));
                if (file_size > 0) {
                    filetrack = (filetrack < DIRTRACK_D41_D71) ? (filetrack - 1) : (filetrack + 1);
                }
            }
            int spare_blocks = (0 - file_size) / TRANSWARPBLOCKSIZE;
            int spare_bytes = TRANSWARPBLOCKSIZE - (file->size % TRANSWARPBLOCKSIZE);
            spare_bytes = (spare_blocks * TRANSWARPBLOCKSIZE) + ((spare_bytes != TRANSWARPBLOCKSIZE) ? spare_bytes : 0);
            while (spare_bytes > (0x0801 - 0x0400)) {
                spare_bytes -= TRANSWARPBLOCKSIZE;
            }
            if (spare_bytes > 0) {
                int loadaddress = (filedata[1] << 8) | filedata[0];
                loadaddress -= spare_bytes;
                filedata[0] = loadaddress;
                filedata[1] = loadaddress >> 8;
                memmove(filedata + 2 + spare_bytes, filedata + 2, file->size);
                filedata[spare_bytes + 1] = 0;
                for (int i = 2; i <= spare_bytes; ++i) {
                    filedata[i] = rand();
                }
                file->size += spare_bytes;
                *filesize += spare_bytes;
            }
        }

        memcpy(key, file->key, sizeof key);

        for (int round = TRANSWARPKEYHASHROUNDS; round > 0; --round) {
            for (int i = 0; i < (TRANSWARPKEYSIZE - 1); ++i) {
                key[i] ^= key[i + 1];
            }

            for (int i = TRANSWARPKEYSIZE - 1; i >= 0; --i) {
                int product = key[i] * 0x6b;
                key[i] = product;
                int msb = product >> 8;
                for (int j = i + 1; j < TRANSWARPKEYSIZE; ++j) {
                    msb += key[j];
                    key[j] = msb;
                    msb >>= 8;
                }
            }

            int sum = round;
            for (int i = 0; i < TRANSWARPKEYSIZE; ++i) {
                sum += key[i];
                key[i] = sum;
                sum >>= 8;
            }
        }
    }

    unsigned long long dirdatakey = (key[TRANSWARPKEYSIZE - 1] * (1ULL << 56))
                                    + (key[TRANSWARPKEYSIZE - 2] * (1ULL << 48))
                                    + (key[TRANSWARPKEYSIZE - 3] * (1ULL << 40))
                                    + (key[TRANSWARPKEYSIZE - 4] * (1ULL << 32))
                                    + (key[TRANSWARPKEYSIZE - 5] * (1ULL << 24))
                                    + (key[TRANSWARPKEYSIZE - 6] * (1ULL << 16))
                                    + (key[TRANSWARPKEYSIZE - 7] * (1ULL << 8))
                                    + key[TRANSWARPKEYSIZE - 8];

    int initial_buffer_store_value = key[TRANSWARPKEYSIZE - 9];
    int initial_buffer_recvaccu_value = key[TRANSWARPKEYSIZE - 10];
    int initial_block_recvaccu_value = key[TRANSWARPKEYSIZE - 11];

    unsigned char scramble[4][256];
    for (int i = 0; i < 4; ++i) {
        int set[] = { 0, 2, 4, 1, 3, 5 };
        if (file->have_key) {
            permute(key, 6, set);
        }

        int set2[3][4];
        for (int j = 0; j < 3; ++j) {
            int set3[] = { 0, 1, 2, 3 };
            if (file->have_key) {
                permute(key, 4, set3);
            }
            for (int k = 0; k < 4; ++k) {
                for (int l = 0; l < 4; ++l) {
                    if (k == set3[l]) {
                        set2[j][k] = l;
                        break;
                    }
                }
            }
        }

        for (int j = 0; j < 256; ++j) {
            unsigned char scrambled = (set2[0][(((j >> set[0]) & 1) << 0)
                                               | (((j >> set[3]) & 1) << 1)] << 0)
                                      | (set2[1][(((j >> set[1]) & 1) << 0)
                                                 | (((j >> set[4]) & 1) << 1)] << 2)
                                      | (set2[2][(((j >> set[2]) & 1) << 0)
                                                 | (((j >> set[5]) & 1) << 1)] << 4)
                                      | (j & 0xc0);
            scramble[i][j] = scrambled;
        }
    }

    int sectors[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };
    if (file->have_key != 0) {
        permute(key, 17, sectors);
    }

    transwarp_encode_context ctx;
    memset(&ctx, 0, sizeof ctx);
    ctx.version = version;

    ctx.previous1 = initial_buffer_store_value;

    bool done = false;
    int block_index = 0;
    int total_blocks = 0;

    int filepos = 2;

    for (; !done; (track >= DIRTRACK_D41_D71) ? ++track : --track) {
        if ((track < 1)
                || (track > image_num_tracks(type))) {
            fprintf(stderr, "ERROR: Disk full (track %d out of range) while writing Transwarp file ", track);
            print_filename(stderr, file->pfilename);
            fprintf(stderr, "\n");
            exit(-4);
        }

        int next_track_pos = filepos + (num_sectors(type, track) * TRANSWARPBLOCKSIZE);
        bool last_track = (next_track_pos >= *filesize);
        if (last_track) {
            int sector = 0;
            for (int s = 0; s < num_sectors(type, track); ++s) {
                sectors[s] = sector;
                if ((filepos + ((sector + 1) * TRANSWARPBLOCKSIZE)) >= *filesize) {
                    sector = 0;
                } else {
                    ++sector;
                }
            }
        }
        int next_track_block_index = block_index + num_sectors(type, track);
        int trackpos = filepos;

        transwarp_encode_context trackctx = ctx;

        for (int sector = 0; sector < num_sectors(type, track); ++sector) {
            if (is_sector_free(type, image, track, sector, 0 /* numdirblocks */, 0 /* dir_sector_interleave */) == false) {
                fprintf(stderr, "ERROR: t%d/s%d not free for Transwarp file ", track, sector);
                print_filename(stderr, file->pfilename);
                fprintf(stderr, "\n");
                check_bam(type, image);

                exit(-5);
            }

            bool last_block = false;
            int pos = trackpos + (sectors[sector] * TRANSWARPBLOCKSIZE);
            if ((pos + TRANSWARPBLOCKSIZE) >= *filesize) {
                pos = *filesize - TRANSWARPBLOCKSIZE;
                last_block = true;
            }

            unsigned char encoded[320 + 5];
            unsigned char previous = block_index + sectors[sector];
            previous ^= initial_block_recvaccu_value;

            ctx.previous = previous;
            ctx.previous2 = initial_buffer_recvaccu_value;

            int error = encode_transwarp_block((const unsigned char (*)[256]) scramble, gcr_to_nibble, &ctx, filedata, pos, encoded);
            if (error) {
                fprintf(stderr, "ERROR: encoding error on t%d/s%d\n", track, sector);

                exit(-6);
            }

            unsigned char decoded[256];
            int checksum = decode_gcr_block(gcr_to_nibble, encoded, decoded);
            if (checksum < 0) {
                fprintf(stderr, "ERROR: decoding error on t%d/s%d\n", track, sector);

                exit(-7);
            }

            int offset = linear_sector(type, track, sector) * BLOCKSIZE;
            memcpy(image + offset, decoded, BLOCKSIZE);

            ++total_blocks;

            if (last_block) {
                ctx = trackctx;
                done = true;
            }

            mark_sector(type, image, track, sector, 0 /* not free */);
        }

        filepos = next_track_pos;
        block_index = next_track_block_index;
        file->last_track = track;
    }

    file->nrSectors = total_blocks;

    return dirdatakey;
}

/* Write files to disk */
static void
write_files(image_type type, unsigned char *image, imagefile *files, int num_files, int usedirtrack, int dirtracksplit, int shadowdirtrack, int numdirblocks, int dir_sector_interleave)
{
    unsigned char track = 1;
    unsigned char sector = 0;
    int bytes_to_write = 0;
    int lastTrack = track;
    int lastSector = sector;
    int lastOffset = linear_sector(type, lastTrack, lastSector) * BLOCKSIZE;
    int lastMinTrack = track;
    bool transwarp_bootfile_fits_on_dir_track = false;

    /* make sure the first file already takes first sector per track into account */
    if (num_files > 0) {
        sector = (type == IMAGE_D81) ? 0 : files[0].first_sector_new_track;
    }

    int transwarp_version = 100;

    for (int i = 0; i < num_files; i++) {
        imagefile *file = files + i;
        if ((file->mode & MODE_TRANSWARPBOOTFILE) != 0) {
            int fileSize = 0;

            struct stat st;
            if (stat((char*)files[i].alocalname, &st) == 0) {
                fileSize = (int)st.st_size;
            }

            int version_major;
            int version_minor;
            if (sscanf((char *) basename(files[i].alocalname), "transwarp v%d.%d", &version_major, &version_minor) == 2) {
                transwarp_version = (version_major * 100) + version_minor;
            }

            int num_blocks = (fileSize / 254) + ((fileSize % 254 == 0) ? 0 : 1);
            transwarp_bootfile_fits_on_dir_track = (num_blocks <= 17);

            break;
        }
    }

    for (int i = 0; i < num_files; i++) {
        imagefile *file = files + i;
        if (type == IMAGE_D81) {
            file->sectorInterleave = 1; /* caught in command line parsing anyway, but does not hurt */
        }

        int file_usedirtrack = usedirtrack;
        int file_numdirblocks = numdirblocks;

        if ((file->mode & MODE_NOFILE)) {
            if (file->nrSectorsShown == -1) {
                file->nrSectorsShown = file->nrSectors;
            }
            file->track = 0;
            file->sector = 0;
            int entryOffset = linear_sector(type, dirtrack(type), file->direntrysector) * BLOCKSIZE + file->direntryoffset;
            image[entryOffset + FILETRACKOFFSET] = file->track;
            image[entryOffset + FILESECTOROFFSET] = file->sector;
            image[entryOffset + FILEBLOCKSLOOFFSET] = file->nrSectorsShown & 255;
            image[entryOffset + FILEBLOCKSHIOFFSET] = file->nrSectorsShown >> 8;
            if (shadowdirtrack > 0) {
                entryOffset = linear_sector(type, shadowdirtrack, file->direntrysector) * BLOCKSIZE + file->direntryoffset;
                image[entryOffset + FILETRACKOFFSET] = file->track;
                image[entryOffset + FILESECTOROFFSET] = file->sector;
                image[entryOffset + FILEBLOCKSLOOFFSET] = file->nrSectors & 255;
                image[entryOffset + FILEBLOCKSHIOFFSET] = file->nrSectors >> 8;
            }
        } else if (!(file->mode & MODE_LOOPFILE)) { /* loop files are handled later */

            int fileSize = 0;

            struct stat st;
            if (stat((char*)files[i].alocalname, &st) == 0) {
                fileSize = (int)st.st_size;
            }

            if ((file->mode & MODE_TRANSWARPBOOTFILE) != 0) {
                file_usedirtrack = true;
                file_numdirblocks = transwarp_bootfile_fits_on_dir_track ? 2 : 4;
                file->sectorInterleave = -4;
                file->mode = (file->mode & ~MODE_BEGINNING_SECTOR_MASK) | (10 + 1);
                file->first_sector_new_track = 10;
                track = DIRTRACK_D41_D71;
            }

            unsigned char* filedata = (unsigned char*)calloc(fileSize + ((file->filetype & FILETYPETRANSWARPMASK) ? (21 * TRANSWARPBLOCKSIZE) : 0), sizeof(unsigned char));
            if (filedata == NULL) {
                fprintf(stderr, "ERROR: Memory allocation error\n");

                exit(-1);
            }
            FILE* f = fopen((char*)file->alocalname, "rb");
            if (f == NULL) {
                fprintf(stderr, "ERROR: Could not open file \"%s\" for reading\n", file->alocalname);

                exit(-1);
            }
            if (fread(filedata, fileSize, 1, f) != 1) {
                fprintf(stderr, "ERROR: Unexpected filesize when reading %s\n", file->alocalname);
                exit(-1);
            }
            fclose(f);

            if ((!(file->filetype & FILETYPETRANSWARPMASK))
                    && ((file->mode & MODE_MIN_TRACK_MASK) > 0)) {
                int minTrack = (file->mode & MODE_MIN_TRACK_MASK) >> MODE_MIN_TRACK_SHIFT;
                if (lastMinTrack != minTrack) {
                    lastMinTrack = minTrack;
                    track = minTrack;
                    /* note that track may be smaller than lastTrack now */
                    if (track > image_num_tracks(type)) {
                        fprintf(stderr, "ERROR: Invalid minimum track %u for file %s (", track, file->alocalname);
                        print_filename(stderr, file->pfilename);
                        fprintf(stderr, ") specified\n");

                        exit(-1);
                    }
                    while ((!file_usedirtrack)
                            && ((track == dirtrack(type))
                                || (track == shadowdirtrack)
                                || ((type == IMAGE_D71) && (track == (D64NUMTRACKS + dirtrack(type)))))) { /* .d71 track 53 is usually empty except the extra BAM block */
                        ++track; /* skip dir track */
                    }
                    if (abs(((int) track) - lastTrack) > 1) {
                        /* previous file's last track and this file's beginning track have tracks in between */
                        sector = (type == IMAGE_D81) ? 0 : file->first_sector_new_track;
                    }
                }
            } else {
                lastMinTrack = 0;
            }

            if ((file->mode & MODE_BEGINNING_SECTOR_MASK) > 0) {
                sector = (file->mode & MODE_BEGINNING_SECTOR_MASK) - 1;
            }

            if ((!(file->filetype & FILETYPETRANSWARPMASK))
                    && (((file->mode & MODE_SAVETOEMPTYTRACKS) != 0)
                        || ((file->mode & MODE_FITONSINGLETRACK) != 0))) {

                /* find first empty track */
                int found = 0;
                while (!found) {
                    for (int s = 0; s < num_sectors(type, track); s++) {
                        if (is_sector_free(type, image, track, s, file_usedirtrack ? file_numdirblocks : 0, dir_sector_interleave)) {
                            if (s == (num_sectors(type, track) - 1)) {
                                found = 1;
                                /* In first pass, use sector as left by previous file (or as set by -b) to reach first file block quickly. */
                                /* Claus: according to Krill, on real HW tracks are not aligned anyway, so it does not make a difference. */
                                /* Emulators tend to reset the disk angle on track changes, so this should rather be 3. */
                                if (sector >= num_sectors(type, track)) {
                                    if ((file->mode & MODE_BEGINNING_SECTOR_MASK) > 0) {
                                        fprintf(stderr, "ERROR: Invalid beginning sector %u on track %u for file %s (", sector, track, file->alocalname);
                                        print_filename(stderr, file->pfilename);
                                        fprintf(stderr, ") specified\n");

                                        exit(-1);
                                    }

                                    sector %= num_sectors(type, track);
                                }
                            }
                        } else {
                            int prev_track = track;
                            if (file->mode & MODE_SAVECLUSTEROPTIMIZED) {
                                if (track > D64NUMTRACKS) {
                                    int next_track = track - D64NUMTRACKS + 1; /* to next track on first side */
                                    if (next_track < D64NUMTRACKS) {
                                        track = next_track;
                                    } else {
                                        ++track; /* disk full */
                                    }
                                } else {
                                    track += D64NUMTRACKS; /* to same track on second side */
                                }
                            } else {
                                ++track;
                            }
                            while ((!file_usedirtrack)
                                    && ((track == dirtrack(type))
                                        || (track == shadowdirtrack)
                                        || ((type == IMAGE_D71) && (track == D64NUMTRACKS + dirtrack(type))))) { /* .d71 track 53 is usually empty except the extra BAM block */
                                ++track; /* skip dir track */
                            }
                            if (file->mode & MODE_FITONSINGLETRACK) {
                                int file_size = fileSize;
                                int first_sector = -1;
                                for (int s = 0; s < num_sectors(type, prev_track); s++) {
                                    if (is_sector_free(type, image, prev_track, s, file_usedirtrack ? file_numdirblocks : 0, dir_sector_interleave)) {
                                        if (first_sector < 0) {
                                            first_sector = s;
                                        }
                                        file_size -= BLOCKSIZE + BLOCKOVERHEAD;
                                        if (file_size <= 0) {
                                            found = 1;
                                            track = prev_track;
                                            sector = first_sector;
                                            break;
                                        }
                                    }
                                }
                            }

                            if (track > image_num_tracks(type)) {
                                fprintf(stderr, "ERROR: Disk full, file %s (", file->alocalname);
                                print_filename(stderr, file->pfilename);
                                fprintf(stderr, ")\n");

                                exit(-1);
                            }
                            break;
                        }
                    } /* for each sector on track */

                    if ((track == (lastTrack + 2))
                            && ((file->mode & MODE_BEGINNING_SECTOR_MASK) == 0)) {
                        /* previous file's last track and this file's beginning track have tracks in between now */
                        sector = 0;
                    }
                } /* while not found */
            }

            if ((file->mode & MODE_BEGINNING_SECTOR_MASK) > 0) {
                if (sector != ((file->mode & MODE_BEGINNING_SECTOR_MASK) - 1)) {
                    fprintf(stderr, "ERROR: Specified beginning sector of file %s (", file->alocalname);
                    print_filename(stderr, file->pfilename);
                    fprintf(stderr, ") not free on track %u\n", track);

                    exit(-1);
                }
            }

            /* found start track, now save file */
            if (type == IMAGE_D81) {
                sector = 0;
            }

            int byteOffset = 0;
            int bytesLeft = fileSize;

            unsigned long long key0 = 0;
            if (file->filetype & FILETYPETRANSWARPMASK) {
                key0 = write_transwarp_file(type, image, file, filedata, &fileSize, transwarp_version, transwarp_bootfile_fits_on_dir_track);

                bytesLeft = 0;
            }

            while (bytesLeft > 0) {
                /* Find free track & sector, starting from current T/S forward one revolution, then the next track etc... skip dirtrack (unless -t is active) */
                /* If the file didn't fit before dirtrack then restart on dirtrack + 1 and try again (unless -t is active). */
                /* If the file didn't fit before track 36/41/71 then the disk is full. */

                int blockfound = 0;
                int findSector = 0;

                while (!blockfound) {
                    /* find spare block on the current track */
                    for (int s = sector; s < sector + num_sectors(type, track); s++) {
                        findSector = s % num_sectors(type, track);

                        if (is_sector_free(type, image, track, findSector, file_usedirtrack ? file_numdirblocks : 0, dir_sector_interleave)) {
                            blockfound = 1;
                            break;
                        }
                    }

                    if (!blockfound) {
                        /* find next track, use some magic to make up for track seek delay */
                        int prev_track = track;
                        int seek_delay = 1;
                        if (file->mode & MODE_SAVECLUSTEROPTIMIZED) {
                            if (track > D64NUMTRACKS) {
                                track = track - D64NUMTRACKS + 1;
                            } else {
                                track += D64NUMTRACKS;
                                seek_delay = 0; /* switching to the other side, no head movement */
                            }
                        } else {
                            ++track;
                        }
                        if (type == IMAGE_D81) {
                            sector = 0;
                        } else if (file->first_sector_new_track < 0) {
                            sector += seek_delay - 1;
                        } else if ((file->sectorInterleave < 0)
                                   && ((file->mode & MODE_TRANSWARPBOOTFILE) == 0)) {
                            sector += seek_delay;
                        } else {
                            sector = file->first_sector_new_track;
                        }
                        sector += num_sectors(type, prev_track);
                        sector %= num_sectors(type, prev_track);

                        while ((!file_usedirtrack)
                                && ((track == dirtrack(type))
                                    || (track == shadowdirtrack)
                                    || ((type == IMAGE_D71) && (track == D64NUMTRACKS + dirtrack(type))))) { /* .d71 track 53 is usually empty except the extra BAM block */
                            /* Delete old fragments and restart file */
                            if (!dirtracksplit) {
                                if (file->nrSectors > 0) {
                                    int deltrack = file->track;
                                    int delsector = file->sector;
                                    while (deltrack != 0) {
                                        int b = linear_sector(type, deltrack, delsector);
                                        if(b < 0) {
                                            break;
                                        }
                                        int offset = b * BLOCKSIZE;
                                        mark_sector(type, image, deltrack, delsector, 1 /* free */);
                                        deltrack = image[offset + 0];
                                        delsector = image[offset + 1];
                                        memset(image + offset, 0, BLOCKSIZE);
                                    }
                                }

                                bytesLeft = fileSize;
                                byteOffset = 0;
                                file->nrSectors = 0;
                            }
                            ++track;
                        }

                        if (track > image_num_tracks(type)) {
                            if (verbose) {
                                print_file_allocation(type, image, files, num_files);
                                check_bam(type, image);
                            }

                            fprintf(stderr, "ERROR: Disk full, file %s (", file->alocalname);
                            print_filename(stderr, file->pfilename);
                            fprintf(stderr, ")\n");
                            free(filedata);

                            exit(-1);
                        }
                    }
                } /* while not block found */

                sector = findSector;
                int offset = linear_sector(type, track, sector) * BLOCKSIZE;

                if (bytesLeft == fileSize) {
                    file->track = track;
                    file->sector = sector;
                    lastTrack = track;
                    lastSector = sector;
                    lastOffset = offset;
                } else {
                    image[lastOffset + 0] = track;
                    image[lastOffset + 1] = sector;
                }

                /* write sector */
                bytes_to_write = min(BLOCKSIZE - BLOCKOVERHEAD, bytesLeft);
                memcpy(image + offset + 2, filedata + byteOffset, bytes_to_write);

                bytesLeft -= bytes_to_write;
                byteOffset += bytes_to_write;

                lastTrack = track;
                lastSector = sector;
                lastOffset = offset;

                mark_sector(type, image, track, sector, 0 /* not free */);

                if (num_sectors(type, track) <= abs(file->sectorInterleave)) {
                    fprintf(stderr, "ERROR: Invalid interleave %d on track %u (%d sectors), file %s (", file->sectorInterleave, track, num_sectors(type, track), file->alocalname);
                    print_filename(stderr, file->pfilename);
                    fprintf(stderr, ")\n");

                    exit(-1);
                }

                sector += abs(file->sectorInterleave);
                if (sector >= num_sectors(type, track)) {
                    sector -= num_sectors(type, track);
                    if ((file->sectorInterleave >= 0) && (sector > 0)) {
                        --sector; /* subtract one after wrap (supposedly due to large tail gap) */
                    }
                }

                file->nrSectors++;
            } /* while bytes left */

            if (!(file->filetype & FILETYPETRANSWARPMASK)) {
                image[lastOffset + 0] = 0x00;
                image[lastOffset + 1] = bytes_to_write + 1;
            }

            /* update directory entry */
            int entryOffset = linear_sector(type, dirtrack(type), file->direntrysector) * BLOCKSIZE + file->direntryoffset;
            image[entryOffset + FILETRACKOFFSET] = file->track;
            image[entryOffset + FILESECTOROFFSET] = file->sector;

            if (file->filetype & FILETYPETRANSWARPMASK) {
                image[entryOffset + FILETRACKOFFSET] = 0;
                image[entryOffset + FILESECTOROFFSET] = 0;

                image[entryOffset + TRANSWARPSIGNATROFFSLO] = TRANSWARPSIGNATURELO;
                image[entryOffset + TRANSWARPSIGNATROFFSHI] = TRANSWARPSIGNATUREHI;

                image[entryOffset + TRANSWARPTRACKOFFSET] = file->track;

                image[entryOffset + FILEBLOCKSLOOFFSET] = file->nrSectors;
                image[entryOffset + FILEBLOCKSHIOFFSET] = (file->nrSectors >> 8);
                if (image[entryOffset + FILEBLOCKSHIOFFSET] > 0) {
                    fprintf(stderr, "ERROR: Transwarp file \"%s\" is %d > 255 blocks big\n", file->alocalname, file->nrSectors);

                    exit(-8);
                }

                int loadaddress = (filedata[1] << 8) | filedata[0];
                image[entryOffset + LOADADDRESSLOOFFSET] = loadaddress;
                image[entryOffset + LOADADDRESSHIOFFSET] = (loadaddress >> 8);
                int endaddress = loadaddress + fileSize - 2;
                image[entryOffset + ENDADDRESSLOOFFSET] = endaddress;
                image[entryOffset + ENDADDRESSHIOFFSET] = (endaddress >> 8);

                unsigned char file_checksum = 0xff;
                for (int i = 2; i < fileSize; ++i) {
                    file_checksum ^= filedata[i];
                    file_checksum = crc8(file_checksum);
                }
                image[entryOffset + FILECHECKSUMOFFSET] = file_checksum;

                image[entryOffset + DIRDATACHECKSUMOFFSET] = 0;
                image[entryOffset + DIRDATACHECKSUMOFFSET] = (0x0100 - transwarp_dirdata_checksum(image, entryOffset));
                unsigned char dirdata_checksum = transwarp_dirdata_checksum(image, entryOffset);
                if (dirdata_checksum != 0) {
                    if (dirdata_checksum == 1) {
                        --image[entryOffset + DIRDATACHECKSUMOFFSET];
                    }
                    dirdata_checksum = transwarp_dirdata_checksum(image, entryOffset);
                    if (dirdata_checksum != 0) {
                        fprintf(stderr, "ERROR: Encoding error with \"%s\", 0x%x\n", file->alocalname, dirdata_checksum);

                        exit(-9);
                    }
                }

                if (file->have_key != 0) {
                    for (int offset = DIRDATACHECKSUMOFFSET; offset <= FILEBLOCKSLOOFFSET; ++offset) {
                        image[entryOffset + offset] ^= key0;
                        key0 >>= 8;
                    }

                    file->nrSectors = image[entryOffset + FILEBLOCKSLOOFFSET];
                }
            }

            if (file->nrSectorsShown < 0) {
                file->nrSectorsShown = file->nrSectors;
            }
            image[entryOffset + FILEBLOCKSLOOFFSET] = file->nrSectorsShown & 255;
            image[entryOffset + FILEBLOCKSHIOFFSET] = file->nrSectorsShown >> 8;

            if (shadowdirtrack > 0) {
                entryOffset = linear_sector(type, shadowdirtrack, file->direntrysector) * BLOCKSIZE + file->direntryoffset;
                image[entryOffset + FILETRACKOFFSET] = file->track;
                image[entryOffset + FILESECTOROFFSET] = file->sector;

                image[entryOffset + FILEBLOCKSLOOFFSET] = file->nrSectors & 255;
                image[entryOffset + FILEBLOCKSHIOFFSET] = file->nrSectors >> 8;
            }

            free(filedata);
        }
    } /* for each file */

    /* Set track/sector of Transwarp file entries to Transwarp bootfile */
    int transwarp_boot_track = 0;
    int transwarp_boot_sector = 0;
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }

    for (int i = 0; i < num_files; i++) {
        imagefile *file = files + i;
        if (file->filetype & FILETYPETRANSWARPMASK) {
            if (transwarp_boot_track == 0) {
                /* find Transwarp bootfile */

                int t = dirtrack(type);
                int s = (type == IMAGE_D81) ? 3 : 1;
                int o = 0;

                do {
                    int b = linear_sector(type, t, s) * BLOCKSIZE + o;
                    int filetype = image[b + FILETYPEOFFSET] & 0xf;
                    if (filetype == FILETYPEDEL) {
                        continue;
                    }

                    if (is_transwarp_file(image, b)) {
                        continue;
                    }

                    if (!is_transwarp_bootfile(image, b)) {
                        continue;
                    }

                    int filetrack = image[b + FILETRACKOFFSET];
                    int filesector = image[b + FILESECTOROFFSET];
                    if (filetrack == 0) {
                        continue;
                    }

                    if (verbose) {
                        printf("\nTranswarp bootfile at T%d/S%d\n", filetrack, filesector);
                    }

                    transwarp_boot_track = filetrack;
                    transwarp_boot_sector = filesector;

                    break;
                } while (next_dir_entry(type, image, &t, &s, &o, blockmap));
            }

            if (transwarp_boot_track == 0) {
                fprintf(stderr, "ERROR: No Transwarp bootfile provided\n");

                exit(-10);
            }

            int b = linear_sector(type, dirtrack(type), file->direntrysector) * BLOCKSIZE + file->direntryoffset;
            image[b + FILETRACKOFFSET] = transwarp_boot_track;
            image[b + FILESECTOROFFSET] = transwarp_boot_sector;
        }
    }
    free(blockmap);

    /* update loop files */
    for (int i = 0; i < num_files; i++) {
        imagefile *file = files + i;
        if (((file->filetype & 0xf) != FILETYPEDEL) && (file->mode & MODE_LOOPFILE)) {
            int track, sector, offset;
            int index;
            if (find_existing_file(type, image, file->plocalname, &index, &track, &sector, &offset)) {
                /* read track/sector and nrSectors from disk image */
                int b = linear_sector(type, track, sector) * BLOCKSIZE + offset;
                file->track = image[b + FILETRACKOFFSET];
                file->sector = image[b + FILESECTOROFFSET];
                file->nrSectors = image[b + FILEBLOCKSLOOFFSET] + (image[b + FILEBLOCKSHIOFFSET] << 8);

                /* update directory entry */
                b = linear_sector(type, dirtrack(type), file->direntrysector) * BLOCKSIZE + file->direntryoffset;
                image[b + FILETRACKOFFSET] = file->track;
                image[b + FILESECTOROFFSET] = file->sector;

                if (file->nrSectorsShown == -1) {
                    file->nrSectorsShown = file->nrSectors;
                }
                image[b + FILEBLOCKSLOOFFSET] = file->nrSectorsShown & 255;
                image[b + FILEBLOCKSHIOFFSET] = file->nrSectorsShown >> 8;

                if (shadowdirtrack > 0) {
                    b = linear_sector(type, shadowdirtrack, file->direntrysector) * BLOCKSIZE + file->direntryoffset;
                    image[b + FILETRACKOFFSET] = file->track;
                    image[b + FILESECTOROFFSET] = file->sector;

                    image[b + FILEBLOCKSLOOFFSET] = file->nrSectors & 255;
                    image[b + FILEBLOCKSHIOFFSET] = file->nrSectors >> 8;
                }

                for (int j = 0; j < num_files; j++) {
                    imagefile *other_file = files + j;
                    if ((i != j)
                            && (file->track == other_file->track)
                            && (file->sector == other_file->sector)) {
                        file->sectorInterleave = other_file->sectorInterleave;

                        break;
                    }
                }

                continue;
            } else {
                fprintf(stderr, "ERROR: Loop source file '%s' (%d) not found\n", file->alocalname, i + 1);
                exit(-1);
            }
        }
    }
}

/* Writes 16 bit value to file */
static size_t
write16(unsigned int value, FILE* f)
{
    char byte = value & 0xff;
    size_t bytes_written = fwrite(&byte, 1, 1, f);

    byte = (value >> 8) & 0xff;
    bytes_written += fwrite(&byte, 1, 1, f);

    return bytes_written;
}

/* Writes 32 bit value to file */
static size_t
write32(unsigned int value, FILE* f)
{
    size_t bytes_written = write16(value, f);
    bytes_written += write16(value >> 16, f);

    return bytes_written;
}

/* Writes image as G64 file */
static int
generate_uniformat_g64(unsigned char* image, const char *imagepath)
{
    FILE* f = fopen(imagepath, "wb");

    size_t filepos = 0;

    static const char signature[] = "GCR-1541";
    filepos += fwrite(signature, 1, sizeof signature - 1, f);

    const char version = 0;
    filepos += fwrite(&version, 1, 1, f);

    const char num_tracks = 35 * 2;
    filepos += fwrite(&num_tracks, 1, 1, f);

    const unsigned int track_size = 7692; /* = track_bytes on tracks 1..17 */
    filepos += write16(track_size, f);

    const unsigned int table_size = num_tracks * 4;
    const unsigned int tracks_offset = (int)filepos + (table_size * 2);

    for (int track = 0; track < num_tracks; ++track) {
        unsigned int track_offset = 0;

        if ((track & 1) == 0) {
            track_offset = tracks_offset + ((track >> 1) * (2 + track_size));
        }

        filepos += write32(track_offset, f);
    }

    for (int track = 0; track < num_tracks; ++track) {
        unsigned int bit_rate = 0;

        if ((track & 1) == 0) {
            switch (sectors_per_track[track >> 1]) {
            case 21:
                bit_rate = 3;
                break;
            case 19:
                bit_rate = 2;
                break;
            case 18:
                bit_rate = 1;
                break;
            case 17:
                bit_rate = 0;
                break;
            }
        }

        filepos += write32(bit_rate, f);
    }

    const unsigned char sync[] = { 0xff, 0xff, 0xff, 0xff, 0xff };
    const char gap[] = { 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };
    char header_gcr[10];

    const unsigned int block_size =
        (sizeof sync)
        + (sizeof header_gcr)
        + (sizeof gap)
        + (sizeof sync)
        + 325; /* data */

    const char id[3] = { '2', 'A', '\0' };

    bool is_uniform = true;

    for (int track = 0; track < (num_tracks >> 1); ++track) {
        int track_bytes = 0;
        int num_sectors = sectors_per_track[track];
        switch (num_sectors) {
        case 21:
            track_bytes = 7692;
            break; /* = track_size */
        case 19:
            track_bytes = 7142;
            break;
        case 18:
            track_bytes = 6666;
            break;
        case 17:
            track_bytes = 6250;
            break;
        }

        filepos += write16(track_bytes, f);
        size_t track_begin = filepos;

        int data_bytes = num_sectors * block_size;
        int gap_size = (track_bytes - data_bytes) / num_sectors;
        if (gap_size < 0) {
            printf("\nERROR: Track too small for G64 output\n");
            fclose(f);

            return -1;
        }

        float average_gap_remainder = (((float) (track_bytes - data_bytes)) / num_sectors) - gap_size;
        if (average_gap_remainder >= 1.0f) {
            average_gap_remainder = 0.0f; /* 0..1 */
        }

        float remainder = 0.0f;
        for (int sector = 0; sector < num_sectors; ++sector) {
            unsigned int gap_bytes = gap_size;
            remainder += average_gap_remainder;
            if (remainder >= 0.5f) {
                remainder -= 1.0f;
                ++gap_bytes;
            }

            filepos += fwrite(sync, 1, sizeof sync, f);
            char header[8] = {
                0x08, /* header ID */
                (char) (sector ^ (track + 1) ^ id[1] ^ id[0]), /* checksum */
                (char) sector,
                (char) (track + 1),
                id[1],
                id[0],
                0x0f, 0x0f
            };

            encode_4_bytes_gcr(header, header_gcr);
            encode_4_bytes_gcr(header + 4, header_gcr + 5);

            filepos += fwrite(header_gcr, 1, sizeof header_gcr, f);
            filepos += fwrite(gap, 1, sizeof gap, f);

            filepos += fwrite(sync, 1, sizeof sync, f);

            char group[5];

            char checksum = image[0] ^ image[1] ^ image[2];
            char data[4] = { 0x07, (char) image[0], (char) image[1], (char) image[2] };
            encode_4_bytes_gcr(data, group);
            filepos += fwrite(group, 1, sizeof group, f);
            for (int i = 0; i < 0x3f; ++i) {
                data[0] = image[(i * 4) + 3];
                data[1] = image[(i * 4) + 4];
                data[2] = image[(i * 4) + 5];
                data[3] = image[(i * 4) + 6];
                encode_4_bytes_gcr(data, group);
                filepos += fwrite(group, 1, sizeof group, f);
                checksum ^= (data[0] ^ data[1] ^ data[2] ^ data[3]);
            }
            data[0] = image[0xff];
            data[1] = data[0] ^ checksum;
            data[2] = 0;
            data[3] = 0;
            encode_4_bytes_gcr(data, group);
            filepos += fwrite(group, 1, sizeof group, f);

            for (int i = gap_bytes; i > 0; --i) {
                filepos += fwrite(gap, 1, 1, f);
            }

            image += 0x0100;
        } /* for each sector */

        size_t tail_gap = track_bytes - filepos + track_begin;
        if (tail_gap > 0) {
            for (size_t i = tail_gap; i > 0; --i) {
                filepos += fwrite(gap, 1, 1, f);
            }

            is_uniform = false;
        }

        for (int i = (track_size - track_bytes); i > 0; --i) {
            filepos += fwrite(sync, 1, 1, f);
        }
    } /* for each track */

    fclose(f);

    if (!is_uniform) {
        printf("\nWARNING: \"%s\" is not UniFormAt'ed\n", imagepath);
    }

    return 0;
}

/* Generates a unique filename, either based on the proposed name, or using track and sector. */
static void
generate_unique_filename(image_type type, unsigned char *image, unsigned char *name, int track, int sector, int start, char marker)
{
    int i, t, s, o;
    if(name[0] == 0xa0 || name[0] == 0) {
        /* no name provided, create one */
        name[0] = a2p('t');
        pputnum2(name+1, track);
        name[3] = a2p('s');
        pputnum2(name+4, sector);
        name[6] = a2p('$');
        pputhex(name+7, start);
        for(int pos = 11; pos < FILENAMEMAXSIZE; pos++) {
            name[pos] = 0xa0;
        }
    }
    int marker_pos = pstrlen(name);
    if(marker_pos > FILENAMEMAXSIZE-1) {
        marker_pos = FILENAMEMAXSIZE-1;
    }
    if(marker) {
        name[marker_pos] = marker;
    } else {
        marker_pos++;
    }
    int appendix = 1;
    int appendix_len = 2;
    int namelen = pstrlen(name);
    while(find_existing_file(type, image, name, &i, &t, &s, &o)) {
        marker_pos = namelen + appendix_len;
        if(marker_pos > FILENAMEMAXSIZE-1) {
            marker_pos = FILENAMEMAXSIZE-1;
            if(!marker) {
                marker_pos++;
            }
        }
        if(marker) {
            name[marker_pos] = marker;
        }
        int appendix_pos = marker_pos - appendix_len;
        name[appendix_pos] = '.';
        pputnum(name + appendix_pos + 1, appendix);
        appendix++;
        if(appendix == 10) {
            appendix_len++;
        }
        if(appendix == 100) {
            appendix_len++;
        }
    }
}

/* Count number of blocks in file, assumes valid t/s chain */
static int
count_blocks(image_type type, unsigned char *image, unsigned int track, int sector)
{
    int num = 0;
    while(track != 0) {
        int b = linear_sector(type, track, sector);
        if(b < 0) {
            break;
        }
        int block_offset = b * BLOCKSIZE;
        track = image[block_offset + TRACKLINKOFFSET];
        sector = image[block_offset + SECTORLINKOFFSET];
        num++;
    };
    return num;
}

/* Overwrites all blocks that are marked as potentially allocated with the given value */
static void
mark_sector_chain(image_type type, unsigned char *image, char* atab, unsigned int track, int sector, unsigned int last_track, int last_sector, int mark)
{
    if(track <= 0 || track > image_num_tracks(type) || sector < 0 || sector > num_sectors(type, track)) {
        return;
    }
    while(1) {
        int b = linear_sector(type, track, sector);
        atab[b] = mark;
        if(track == last_track && sector == last_sector) {
            break;
        }
        int block_offset = b * BLOCKSIZE;
        track = image[block_offset + TRACKLINKOFFSET];
        sector = image[block_offset + SECTORLINKOFFSET];
    };
}

/* Validates sector chain starting at track/sector, returns how and in which t/s the chain ends */
static int
validate_sector_chain(image_type type, unsigned char* image, char* atab, unsigned int track, int sector, unsigned int *last_track, int *last_sector)
{
    *last_track = track;
    *last_sector = sector;
    if(track == 0 || track > image_num_tracks(type)) {
        return FIRST_BROKEN;
    }
    if(sector >= num_sectors(type, track)) {
        return FIRST_BROKEN;
    }
    if(atab[linear_sector(type, track, sector)] != UNALLOCATED) {
        return FIRST_BROKEN;
    }
    while (1) {
        atab[linear_sector(type, track, sector)] = POTENTIALLYALLOCATED;
        int block_offset = linear_sector(type, track, sector) * BLOCKSIZE;
        unsigned int next_track = image[block_offset + TRACKLINKOFFSET];
        int next_sector = image[block_offset + SECTORLINKOFFSET];
        if (next_track == 0) {
            return VALID; /* end of valid chain */
        }
        if (next_track > image_num_tracks(type)) {
            return ILLEGAL_TRACK;
        }
        if (next_sector >= num_sectors(type, next_track)) {
            return ILLEGAL_SECTOR;
        }
        switch(atab[linear_sector(type, next_track, next_sector)]) {
        case POTENTIALLYALLOCATED:
            return LOOP;
        case ALLOCATED:
            return COLLISION;
        case FILESTART:
            return CHAINED;
        case FILESTART_TRUNCATED:
            return CHAINED_TRUNCATED;
        }
        track = next_track;
        sector = next_sector;
        *last_track = track;
        *last_sector = sector;
    }
}

static void
init_atab(image_type type, unsigned char* image, char* atab)
{
    int dt = dirtrack(type);
    int ds = (type == IMAGE_D81) ? 3 : 1;
    int offset = 0;
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }

    do {
        int db = linear_sector(type, dt, ds);
        atab[db] = ALLOCATED;
        int filetype = image[db * BLOCKSIZE + offset + FILETYPEOFFSET] & 0xf;
        if(filetype != FILETYPEDEL) {
            int track = image[db * BLOCKSIZE + offset + FILETRACKOFFSET];
            int sector = image[db * BLOCKSIZE + offset + FILESECTOROFFSET];
            unsigned int last_track;
            int last_sector;
            int error = validate_sector_chain(type, image, atab, track, sector, &last_track, &last_sector);
            if(error != VALID) {
                printf("WARNING: file ");
                print_filename(stdout, &image[db * BLOCKSIZE + offset + FILENAMEOFFSET]);
                printf(" seems corrupt (%s)\n", error_name[error]);
            }
            mark_sector_chain(type, image, atab, track, sector, last_track, last_sector, ALLOCATED);
        }
    } while(next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);
}

/* Write atab into BAM */
static void
write_atab(image_type type, unsigned char* image, char* atab)
{
    for(unsigned int t = 1; t <= image_num_tracks(type); t++) {
        for(int s = 0; s < num_sectors(type, t); s++) {
            int b = linear_sector(type, t, s);
            int free = (atab[b] == UNALLOCATED);
            if(!free) {
                /* keep allocated BAM entries, only add new ones */
                mark_sector(type, image, t, s, 0);
            }
        }
    }
}

/* Try to undelete a file given the directory entry, returns true if successful */
static bool
undelete_file(image_type type, unsigned char* image, int dt, int ds, int offset, char* atab, int level)
{
    unsigned int last_track;
    int last_sector;
    char marker = 0;
    int dirblock = linear_sector(type, dt, ds) * BLOCKSIZE;
    int track = image[dirblock + offset + FILETRACKOFFSET];
    int sector = image[dirblock + offset + FILESECTOROFFSET];
    int error = validate_sector_chain(type, image, atab, track, sector, &last_track, &last_sector);
    if(error == VALID || ((level == RESTORE_DIR_ONLY || level >= RESTORE_INVALID_FILES) && error != FIRST_BROKEN)) {
        unsigned char name[17];
        if(error != VALID) {
            if(level != RESTORE_DIR_ONLY) {
                /* terminate chain */
                int block_offset = linear_sector(type, last_track, last_sector) * BLOCKSIZE;
                image[block_offset + TRACKLINKOFFSET] = 0;
                marker = '<';
            }
        }
        /* restore dir entry */
        memcpy(&name, &image[dirblock + offset + FILENAMEOFFSET], 16);
        name[16] = 0;
        int b = linear_sector(type, track, sector);
        int address = image[b * BLOCKSIZE + 2] + 256 * image[b * BLOCKSIZE + 3];
        generate_unique_filename(type, image, name, track, sector, address, marker);
        memcpy(&image[dirblock + offset + FILENAMEOFFSET], &name, 16);
        image[dirblock + offset + FILETYPEOFFSET] = 0x82; /* original file type is lost, use closed PRG instead */
        mark_sector_chain(type, image, atab, track, sector, last_track, last_sector, ALLOCATED);
        if(level != RESTORE_DIR_ONLY) {
            int size = count_blocks(type, image, track, sector);
            image[dirblock + offset + FILEBLOCKSHIOFFSET] = size / 256;
            image[dirblock + offset + FILEBLOCKSLOOFFSET] = size % 256;
        }
        return true;
    }
    if(error != FIRST_BROKEN) {
        mark_sector_chain(type, image, atab, track, sector, last_track, last_sector, UNALLOCATED);
    }
    return false;
}

/* search for scratched directory entries and restore them */
static int
undelete(image_type type, unsigned char* image, char* atab, int level)
{
    int dt = dirtrack(type);
    int ds = (type == IMAGE_D81) ? 3 : 1;
    int nsectors = num_sectors(type, dt);
    int offset = 0;
    int num_undeleted = 0;
    int final_dt, final_ds; /* last linked directory sector and track */
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }

    bool* searched = calloc(nsectors, sizeof(bool));
    if(searched == NULL) {
        fprintf(stderr, "ERROR: error allocating memory");
        exit(-1);
    }

    /* go through directory sector chain */
    do {
        if(dt == dirtrack(type)) {
            searched[ds] = true;
        }
        final_dt = dt;
        final_ds = ds;
        int db = linear_sector(type, dt, ds);
        mark_sector(type, image, dt, ds, 0); /* needs to be updated in BAM, so that dir allocation works correctly later */
        atab[db] = ALLOCATED;
        int dirblock = db * BLOCKSIZE;
        int filetype = image[dirblock + offset + FILETYPEOFFSET];
        if(filetype == FILETYPEDEL && image[dirblock + offset + FILENAMEOFFSET] != 0 && undelete_file(type, image, dt, ds, offset, atab, level)) { /* filename starting with 0 means that likely there was no file */
            num_undeleted++;
        }
    } while(next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);

    /* search for unlinked dir sectors */
    dt = dirtrack(type);
    for(ds = (type == IMAGE_D81) ? 3 : 1; ds < nsectors; ds++) {
        if(!searched[ds]) {
            int found = 1;
            for(offset = 0; offset < DIRENTRIESPERBLOCK*DIRENTRYSIZE; offset += DIRENTRYSIZE) {
                if(!undelete_file(type, image, dt, ds, offset, atab, level)) {
                    found = 0;
                    break;
                }
            }
            if(found) {
                num_undeleted += 8;
                if(!quiet) {
                    printf("Relinking directory sector %d\n", ds);
                }
                /* link found dir sector */
                int block_offset = linear_sector(type, final_dt, final_ds) * BLOCKSIZE;
                image[block_offset + TRACKLINKOFFSET] = dt;
                image[block_offset + SECTORLINKOFFSET] = ds;
                /* terminate found dir sector */
                int db = linear_sector(type, dt, ds);
                block_offset = db * BLOCKSIZE;
                image[block_offset + TRACKLINKOFFSET] = 0;
                final_dt = dt;
                final_ds = ds;
                mark_sector(type, image, dt, ds, 0); /* needs to be updated in BAM, so that dir allocation works correctly later */
                atab[db] = ALLOCATED;
            }
        }
    }
    free(searched);
    write_atab(type, image, atab);
    return num_undeleted;
}

/* add new DIR entries for wild chains */
static void
add_wild_to_dir(image_type type, unsigned char* image, char* atab, imagefile files[])
{
    /* create a DIR entry for each FILESTART */
    for(unsigned int t = 1; t <= image_num_tracks(type); t++) {
        for(int s = 0; s < num_sectors(type, t); s++) {
            int b = linear_sector(type, t, s);
            if(atab[b] == FILESTART || atab[b] == FILESTART_TRUNCATED) {
                char marker = atab[b] == FILESTART ? 0 : '<';
                unsigned char name[17];
                name[0] = 0xa0;
                int dir_index, dir_sector, dir_offset;
                atab[b] = ALLOCATED;
                new_dir_slot(type, image, (type == IMAGE_D81 ? 1 : 3), 0, &dir_index, &dir_sector, &dir_offset, files); /* TODO: handle full directory more gracefully */
                int db = linear_sector(type, dirtrack(type), dir_sector);
                atab[db] = ALLOCATED; /* make sure that potentially new dir block is marked as used */
                int offset = db * BLOCKSIZE + dir_offset;
                image[offset + FILETYPEOFFSET] = 0x82; /* closed PRG */
                image[offset + FILETRACKOFFSET] = t;
                image[offset + FILESECTOROFFSET] = s;
                image[offset + FILENAMEOFFSET] = 0xa0; /* no proposed filename */
                int address = image[b * BLOCKSIZE + 2] + 256 * image[b * BLOCKSIZE + 3];
                generate_unique_filename(type, image, name, t, s, address, marker);
                memcpy(&image[offset + FILENAMEOFFSET], name, 16);
                int size = count_blocks(type, image, t, s);
                image[offset + FILEBLOCKSHIOFFSET] = size / 256;
                image[offset + FILEBLOCKSLOOFFSET] = size % 256;
            }
        }
    }
}

/* search for wild valid chains of unallocated sectors */
static int
undelete_wild(image_type type, unsigned char* image, char* atab, int level, imagefile files[])
{
    int num_undeleted = 0;
    int max_bam_sector = (type == IMAGE_D81) ? 2 : 0;
    unsigned int dt = dirtrack(type);
    /* search for well terminated sector chains */
    for(unsigned int t = 1; t <= image_num_tracks(type); t++) {
        for(int s = 0; s < num_sectors(type, t); s++) {
            int b = linear_sector(type, t, s);
            if(atab[b] == UNALLOCATED && (t != dt || s > max_bam_sector)) { /* ignore bam sectors */
                unsigned int last_track;
                int last_sector;
                int error = validate_sector_chain(type, image, atab, t, s, &last_track, &last_sector);
                int last_block = linear_sector(type, last_track, last_sector);
                if(error == CHAINED || error == CHAINED_TRUNCATED) {
                    mark_sector_chain(type, image, atab, t, s, last_track, last_sector, ALLOCATED);
                    atab[b] = (error == CHAINED) ? FILESTART : CHAINED_TRUNCATED;
                    int chained_track = image[last_block * BLOCKSIZE + TRACKLINKOFFSET];
                    int chained_sector = image[last_block * BLOCKSIZE + SECTORLINKOFFSET];
                    int chained_block = linear_sector(type, chained_track, chained_sector);
                    atab[chained_block] = ALLOCATED; /* overwrite FILESTART */
                } else if(error == VALID) {
                    if(last_track == t && last_sector == s) {
                        if(level == RESTORE_INVALID_SINGLES) {
                            /* single block files should have all 0 after file end */
                            bool valid = false;
                            int last_byte = image[last_block * BLOCKSIZE + SECTORLINKOFFSET];
                            if(image[last_block * BLOCKSIZE + TRACKLINKOFFSET] == 0 && last_byte >= 2) {
                                valid = true;
                                for(int i = last_byte + 1; i <= 255; i++) {
                                    if(image[last_block * BLOCKSIZE + i] != 0) {
                                        valid = false;
                                        break;
                                    }
                                }
                            }
                            if(valid) {
                                atab[b] = FILESTART;
                                num_undeleted++;
                            } else {
                                atab[b] = UNALLOCATED;
                            }
                            atab[b] = valid ? FILESTART : UNALLOCATED;
                        } else {
                            atab[b] = UNALLOCATED;
                        }
                    } else {
                        mark_sector_chain(type, image, atab, t, s, last_track, last_sector, ALLOCATED);
                        atab[b] = FILESTART;
                        image[last_block * BLOCKSIZE + TRACKLINKOFFSET] = 0;
                        num_undeleted++;
                    }
                } else {
                    if(error != FIRST_BROKEN) {
                        mark_sector_chain(type, image, atab, t, s, last_track, last_sector, UNALLOCATED);
                    }
                }
            }
        }
    }
    if(num_undeleted) {
        write_atab(type, image, atab);
        add_wild_to_dir(type, image, atab, files);
    }
    return num_undeleted;
}

/* search for wild invalid chains of unallocated sectors and fix them */
static int
undelete_fix_wild(image_type type, unsigned char* image, char* atab, imagefile files[])
{
    int num_undeleted = 0;
    int max_bam_sector = (type == IMAGE_D81) ? 2 : 0;
    unsigned int dt = dirtrack(type);
    /* search for broken sector chains */
    for(unsigned int t = 1; t <= image_num_tracks(type); t++) {
        for(int s = 0; s < num_sectors(type, t); s++) {
            int b = linear_sector(type, t, s);
            if(atab[b] == UNALLOCATED && (t != dt || s > max_bam_sector)) { /* ignore bam sectors */
                unsigned int last_track;
                int last_sector;
                int error = validate_sector_chain(type, image, atab, t, s, &last_track, &last_sector);
                int last_block = linear_sector(type, last_track, last_sector);
                if(error == CHAINED || error == CHAINED_TRUNCATED) {
                    mark_sector_chain(type, image, atab, t, s, last_track, last_sector, ALLOCATED);
                    atab[b] = (error == CHAINED) ? FILESTART : FILESTART_TRUNCATED;
                    int chained_track = image[last_block * BLOCKSIZE + TRACKLINKOFFSET];
                    int chained_sector = image[last_block * BLOCKSIZE + SECTORLINKOFFSET];
                    int chained_block = linear_sector(type, chained_track, chained_sector);
                    atab[chained_block] = ALLOCATED; /* overwrite FILESTART */
                } else if(t != last_track || s != last_sector) {
                    mark_sector_chain(type, image, atab, t, s, last_track, last_sector, ALLOCATED);
                    atab[b] = FILESTART_TRUNCATED;
                    image[last_block * BLOCKSIZE + TRACKLINKOFFSET] = 0;
                    image[last_block * BLOCKSIZE + SECTORLINKOFFSET] = 255;
                    num_undeleted++;
                } else {
                    if(error != FIRST_BROKEN) {
                        mark_sector_chain(type, image, atab, t, s, last_track, last_sector, UNALLOCATED);
                    }
                }
            }
        }
    }
    if(num_undeleted) {
        write_atab(type, image, atab);
        add_wild_to_dir(type, image, atab, files);
    }
    return num_undeleted;
}

/* Tries to restore any deleted or formatted files */
static void
restore(image_type type, unsigned char* image, int level, imagefile files[])
{
    int num_undeleted = 0;
    /* create block allocation table */
    char *atab = (char *)calloc(image_num_blocks(type), sizeof(char));
    if (atab == NULL) {
        fprintf(stderr, "ERROR: error allocating memory");
        exit(-1);
    }
    init_atab(type, image, atab);
    if(level == RESTORE_DIR_ONLY) {
        num_undeleted += undelete(type, image, atab, RESTORE_DIR_ONLY);
    } else {
        num_undeleted += undelete(type, image, atab, RESTORE_VALID_FILES);
        if(level >= RESTORE_VALID_CHAINS) {
            num_undeleted += undelete_wild(type, image, atab, RESTORE_VALID_CHAINS, files);
        }
        if(level >= RESTORE_INVALID_FILES) {
            num_undeleted += undelete(type, image, atab, RESTORE_INVALID_FILES);
        }
        if(level >= RESTORE_INVALID_CHAINS) {
            num_undeleted += undelete_fix_wild(type, image, atab, files);
        }
        if(level >= RESTORE_INVALID_SINGLES) {
            num_undeleted += undelete_wild(type, image, atab, RESTORE_INVALID_SINGLES, files);
        }
    }
    free(atab);
    if(num_undeleted) {
        modified = 1;
    }
    if(!quiet) {
        printf("%d files undeleted", num_undeleted);
        if(num_undeleted) {
            printf(", '<' at filename end marks truncated files");
        }
        printf("\n");
    }
}

/* Prints a command line to create dir art like the given image */
static void
convert_to_commandline(image_type type, unsigned char* image)
{
    char *blockmap = calloc(image_num_blocks(type), sizeof(char));
    if(blockmap == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        exit(-1);
    }
    printf("\nCommandline to create directory art: -m -n \"");
    unsigned int bam = linear_sector(type, dirtrack(type), 0) * BLOCKSIZE;
    print_filename_with_escapes(image + bam + get_header_offset(type), FILENAMEMAXSIZE);
    printf("\" -i \"");
    print_filename_with_escapes(image + bam + get_id_offset(type), 5);
    printf("\" ");

    int ds = (type == IMAGE_D81) ? 3 : 1;
    int dt = dirtrack(type);
    int offset = 0;
    do {
        int dirblock = linear_sector(type, dt, ds) * BLOCKSIZE + offset;
        int filetype = image[dirblock + FILETYPEOFFSET];
        if (filetype) {
            if(filetype != 0x82) {
                printf("-T %d ", filetype);
            }
            int size = image[dirblock + FILEBLOCKSLOOFFSET] + 256 * image[dirblock + FILEBLOCKSHIOFFSET];
            if(size != 0) {
                printf("-B %d ", size);
            }
            unsigned char *filename = (unsigned char *) image + dirblock + FILENAMEOFFSET;
            printf("-N -f \"");
            print_filename_with_escapes(filename, FILENAMEMAXSIZE);
            printf("\" -L ");
        }
    } while (next_dir_entry(type, image, &dt, &ds, &offset, blockmap));
    free(blockmap);
    printf("\n\n");
}

/* Performs strict CBM DOS validation on the image */
static void
validate(image_type type, unsigned char* image)
{
    /* create block allocation table */
    char *atab = (char *)calloc(image_num_blocks(type), sizeof(int));
    if (atab == NULL) {
        fprintf(stderr, "ERROR: error allocating memory");
        exit(-1);
    }
    /* check format specifier */
    int format = image[linear_sector(type, dirtrack(type), 0) * BLOCKSIZE + 2];
    if (format != 0x41) {
        fprintf(stderr, "ERROR: validation failed, format specifier in directory (0x%02x) does not specify 1541 (0x41)\n", format);
        exit(-1);
    }
    /* check each directory entry and set block allocation table */
    atab[linear_sector(type, dirtrack(type), 0)] = ALLOCATED;
    unsigned int dt = dirtrack(type);
    int dirsector = 1;
    unsigned int start_track = 1;
    while (start_track != 0) {
        atab[linear_sector(type, dt, dirsector)] = ALLOCATED;
        int dirblock = linear_sector(type, dt, dirsector) * BLOCKSIZE;
        for (int direntry = 0; direntry < DIRENTRIESPERBLOCK; direntry++) {
            int entryOffset = direntry * DIRENTRYSIZE;
            int filetype = image[dirblock + entryOffset + FILETYPEOFFSET] & 0xf;
            if (filetype > 4) {
                fprintf(stderr, "ERROR: validation failed, illegal file type (0x%02x) in directory\n", filetype);
                exit(-1);
            }
            if (filetype != 0) { /* skip deleted entries */
                start_track = image[dirblock + entryOffset + FILETRACKOFFSET];
                int start_sector = image[dirblock + entryOffset + FILESECTOROFFSET];
                if (start_track == 0 || start_track > image_num_tracks(type)) {
                    fprintf(stderr, "ERROR: validation failed, illegal track reference (%u) in directory\n", start_track);
                    exit(-1);
                }
                if (start_sector >= num_sectors(type, start_track)) {
                    fprintf(stderr, "ERROR: validation failed, illegal sector reference (track %u, sector %d) in directory\n", start_track, start_sector);
                    exit(-1);
                }
                if (atab[linear_sector(type, start_track, start_sector)] == ALLOCATED) {
                    fprintf(stderr, "ERROR: validation failed, file starts in the middle of another file (track %u, sector %d)\n", start_track, start_sector);
                    exit(-1);
                }
                if (atab[linear_sector(type, start_track, start_sector)] != FILESTART) { /* loop files are allowed */
                    unsigned int error_track;
                    int error_sector;
                    int result = validate_sector_chain(type, image, atab, start_track, start_sector, &error_track, &error_sector);
                    switch(result) {
                    case ILLEGAL_TRACK:
                        fprintf(stderr, "ERROR: validation failed, illegal track reference in file sector chain at track %d, sector %d\n", error_track, error_sector);
                        exit(-1);
                    case ILLEGAL_SECTOR:
                        fprintf(stderr, "ERROR: validation failed, illegal sector reference in file sector chain at track %d, sector %d\n", error_track, error_sector);
                        exit(-1);
                    case LOOP:
                        fprintf(stderr, "ERROR: validation failed, loop in file sector chain at track %d, sector %d\n", error_track, error_sector);
                        exit(-1);
                    case COLLISION:
                    case CHAINED:
                        fprintf(stderr, "ERROR: validation failed, collision with existing file in file sector chain at track %d, sector %d\n", error_track, error_sector);
                        exit(-1);
                    }
                    atab[linear_sector(type, start_track, start_sector)] = FILESTART;
                }
            }
        }
        dt = image[dirblock + TRACKLINKOFFSET];
        dirsector = image[dirblock + SECTORLINKOFFSET];
        if (dt == 0) {
            break;
        }
    }
    /* check BAM for consistency with block allocation table */
    for (unsigned int t = 1; t <= image_num_tracks(type); t++) {
        unsigned char* bitmap = image + get_bam_offset(type, t);
        int num_free = 0;
        for (int s = 0; s < num_sectors(type, t); s++) {
            int atab_used = (atab[linear_sector(type, t, s)] != UNALLOCATED);
            int bam_used = ((bitmap[s >> 3] & (1 << (s & 7))) == 0);
            num_free += (1 - bam_used);
            if (bam_used != atab_used) {
                fprintf(stderr, "ERROR: validation failed, BAM (%s) is not consistent with files (%s) for track %u sector %d\n", bam_used ? "used" : "free", atab_used ? "used" : "free", t, s);
                exit(-1);
            }
        }
        if (*(bitmap - 1) != num_free) {
            fprintf(stderr, "ERROR: validation failed, BAM number of free blocks (%d) is not consistent with bitmap (%#02x%#02x%#02x) for track %u\n", *(bitmap - 1), *bitmap, *(bitmap + 1), *(bitmap + 2), t);
            exit(-1);
        }
    }
    free(atab);
    if(!quiet) {
        fprintf(stderr, "CBM DOS validation passed\n");
    }
}

int
main(int argc, char* argv[])
{
    imagefile files[MAXNUMFILES_D81];
    memset(files, 0, sizeof files);

    image_type type = IMAGE_D64;
    char* imagepath = NULL;
    char* filename_g64 = NULL;
    unsigned char* header  = (unsigned char*)"cc1541";
    unsigned char* id      = (unsigned char*)"00 2a";
    int dirtracksplit = 1;
    int usedirtrack = 0;
    unsigned int shadowdirtrack = 0;

    int default_first_sector_new_track = 0;
    int first_sector_new_track = 0;
    int defaultSectorInterleave = 10;
    int sectorInterleave = 0;
    int dir_sector_interleave = 3;
    int numdirblocks = 2;
    int nrSectorsShown = -1;
    unsigned char* filename = NULL;
    int set_header = 0;
    int nooverwrite = 0;
    int dovalidate = 0;
    int restore_level = -1;
    int ignore_collision = 0;
    int filetype = 0x82; /* default is closed PRG */
    bool filetype_set = false;
    bool print_art_commandline = false;

    /* flags to detect illegal settings for Transwarp or D81 */
    int transwarp_set = 0;
    int sector_interleave_set = 0;
    int default_sector_interleave_set = 0;
    int file_start_sector_set = 0;
    int new_track_start_sector_set = 0;

    int retval = 0;

    int i, j;

    if (argc == 1 || strcmp(argv[argc-1], "-h") == 0) {
        usage();
    }
    for (j = 1; j < argc - 1; j++) {
        if (strcmp(argv[j], "-n") == 0) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for -n\n");
                return -1;
            }
            header = (unsigned char*)argv[++j];
            set_header = 1;
            modified = 1;
        } else if (strcmp(argv[j], "-i") == 0) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for -i\n");
                return -1;
            }
            id = (unsigned char*)argv[++j];
            set_header = 1;
            modified = 1;
        } else if (strcmp(argv[j], "-M") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &max_hash_length)) {
                fprintf(stderr, "ERROR: Error parsing argument for -M\n");
                return -1;
            }
            if ((max_hash_length < 1) || (max_hash_length > FILENAMEMAXSIZE)) {
                fprintf(stderr, "ERROR: Hash computation maximum filename length %d specified\n", max_hash_length);
                return -1;
            }
        } else if (strcmp(argv[j], "-m") == 0) {
            ignore_collision = 1;
        } else if (strcmp(argv[j], "-F") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &first_sector_new_track)) {
                fprintf(stderr, "ERROR: Error parsing argument for -F\n");
                return -1;
            }
            new_track_start_sector_set = 1;
        } else if (strcmp(argv[j], "-S") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &defaultSectorInterleave)) {
                fprintf(stderr, "ERROR: Error parsing argument for -S\n");
                return -1;
            }
            default_sector_interleave_set = 1;
        } else if (strcmp(argv[j], "-s") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &sectorInterleave)) {
                fprintf(stderr, "ERROR: Error parsing argument for -s\n");
                return -1;
            }
            sector_interleave_set = 1;
        } else if (strcmp(argv[j], "-f") == 0) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for -f\n");
                return -1;
            }
            filename = (unsigned char*)argv[++j];
        } else if (strcmp(argv[j], "-e") == 0) {
            files[num_files].mode |= MODE_SAVETOEMPTYTRACKS;
        } else if (strcmp(argv[j], "-E") == 0) {
            files[num_files].mode |= MODE_FITONSINGLETRACK;
        } else if (strcmp(argv[j], "-r") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &i)) {
                fprintf(stderr, "ERROR: Error parsing argument for -r\n");
                return -1;
            }
            if ((i < 1) || (((i << MODE_MIN_TRACK_SHIFT) & MODE_MIN_TRACK_MASK) != (i << MODE_MIN_TRACK_SHIFT))) {
                fprintf(stderr, "ERROR: Invalid minimum track %d specified\n",  i);
                return -1;
            }
            files[num_files].mode = (files[num_files].mode & ~MODE_MIN_TRACK_MASK) | (i << MODE_MIN_TRACK_SHIFT);
        } else if (strcmp(argv[j], "-b") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &i)) {
                fprintf(stderr, "ERROR: Error parsing argument for -b\n");
                return -1;
            }
            if ((i < 0) || (i >= num_sectors(type, 1))) {
                fprintf(stderr, "ERROR: Invalid beginning sector %d specified\n", i);
                return -1;
            }
            files[num_files].mode = (files[num_files].mode & ~MODE_BEGINNING_SECTOR_MASK) | (i + 1);
            file_start_sector_set = 1;
        } else if (strcmp(argv[j], "-c") == 0) {
            files[num_files].mode |= MODE_SAVECLUSTEROPTIMIZED;
        } else if (strcmp(argv[j], "-o") == 0) {
            nooverwrite = 1;
        } else if (strcmp(argv[j], "-V") == 0) {
            dovalidate = 1;
        } else if (strcmp(argv[j], "-T") == 0) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for -T\n");
                return -1;
            }
            if (strcmp(argv[j + 1], "DEL") == 0) {
                filetype = (filetype & 0xf0) | FILETYPEDEL;
            } else if (strcmp(argv[j + 1], "SEQ") == 0) {
                filetype = (filetype & 0xf0) | FILETYPESEQ;
            } else if (strcmp(argv[j + 1], "PRG") == 0) {
                filetype = (filetype & 0xf0) | FILETYPEPRG;
            } else if (strcmp(argv[j + 1], "USR") == 0) {
                filetype = (filetype & 0xf0) | FILETYPEUSR;
            } else if (strcmp(argv[j + 1], "REL") == 0) {
                filetype = (filetype & 0xf0) | FILETYPEREL;
            } else {
                char* dummy;
                filetype = strtol(argv[j + 1], &dummy, 10);
                if(dummy == argv[j + 1] || *dummy != 0 || filetype < 0 || filetype > 255) {
                    fprintf(stderr, "ERROR: Error parsing argument for -T\n");
                    return -1;
                }
            }
            filetype_set = true;
            j++;
        } else if (strcmp(argv[j], "-O") == 0) {
            filetype &= 0x7f;
        } else if (strcmp(argv[j], "-P") == 0) {
            filetype |= 0x40;
        } else if (strcmp(argv[j], "-N") == 0) {
            files[num_files].force_new = 1;
        } else if (strcmp(argv[j], "-K") == 0) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for -K\n");
                return -1;
            }
            evalhexescape((unsigned char *) argv[++j], files[num_files].key, TRANSWARPKEYSIZE, 0);
            files[num_files].have_key = true;
        } else if ((strcmp(argv[j], "-w") == 0)
                   || (strcmp(argv[j], "-W") == 0)) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for %s\n", argv[j]);
                return -1;
            }
            files[num_files].alocalname = (unsigned char*)argv[j + 1];
            if (filename == NULL) {
                ascii2petscii(basename(files[num_files].alocalname), files[num_files].pfilename, FILENAMEMAXSIZE); /* do not eval escapes when converting the filename, as the local filename could contain the escape char */
            } else {
                evalhexescape(filename, files[num_files].pfilename, FILENAMEMAXSIZE, FILENAMEEMPTYCHAR);
            }
            files[num_files].sectorInterleave = sectorInterleave ? sectorInterleave : defaultSectorInterleave;
            files[num_files].first_sector_new_track = first_sector_new_track;
            files[num_files].nrSectorsShown = nrSectorsShown;
            files[num_files].filetype = filetype;
            files[num_files].direntryindex = -1;

            if (strcmp(argv[j], "-W") == 0) {
                transwarp_set = true;
                if(!filetype_set && files[num_files].have_key) {
                    files[num_files].filetype = (filetype & 0xf0) | FILETYPEUSR | FILETYPETRANSWARPMASK;
                } else {
                    files[num_files].filetype = filetype | FILETYPETRANSWARPMASK;
                }
                files[num_files].sectorInterleave = 1;
            }

            first_sector_new_track = default_first_sector_new_track;
            filename = NULL;
            sectorInterleave = 0;
            nrSectorsShown = -1;
            filetype = 0x82;
            filetype_set = false;
            num_files++;
            modified = 1;
            j++;
        } else if (strcmp(argv[j], "-l") == 0) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for -l\n");
                return -1;
            }
            files[num_files].alocalname = (unsigned char*)argv[j + 1];
            evalhexescape(files[num_files].alocalname, files[num_files].plocalname, FILENAMEMAXSIZE, FILENAMEEMPTYCHAR);
            if (filename == NULL) {
                fprintf(stderr, "ERROR: Loop files require a filename set with -f\n");
                return -1;
            }
            evalhexescape(filename, files[num_files].pfilename, FILENAMEMAXSIZE, FILENAMEEMPTYCHAR);
            if(memcmp(files[num_files].pfilename, files[num_files].plocalname, FILENAMEMAXSIZE) == 0 && !files[num_files].force_new) {
                fprintf(stderr, "ERROR: Loop file cannot have the same name as the file they refer to, unless with -N\n");
                return -1;
            }
            files[num_files].mode |= MODE_LOOPFILE;
            files[num_files].sectorInterleave = 0;
            files[num_files].first_sector_new_track = first_sector_new_track;
            first_sector_new_track = default_first_sector_new_track;
            files[num_files].nrSectorsShown = nrSectorsShown;
            files[num_files].filetype = filetype;
            files[num_files].direntryindex = -1;
            filename = NULL;
            sectorInterleave = 0;
            nrSectorsShown = -1;
            filetype = 0x82;
            filetype_set = false;
            num_files++;
            modified = 1;
            j++;
        } else if (strcmp(argv[j], "-L") == 0) {
            if (filename == NULL) {
                fprintf(stderr, "ERROR: Writing no file using -L requires disk filename set with -f\n");
                return -1;
            }
            evalhexescape(filename, files[num_files].pfilename, FILENAMEMAXSIZE, FILENAMEEMPTYCHAR);
            files[num_files].nrSectorsShown = nrSectorsShown;
            files[num_files].filetype = filetype;
            files[num_files].direntryindex = -1;
            files[num_files].mode |= MODE_NOFILE;

            first_sector_new_track = default_first_sector_new_track;
            filename = NULL;
            sectorInterleave = 0;
            nrSectorsShown = -1;
            filetype = 0x82;
            filetype_set = false;
            num_files++;
            modified = 1;
        } else if (strcmp(argv[j], "-x") == 0) {
            dirtracksplit = 0;
        } else if (strcmp(argv[j], "-t") == 0) {
            usedirtrack = 1;
        } else if (strcmp(argv[j], "-d") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%u", &shadowdirtrack)) {
                fprintf(stderr, "ERROR: Error parsing argument for -d\n");
                return -1;
            }
            modified = 1;
        } else if (strcmp(argv[j], "-u") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &numdirblocks)) {
                fprintf(stderr, "ERROR: Error parsing argument for -u\n");
                return -1;
            }
        } else if (strcmp(argv[j], "-B") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &nrSectorsShown)) {
                fprintf(stderr, "ERROR: Error parsing argument for -B\n");
                return -1;
            }
            if (nrSectorsShown < 0 || nrSectorsShown > 65535) {
                fprintf(stderr, "ERROR: Argument must be between 0 and 65535 for -B\n");
                return -1;
            }
        } else if (strcmp(argv[j], "-4") == 0) {
            type = IMAGE_D64_EXTENDED_SPEED_DOS;
            modified = 1;
        } else if (strcmp(argv[j], "-R") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &restore_level)) {
                fprintf(stderr, "ERROR: Error parsing argument for -R\n");
                return -1;
            }
            if(restore_level < 0 || restore_level > 5) {
                fprintf(stderr, "ERROR: Argument must be between 0 and 5 for -R\n");
                return -1;
            }
        } else if (strcmp(argv[j], "-5") == 0) {
            type = IMAGE_D64_EXTENDED_DOLPHIN_DOS;
            modified = 1;
        } else if (strcmp(argv[j], "-a") == 0) {
            print_art_commandline = true;
        } else if(strcmp(argv[j], "-g") == 0) {
            if (argc < j + 2) {
                fprintf(stderr, "ERROR: Error parsing argument for -g\n");
                return -1;
            }
            filename_g64 = argv[++j];
        } else if (strcmp(argv[j], "-U") == 0) {
            if ((argc < j + 2) || !sscanf(argv[++j], "%d", &unicode)) {
                fprintf(stderr, "ERROR: Error parsing argument for -U\n");
                return -1;
            }
            if(unicode < 0 || unicode > 2) {
                fprintf(stderr, "ERROR: Argument must be between 0 and 2 for -U\n");
                return -1;
            }
        } else if (strcmp(argv[j], "-q") == 0) {
            quiet = 1;
        } else if (strcmp(argv[j], "-v") == 0) {
            verbose = 1;
        } else if (strcmp(argv[j], "-h") == 0) {
            usage();
        } else {
            fprintf(stderr, "ERROR: Error parsing command line at \"%s\"\n", argv[j]);
            printf("Use -h for help.\n");
            return -1;
        }
    }
    if (j >= argc) {
        fprintf(stderr, "ERROR: No image file provided, or misparsed last option\n");
        return -1;
    }
    imagepath = argv[argc-1];

    if (strlen(imagepath) >= 4) {
        if (strcmp(imagepath + strlen(imagepath) - 4, ".d71") == 0) {
            if ((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) {
                fprintf(stderr, "ERROR: Extended .d71 images are not supported\n");
                return -1;
            }
            type = IMAGE_D71;
        } else if (strcmp(imagepath + strlen(imagepath) - 4, ".d81") == 0) {
            if ((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) {
                fprintf(stderr, "ERROR: Extended .d81 images are not supported\n");
                return -1;
            }
            type = IMAGE_D81;
            dir_sector_interleave = 1;
        }
    }

    if(shadowdirtrack > image_num_tracks(type) || (int)shadowdirtrack == dirtrack(type) || (type == IMAGE_D71 && (int)shadowdirtrack == dirtrack(type) + D64NUMTRACKS)) {
        fprintf(stderr, "ERROR: Invalid shadow directory track\n");
        return -1;
    }

    if (type != IMAGE_D64) {
        if (transwarp_set
                && (type != IMAGE_D64_EXTENDED_SPEED_DOS)
                && (type != IMAGE_D64_EXTENDED_DOLPHIN_DOS)) {
            fprintf(stderr, "ERROR: Transwarp encoding is not supported for non-D64 images\n");
            return -1;
        }

        if (filename_g64 != NULL) {
            fprintf(stderr, "ERROR: G64 output is only supported for non-extended D64 images\n");
            return -1;
        }
    }

    /* Check for unsupported settings for D81 */
    if (type == IMAGE_D81) {
        if (default_sector_interleave_set) {
            fprintf(stderr, "ERROR: -S is not supported for D81 images\n");
            return -1;
        }
        if (sector_interleave_set) {
            fprintf(stderr, "ERROR: -s is not supported for D81 images\n");
            return -1;
        }
        if (new_track_start_sector_set) {
            fprintf(stderr, "ERROR: -F is not supported for D81 images\n");
            return -1;
        }
        if (file_start_sector_set) {
            fprintf(stderr, "ERROR: -b is not supported for D81 images\n");
            return -1;
        }
    }

    /* Change locale from C to default to allow unicode printouts */
    if(unicode != 0) {
        setlocale(LC_ALL, "");
    }

    /* quiet has precedence over verbose */
    if(quiet) {
        verbose = 0;
    }

    /* open image */
    unsigned int imagesize = image_size(type);
    unsigned char* image = (unsigned char*)calloc(imagesize, sizeof(unsigned char));
    if (image == NULL) {
        fprintf(stderr, "ERROR: Memory allocation error\n");
        return -1;
    }
    FILE* f = fopen(imagepath, "rb");
    if (f == NULL) {
        modified = 1;
        if (!quiet) {
            printf("Adding %d files to new image %s\n", num_files, basename((unsigned char*)imagepath));
        }
        initialize_directory(type, image, header, id, shadowdirtrack);
    } else {
        if (!quiet) {
            printf("Adding %d files to existing image %s\n", num_files, basename((unsigned char*)imagepath));
        }
        size_t read_size = fread(image, 1, imagesize, f);
        fclose(f);
        if (read_size != imagesize) {
            if (((type == IMAGE_D64_EXTENDED_SPEED_DOS) || (type == IMAGE_D64_EXTENDED_DOLPHIN_DOS)) && (read_size == D64SIZE)) {
                /* Clear extra tracks */
                memset(image + image_size(IMAGE_D64), 0, image_size(type) - image_size(IMAGE_D64));

                /* Mark all extra sectors unused */
                for (unsigned int t = D64NUMTRACKS + 1; t <= image_num_tracks(type); t++) {
                    for (int s = 0; s < num_sectors(type, t); s++) {
                        mark_sector(type, image, t, s, 1 /* free */);
                    }
                }
            } else {
                fprintf(stderr, "ERROR: Wrong filesize: expected to read %u bytes, but read %u bytes\n", imagesize, (unsigned int) read_size);
                return -1;
            }
        }
        if (dovalidate) {
            validate(type, image);
        }
        if (restore_level >= 0) {
            restore(type, image, restore_level, files);
        }
        if (set_header) {
            update_directory(type, image, header, id, shadowdirtrack);
        }
    }

    /* Print command line before adding anything to the image */
    if(print_art_commandline) {
        convert_to_commandline(type, image);
    }

    /* Create directory entries */
    create_dir_entries(type, image, files, num_files, dir_sector_interleave, shadowdirtrack, nooverwrite);

    /* Write files and mark sectors in BAM */
    write_files(type, image, files, num_files, usedirtrack, dirtracksplit, shadowdirtrack, numdirblocks, dir_sector_interleave);

    /* Print allocation info */
    if (verbose) {
        print_file_allocation(type, image, files, num_files);
    }
    int blocks_free = check_bam(type, image);

    /* Print directory */
    if (!quiet) {
        print_directory(type, image, blocks_free);
    }

    /* Show directory issues if present */
    if(dir_error != DIR_OK) {
        fprintf(stdout, "WARNING: %s\n", dir_error_string[dir_error]);
    }

    /* Save image */
    if(modified) {
        f = fopen(imagepath, "wb");
        if (f == NULL || fwrite(image, imagesize, 1, f) != 1) {
            fprintf(stderr, "ERROR: Failed to write %s\n", imagepath);
            retval = -1;
        }
        if (f != NULL) {
            fclose(f);
        }
    }

    /* Save optional g64 image */
    if (filename_g64 != NULL) {
        /* retval might be set to -1 already.  Thus we need to take its
        previous state and OR it with the following return value. */
        retval |= generate_uniformat_g64(image, filename_g64);
    }

    if (!ignore_collision && check_hashes(type, image)) {
        fprintf(stderr, "\nERROR: Filename hash collision detected, image is not compatible with Krill's Loader. Use -m to ignore this error.\n");
        retval = -1;
    }

    free(image);

    return retval;
}
