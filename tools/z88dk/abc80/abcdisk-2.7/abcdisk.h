#ifndef ABCDISK_H
#define ABCDISK_H

#include "compiler.h"
#include "patchlevel.h"

/* Integer divide with rounding */
#define DIV_ROUND(x,y) (((x)+(y)/2)/(y))

/*
 * Default cassette speeds; this seems to be what ABC800 generates
 * (slightly rounded numbers.)  A real ABC80 seems slightly quicker,
 * around 750 baud, but can be interrupted by NMI generating a glitch.
 */
#define CAS_DIVISOR_ABC80	129
#define CAS_DIVISOR_ABC800	35
#define CAS_SIO_CLOCK		(1500000/16)

#define CAS_BAUDRATE_ABC80	DIV_ROUND(CAS_SIO_CLOCK, CAS_DIVISOR_ABC80)
#define CAS_BAUDRATE_ABC800	DIV_ROUND(CAS_SIO_CLOCK, CAS_DIVISOR_ABC800)

/*
 * Windows has a different signature for mkdir(), but fortunately also
 * has an alias _mkdir() to look for...
 */
#ifdef HAVE__MKDIR
# define make_dir(x) _mkdir(x)
#else
# define make_dir(x) mkdir((x), 0777)
#endif

struct diskfmt {
    const char *name;	        /* Format name */
    unsigned int c, h, s;	/* Disk geometry, if known */
    unsigned int sectors;       /* Total sectors (max 1911 clusters) */
    unsigned int cluster_shift; /* Size of a cluster, log2 */
    unsigned int syssec;	/* First system sector (bitmap) */
    unsigned int dirsec[2];	/* First directory sector (main, backup) */
    unsigned int dirsecs;	/* Number of directory sectors */
    unsigned int il;		/* Interleave factor (normally 1) */
    unsigned int iloffs;	/* Interleave offset (always 0?) */
    unsigned int dstart;	/* Beginning of actual data */
    bool chflip;		/* Flip heads and cylinders? */
};

struct disk;

struct directory {
    struct disk *disk;          /* This directory belongs to... */
    unsigned char *bitmap;      /* Directory entry "bitmap" area */
    unsigned int dirsec;	/* First directory sector */
    unsigned int headsec;       /* File header sector for UFD */
};

struct sectmeta {
    unsigned char flags;
};

struct disk {
    const struct diskfmt *fmt;  /* Format descriptor */
    unsigned char *image;       /* Image of the disk */
    unsigned char *bitmap;      /* Pointer to the bitmap inside the disk */
    struct sectmeta *meta;	/* Per sector backend metadata */
    struct directory mfd;       /* MFD directory descriptor */
};

#define MAX_CLUST      (239*8)	/* Absolute theoretical maximum */
#define MAX_CLUSTSHIFT 5	/* Otherwise we get > 64K sectors */
#define MAX_SECTORS    (MAX_CLUST << MAX_CLUSTSHIFT)

const struct diskfmt *get_format(const char *);
void show_formats(FILE *);
struct disk * safe_alloc
allocate_disk(const struct diskfmt *fmt);
void free_disk(struct disk *);
void disk_update_ptrs(struct disk *);

unsigned char *make_abcfile(FILE *f, int *sizep);

extern const char *program;
void * safe_malloc(1) xmalloc(size_t);
void * safe_malloc(1) xzalloc(size_t);
void *xrealloc(void *, size_t);

static inline void mark_used(struct disk *disk, unsigned int cluster)
{
    disk->bitmap[cluster >> 3] |= (0x80 >> (cluster & 7));
}

static inline int is_used(struct disk *disk, unsigned int cluster)
{
    return !!(disk->bitmap[cluster >> 3] & (0x80 >> (cluster & 7)));
}

/*
 * Get a pointer to a certain sector, accounting for all the
 * various permutation options.
 */
uint8_t *get_sector(const struct disk *disk, unsigned int sectnr);

/*
 * Convert a filename in ABC format to a lower-case filename
 * in host encoding
 */
size_t unmangle_filename(char *out, const unsigned char *in);

/*
 * Convert a filename in host encoding to a filename in ABC format
 * Returns 0 for OK, -1 for failure (filename cannot be converted.)
 */
int mangle_name(unsigned char *dst, const char *src);

/*
 * Makes a filename suitable for embedding in a buffer.
 */
int embed_name(unsigned char *dst, size_t len, const char *src);

#endif /* ABCDISK_H */
