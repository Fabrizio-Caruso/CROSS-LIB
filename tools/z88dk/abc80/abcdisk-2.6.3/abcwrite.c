/*
 * abcwrite.c
 *
 * Program to build a populated ABC-DOS hard disk image
 */
#include "abcdisk.h"

/*
 * Find a free cluster in the bitmap; return the cluster number
 */
static int get_cluster(struct disk *disk, unsigned int cluster_hint)
{
    unsigned int nclust, i, x, nc, best, bnc;

    best = -1;
    bnc = 0;

    nclust = disk->fmt->sectors << disk->fmt->cluster_shift;

    for (i = 0; i < nclust; i++) {
        if (!is_used(disk, i)) {
            /* How long is this? */
            for (x = i, nc = 0; i < nclust && nc < cluster_hint; i++) {
                if (is_used(disk, i))
                    break;
                nc++;
            }
            if (nc == cluster_hint) {
                /* Found as big as we wanted */
                mark_used(disk, x);
                return x;
            } else if (nc > bnc) {
                best = x;
                bnc = nc;
            }
        }
    }

    if (bnc) {
        /* We found *something* at least... */
        mark_used(disk, best);
        return best;
    }

    fprintf(stderr, "%s: output disk full\n", program);
    exit(1);
}

/*
 * Create a file object and return the first sector number.
 */
static unsigned int
create_file(struct directory *dir, const unsigned char *mangled_name,
	    unsigned char *data, int size, int ufd)
{
    struct disk *disk = dir->disk;
    int cluster;
    unsigned int nblocks, secperclust, clusters;
    unsigned int dirs, dirp;             /* Directory (sector,position) */
    unsigned char *dirent;
    unsigned int blockno, sector, header, cleft;
    unsigned char *hdata, *dp;

    if (!data)
        return 0;	       /* No data, not even an empty buffer */

    nblocks = (size + 252) / 253 + 1;
    secperclust = 1 << disk->fmt->cluster_shift;
    clusters = (nblocks + secperclust - 1) >> disk->fmt->cluster_shift;

    /* Find and create directory entry */
    dirp = -1;
    for (dirs = 0; dirs < disk->fmt->dirsecs; dirs++) {
        if (dir->bitmap[0xEF + dirs] < 0x10) {
            dirp = dir->bitmap[0xEF + dirs]++;
            break;
        }
    }

    if (dirs >= disk->fmt->dirsecs) {
        /* Directory full */
        fprintf(stderr, "%s: directory full\n", program);
        return 0;
    }

    dirent = get_sector(disk, dir->dirsec + dirs) + (dirp << 4);
    memcpy(dirent + 4, mangled_name, 11);

    /* Allocate first cluster */
    cluster = get_cluster(disk, clusters--);
    if (cluster < 0) {
        fprintf(stderr, "get_cluster = %d\n", cluster);
        return 0;
    }

    dirent[0] = cluster >> 3;
    dirent[1] = cluster << 5;
    dirent[2] = nblocks & 0xFF;
    dirent[3] = nblocks >> 8;

    /* Create "file header" */
    header = sector = cluster << disk->fmt->cluster_shift;
    hdata = dp = get_sector(disk, sector++);
    cleft = secperclust - 1;    /* Left in the first cluster */

    memset(hdata, 0, 256);

    *hdata++ = (dirp << 4) + dirs;
    *hdata++ = 0;               /* Header = block 0 */
    *hdata++ = 0;               /* Header = block 0 */
    *hdata++ = 0xFF;
    /* Starting extent has one cluster in it */
    hdata[0] = cluster >> 3;
    hdata[1] = (cluster << 5) + 0;      /* Bits[4:0] = extend size minus one */

    /* Actually copy the file */
    blockno = 0;
    while (size) {
        int nbytes = (size > 253) ? 253 : size;

        blockno++;

        if (cleft) {
	    dp = get_sector(disk, sector++); /* Subsequent sector */
            cleft--;
        } else {
            cluster = get_cluster(disk, clusters--);
            if (cluster < 0) {
                fprintf(stderr, "get_cluster = %d\n", cluster);
                return 0;
            }
	    sector = cluster << disk->fmt->cluster_shift;
	    dp = get_sector(disk, sector++);
	    cleft = secperclust - 1;    /* Left in the first cluster */
            if (cluster == ((hdata[0] << 3) + (hdata[1] >> 5) + 1) &&
                (hdata[1] & 31) < 31) {
                hdata[1]++;
            } else {
                hdata += 2;
                hdata[0] = cluster >> 3;
                hdata[1] = (cluster << 5) + 0;
            }
        }

        if (ufd && blockno > 1) {
            memset(dp, 0xff, 256);
        } else {
            dp[0] = (dirp << 4) + dirs;
            dp[1] = blockno & 0xFF;
            dp[2] = blockno >> 8;
            memcpy(dp + 3, data, 253);
            data += 253;
        }

        size -= nbytes;
    }

    /* Terminate the hdata */
    hdata += 2;
    hdata[0] = 0xFF;
    hdata[1] = 0xFF;

    return header;
}

