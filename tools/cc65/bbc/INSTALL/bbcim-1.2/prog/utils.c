/* BBCIM: utils: utilities for the bbcim program

   Copyright (C) W.H.Scholten 1996, 1997, 1999, 2001, 2003

   Permission to use, copy, modify, distribute, and sell this software
   and its documentation for any purpose is hereby granted without fee,
   provided that the above copyright notice appear in all copies and
   that both that copyright notice and this permission notice appear in
   supporting documentation, and that the name of the copyright holder
   not be used in advertising or publicity pertaining to distribution
   of the software without specific, written prior permission. The
   copyright holder makes no representations about the suitability of
   this software for any purpose. It is provided "as is" without express
   or implied warranty.
  
   THE COPYRIGHT HOLDER DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
   SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
   FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
   SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
   RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
   CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
   CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
  
  
   Send comments and bug-reports to 
  
      swhs-web@xs4all.nl
  
*/



/*__________________INCLUDES___________________*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "_config.h"
#include <bbcim.h>

#include "../lib/bbcim_internal.h" /* temp */


/*__________________DATA___________________*/



/*__________________PROGRAMMA___________________*/
extern void argc_check(int argc, int options, int required);

/* Prototypes: */
void set_diskimage_defaults(struct bbc_archive__diskimage *image);
void set_diskimage_invalid(struct bbc_archive__diskimage *image);
int copy_disktype(struct bbc_archive__diskimage *from_image1, struct bbc_archive__diskimage *to_image2);
int get_side(char **argv, int argc);
int set_disktype(struct bbc_archive__diskimage *image, char **argv, int argc, int verbose);
int get_disk_type_name_side(char **argv, int argc, struct bbc_archive__diskimage *image, int *side);


/* Functions: */

/* Disk defaults: */
void set_diskimage_defaults(struct bbc_archive__diskimage *image) {
//	image->filename = disk;

	image->attr->type = BBC_ARCHIVE__DFS_DISK;
	image->attr->sides = 1;
	image->attr->tracks = 80;
	image->attr->sectors_per_track = BBCIM__SD_SECTORSPERTRACK;
	image->attr->bytes_per_sector = BBCIM__SD_SECTORSIZE;
}
void set_diskimage_invalid(struct bbc_archive__diskimage *image) {
//	image->filename = disk;

	image->attr->type = BBC_ARCHIVE__UNKNOWN_DISK;
	image->attr->sides = 0;
	image->attr->tracks = 0;
	image->attr->sectors_per_track = 0;
	image->attr->bytes_per_sector = 0;
}

int copy_disktype(struct bbc_archive__diskimage *from_image1, struct bbc_archive__diskimage *to_image2) {
	to_image2->attr->type = from_image1->attr->type;
	to_image2->attr->sides = from_image1->attr->sides;
	to_image2->attr->tracks = from_image1->attr->tracks;
	to_image2->attr->sectors_per_track = from_image1->attr->sectors_per_track;
	to_image2->attr->bytes_per_sector = from_image1->attr->bytes_per_sector;

	return 0;
}


int get_side(char **argv, int argc) {
	int side = -1;

	if (argc < 2) return -1;
	if (!strcmp(argv[0], "-side")) {
		side = atol(argv[1]);
	}

	return side;
}


int set_disktype(struct bbc_archive__diskimage *image, char **argv, int argc, int verbose) {
//	int i;
	int option = 0;

	for (;;) {
		char *arg = argv[option];
		if (argc - option < 2) break;

		if (bbcim_debug) fprintf(stderr, "set_disktype: option %s\n", arg);

		if (arg == NULL) break;
		if (arg[0] != '-') break;

		if (!strcmp(arg, "-type")) {
			if (bbc_archive__diskinfo_from_string(argv[option+1], image->attr)) {
				bbcim_error = "Bad option\n";
				bbcim_errno = BBCIM_ERROR__NO_INF_FOUND;
				if (verbose) fprintf(stderr, "set_disktype: bad option %s\n", arg);
//				return bbcim_errno;
				return 0;
			}
			option += 2;
		}
		else if (!strcmp(arg, "-bytes")) {
			image->attr->bytes_per_sector = atol(argv[option+1]);
			option += 2;
		}
		else if (!strcmp(arg, "-sectors")) {
			image->attr->sectors_per_track = atol(argv[option+1]);
			option += 2;
		}
		else if (!strcmp(arg, "-tracks")) {
			image->attr->tracks = atol(argv[option+1]);
			option += 2;
		}
		else if (!strcmp(arg, "-sides")) {
			image->attr->sides = atol(argv[option+1]);
			option += 2;
		}
		else break;
	}

	return option;
}


int get_disk_type_name_side(char **argv, int argc, struct bbc_archive__diskimage *image, int *side) {
	struct bbc_archive__diskimage image1;
	struct bbc_archive__diskimage_attr image_attr1;
	int  options;
	int  tmp_side;
	int  st;


	options = 0;
	image1.attr = &image_attr1;
//	set_diskimage_defaults(&image1);
	set_diskimage_invalid(&image1);

	st = set_disktype(&image1, argv+options, argc-options, 1);
	if (st > 0) { options += st; }

	argc_check(argc, options, 1);
	strcpy(image->filename, argv [ options ]); options++;
	if (strlen(image->filename) >= 4) {
		if (!strcmp(image->filename + strlen(image->filename)-4, ".ssd")) {
			set_diskimage_defaults(&image1);
			image1.attr->sides = 1;
		}
		else if (!strcmp(image->filename + strlen(image->filename)-4, ".dsd")) {
			set_diskimage_defaults(&image1);
			image1.attr->sides = 2;
		}
	}
	if (bbcim_debug) printf("DEBUG: image->filename=%s\n,      image1->sides=%d\n", image->filename, image1.attr->sides);

	copy_disktype(&image1, image);
	if (bbcim_debug) printf("DEBUG: image->sides=%d\n", image->attr->sides);
	if (bbc_archive__read_disk_attributes(image->filename, image->attr, 1)) {
		if (bbcim_debug) printf("no attributes found\n");
//		copy_disktype(&image1, image);
//		printf("image->attr->type=%d\n", image->attr->type);
	}


	/* Now set defaults for things not set by options/filename: */
	if (image->attr->type == BBC_ARCHIVE__UNKNOWN_DISK)
		image->attr->type = BBC_ARCHIVE__DFS_DISK;
	if (image->attr->sides == 0)
		image->attr->sides = 1;
	if (image->attr->tracks == 0)
		image->attr->tracks = 80;
	if (image->attr->sectors_per_track == 0)
		image->attr->sectors_per_track = BBCIM__SD_SECTORSPERTRACK;
	if (image->attr->bytes_per_sector == 0)
		image->attr->bytes_per_sector = BBCIM__SD_SECTORSIZE;

//	argc_check(argc, options, 1);

	tmp_side = get_side(argv+options, argc-options);
	if (tmp_side >= 0) {
		*side = tmp_side;
		options += 2;
	}

	if (options == 0) {
		fprintf(stderr,
				TEKST_NL("Disk?\n")
				TEKST_ENG("Disk?"));
		exit(1);
	}

	return options;
}
