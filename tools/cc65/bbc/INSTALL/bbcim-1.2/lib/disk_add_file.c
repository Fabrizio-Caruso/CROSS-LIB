/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_add_file.c:
 * NEDERLANDS: voeg bestanden aan Acorn DFS diskbeeld toe.
 * ENGLISH:
 *
 * Copyright (C) W.H.Scholten 1996,1997, 2003
 */


/* Prototypes */
int d_add_file(struct bbc_archive__diskimage *image, int side, struct bbc_archive__file *beeb_file, int flags, int verbose);



/* voor Acorn DFS */
int bbcim_disk_add_file(struct bbc_archive__diskimage *image, int side, char *hostfilename, struct bbc_archive__file *beeb_file, int flags, int verbose) {
	int st;
	FILE *fpadd;
	struct bbc_archive__file_attr *attr = beeb_file->attr;

	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	if (side < 0 || (side >= image->attr->sides))
		return bbcim_print_error_bad_side("bbcim_disk_add_file", image, side, verbose);

	if (image->attr->type != BBC_ARCHIVE__DFS_DISK)
		return bbcim_print_error_unsupported_disktype("bbcim_disk_add_file", image, verbose);


	/* .inf bestanden overslaan: */
	if (bbc_archive__is_inf_file(image->filename)) goto leave_add_file;

	st = bbc_archive__read_file_attributes(hostfilename, beeb_file->attr, verbose);

//	printf("bbcim_errno=%d\n", bbcim_errno);
//	if (bbcim_errno) {
	if (st) {
		if (!(flags & BBCIM_DISK_INFO)) {
			attr->load_address = 0;
			attr->exec_address = 0;
			attr->name = malloc(strlen(hostfilename)+1+2);
			if (attr->name == NULL) {
				fprintf(stderr, "Out of memory\n");
				exit(1);
			}
			if (strlen(hostfilename) >= 2 && hostfilename[1] == '.') {
				strncpy(attr->name, hostfilename, 9);
				attr->name[9] = 0;
			}
			else {
				strcpy(attr->name, "$."); strncat(attr->name, hostfilename, 7);
				attr->name[9] = 0;
			}
		}
		else {



			if (verbose) {
				fprintf(stderr, 
						TEKST_NL("Info bestand voor %s kan niet geopend worden, sla over\n")
						TEKST_ENG("Info file for %s cannot be opened, skipping file\n")
						, hostfilename);
			}
			goto leave_add_file;
		}
	}

	/* Moet nu inladen in beeb_file.data OF bbcfile uitbreiden met FILE *fpdata */
	fpadd = fopen(hostfilename, "rb");
	if (fpadd == NULL) {
		TEKST_NL(fprintf(stderr, "Archief bestand %s kan niet geopend worden\n", hostfilename));
		TEKST_ENG(fprintf(stderr, "Archive file %s cannot be opened\n", hostfilename));
		goto leave_add_file;
	}

	fseek(fpadd, 0L, SEEK_END);
	attr->length = ftell(fpadd);

//	    beeb_file->data = malloc(length);
/* Temp: */
//		beeb_file->data = NULL;
	beeb_file->filename = hostfilename;

//	    fread(beeb_file->data, beeb_file->length, 1, fpadd);
	fclose(fpadd);

	d_add_file(image, side, beeb_file, flags, verbose);
//	    free(beeb_file->data);

  leave_add_file:

	return 0;
}