/*
 * Copy a file
 */
static int copy_file(struct directory *dir, const char *filename)
{
    int size;
    unsigned char mangled_name[12];
    unsigned char *data;
    unsigned int header;

    data = make_abcfile(fopen(filename, "rb"), &size);

    mangle_name(mangled_name, filename);

    header = create_file(dir, mangled_name, data, size, 0);
    free(data);
    return header ? 0 : -1;
}

/*
 * Create a UFD
 */
static struct directory *make_ufd(struct directory *dir, const char *filename)
{
    unsigned char mangled_name[12];
    unsigned char bitmap[253];
    unsigned int header;
    const uint8_t *hdata;
    struct directory *ufd;
    int dirs_block = dir->headsec ? dir->headsec + 2 : 0;
    struct disk *disk = dir->disk;

    ufd = xmalloc(sizeof(struct directory));
    memset(ufd, 0, sizeof *ufd);
    ufd->disk = disk;

    mangle_name(mangled_name, filename);
    memcpy(mangled_name + 8, "Ufd", 4);

    memset(bitmap, 0, sizeof bitmap);
    bitmap[0] = dirs_block & 0xFF;
    bitmap[1] = dirs_block >> 8;

    header = create_file(dir, mangled_name, bitmap,
                         (disk->fmt->dirsecs + 1) * 253, 1);
    if (!header) {
        fprintf(stderr, "%s: invalid file header\n", program);
        return NULL;
    }
    hdata = get_sector(disk, header);

    if ((hdata[6] & hdata[7]) != 0xff) {
        fprintf(stderr, "%s: discontiguous UFD (disk too fragmented)\n",
                program);
        return NULL;
    }

    ufd->bitmap  = get_sector(disk, header + 1);
    ufd->dirsec  = header + 2;
    ufd->headsec = header;

    /* The UFD bitmap contains two copies of the self-pointer... */
    ufd->bitmap[5] = ufd->bitmap[0];

    return ufd;
}

/* Copy files as given by tree on stdin */
static int get_files(struct directory *ufd, const char *dirpath)
{
    struct stat st;
    DIR *d;
    struct dirent *de;
    struct directory *subdir;
    size_t dirlen, filelen;
    char *filename;

    d = opendir(dirpath);
    if (!d) {
        /* Directory error or not a directory */

        int err = errno;
        int rv;

        /* Is it a single file? */
        rv = copy_file(ufd, dirpath);
        if (rv) {
            errno = err;
            perror(dirpath);
        }
        return rv;
    }

    dirlen = strlen(dirpath);

    while ((de = readdir(d))) {
        if (de->d_name[0] == '.')
            continue;

        filelen = strlen(de->d_name);
        filename = xmalloc(dirlen + filelen + 2);
        memcpy(filename, dirpath, dirlen);
        filename[dirlen] = '/';
        memcpy(filename + dirlen + 1, de->d_name, filelen + 1);

        if (stat(filename, &st)) {
            perror(filename);
            continue;
        }
        if (S_ISDIR(st.st_mode)) {
            subdir = make_ufd(ufd, filename);
            if (subdir)
                get_files(subdir, filename);
            else
                fprintf(stderr, "%s: make_ufd(%s) failed\n", program, filename);
        } else {
            copy_file(ufd, filename);
        }
        free(filename);
    }
    closedir(d);

    return 0;
}

