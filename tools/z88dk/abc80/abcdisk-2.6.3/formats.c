#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "abcdisk.h"

#define F(c,h,s) (c),(h),(s),((c)*(h)*(s))

static const struct diskfmt formats[] = {
    { "fd2",	    F( 40,1, 8), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "dd80",	    F( 40,1, 8), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "fd2d",	    F( 40,1,16), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "dd82",	    F( 40,1,16), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "abc830",     F( 40,1,16), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "mo",         F( 40,1,16), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "abc830-ufd", F( 40,1,16), 0,  6, { 16, 0 }, 16, 1, 0, 0, false },
    { "fd4d",       F( 40,2,16), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "dd84",       F( 40,2,16), 0,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "abc832",     F( 80,2,16), 2, 14, { 16, 0 }, 16, 1, 0, 0, false },
    { "abc832-ufd", F( 80,2,16), 2, 14, { 8, 16 },  8, 1, 0, 0, false },
    { "abc834",     F( 80,2,16), 2, 14, { 16, 0 }, 16, 1, 0, 0, false },
    { "mf",         F( 80,2,16), 2, 14, { 16, 0 }, 16, 1, 0, 0, false },
    /*
     * 8" floppies have a different logical sector/track (126/2/16)
     * mapping than the physical one (which is 77/2/26), and
     * then the last virtual track is only partially filled.
     * Furthermore, the first physical track (26 sectors) is unused.
     */
    { "abc838",     77, 2, 26, 3978, 2,  6, { 8, 16 },  8, 1, 0, 0, false },
    { "abc838-ufd", 77, 2, 26, 3978, 2, 14, { 16, 0 }, 16, 1, 0, 0, false },
    { "sf",         77, 2, 26, 3978, 2, 14, { 16, 0 }, 16, 1, 0, 0, false },
    { "hd",         F(238,8,32),     5, 14, { 16, 0 }, 16, 1, 0, 0, false },
};

static const int nformats = (sizeof(formats)/sizeof(formats[0]));

const struct diskfmt *get_format(const char *name)
{
    const struct diskfmt *fmt;
    int i;

    for (i = 0, fmt = formats; i < nformats; i++, fmt++) {
	if (!strcmp(fmt->name, name))
	    return fmt;
    }

    return NULL;
}

void show_formats(FILE *f)
{
    const struct diskfmt *fmt;
    int i;

    fprintf(f, "Format      Tracks  Sides  Sec/trk  Cluster  Sectors  Total  System   MFD\n");

    for (i = 0, fmt = formats; i < nformats; i++, fmt++) {
	fprintf(f, "%-11s %4d %6d %7d %9d %9d %6dK %7d %3d/%2d\n",
		fmt->name, fmt->c, fmt->h, fmt->s,
		1 << fmt->cluster_shift,
		fmt->sectors, fmt->sectors >> 2, fmt->syssec,
		fmt->dirsec[0], fmt->dirsecs);
    }
}
