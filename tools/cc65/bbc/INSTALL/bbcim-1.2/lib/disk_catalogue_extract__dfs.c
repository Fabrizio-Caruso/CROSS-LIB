int  _bbcim__disk_dfscat_extract_crc(struct bbc_archive__diskimage *image, int side, struct bbc_archive__file* beeb_file, int flags, int verbose) {
	FILE *fpdisk = NULL;
	FILE *fpcat = NULL;
	FILE *fpextract = NULL;
	FILE *fpinf = NULL;
//	char catalogus[256];
	char extract[256];
	char inf_file[256], basisnaam[256];
	char bbcdisk_naam[15], bbcfile_naam[15];
	char unix_bbcfile_naam[15];
	char testnaam[15];
	char *disk;

	unsigned char filebyte, H;

	int  filesizesum = 0;
	int  extr_list = False; /* list of files given ? */
	int  extr_list_no, antwoord;

	int  i, k;
	int  rfiles, bootoption;
	unsigned char files = 0;
	unsigned int sectorsondisk;
	int  file;
	int  locked;
	unsigned char eellddss; 

	int  startsector;
	unsigned int loadaddress, execaddress, length;

	unsigned int crc;
	static int find_file_infoaddr; /* for BBCIM_GET_ATTRIBUTES */
	mode_t dir_mode;


	if (flags & BBCIM_NEW_DISK) {
		find_file_infoaddr = -1;
		return 0;
	}
	find_file_infoaddr = -1; //DEBUG

	disk = image->filename;

	if (bbcim_debug) printf("DFS disk type (%s)\n", disk);
	/* -- DFS diskbeelden -- */
	/* Naming scheme: file 'a.f' in disk image 'disk' (i.e. DFS directory 'a', DFS file name 'f') becomes:
	 * -ed         disk.d/a.f
	 * -ed@        disk.d/a.f  -> except if a='$' then the name becomes disk.d/f
	 * -e@         disk.a.f  -> except if a='$' then the name becomes disk.f
	 * -e          disk.a.f
	 * -es         a.f
	 * -es@        af  -> except if a='$' then the name becomes f
	 */

	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;
	if (bbcim_debug) printf("DFS sanity ok\n");

	strcpy(basisnaam, disk);
//OLD This was useful for DOS, e.g. disk file.bbc gives a directory: mydisk/
//OLD	basisnaam[strcspn(disk, ".")] = 0;

//		strcpy(catalogus, basisnaam);
//		strcat(catalogus, ".cat");

	fpdisk = fopen(disk, "rb");


	if (flags & BBCIM_EXTRACT_DIR) {
		if (image->attr->sides == 2) {
			if (side == 0)
				strcat(basisnaam, ".0");
			else
				strcat(basisnaam, ".1");
		}
		strcat(basisnaam, ".d");

		bbc_rename_sequential(basisnaam); /* in case a file (dir too?)  already exists named basisnaam */

		dir_mode = 0777; /* CHANGE */
		mkdir(basisnaam, dir_mode);
	}


	/* What is the disk title? (for cat, crc, extract) */
	for (i=0; i<8; i++) {
		if (bbcim_disk_read(image, fpdisk, i, side, &filebyte)) return -1;
		bbcdisk_naam[i] = filebyte;
		if (filebyte == 0) break;
	}
	for (i=0; i<4; i++) {
		if (bbcim_disk_read(image, fpdisk, 256L+i, side, &filebyte)) return -1;
		bbcdisk_naam[i+8] = filebyte;
		if (filebyte == 0) break;
	}
	bbcdisk_naam[12] = 0; /* terminate just in case the title is the full 12 chars */


	if (bbcim_disk_read(image, fpdisk, 256L+5L, side, &files)) return -1;

	rfiles = files/8;
//		extr_list = ((flags & BBCIM_EXTRACT_FILES) && (args >0 ));
	extr_list = (beeb_file != NULL);
	if (bbcim_debug) printf("extr_list=%d\n", extr_list);

	if (bbcim_disk_read(image, fpdisk, 256L+6L, side, &filebyte)) return -1;

	bootoption = filebyte >> 4;
	H = filebyte & 3;
	if (bbcim_disk_read(image, fpdisk, 256L+7L, side, &filebyte)) return -1;

	sectorsondisk = filebyte+H*256L;


//		if ((flags & BBCIM_EXTRACT_FILES) && !extr_list)
//			fpcat = fopen(catalogus, "w");
//		else
	fpcat = stdout;


//    if (flags & BBCIM_SHOW_FILES) goto skip_diskinfo;

	TEKST_NL(fprintf(fpcat, "\nDiskette :%s", strlen(bbcdisk_naam) == 0 ? "(geen naam)" : bbcdisk_naam));
	TEKST_NL(fprintf(fpcat, "\n%d sectoren op de diskette\n", sectorsondisk));
	TEKST_NL(fprintf(fpcat, "bootoptie: "));

	TEKST_ENG(fprintf(fpcat, "\nDisk :%s", strlen(bbcdisk_naam) == 0 ? "(no name)" : bbcdisk_naam));
	TEKST_ENG(fprintf(fpcat, "\n%d sectors on disk\n", sectorsondisk));
	TEKST_ENG(fprintf(fpcat, "bootoption: "));

	switch(bootoption) {
	case 0 :
		TEKST_NL(fprintf(fpcat, "geen"));
		TEKST_ENG(fprintf(fpcat, "none"));
		break;

	case 1 :
		fprintf(fpcat, "*LOAD !BOOT");
		break;

	case 2 :
		fprintf(fpcat, "*RUN !BOOT");
		break;

	case 3 :
		fprintf(fpcat, "*EXEC !BOOT");
		break;
	}
	fprintf(fpcat, "\n");

	TEKST_NL(fprintf(fpcat, "Bestand    Laad  Start  Lengte Toegang startsector\n"));
	TEKST_ENG(fprintf(fpcat, "File       Load   Exec  Length Access  startsector\n"));


//	  skip_diskinfo:

	/* Hoofdlus: alle bestanden opzoeken */
	if (files > 0) {
		extr_list_no = 0;

		for (file=0; file<files; file +=8) {
			strcpy(extract, basisnaam);
			if (flags & BBCIM_EXTRACT_DIR) strcat(extract, "/"); else strcat(extract, ".");
			if (flags & BBCIM_SHORT_NAME) extract[0] = 0;

			if (bbcim_disk_read(image, fpdisk, file+15L, side, &filebyte)) return -1;
			locked = filebyte >> 7;

			bbcfile_naam[0] = (filebyte & 0x7F);
			bbcfile_naam[1] = '.';
			for (i=0; i<7; i++) {
				if (bbcim_disk_read(image, fpdisk, file+8L+i, side, &filebyte)) return -1;
				bbcfile_naam[i+2] = filebyte;
			}
			bbcfile_naam[9] = 0;

			/* spaties aan het einde van BBC naam weghalen: geeft anders
			 * bestands namen met spaties (in unix). */
			for (i=8; i>0 && (bbcfile_naam[i] == ' '); i--);
			bbcfile_naam[i+1] = 0;


			/* If file list given, see if name is in disk image */
			if (extr_list) {
				if (beeb_file->attr->name[1]=='.')
					strncpy(testnaam, beeb_file->attr->name, 9);
				else {
					testnaam[0] = '$'; testnaam[1] = '.';
					strncpy(testnaam+2, beeb_file->attr->name, 7);
				}
				testnaam[9] = 0;
				//printf("DEBUG: testnaam %s\n", testnaam);
				if (strcmp(bbcfile_naam, testnaam)) continue; /* not found yet */

				extr_list_no++;
			}


			/* load/exec  adressen + lengte bepalen */
			if (bbcim_disk_read(image, fpdisk, 256L+file+14L, side, &eellddss)) return -1;


			/* STARTSECTOR */
			if (bbcim_disk_read(image, fpdisk, 256L+file+15L, side, &filebyte)) return -1;
			startsector = filebyte+(eellddss & 3)*256L;


			/* LOADADDRESS */
			if (bbcim_disk_read(image, fpdisk, 256L+file+8L, side, &filebyte)) return -1;
			if (bbcim_disk_read(image, fpdisk, 256L+file+9L, side, &H)) return -1;

			loadaddress = H*256L+filebyte+(eellddss & 0x0C)*16384L;
			if (loadaddress & 0x30000) loadaddress |= 0xFF0000;

			/* EXECADDRESS */
			if (bbcim_disk_read(image, fpdisk, 256L+file+10L, side, &filebyte)) return -1;
			if (bbcim_disk_read(image, fpdisk, 256L+file+11L, side, &H)) return -1;

			execaddress = H*256L+filebyte+(eellddss & 0xC0)*4L*256L;
			if (execaddress & 0x30000) execaddress |= 0xFF0000;

			/* FILELENGTE */
			if (bbcim_disk_read(image, fpdisk, 256L+file+12L, side, &filebyte)) return -1;
			if (bbcim_disk_read(image, fpdisk, 256L+file+13L, side, &H)) return -1;

			length = H*256L+filebyte+(eellddss & 0x30)*16L*256L;

			filesizesum += length;

			if (flags & BBCIM_GET_ATTRIBUTES) {
				if (file > find_file_infoaddr) {
					/* new file found */
					strcpy(beeb_file->attr->name, bbcfile_naam);
					beeb_file->attr->load_address = loadaddress;
					beeb_file->attr->exec_address = execaddress;
					beeb_file->attr->length = length;
					beeb_file->attr->attributes = locked;
					return 0;
				}
			}
			if (flags & BBCIM_LOAD_FILE) {
				//fseek(fpdisk, startsector*256, SEEK_SET);
				//fread(beeb_file->data, length, 1, fpdisk);

				fprintf(stderr, "LOAD file not implemented\n");
				return -1;
			}

			/* Uitvoer van deze gegevens naar catalogus */
			if (flags & BBCIM_SHOW_FILES) {
				fprintf(fpcat, "%-9s %6X %6X %6X", bbcfile_naam, loadaddress, execaddress, length);
				if (locked) fprintf(fpcat, " Locked");
				else        fprintf(fpcat, "       "); /* To line up for CRC */
			}

			strcpy(unix_bbcfile_naam, bbcfile_naam);
			bbc_replace_bad_chars(unix_bbcfile_naam);
//				if (bbcim_debug) printf("flags=0x%x\n", flags);
			if (!(flags & (BBCIM__NAME__KEEP_ROOT_DIR | BBCIM__NAME__KEEP_NONROOT_DIR)))
				bbcim_reduce_bbcname__no_dir(extract+strlen(extract), unix_bbcfile_naam);
			else if (flags & BBCIM__NAME__KEEP_ROOT_DIR)
				bbcim_reduce_bbcname__no_rootdir(extract+strlen(extract), unix_bbcfile_naam);
			else {
				strcpy(extract+strlen(extract), unix_bbcfile_naam);
			}


			if (flags & BBCIM_EXTRACT_FILES) {
				/* Name check: does it already exist? */
 #warning "Remove overwrite question from the library?"
				char nieuwe_naam[55];

				strcpy(nieuwe_naam, extract);
				if ((fpextract = fopen(extract, "rb")) != NULL) {
					fclose(fpextract);
					antwoord = bbcim_overschrijf_vraag(extract, nieuwe_naam);

					switch(antwoord) {
					case BBCIM_OVERSCHRIJVEN:
						break;
					case BBCIM_HERNOEM_NIEUW:
						strcpy(extract, nieuwe_naam); break;
					case BBCIM_HERNOEM_BESTAAND:
						rename(extract, nieuwe_naam);
						/* altijd ook info bestand hernoemen: */
						strcpy(inf_file, extract);
						strcat(inf_file, ".inf");

						strcat(nieuwe_naam, ".inf");

						rename(inf_file, nieuwe_naam);
						break;
					case BBCIM_SLA_OVER:
						continue; /*to for*/
					} /*switch */
				}
				/* End name check */


				fpextract = fopen(extract, "wb");

				for (i=0; i<length; i++) {
					if (bbcim_disk_read(image, fpdisk, startsector*256+i, side, &filebyte)) {
						if (feof(fpdisk)) {
							return bbcim_print_error_diskimage_truncated("dfs cat/extr/crc", image, NULL, verbose);
						}
						return -1;
					}
					fwrite(&filebyte, 1, 1, fpextract);
				}
				fclose(fpextract);
			}


			if (flags & BBCIM_DISK_INFO) { /* should be INFO_FILE */ 
				strcpy(inf_file, extract);
				strcat(inf_file, ".inf");
				fpinf = fopen(inf_file, "w");
				fprintf(fpinf, "%-9s", bbcfile_naam);
				/* I no longer include the (superfluous) file length in
				 * the .inf file. */
				fprintf(fpinf, " %6X %6X", loadaddress, execaddress);
				if (locked) fprintf(fpinf, " Locked");
			} /* N.B. info bestand wordt in crc deel gesloten */


			crc = 0;
			if (flags & (BBCIM_CALC_CRC | BBCIM_DISK_INFO)) {
				for (i=0; i<length; i++) {
					if (bbcim_disk_read(image, fpdisk, startsector*256+i, side, &filebyte)) return -1;

					if (feof(fpdisk)) {
						return bbcim_print_error_diskimage_truncated("dfs cat/extr/crc", image, NULL, verbose);
					}

					crc ^= (filebyte << 8);
					for (k=0; k<8; k++) {
						if (crc & 32768)
							crc = (((crc ^ 0x0810) & 32767) << 1)+1;
						else
							crc = crc << 1;
					}
				}

				if (flags & BBCIM_DISK_INFO) {
					fprintf(fpinf, " CRC=%04X", crc); fclose(fpinf);
				}
			}

			if (extr_list)
				fprintf(fpcat, " %s\n", extract);
			else
				fprintf(fpcat, "   %4d", (int) startsector);

			if (flags & BBCIM_CALC_CRC)
				fprintf(fpcat, "       CRC= %04X\n", crc);
			else
				fprintf(fpcat, "\n");
		}

		/* Show no of (extracted) files */
		if (extr_list) {
			rfiles = extr_list_no;
			fprintf(fpcat, 
				TEKST_NL("aantal bestanden: %d\n")
				TEKST_ENG("number of files: %d\n")
				, rfiles);
		}
		fprintf(fpcat, TEKST_NL("\n%d bestand%s\n") TEKST_ENG("\n%d file%s\n"), rfiles, (rfiles != 1) ? TEKST_NL("en") TEKST_ENG("s") : "");


		fclose(fpdisk);
//OBSOLETE		if ((flags & BBCIM_REMOVE_DISK) && !extr_list) remove(disk);
	}
	fprintf(fpcat,
			TEKST_NL("Totaal %d bytes\n")
			TEKST_ENG("Total %d bytes\n")
			, filesizesum);

	if (flags & BBCIM_LOAD_FILE) return BBCIM_ERROR__FILE_NOT_FOUND; /* Not found */

	if (fpcat != stdout) fclose(fpcat);

	return 0;
}