int d_add_file(struct bbc_archive__diskimage *image, int side, struct bbc_archive__file *beeb_file, int flags, int verbose) {
	FILE *fpdisk, *fpadd;
	unsigned char filebyte,H;
	char inf_file[100];
//    char bbcfile_naam[15];
	unsigned char afiles = 0;
//    static int  addfiles;
	unsigned int loadaddress, execaddress;
	int  i;
	int startsector, length, j;
	unsigned char eellddss, locked;
	unsigned int  sectorsondisk;
	char *disk;
	struct bbcim_disk_data disk_data;
	struct bbc_archive__file_attr *attr = beeb_file->attr;

	loadaddress = attr->load_address;
	execaddress = attr->exec_address;

//	if (beeb_file->data == NULL) {
	{
//		if (bbcim_debug) printf("add open\n");
		fpadd = fopen(beeb_file->filename, "rb");
	}
	if (bbcim_debug) printf("add 2\n");

	if (bbcim_debug) printf("file length=%d\n", attr->length);

	disk = image->filename;

	fpdisk = fopen(disk, "rb+");
	if (bbcim_debug) printf("disk=%s\n", disk);
	/* number of files (afiles = 8*'real number'): */
	bbcim_disk_read(image, fpdisk, 256L+5L, side, &afiles);


	if (afiles == 31*8) {
		bbcim_error = 
			TEKST_NL("Er zijn al 31 bestanden in het diskbeeld")
			TEKST_ENG("There are already 31 files in the disk image");
		if (verbose) fprintf(stderr, "%s\n", bbcim_error);
		bbcim_errno = BBCIM_ERROR__DISK_FULL;
		return bbcim_errno;
	}



	/* GET PREVIOUS STARTSECTOR AND FILE LENGTH */
	if (afiles) {
		bbcim_disk_read(image, fpdisk, 256L+14L, side, &eellddss);
		/* STARTSECTOR */
		bbcim_disk_read(image, fpdisk, 256L+15L, side, &filebyte);

		startsector = filebyte + (int)(eellddss & 0x3)*256L;

		/* FILELENGTE */
		bbcim_disk_read(image, fpdisk, 256L+12L, side, &filebyte);
		bbcim_disk_read(image, fpdisk, 256L+13L, side, &H);

		length = H*256L+filebyte+(eellddss & 0x30)*16L*256L;

		startsector += (length + 255) >> 8;
	}
	else startsector = 2;
	if (bbcim_debug) printf("start sector=%d\n", startsector);

	/* Now test if the files stay within sectors-on-disk: */
	bbcim_disk_read(image, fpdisk, 256L+6L, side, &filebyte);

	H = filebyte & 3;
	bbcim_disk_read(image, fpdisk, 256L+12L, side, &filebyte);

	sectorsondisk = filebyte + H*256L;

	fseek(fpadd, 0L, SEEK_END);
	length = ftell(fpadd);
	if (bbcim_debug) printf("length=%d\n", length);
	if (bbcim_debug) printf("sectorsondisk=%d\n", sectorsondisk);

	if (startsector + ((length+255) >> 8) > sectorsondisk) {
		fclose(fpadd);
		TEKST_NL(fprintf(stderr, "Niet genoeg ruimte in het diskbeeld\n"));
		TEKST_ENG(fprintf(stderr, "Not enough space in the disk image\n"));
		bbcim_errno = BBCIM_ERROR__DISK_FULL;
		return bbcim_errno;
	}


	/* Copy the file: */
	for (j=0; j<(((length+255) >> 8)*256); j++) {
		fseek(fpadd, j, SEEK_SET);
		if (!fread(&filebyte, 1, 1, fpadd)) filebyte = 0;

		bbcim_disk_write(image, fpdisk, startsector*256L+j, side, &filebyte);
	}

	fclose(fpadd);

	if (flags & BBCIM_REMOVE_FILE) {
		remove(beeb_file->filename);
		remove(inf_file);
	}

	/* Add filename to catalogue */
	/* First move the catalogue entries (shift them 8 bytes) */
	for (j=255-8; j>7; j--) {
		bbcim_disk_read(image, fpdisk, j, side, &filebyte);
		bbcim_disk_write(image, fpdisk, j+8L, side, &filebyte);

		bbcim_disk_read(image, fpdisk, j+256L, side, &filebyte);
		bbcim_disk_write(image, fpdisk, j+8L+256L, side, &filebyte);
	}



	/*tmp: */
	locked = 0;

	filebyte = attr->name[0] + (locked << 7);
	bbcim_disk_write(image, fpdisk, 15L, side, &filebyte);

	for(i=0; i<7; i++) {
		filebyte = attr->name[i+2];
		if (filebyte == 0) {
			filebyte = ' '; attr->name[i+3] = 0;
		}
		bbcim_disk_write(image, fpdisk, 8L+i, side, &filebyte);
	}


	/* OLD */
	afiles += 8;
	bbcim_disk_write(image, fpdisk, 256L+5L, side, &afiles);


/* MISSING INIT'S : */
/* load address == ?? */
/* exec address == ?? */
	/* load/exec  adressen + lengte schrijven */
	eellddss  = ((startsector >>  8) & 0x03) + ((loadaddress >> 14) & 0x0C);
	eellddss += ((execaddress >> 10) & 0xC0);
	eellddss += ((length >> 12) & 0x30);
	bbcim_disk_write(image, fpdisk, 256L+14L, side, &eellddss);


	/* Start sector */
	filebyte = startsector & 0xFF;
	bbcim_disk_write(image, fpdisk, 256L+15L, side, &filebyte);

	/* Load address */
	filebyte = loadaddress & 0xFF;
	H = (loadaddress >> 8) & 0xFF;
	bbcim_disk_write(image, fpdisk, 256L+8L, side, &filebyte);
	bbcim_disk_write(image, fpdisk, 256L+9L, side, &H);

	/* Exec address */
	filebyte = execaddress & 0xFF;
	H = (execaddress >> 8) & 0xFF;
	bbcim_disk_write(image, fpdisk, 256L+10L, side, &filebyte);
	bbcim_disk_write(image, fpdisk, 256L+11L, side, &H);

	/* File length */
	filebyte = length & 0xFF;
	H = (length >> 8) & 0xFF;
	bbcim_disk_write(image, fpdisk, 256L+12L, side, &filebyte);
	bbcim_disk_write(image, fpdisk, 256L+13L, side, &H);

	fclose(fpdisk);


	disk_data.file_address = 0; /* Change */
	disk_data.cat_address = 2*256;

	/* d_seek_file(image, file, disk_data); */

	/* d_write_catalogue_entry(image, beeb_file, &disk_data);*/


	if (verbose) {
		TEKST_NL(printf("bestand %s toegevoegd\n", beeb_file->filename));
		TEKST_ENG(printf("added file %s\n", beeb_file->filename));
	}

	return 0;
}
