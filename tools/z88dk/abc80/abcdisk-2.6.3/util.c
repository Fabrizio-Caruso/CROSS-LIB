#include "abcdisk.h"

const char *program;

void *xmalloc(size_t len)
{
    void *p;

    if (!len)
	len = 1;

    p = malloc(len);

    if (!p) {
        perror(program);
        exit(1);
    }

    return p;
}

void *xzalloc(size_t len)
{
    void *p;

    if (!len)
	len = 1;

    p = calloc(len, 1);

    if (!p) {
        perror(program);
        exit(1);
    }

    return p;
}

void *xrealloc(void *p, size_t newlen)
{
    if (!newlen)
	newlen = 1;

    p = realloc(p, newlen);

    if (!p) {
	perror(program);
	exit(1);
    }

    return p;
}

/*
 * Get a pointer to a certain sector, accounting for all the
 * various permutation options.
 */
uint8_t *get_sector(const struct disk *disk, unsigned int sectnr)
{
    const struct diskfmt *fmt = disk->fmt;
    unsigned int sintrack, track, sector;

    sectnr  += fmt->dstart;
    sintrack = sectnr % fmt->s;
    track    = sectnr / fmt->s;

    if (fmt->chflip) {
	unsigned int head     = track % fmt->h;
	unsigned int cyl      = track / fmt->h;

	track = cyl + head * fmt->c;
    }

    sector = (track * fmt->s) + ((sintrack * fmt->il + fmt->iloffs) % fmt->s);
    return disk->image + (sector << 8);
}

/* Update data pointers after a change in fmt */
void disk_update_ptrs(struct disk *disk)
{
    size_t metasize;
    const struct diskfmt *fmt = disk->fmt;

    if (!fmt)
	return;

    /* The MFD bitmap is the system sector */
    disk->mfd.disk     = disk;
    disk->mfd.bitmap  = disk->bitmap = get_sector(disk, fmt->syssec);
    disk->mfd.dirsec  = fmt->dirsec[0];
    disk->mfd.headsec = 0;

    /* Per-sector metadata available to programs */
    metasize = fmt->sectors * sizeof(struct sectmeta);
    disk->meta = xrealloc(disk->meta, metasize);
    memset(disk->meta, 0, metasize);
}

struct disk *allocate_disk(const struct diskfmt *fmt)
{
    struct disk *disk = xzalloc(sizeof(struct disk));
    size_t size = (fmt ? fmt->sectors : MAX_SECTORS) << 8;

    disk->fmt = fmt;
    disk->image = xmalloc(size);

    /* Fill with FF mostly in case of a truncated disk image */
    memset(disk->image, 0xff, size);

    disk_update_ptrs(disk);
    return disk;
}

void free_disk(struct disk *disk)
{
    if (disk) {
	free(disk->image);
	free(disk->meta);
	free(disk);
    }
}
