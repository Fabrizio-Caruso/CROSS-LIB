/*
 * abcread.c
 *
 * Program to extract the contents from an ABC/UFD-DOS filesystem
 */
#include "abcdisk.h"

enum sectflags {
    SF_NOT_SEEN,
    SF_SEEN_SYS,
    SF_SEEN_MFD,
    SF_SEEN_UFD,
    SF_SEEN_HDR,
    SF_SEEN_FILE,
};

static const char * const seen[] =
{
    "???", "system", "mfd", "ufd", "header", "file"
};

struct pathinfo {
    const char *infile;	       /* Host input file */
    const char *outdir;	       /* Host output directory */
    char *dirpath;	       /* Host directory path buffer */
    char *abcpath;	       /* Pointer into dirpath after outdir */
    size_t dirpath_len;	       /* Current length of dirpath */
    size_t dirpath_size;       /* Allocated length of dirpath */
};

/* Copy files as given by tree on stdin */
static int
extract_dir_sector(struct pathinfo *path, struct directory *dir,
		   int sectnr, int sectindex)
{
    struct disk *disk = dir->disk;
    const uint8_t *sector = get_sector(disk, sectnr);
    int err = 0;
    int i;
    size_t dirpath_len = path->dirpath_len;

#ifdef DEBUG
    printf("directory: %s, sectnr %d (0x%x), sectindex %d (%d)\n",
	   *path->abcpath ? path->abcpath : "/", sectnr, sectnr,
	   sectindex, dir->bitmap[sectindex]);
#endif

    /*
     * 16 directory entries per sector, but get the number
     * from the "bitmap".
     */
    for (i = 0; i < dir->bitmap[sectindex]; i++) {
	unsigned int hsect, dsect;
	const uint8_t *header;
	const uint8_t *entry = sector + (i << 4);
	uint8_t *sector;
	unsigned skip, nsec, cleft, len;
	bool is_ufd;
	struct directory ufd;
	FILE *f = NULL;
	enum sectflags whatami;
	bool check_sector;

	path->dirpath_len = dirpath_len;

	hsect = (entry[0] << 3) + (entry[1] >> 5);
	hsect <<= disk->fmt->cluster_shift;
	if (!hsect || (entry[0] & entry[1]) == 0xff ||
	    entry[4] == 0x00 || entry[4] == 0xff)
	    continue;           /* Empty slot */

	header = get_sector(disk, hsect);

	/* Length of file in sectors, including header block */
	len = entry[2] + (entry[3] << 8);

	/*
	 * Early version of ABC-DOS didn't fill in the length field; those
	 * versions all had cluster size == 1 so we can just read until the
	 * header block runs out...
	 */
	if (len == 0)
	    len = disk->fmt->sectors;

	/* 128 is a kind of extremely conservative value due to multibyte
	   blowup */
	if (path->dirpath_size - path->dirpath_len < 128) {
	    size_t abcpath_delta = path->abcpath - path->dirpath;
	    path->dirpath_size += 4096;
	    path->dirpath = xrealloc(path->dirpath, path->dirpath_size);
	    path->abcpath = path->dirpath + abcpath_delta;
	}

	path->dirpath_len +=
	    unmangle_filename(path->dirpath + path->dirpath_len, entry + 4);

	is_ufd = !memcmp(entry + 12, "Ufd", 3);

	if (hsect >= disk->fmt->sectors) {
	    fprintf(stderr,
		    "%s:%s: header sector out of range %u/%u (dir %d:%d)\n",
		    path->infile, path->abcpath,
		    hsect, disk->fmt->sectors, sectnr, i);
	    err++;
	    continue;
	} else if (disk->meta[hsect].flags &&
		   (!is_ufd && disk->meta[hsect].flags != SF_SEEN_HDR)) {
	    /*
	     * Note: a file header can be seen more than once, because
	     * some disks actually use something like "hard links"
	     * (shared data). Don't allow this for UFDs, or we could
	     * end up recursing indefinitely...
	     */
	    fprintf(stderr, "%s:%s: header sector already visited as %s: %u/%u (dir %d:%d)\n",
		    path->infile, path->abcpath,
		    seen[disk->meta[hsect].flags],
		    hsect, disk->fmt->sectors, sectnr, i);
	    err++;
	    continue;
	}
	disk->meta[hsect].flags = SF_SEEN_HDR;

	if (is_ufd) {
	    if (path->outdir)
		make_dir(path->dirpath);
	    path->dirpath[path->dirpath_len++] = '/';
	    path->dirpath[path->dirpath_len] = '\0';
	    memset(&ufd, 0, sizeof ufd);
	    ufd.disk = disk;
	    ufd.headsec = hsect;
	    ufd.dirsec = hsect + 2;
	    len = 18;		/* Header+bitmap+16 directory sectors, max */
	    if (!path->outdir) {
		/* Directory listing, print directories before their content */
		printf("      %s\n", path->abcpath);
	    }
	} else if (path->outdir) {
            f = fopen(path->dirpath, "wb");
	}

        /* Skip the header block for a file, header block and bitmap for UFD */
        skip = 1;
        cleft = 0;
        dsect = -1;
	sector = NULL;
        header += 4;
	nsec = 1;
	check_sector = true;

        while (len--) {
            if (cleft) {
                dsect++;
            } else {
                if ((header[0] & header[1]) == 0xff)
                    break;      /* Premature end of data */

                dsect = (header[0] << 3) + (header[1] >> 5);
                dsect <<= disk->fmt->cluster_shift;
                cleft = ((header[1] & 31) + 1) << disk->fmt->cluster_shift;

#ifdef DEBUG
		printf("%s: hsect %d (0x%x) dsect %d (0x%x), count %d\n",
		       path->abcpath, hsect, hsect, dsect, dsect, cleft);
#endif

                header += 2;
            }
            cleft--;

            if (skip) {
                skip--;
		continue;
	    }

	    whatami = is_ufd ? SF_SEEN_UFD : SF_SEEN_FILE;
	    if (dsect >= disk->fmt->sectors) {
		fprintf(stderr,
			"%s:%s: %s sector out of range %u/%u (dir %d:%d hdr %u)\n",
			path->infile, path->abcpath,
			is_ufd ? "ufd" : "file",
			dsect, disk->fmt->sectors, sectnr, i, hsect);
		err++;
		break;
	    } else if (disk->meta[dsect].flags &&
		       (whatami == SF_SEEN_UFD ||
			disk->meta[dsect].flags != whatami)) {
		fprintf(stderr,
			"%s:%s: %s sector already visited as %s: %u/%u (dir %d:%d hdr %u)\n",
			path->infile, path->abcpath,
			is_ufd ? "ufd" : "file",
			seen[disk->meta[dsect].flags],
			dsect, disk->fmt->sectors, sectnr, i, hsect);
		err++;
		break;
	    }
	    disk->meta[dsect].flags = whatami;

	    sector = get_sector(disk, dsect);

	    if (check_sector) {
		unsigned int sect_seq = sector[1] + (sector[2] << 8);
		if (sector[0] != (i << 4) + sectindex) {
		    fprintf(stderr, "%s:%s%s: bad ID in prefix (sec %u #%u dir %d:%d hdr %u prefix %02x)\n",
			    path->infile, path->abcpath,
			    is_ufd ? " (ufd)" : "",
			    dsect, nsec, sectnr, i, hsect, sector[0]);
		} else if (sect_seq != nsec) {
		    fprintf(stderr, "%s:%s%s: bad sequence number %u (sec %u #%u dir %d:%d hdr %u)\n",
			    path->infile, path->abcpath,
			    is_ufd ? " (ufd)" : "", sect_seq,
			    dsect, nsec, sectnr, i, hsect);
		}
	    }

	    if (is_ufd) {
		if (!ufd.bitmap) {
		    ufd.bitmap = sector + 239;
		    /*
		     * The actual UFD sectors don't have the normal
		     * 3-byte prefix
		     */
		    check_sector = false;
		} else {
		    err += extract_dir_sector(path, &ufd, dsect, nsec-2);
		}
            } else if (f) {
		fwrite(sector + 3, 1, 253, f);
            }

	    nsec++;
	}

	if (f) {
	    if (ferror(f) || fflush(f) || fclose(f)) {
		fprintf(stderr, "%s: %s: %s\n", program, path->dirpath,
			strerror(errno));
		exit(1);
	    }
	}

	if (!path->outdir && !is_ufd) {
	    /* Directory listing */
	    printf("%5u %s\n", nsec, path->abcpath);
	}
    }

    path->dirpath[path->dirpath_len = dirpath_len] = '\0';

    return err;
}

