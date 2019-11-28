/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_delete_file.c:
 *
 * NEDERLANDS: verwijder bestand uit diskbeeld
 *
 * Copyright (C) W.H.Scholten 1996, 1999, 2000, 2003
 */

// Er zou een file-link ipv side gebruikt moeten worden die voor vanaalles gebruikt kan worden
// struct { offset, side, }

/* For Acorn DFS at the moment */
int bbcim_disk_del_file(struct bbc_archive__diskimage *image, int side, char *del_bbcfilename, int flags, int verbose) {
	FILE *fpdisk;
	char bbcfile_naam[15];
	unsigned char filebyte;
	unsigned char files;
	int  rfiles;
	int  file, k;
	int  locked;
	int  removed;
	int  i;
	char testnaam[15];
	char *disk;


	disk = image->filename;
	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	if (side < 0 || (side >= image->attr->sides))
		return bbcim_print_error_bad_side("bbcim_disk_del_file", image, side, verbose);

	if (image->attr->type != BBC_ARCHIVE__DFS_DISK)
		return bbcim_print_error_unsupported_disktype("bbcim_disk_del_file", image, verbose);

	fpdisk = fopen(disk, "rb+");

	bbcim_disk_read(image, fpdisk, 256L+5L, side, &files);

	rfiles = files/8;

	DEB(printf("aantal bestanden op diskbeeld: %d\n", rfiles));

	removed = 0;

	/* Hoofdlus: alle bestanden opzoeken. */
	if (files > 0) {
		for (file=0; file<files; file +=8) {
			bbcim_disk_read(image, fpdisk, file+15L, side, &filebyte);

			locked = filebyte >> 7;

			bbcfile_naam[0] = (filebyte & 0x7F);
			bbcfile_naam[1] = '.';

			for (i=0; i<7; i++) {
				bbcim_disk_read(image, fpdisk, file+8L+i, side, &filebyte);
				bbcfile_naam[i+2] = filebyte;
			}
			bbcfile_naam[9] = 0;

			/* Spaties aan het einde van extract weghalen: geeft anders
			 * bestands namen met spaties aan het einde. */
			for (i=8; i>0 && (bbcfile_naam[i]==' '); i--) ;
			bbcfile_naam[i+1] = 0;

			if (del_bbcfilename[1] == '.')
				strncpy(testnaam, del_bbcfilename, 9);
			else {
				testnaam[0] = '$'; testnaam[1] = '.';
				strncpy(testnaam+2, del_bbcfilename, 7);
			}
			testnaam[9] = 0;

			if (!strcasecmp(bbcfile_naam, testnaam)) {
				for (k=(file+8)+8; k<256; k++) {
					bbcim_disk_read(image, fpdisk, k, side, &filebyte);
					bbcim_disk_write(image, fpdisk, k-8L, side, &filebyte);

					bbcim_disk_read(image, fpdisk, k+256L, side, &filebyte);
					bbcim_disk_write(image, fpdisk, k-8L+256L, side, &filebyte);
				}
				TEKST_NL(printf("Bestand %s verwijderd\n", testnaam));
				TEKST_ENG(printf("File %s removed\n", testnaam));
				removed++;
				//continue;
				break;
			}
		} /*for*/

		files -= removed*8;
		bbcim_disk_write(image, fpdisk, 256L+5L, side, &files);


		TEKST_NL(printf("  %d bestand%s verwijderd\n", removed, (removed != 1) ? "en" : ""));
		TEKST_ENG(printf("  %d file%s removed\n", removed, (removed != 1) ? "s" : ""));

	} /* files > 0 */

	fclose(fpdisk);

	return 0;
}
