/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_combine_ss.c:
 *   NEDERLANDS: maak om-en-om diskbeeld, single/double density
 *   ENGLISH: interleave single sided disk images
 *
 * Copyright (C) W.H.Scholten 1996, 1997, 1999, 2000, 2001, 2003
 */


#include <errno.h>


/* Interleave 2 single sided disk images (with identical bytes/sector and
 * sectors/track) into a double sided disk image.
 */
int bbcim_interleave_singlesided(struct bbc_archive__diskimage *image0, struct bbc_archive__diskimage *image1, char *interleaved, int verbose) {
	FILE *fp0, *fp1, *fp_inter;
	int  track;
	int  track_size;

//	struct bbc_archive__diskimage image; /* interleaved image */

	if (bbcim_file_sanitycheck(image0->filename, verbose))
		return bbcim_errno;
	if (bbcim_file_sanitycheck(image1->filename, verbose))
		return bbcim_errno;

	if (image0->attr->sides != 1 || image1->attr->sides != 1) {
		bbcim_error = "bbcim_interleave_singlesided: disk images must be single sided";
		bbcim_errno = -1;
		if (verbose) fprintf(stderr, "%s\n", bbcim_error);
		return bbcim_errno;
	}
	if ((image0->attr->bytes_per_sector != image1->attr->bytes_per_sector) || (image0->attr->sectors_per_track != image1->attr->sectors_per_track)) {
		bbcim_error = "bbcim_interleave_singlesided: disk images must have same no. of bytes/sectors";
		bbcim_errno = -1;
		if (verbose) fprintf(stderr, "%s\n", bbcim_error);
		return bbcim_errno;
	}

	fp_inter = fopen(interleaved, "wb");
	if (fp_inter == NULL) return -1; //goto leave_bbcim_interleave_singlesided

//	if (bbcim_debug) printf("  %s + %s > %s\n\n", image0->filename, image1->filename, interleaved);


	fp0 = fopen(image0->filename, "rb");
	fp1 = fopen(image1->filename, "rb");

	fseek(fp0, 0L, SEEK_SET);
	fseek(fp1, 0L, SEEK_SET);
	fseek(fp_inter, 0L, SEEK_SET);

	track_size = (image0->attr->bytes_per_sector * image0->attr->sectors_per_track);

	for (track=0; track< image0->attr->tracks;track++) {
		int  i;
		unsigned char filebyte;

		fseek(fp0, 0L + track * track_size, SEEK_SET);
		for (i=0; i<track_size; i++) {
			if (feof(fp0) && feof(fp1)) return 0;

			if (feof(fp0)) filebyte = 0; else fread(&filebyte, 1, 1, fp0);
			fwrite(&filebyte, 1, 1, fp_inter);
		}
		fseek(fp1, 0L + track * track_size, SEEK_SET);
		for (i=0; i<track_size; i++) {
			if (feof(fp1) && feof(fp0)) return 0;

			if (feof(fp1)) filebyte = 0; else fread(&filebyte, 1, 1, fp1);
			fwrite(&filebyte, 1, 1, fp_inter);
		}
	}
	fclose(fp0);
	fclose(fp1);
	fclose(fp_inter);

	/* Now write a .dsk file */
	bbc_archive__write_disk_attributes2(
		interleaved,
		2, image0->attr->tracks, image0->attr->sectors_per_track, image0->attr->bytes_per_sector, image0->attr->type, 1);

	return 0;
}