static void usage(void)
{
    fprintf(stderr, "abcdisk %s\n", VERSION);
    fprintf(stderr, "Usage: %s format|auto diskimage [outdir]\n", program);
    fprintf(stderr, "       if outdir is omitted, show directory listing\n");
    show_formats(stderr);
    exit(64);			/* EX_USAGE */
}

static bool valid_format(struct disk *disk, struct diskfmt *fmt)
{
    int i;
    unsigned int minclust, maxclust, nclust;
    unsigned int sectors, minsectors;
    int cluster_shift;
    const uint8_t *syssec, *empty; /* System and empty-system sectors */

    disk->fmt = fmt;
    syssec = get_sector(disk, fmt->syssec);
    empty  = get_sector(disk, fmt->syssec+1);

    if (fmt->cluster_shift) {
	if (empty[255] != 0 && empty[255] != fmt->cluster_shift)
	    return false;
	cluster_shift = fmt->cluster_shift;
    } else {
	cluster_shift = empty[255];
    }
    if (cluster_shift > 5)
	return false;		/* Otherwise > 64K sectors */

    /*
     * For old DOS we need 24 system sectors; for UFD-DOS we need
     * 32.
     */
    minsectors = fmt->syssec + 18;

    /*
     * To be valid, both the syssec and the empty sec need to follow:
     *
     * 1. At least 24 sectors at the start need to be marked busy.
     * 2. Any bit set in the first 239 bytes of the empty sector
     *    must also be set in the system sector.
     * 3. Bytes 239-254 in the system sector must be in the range 0-16,
     *    with the number in the empty sector less than or equal
     *    to the system sector at least for the first 8.
     * 4. Byte 239 cannot be 0.
     * 5. Byte 255 in the system sector must be 0 or = byte 255 in the
     *    empty sector (= cluster size.)
     */

    for (minclust = 0; minclust < 239*8; minclust++) {
	if (!(empty[minclust >> 3] & (0x80 >> (minclust & 7))))
	    break;
    }

    if (minclust == 239*8)
	return false;		/* Not a single usable cluster */
    if ((minclust << cluster_shift) < minsectors)
	return false;		/* Not enough space for the system */

    for (i = 0; i < 239; i++) {
	if (empty[i] & ~syssec[i])
	    return false;
    }

    if (syssec[239] == 0)
	return false;

    for (i = 239; i < 239+8; i++) {
	if (syssec[i] > 16 || empty[i] > syssec[i])
	    return false;
    }

    /*
     * Some DOS versions seem to stuff different information in the
     * empty sector here, like free sectors, so don't check
     * the empty sector in this range?
     */
    for (i = 239+8; i < 239+16; i++) {
	if (syssec[i] > 16)
	    return false;
    }

    if (syssec[255] != 0 && syssec[255] != empty[255])
	return false;

    /*
     * Clusters marked unusable in the backup system track at the end
     * can be interpreted as the length of the disk. It MAY be caused
     * by bad sectors, but it doesn't actually matter.
     */
    for (maxclust = 239*8-1; maxclust >= minclust; maxclust--) {
	if (!(empty[maxclust >> 3] & (0x80 >> (maxclust & 7))))
	    break;
    }

    nclust = maxclust + 1;
    sectors = nclust << cluster_shift;
    if (sectors < minsectors)
	return false;

    /*
     * Try to figure out the location and length of the MFD. 16 refers
     * to the backup directory on ABC-DOS and the only directory on
     * UFD-DOS, so it is always safe for reading.
     */
    fmt->dirsec[0] = fmt->dirsec[1] = 16;
    for (i = 0; i < 16; i++) {
	if (syssec[239+i] < 1 || syssec[239+i] > 16)
	    break;
    }

    fmt->cluster_shift = cluster_shift;
    fmt->sectors = sectors;
    fmt->dirsecs = i;

#ifdef DEBUG
    printf("interleave: %u  syssec: %u  cluster: %d  sectors: %u\n",
	   fmt->il, fmt->syssec, 1 << cluster_shift, fmt->sectors);
#endif

    disk_update_ptrs(disk);
    return true;
}

