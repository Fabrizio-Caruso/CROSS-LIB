/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_new.c
 *
 * Copyright (C) 1996 - 2003 W.H. Scholten
 */


/* Acorn DFS only at the moment */
int bbcim_disk_new(struct bbc_archive__diskimage *image, int verbose) {
	FILE *fpdisk;
	unsigned char filebyte;
	int i;
	int side, sectors;

	bbcim_errno = 0;
	if (image->attr->type != BBC_ARCHIVE__DFS_DISK)
		return bbcim_print_error_unsupported_disktype("bbcim_disk_new", image, verbose);


	fpdisk = fopen(image->filename, "wb");

	switch(image->attr->type) {
	case BBC_ARCHIVE__DFS_DISK:
//		if (image->attr->sectors_per_track != 10 /* SD */ && image->attr->sectors_per_track != 18 /* DD */) return -1;
//		if (image->attr->bytes_per_sectors != 256) return -1;
		sectors = image->attr->sectors_per_track * image->attr->tracks;
		if (sectors > 0xFFF) sectors = 0xFFF; /* not allowed in true DFS but might be useful for big disks in beebem. */

		for (side = 0; side < image->attr->sides; side++) {

			filebyte = 0;
			for (i=0; i<512; i++) {
				bbcim_disk_write(image, fpdisk, i, side, &filebyte);
			}

			/* Set disk size (sectors) & default boot option */
			filebyte = (sectors >> 8);
			filebyte |= 3 << 4;  /* i.e. bootoption = *EXEC, sectors-on-disk = 'sectors' */
			bbcim_disk_write(image, fpdisk, 256+6, side, &filebyte);
			filebyte = (sectors & 0xFF);
			bbcim_disk_write(image, fpdisk, 256+6 + 1, side, &filebyte);
		}

		image->attr->tracks = sectors / image->attr->sectors_per_track;
		break;

	case BBC_ARCHIVE__ADFS_DISK:

	default:
		break;
	}

	/* Now write a .dsk file */
	bbc_archive__write_disk_attributes(image->filename, image->attr, verbose);

	return bbcim_errno;
}
