/* BBCIM: Acorn BBC computer disk image manipulation and more
 *
 * disk_expand.c
 *
 * Copyright (C) 1996,2003 W.H. Scholten
 */


/* For Acorn DFS only */
int bbcim_disk_expand(struct bbc_archive__diskimage *image, int sectors, int verbose) {
	FILE *fpdisk;
	int file_length;
	int expand_length;


	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	if (image->attr->type != BBC_ARCHIVE__DFS_DISK)
		return bbcim_print_error_unsupported_disktype("bbcim_disk_expand", image, verbose);

	switch(image->attr->type) {
	case BBC_ARCHIVE__DFS_DISK:
		fpdisk = fopen(image->filename, "rb+");
		fseek(fpdisk, 0, SEEK_END);
		file_length = ftell(fpdisk);
		expand_length = sectors * 256 * image->attr->sides;

		if (expand_length >= file_length) {
			unsigned char bootoption;
			unsigned char filebyte;
			int  i, side;

			filebyte = 0; /* What about E5? Acorn uses that for formatting */
			for (i=0; i<(expand_length - file_length); i++)
				fwrite(&filebyte, 1, 1, fpdisk);


			/* Adjust sectors-on-disk */
			for (side=0; side<image->attr->sides; side++) {
				/* Rescue the bootoption: */
				bbcim_disk_read(image, fpdisk, 256L+6L, side, &filebyte);
				bootoption = filebyte  & (3 << 4);

				/* Write new disk size and old bootoption */
				filebyte = ((sectors >> 8) & 3) | bootoption;
				bbcim_disk_write(image, fpdisk, 256L+6L, side, &bootoption);
				filebyte = (sectors & 0xFF);
				bbcim_disk_write(image, fpdisk, 256L+7L, side, &filebyte);
			}
		}
		fclose(fpdisk);
		break;

	default:
//		bbcim_errno = BBCIM_ERROR__NOT_IMPLEMENTED;
		break;
	}

	/* Now write a .dsk file */
	bbc_archive__write_disk_attributes(image->filename, image->attr, verbose);

	return bbcim_errno;
}
