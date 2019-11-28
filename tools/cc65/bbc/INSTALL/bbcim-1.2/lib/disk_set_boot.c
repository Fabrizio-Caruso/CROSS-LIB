/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_set_boot.c
 *
 * Copyright (C) 1997, 2001, 2003 W.H. Scholten
 */


/* Acorn DFS */
int bbcim_disk_set_boot(struct bbc_archive__diskimage *image, int side, int bootoption, int verbose) {
	FILE *fpdisk;
	unsigned char filebyte;

	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	if (side < 0 || (side >= image->attr->sides))
		return bbcim_print_error_bad_side("bbcim_disk_set_boot", image, side, verbose);

	if (image->attr->type != BBC_ARCHIVE__DFS_DISK)
		return bbcim_print_error_unsupported_disktype("bbcim_disk_set_boot", image, verbose);


	fpdisk = fopen(image->filename, "rb+");
	filebyte = 0;

	switch(image->attr->type) {
	case BBC_ARCHIVE__DFS_DISK:
		bbcim_disk_read(image, fpdisk, 256L+6L, side, &filebyte);
		filebyte &= 0x0f;
		filebyte |= bootoption << 4;
		bbcim_disk_write(image, fpdisk, 256L+6L, side, &filebyte);
		break;

	default:
//		bbcim_errno = BBCIM_ERROR__NOT_IMPLEMENTED;
		break;
	}

	fclose(fpdisk);

	return bbcim_errno;
}