static const struct diskfmt *guess_format(struct disk *disk)
{
    static struct diskfmt fmt;
    static const unsigned char clust_shifts[] = { 0, 2, 5 };
    struct trackperm {
	unsigned int s;
	unsigned int dstart;
	bool chflip;
    };
    static const struct trackperm trackperm[] = {
	{ 16, 0, false },	/* Most common */
	{ 26, 0, false },	/* 8" disks */
	{ 26, 26, false },	/* 8" disks without stripped track 0 */
	{ 26, 26, true },	/* d:o with sec/cyl reversed */
    };
    size_t cf, tp;

    disk->fmt = &fmt;
    fmt.name = "auto";

    /*
     * These apply to 8" drives, which is the only case
     * where this actually matters for reading...
     */
    fmt.c = 77;
    fmt.h =  2;

    /*
     * Last resort loop: try to force the cluster shift
     * (0 = attempt autodetect)
     */
    for (cf = 0; cf < ARRAY_SIZE(clust_shifts); cf++) {
	fmt.cluster_shift = clust_shifts[cf];

	/*
	 * This is used for interleave detection. Not sure if there
	 * is any sane way to look for other interleaved track sizes...
	 * definitely not without analyzing disk dumps in more detail.
	 */
	for (tp = 0; tp < ARRAY_SIZE(trackperm); tp++) {
	    fmt.s      = trackperm[tp].s;
	    fmt.dstart = trackperm[tp].dstart;
	    fmt.chflip = trackperm[tp].chflip;

	    /* Try no interleaving first of all */
	    fmt.iloffs = 0;
	    fmt.il = 1;

	    do {
		/* Try 6 and 14 as the system sector */
		for (fmt.syssec = 6; fmt.syssec <= 14; fmt.syssec += 8) {
		    if (valid_format(disk, &fmt))
			return &fmt;
		}

		/* The most common seem to be 1 and 7, so try in that order... */
		fmt.il = (fmt.il + 6) % fmt.s;
	    } while (fmt.il > 1);
	}
    }

    return disk->fmt = NULL;	/* Can't autodetect */
}

