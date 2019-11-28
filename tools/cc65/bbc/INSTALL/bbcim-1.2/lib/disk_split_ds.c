/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_split_ds.c:
 *   NEDERLANDS: splits om-en-om diskbeelden, voor single/double density
 *   ENGLISH: split double sided disk images, for single/double density
 *
 * Copyright (C) W.H.Scholten 1996, 1997, 1999, 2001, 2003
 */


#include <errno.h>


/* Split an interleaved (double sided) disk image into 2 separate single sided
 * diskimages.
 */

int bbcim_split_doublesided(struct bbc_archive__diskimage *image, int verbose) {
	char  side0 [ 256 ];
	char  side1 [ 256 ];
	FILE *fp0 = NULL, *fp1 = NULL, *fp_inter = NULL;

	unsigned char filebyte;
	int  track;

	if (bbcim_debug) printf("DEBUG: bbcim_split_doublesided: %s\n", image->filename);
	strcpy(side0, image->filename);
	strcpy(side1, image->filename);
	strcat(side0, ".0");
	strcat(side1, ".1");

	if (bbcim_file_sanitycheck(image->filename, verbose)) return bbcim_errno;

	if (image->attr->sides != 2) {
		bbcim_error = "Bad number of sides";
		bbcim_errno = BBCIM_ERROR__BAD_DISK_TYPE_ARGUMENT;
		goto leave_split_ds;
	}

	fp_inter = fopen(image->filename, "rb");
	fp0 = fopen(side0, "wb");
	fp1 = fopen(side1, "wb");

	if (fp_inter == NULL || fp0 == NULL || fp1 == NULL) {
		bbcim_error = "Open failed";
		bbcim_errno = BBCIM_ERROR__OPEN_FAILED;
		goto leave_split_ds;
	}


	fseek(fp_inter, 0L, SEEK_SET);
	for (track=0; track<image->attr->tracks; track++) {
		int  i;
		int  track_size = image->attr->sectors_per_track * image->attr->bytes_per_sector;

		for (i=0; i<track_size; i++) {
			if (!fread(&filebyte, 1, 1, fp_inter)) {
				bbcim_error = "Disk truncated"; bbcim_errno = BBCIM_ERROR__DISK_TRUNCATED;
				goto leave_split_ds;
			}
			fwrite(&filebyte, 1, 1, fp0);
		}
		for (i=0; i<track_size; i++) {
			if (!fread(&filebyte, 1, 1, fp_inter)) {
				bbcim_error = "Disk truncated"; bbcim_errno = BBCIM_ERROR__DISK_TRUNCATED;
				goto leave_split_ds;
			}
			fwrite(&filebyte, 1, 1, fp1);
		}
	}

  leave_split_ds:
	if (fp0) {
		fclose(fp0);

		/* Now write a .dsk file */
		bbc_archive__write_disk_attributes2(
			side0,
			1, image->attr->tracks, image->attr->sectors_per_track, image->attr->bytes_per_sector, image->attr->type, verbose);
	}

	if (fp1) {
		fclose(fp1);

		/* Now write a .dsk file */
		bbc_archive__write_disk_attributes2(
			side1,
			1, image->attr->tracks, image->attr->sectors_per_track, image->attr->bytes_per_sector, image->attr->type, verbose);
	}

	if (fp_inter) fclose(fp_inter);


	if (verbose && bbcim_errno) fprintf(stderr, "%s: %s\n", image->filename, bbcim_error);

	return bbcim_errno;
}