#define FORMAT_BYTE	0x40	/* What DOSGEN uses */

/* Create an empty filesystem; disk->fmt must be valid */
static void dosgen(struct disk *disk)
{
    unsigned int i, j, starter;
    const struct diskfmt *fmt = disk->fmt;
    size_t size = fmt->sectors << 8;
    int secperclust = 1 << fmt->cluster_shift;
    uint8_t *emptysys;

    disk_update_ptrs(disk);

    /* Fill disk with format byte */
    memset(disk->image, FORMAT_BYTE, size);

    /* Create empty bitmap */
    memset(disk->bitmap, 0, 256);
    for (i = fmt->sectors >> fmt->cluster_shift; i < MAX_CLUST; i++)
        mark_used(disk, i);     /* Mark nonexistent clusters busy */

    /*
     * 239..254 is the MFD "bitmap"; some DOSGEN seem to only set 0x01
     * for the first directory sector?
     */
    memset(&disk->bitmap[239], 0x01, fmt->dirsecs);

    /* Mark all clusters containing system sectors used */
    starter = ((fmt->dirsec[0] > fmt->dirsec[1])
	       ? fmt->dirsec[0] : fmt->dirsec[1]) + fmt->dirsecs;
    starter = (starter + secperclust - 1) >> fmt->cluster_shift;
    for (i = 0; i < starter; i++)
        mark_used(disk, i);

    /* Copy bitmap to backup empty bitmap (= bad block map) */
    emptysys = get_sector(disk, fmt->syssec+1);
    memcpy(emptysys, disk->bitmap, 256);

    /*
     * UFD-DOS DOSGEN writes the cluster shift to the last byte in the
     * *empty* bitmap only.
     */
    emptysys[255] = fmt->cluster_shift;

    /* Create MFD directory sectors */
    for (i = 0; i < 2; i++) {
	unsigned int dsec = fmt->dirsec[i];
	if (!dsec)
	    continue;
	for (j = 0; j < fmt->dirsecs; j++) {
	    uint8_t *p = get_sector(disk, dsec + j);
	    memset(p, 0x00, 16);	/* Reserved entry */
	    memset(p + 16, 0xff, 256-16);
	}
    }
}

int main(int argc, char *argv[])
{
    struct disk *disk;
    const struct diskfmt *fmt;
    FILE *f;

    setlocale(LC_ALL, "");

    program = argv[0];

    if (argc != 4 || !(fmt = get_format(argv[1]))) {
	fprintf(stderr, "abcdisk %s\n", VERSION);
        fprintf(stderr, "Usage: %s format indir outfile\n", argv[0]);
        show_formats(stderr);
        return 1;
    }

    /* Allocate disk buffer and create a filesyste */
    disk = allocate_disk(fmt);
    dosgen(disk);

    /* Install files */
    if (get_files(&disk->mfd, argv[2]))
        return 1;               /* No files to be had */

    /* Create backup MFD if it exists */
    if (fmt->dirsec[1]) {
	unsigned int ds;
	for (ds = 0; ds < fmt->dirsecs; ds++) {
	    memcpy(get_sector(disk, fmt->dirsec[1] + ds),
		   get_sector(disk, fmt->dirsec[0] + ds), 255);
	}
    }

    /* Finally, write out image */
    f = !strcmp(argv[3], "-") ? stdout : fopen(argv[3], "wb");
    if (!f) {
        perror(argv[3]);
        return 1;
    }
    fwrite(disk->image, 1, fmt->sectors << 8, f);
    fflush(f);
    if (ferror(f)) {
      perror(argv[3]);
      return 1;
    }
    if (f != stdout)
        fclose(f);

    free_disk(disk);

    return 0;
}