int main(int argc, char *argv[])
{
    struct disk *disk;
    const struct diskfmt *fmt;
    FILE *f;
    unsigned int i;
    const char *format, *infile, *outdir;
    struct pathinfo path;
    size_t outdir_len;
    int err = 0;

    setlocale(LC_ALL, "");

    program = argv[0];

    if (argc < 3 || argc > 4)
	usage();

    format = argv[1];
    infile = argv[2];
    outdir = argv[3];

    f = !strcmp(infile, "-") ? stdin : fopen(infile, "rb");
    if (!f) {
       perror(infile);
       exit(65);		/* EX_NOINPUT */
    }

    if (!strcmp(format, "auto") || !format[0]) {
	fmt = NULL;
    } else if (!(fmt = get_format(format))) {
	usage();
    }

    disk = allocate_disk(fmt);

    /* Don't error out on a short image; it might still be readable */
    fread(disk->image, 1, (fmt ? fmt->sectors : MAX_SECTORS) << 8, f);
    if (ferror(f)) {
       perror(infile);
       exit(1);
    }
    if (f != stdin)
        fclose(f);

    if (!fmt) {
	fmt = guess_format(disk);
	if (!fmt) {
	    fprintf(stderr, "%s: unable to guess disk format (corrupt?)\n", infile);
	    exit(76);		/* EX_PROTOCOL */
	}
    }

    /* If the target directory doesn't exist, try to create it */
    if (outdir && make_dir(outdir) && errno != EEXIST) {
	perror(outdir);
	exit(73);		/* EX_CANTCREAT */
    }

    memset(&path, 0, sizeof path);
    path.infile       = infile;
    path.outdir       = outdir;
    path.dirpath_len  = outdir_len = outdir ? strlen(outdir) : 0;
    path.dirpath_size = path.dirpath_len + 8192; /* Kind of arbirary */
    path.dirpath      = xmalloc(path.dirpath_size);
    if (outdir_len) {
	char term = outdir[outdir_len-1];

	memcpy(path.dirpath, outdir, outdir_len);

	if (term != '/'
#if defined(__MSDOS__) || defined(__WIN32__)
	    && term != '\\' && term != ':'
#endif
	    ) {
	    path.dirpath[path.dirpath_len++] = '/';
	}
    }
    path.abcpath = path.dirpath + path.dirpath_len;
    *path.abcpath = '\0';

    /* Process the MFD */
    disk->mfd.disk    = disk;
    disk->mfd.bitmap  = get_sector(disk, disk->fmt->syssec) + 239;
    disk->mfd.headsec = 0;	/* MFDs don't have file headers */

    for (i = 0; i < disk->fmt->dirsecs; i++) {
	err += extract_dir_sector(&path, &disk->mfd, disk->fmt->dirsec[0]+i, i);
    }
    free_disk(disk);
    free(path.dirpath);

    return err ? 64 : 0;	/* EX_DATAERR */
}
