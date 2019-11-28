/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_minimise.c:
 * NEDERLANDS: minimaliseer bbc diskbeeld (SINGLE DENSITY, DFS)
 *
 * Copyright (C) W.H.Scholten 1996 - 2001, 2003
 */


/* Acorn DFS */
int bbcim_disk_minimise(struct bbc_archive__diskimage *image, int verbose) {
	FILE *fpdisk, *fpdisk2;
	char backupnaam[256];
	char bbcfile_naam[12];
	unsigned char filebyte, H, eellddss;

	int startsector, file;
	unsigned char files;
	int length;
//	int max_sectors = INT_MAX;
	int side;

	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	if (image->attr->type != BBC_ARCHIVE__DFS_DISK)
		return bbcim_print_error_unsupported_disktype("bbcim_disk_minimise", image, verbose);

//	fpdisk = fopen(image->filename, "rb");
//	fclose(fpdisk);

	strcpy(backupnaam, image->filename); strcat(backupnaam, "~");

//OLD	remove(backupnaam);
//OLD	rename(image->filename, backupnaam);
//	fprintf(stderr, "min, before rename\n");
	if (bbcim_rename_diskimage(image->filename, backupnaam, verbose)) return bbcim_errno;
//	fprintf(stderr, "min, after rename\n");

	fpdisk = fopen(backupnaam, "rb");
	fpdisk2 = fopen(image->filename, "wb");



	for (side=0; side<image->attr->sides; side++) {
		int startsector2 = 2;
		int i;

		bbcim_disk_read(image, fpdisk, OFFSET__DFS_entries /*256L+5L*/, side, &files);
//		if (files > 0) {
			/* Copy the catalogue */
			for (i=0; i<512; i++) {
				bbcim_disk_read(image, fpdisk, i, side, &filebyte);
				bbcim_disk_write(image, fpdisk2, i, side, &filebyte);
			}


			/* Main loop: find all files. */
			for (file=files; file>0; file-=8) {
				/* The following is only needed for filename printing: */
				bbcim_disk_read(image, fpdisk, file+15L, side, &filebyte);
				bbcfile_naam[0] = (filebyte & 0x7F);
				bbcfile_naam[1] = '.';
				for (i=0; i<7; i++) {
//FOUT?					bbcim_disk_read(image, fpdisk, file+8L+i, side, &filebyte);
					bbcim_disk_read(image, fpdisk, file+i, side, &filebyte); //ok?

					bbcfile_naam[i+2] = filebyte;
				}
				bbcfile_naam[9] = 0;
				fprintf(stderr, "DEBUG filenaam=%s\n", bbcfile_naam);

				/* File length */
				bbcim_disk_read(image, fpdisk, file+256L+4L, side, &filebyte);
				bbcim_disk_read(image, fpdisk, file+256L+5L, side, &H);
				bbcim_disk_read(image, fpdisk, file+256L+6L, side, &eellddss);

				length = H*256L+filebyte+(eellddss & 0x30)*16L*256L;



				/* Start sector */
//FOUT?				bbcim_disk_read(image, fpdisk, file+256L+7L, side, &eellddss);
				bbcim_disk_read(image, fpdisk, file+256L+7L, side, &filebyte);
				startsector = filebyte+(eellddss & 3)*256L;

				/* New startsector */
				eellddss = (eellddss & 0xFC)+(startsector2>>8);
				bbcim_disk_write(image, fpdisk2, file+256L+6L, side, &eellddss);

				filebyte = startsector2 & 0xFF;
				bbcim_disk_write(image, fpdisk2, file+256L+7L, side, &filebyte);

				if (bbcim_debug) printf("startsector %3d > %3d\n", (int) startsector, (int) startsector2);


				for (i=0; i< ((length+255) >> 8)*256; i++) {
//FOUT?					bbcim_disk_read(image, fpdisk, file+startsector*256+i, side, &filebyte);
					bbcim_disk_read(image, fpdisk, startsector*256+i, side, &filebyte);
					if (feof(fpdisk) && (i < length)) {
//						return bbcim_print_error_diskimage_truncated("disk_minimize", image, bbcfile_naam, verbose);
						bbcim_print_error_diskimage_truncated("disk_minimize", image, bbcfile_naam, verbose);
						break;
					}
					bbcim_disk_write(image, fpdisk2, startsector2*256+i, side, &filebyte);
				}
				startsector2 = startsector2 + ((length+255) >> 8);
			} /* loop over all files */
//		} /* files > 0 */
	} /* loop over all sides */
	fclose(fpdisk);
	fclose(fpdisk2);

	/* Now write a .dsk file */
	bbc_archive__write_disk_attributes(image->filename, image->attr, verbose);

	return 0;
}
