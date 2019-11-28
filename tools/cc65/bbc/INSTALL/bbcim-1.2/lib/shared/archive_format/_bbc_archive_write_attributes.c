/*  _bbc_archive_write_attributes.c
 *
 * Functions to write Acorn BBC computer file attributes (.inf) and disk image attributes (.dsk)
 * These functions originated in 'bbcim'. Later separated for use in 'bbctape', 'bbccom' and other programs.
 
 
   Copyright (C) W.H. Scholten 1996, 1997, 2001, 2003, 2009, 2014
 
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
 */



#include <_bbc_archive.h>

/* Return 1 if a file is a bbc attributes file, 0 if not */
int bbc_archive__is_inf_file(char *filename) {
	int l = strlen(filename);
	if (l > 4)
		if (!strncmp(filename+l-4, ".inf", 4)) return 1;

	return 0;
}


/* Allocate space for and make a .inf filename corresponding to the given
 * bare file's filename. */

//char *bbc_archive__new_inf_filename(char *filename, struct bbc_error *error) {


char *bbc_archive__new_inf_filename(char *filename, int verbose) {
	char *inf_file = malloc(strlen(filename) + 1 + 4);
	if (inf_file) {
		strcpy(inf_file, filename);
		strcat(inf_file, ".inf");
	}
	else {
//		bbc_error__set_error(BBC_ERROR__OUT_OF_MEMORY, 0);
//		if (verbose) fprintf(stderr, "create .inf file with %s: %s\n", filename, bbc_archive__error);
		if (verbose) fprintf(stderr, "create .inf file with %s: Out of memory\n", filename);
	}

	return inf_file;
}


/* Allocate space for and make a .dsk filename corresponding to the given
 * bare diskimage's filename. */
char *bbc_archive__new_dsk_filename(char *diskname, int verbose) {
	char *dsk_file = malloc(strlen(diskname) + 1 + 4);
	if (dsk_file) {
		strcpy(dsk_file, diskname);
		strcat(dsk_file, ".dsk");
	}
	else {
//		bbc_error__set_error(BBC_ERROR__OUT_OF_MEMORY, 0);
//		if (verbose) fprintf(stderr, "create .dsk file with %s: %s\n", diskname, bbc_archive__error);
		if (verbose) fprintf(stderr, "create .dsk file with %s: Out of memory\n", diskname);
	}

	return dsk_file;
}


/* */
//int bbc_archive__write_file_attributes(char *filename, struct bbcim_file *file, int verbose)
int bbc_archive__write_file_attributes(char *filename, struct bbc_archive__file_attr *file, int verbose) {
	char *inf = bbc_archive__new_inf_filename(filename, verbose);

	if (inf) {
		FILE *fpinf = fopen(inf, "wb");
		if (fpinf) {
//			fprintf(fpinf, "%s %8X %8X %s CRC=%4X\n", file->attributes->name, file->attributes->load_address, file->attributes->exec_address, (file->attributes->attributes & BBC_ARCHIVE__ATTR__LOCKED) ? "Locked" : "",  file->attributes->crc);
			fprintf(fpinf, "%s %8X %8X%s", file->name, file->load_address, file->exec_address, (file->attributes & BBC_ARCHIVE__ATTR__LOCKED) ? " Locked" : "");
			if (file->crc >= 0) fprintf(fpinf, " CRC=%4X", file->crc);
			fprintf(fpinf, "\n");
			fclose(fpinf);
		}
		else {
			if (verbose) fprintf(stderr, "create .inf file failed\n");
			free(inf);
			return -1;
		}
		free (inf);
	}
	else return -1;

	return 0;
}

char *bbc_archive__dfs_names[] = { "Unknown", "DFS", "DDOS", "W62", "SOLIDISK_DDFS", "ADFS", "HDFS" };
char *bbc_archive__dfs_sides[] = { "SS", "DS" };

int bbc_archive__write_disk_attributes2(char *diskname, int sides, int tracks, int sectors, int bytes, int type, int verbose) {
	struct bbc_archive__diskimage image;
	struct bbc_archive__diskimage_attr image_attr;

	image.attr = &image_attr;

	image.attr->type = type;
	image.attr->sides = sides;
	image.attr->tracks = tracks;
	image.attr->sectors_per_track = sectors;
	image.attr->bytes_per_sector = bytes;

	image.filename = diskname;

	return bbc_archive__write_disk_attributes(diskname, &image_attr, verbose);
}


//int bbc_archive__write_disk_attributes(struct bbc_archive__diskimage *image, int verbose) {
int bbc_archive__write_disk_attributes(char *filename, struct bbc_archive__diskimage_attr *image_attr, int verbose) {
	char *dsk = bbc_archive__new_dsk_filename(filename, verbose);

	if (dsk) {
		FILE *fpdsk = fopen(dsk, "wb");
		if (fpdsk) {
//			fprintf(fpdsk, "%s %s %dT %dS %dB\n", bbc_archive__dfs_names[ image->attributes->type ], bbc_archive__dfs_sides[ image->attributes->sides ], image->attributes->tracks, image->attributes->sectors_per_track, image->attributes->bytes_per_sector);
			fprintf(fpdsk, "%s %s %dT %dS %dB\n", bbc_archive__dfs_names[ image_attr->type ], bbc_archive__dfs_sides[ image_attr->sides-1 ], image_attr->tracks, image_attr->sectors_per_track, image_attr->bytes_per_sector);
			fclose(fpdsk);
		}
		else {
			if (verbose) fprintf(stderr, "create .dsk file failed\n");
			free(dsk);
			return -1;
		}
		free (dsk);
	}
	else return -1;

	return 0;
}
