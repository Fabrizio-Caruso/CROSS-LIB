/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_convert_w62_to_dfs.c
 *
 * Copyright (C) 1997, 2000, 2003 W.H. Scholten
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int bbcim_w62_to_dfs(struct bbc_archive__diskimage *image,  int verbose) {
	int  i;
	int st = 0;
	unsigned char filebyte;
	char disk1[256], disk2[256];
	FILE *fp_w62, *fp_dfs;

	int s = 0; /* side, the code currently only does side 0 */

	struct bbc_archive__diskimage image_dfs;
	struct bbc_archive__diskimage_attr image_dfs_attr;

	image_dfs.attr = &image_dfs_attr;
	image_dfs_attr.type = BBC_ARCHIVE__DFS_DISK;
	image_dfs_attr.sides = 1;
	image_dfs_attr.tracks = 80;
	image_dfs_attr.sectors_per_track = 10;
	image_dfs_attr.bytes_per_sector = 256;


	strcpy(disk1, image->filename);
	strcpy(disk2, image->filename);
//OLD, useful for DOS etc. with limited length filenames:
//OLD	disk1[ strcspn(disk1, ".") ] = 0;
	strcat(disk1, ".w1");
	strcat(disk2, ".w2");

	if (image->attr->type != BBC_ARCHIVE__W62_DISK) {
		/* Perhaps allow .w62 to identify this format? */
		bbcim_error = "Bad disk format";
		bbcim_errno = -1;
		if (verbose) fprintf(stderr, "%s: %s\n", image->filename, bbcim_error);
		return -1;
	}

	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	fp_w62 = fopen(image->filename, "rb");
	if (fp_w62 == NULL) {
		if (verbose) fprintf(stderr, "Can't open %s\n", image->filename);
		return -1;
	}

	/* Put this in bbcim_diskimage_sanitycheck ?*/
	/* Verify it's a Watford 62 file image */
	for (i=0; i<7; i++) {
		if (bbcim_disk_read(image, fp_w62, 512 + OFFSET__DFS_entries, s, &filebyte)) return -1;
		if (filebyte != 0xAA) {
			if (verbose) fprintf(stderr, "%s is not a Watford 62 file image (there should be 8 bytes AA at positions 512-519)\n", image->filename);
			return -1;
		}
	}

	/* are there files on image 2? */
	if (bbcim_disk_read(image, fp_w62, 512 + OFFSET__DFS_entries, s, &filebyte)) return -1;

	if ((filebyte %8) != 0) {
		/* Not a valid 2nd catalogue */
		if (verbose) fprintf(stderr, "%s: not a valid 2nd catalogue\n", image->filename);
		return -1;
	}

	if (filebyte) {
		fp_dfs = fopen(disk2, "wb");
		if (fp_dfs == NULL) {
			if (verbose) fprintf(stderr, "Can't write to %s\n", disk2);
			return -1;
		}

		/* Move the 2nd catalogue 2 sectors down for the new disk */
		for (i=0; i<512; i++) {
			if (bbcim_disk_read(image, fp_w62, 512+i, s, &filebyte)) return -1;
			if (bbcim_disk_write(&image_dfs, fp_dfs, 0+i, 0, &filebyte)) return -1;
		}

		/* Copy the rest of the diskimage to the new diskimage, sectors etc. are correct if the sectors
		 * are copied and not moved up/down. Use -min to gain back space, afterwards. */
		for (i=512; i<200*1024; i++) {
			st |= bbcim_disk_read(image, fp_w62, 512+i, s, &filebyte);
			if (feof(fp_w62)) break;
			st |= bbcim_disk_write(&image_dfs, fp_dfs, 0+i, 0, &filebyte);
		}
		fclose(fp_dfs);

		/* Now write a .dsk file for DFS disk image 2 */
		st |= bbc_archive__write_disk_attributes2(
			disk2,
			image->attr->sides, image->attr->tracks, image->attr->sectors_per_track, image->attr->bytes_per_sector, BBC_ARCHIVE__DFS_DISK, verbose);
	}

	{ /* first diskimage */
		fp_dfs = fopen(disk2, "wb");
		if (fp_dfs == NULL) {
			if (verbose) fprintf(stderr, "Can't write to %s\n", disk1);
			return -1;
		}

		fseek(fp_w62, 0, SEEK_SET);
		for (i=0; i<200*1024; i++) {
			st |= bbcim_disk_read(image, fp_w62, i, s, &filebyte);
			if (feof(fp_w62)) break;
			st |= bbcim_disk_write(&image_dfs, fp_dfs, i, 0, &filebyte);
		}
		filebyte = 0x00; /* for W62 these bytes are 0xAA, DFS anything else. So by zeroing the diskimage will be DFS. */
		for (i=0; i<8; i++) {
			st |= bbcim_disk_write(&image_dfs, fp_dfs, 512+i, 0, &filebyte);
		}
		fclose(fp_dfs);

		/* Now write a .dsk file for DFS disk image 1 */
		bbc_archive__write_disk_attributes2(
			disk1,
			image->attr->sides, image->attr->tracks, image->attr->sectors_per_track, image->attr->bytes_per_sector, BBC_ARCHIVE__DFS_DISK, verbose);
	}

	fclose(fp_w62);

	return st;
}
