/* BBCIM: Acorn BBC computer diskimage manipulation
 *
 * d_write_cat.c:
 * ENGLISH:  write catalogue entry in diskimage
 *
 * Copyright (C) W.H.Scholten 1996,1997
 */


int bbcim_d_write_cat_entry(struct bbcim_diskimage *image, struct bbcim_file *file, struct bbcim_disk_data *disk_data) {
	FILE *fpdisk;
	unsigned char filebyte,H;
//	char inf_file[100];
	char bbcfile_naam[15];
	unsigned char afiles = 0;
//	static int  addfiles;
	unsigned int loadaddress, execaddress;
	int  i;
	int startsector, length;
	unsigned char eellddss, locked;
//	unsigned int  sectorsondisk;


	/* file pointer struct in diskimage struct ?? */
	int  tracksize = image->sectors_per_track*image->bytes_per_sector;
//	int  sides = image->sides;


	if (!(fpdisk = fopen(image->archive_name, "wb")))
		return BBCIM_ERROR__OPEN_FAILED;


	/* XXX locked: init MISSING */

	/* Put name in diskimage */
	filebyte = bbcfile_naam[0]+(locked << 7);
	fseek(fpdisk, 15L, SEEK_SET);
	fwrite(&filebyte, 1, 1, fpdisk);

	fseek(fpdisk, 8L, SEEK_SET);
	for(i=0; i<7; i++) {
		filebyte = bbcfile_naam[i+2];
		if (filebyte == 0) {
			filebyte = ' '; bbcfile_naam[i+3] = 0;
		}
		fwrite(&filebyte, 1, 1, fpdisk);
	}

	afiles += 8;
	fseek(fpdisk, 256+5L, SEEK_SET);
	fwrite(&afiles, 1, 1, fpdisk);




	/* load/exec  adressen + lengte schrijven */
	fseek(fpdisk, 256L+14L+tracksize*image->sides, SEEK_SET);

	/* XXX UNINITIALISED: */
	/*   startsector loadaddress execaddress length */


	eellddss = ((startsector >> 8) & 0x3)+((loadaddress >> 14) & 0xC);
	eellddss += ((execaddress >> 10) & 0xC0);
	eellddss += ((length >> 12) & 0x30);
	fwrite(&eellddss, 1, 1, fpdisk);

	/* STARTSECTOR */
	filebyte = startsector & 0xFF;
	fwrite(&filebyte, 1, 1, fpdisk);

	/* LOADADDRESS */
	fseek(fpdisk, 256L+8L, SEEK_SET);
	filebyte = loadaddress & 0xFF;
	H = (loadaddress >> 8) & 0xFF;
	fwrite(&filebyte, 1, 1, fpdisk);
	fwrite(&H, 1, 1, fpdisk);

	/* EXECADDRESS */
	filebyte = execaddress & 0xFF;
	H = (execaddress >> 8) & 0xFF;
	fwrite(&filebyte, 1, 1, fpdisk);
	fwrite(&H, 1, 1, fpdisk);

	/* FILELENGTE */
	filebyte = length & 0xFF;
	H = (length >> 8) & 0xFF;
	fwrite(&filebyte, 1, 1, fpdisk);
	fwrite(&H, 1, 1, fpdisk);

	fclose(fpdisk);

	return 0;
}

