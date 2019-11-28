/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * disk_convert_ddos_to_dfs.c:
 * NEDERLANDS: Splitst DDOS diskdumps in aparte DFS diskbeelden
 *
 * Copyright (C) W.H. Scholten 1996, 1999, 2001, 2003
 */

int bbcim_ddos_disk_error(int drive, int verbose);

int bbcim_ddos_disk_error(int drive, int verbose) {
	bbcim_errno = BBCIM_ERROR__DISK_TRUNCATED;
	bbcim_error = TEKST_NL("onverwacht einde van DDOS diskbeeld") TEKST_ENG("Unexpected end of DDOS disk image");
	if (verbose) fprintf(stderr, TEKST_NL("%s bij ddos drive %c ")TEKST_ENG("%s at ddos drive %c "), bbcim_error, drive);

	return bbcim_errno;
}


int bbcim_ddos_to_dfs(struct bbc_archive__diskimage *image, int verbose) {
	char  dfs_image_name[256];
	unsigned char filebyte = 0, H = 0;
	int  ddos_no;
	int  i;
	int  sectors_on_volume, length;
	int  starttrack, startsector;
	int  s;
	FILE *fp_dfs, *fp_ddos;

	struct bbc_archive__diskimage image_dfs;
	struct bbc_archive__diskimage_attr image_dfs_attr;

	image_dfs.attr = &image_dfs_attr;
	image_dfs_attr.type = BBC_ARCHIVE__DFS_DISK;
	image_dfs_attr.sides = 1;
	image_dfs_attr.tracks = 80;
	image_dfs_attr.sectors_per_track = 10;
	image_dfs_attr.bytes_per_sector = 256;



	//bbcim_errno = 0;
	/* not necessary as we call bbcim_diskimage_sanitycheck */
	if (bbcim_diskimage_sanitycheck(image, verbose)) return bbcim_errno;

	if (image->attr->type != BBC_ARCHIVE__DDOS_DISK) {
		bbcim_error = "Not a DDOS disk image";
		bbcim_errno = -1;
		if (verbose) fprintf(stderr, "%s: %s\n", image->filename, bbcim_error);
		return -1;
	}


	fp_ddos = fopen(image->filename, "rb");

	/* Alle kanten van de diskette: */
#warning "ddos_dfs: all returns must be removed, close files, next side, etc."

	for (s=0; s < image->attr->sides; s++) {
		/* Alle catalogi */
		for (ddos_no=0; ddos_no < 8; ddos_no++) {
//OLD			fseek(fp_ddos, 16*256L+8L+2*ddos_no, SEEK_SET);
//OLD			fread(&filebyte, 1, 1, fp_ddos);
			if (bbcim_disk_read(image, fp_ddos, 16*256L+8L+2*ddos_no, s, &filebyte)) return -1;

			starttrack = filebyte;

			if (starttrack != 0) {
				strcpy(dfs_image_name, image->filename);
				if (image->attr->sides > 1) sprintf(dfs_image_name+strlen(dfs_image_name), ".%d", s);


				filebyte = ddos_no+'A';
				dfs_image_name[ strlen(dfs_image_name) ] = filebyte;

				/* Nu een test of er uberhaupt iets op de ddos catalogus staat: */
//OLD				fseek(fp_ddos, (ddos_no*2+1)*256+5, SEEK_SET);
//OLD				fread(&filebyte, 1, 1, fp_ddos);
				if (bbcim_disk_read(image, fp_ddos, (ddos_no*2+1)*256+5, s, &filebyte)) return -1;


				if (filebyte == 0) {
					filebyte = 'A'+ddos_no;
					if (verbose) printf(" Catalogus %c is leeg\n", filebyte);
					continue;
				}
				/* (Dus geen lege diskbeelden) */


				/* Open the dfs image file. */
				fp_dfs = fopen(dfs_image_name, "wb");
				if (fp_dfs == NULL) return -1;

//OLD				fseek(fp_ddos, (ddos_no*2+1)*256+6, SEEK_SET);
//OLD				fread(&filebyte, 1, 1, fp_ddos);
				if (bbcim_disk_read(image, fp_ddos, (ddos_no*2+1)*256+6, s, &filebyte)) return -1;


				H = filebyte & 0x03;
//OLD				fread(&filebyte, 1, 1, fp_ddos);
				if (bbcim_disk_read(image, fp_ddos, (ddos_no*2+1)*256+7, s, &filebyte)) return -1;

				sectors_on_volume = filebyte + H*256;

				length = sectors_on_volume*256;

				/* First sector & second sector: */
//OLD				fseek(fp_ddos, (ddos_no*2+0)*256, SEEK_SET);
				for (i=0; i<512; i++) {
//OLD					fread(&filebyte, 1, 1, fp_ddos);
					if (bbcim_disk_read(image, fp_ddos, (ddos_no*2+0)*256+i, s, &filebyte)) {
						if (feof(fp_ddos)) {
							return bbcim_ddos_disk_error(ddos_no + 'A', verbose);
						}
						return -1;
					}
//OLDWRITE					fwrite(&filebyte, 1, 1, fp_dfs);
					if (bbcim_disk_write(&image_dfs, fp_dfs, 256L+i*8+14+2+i, 0, &filebyte)) return -1;
				}

				/* In the second sector, adjust the sector start addresses. */
				/* NB: mag voor allemaal?? (ook als bv 2 best. gebruikt?) Denk wel. */
				for (i=0; i<31; i++) {
//OLD					fseek(fp_ddos, (ddos_no*2+1)*256+14L+i*8, SEEK_SET);
//OLD					fread(&filebyte, 1, 1, fp_ddos);
					if (bbcim_disk_read(image, fp_ddos, (ddos_no*2+1)*256+14L+i*8, s, &filebyte)) return -1;

					H = filebyte;
//OLD					fread(&filebyte, 1, 1, fp_ddos);
					if (bbcim_disk_read(image, fp_ddos, (ddos_no*2+1)*256+14L+i*8+1, s, &filebyte)) return -1;

					startsector = filebyte+H*256+2;
					H = startsector >> 8;
					filebyte = startsector & 0xFF;

//OLDWRITE					fseek(fp_dfs, 256L+i*8+14, SEEK_SET);
//OLDWRITE					fwrite(&H, 1, 1, fp_dfs);
//OLDWRITE					fwrite(&filebyte, 1, 1, fp_dfs);
					if (bbcim_disk_write(image, fp_dfs, 256L+i*8+14, 0, &H)) return -1;
					if (bbcim_disk_write(image, fp_dfs, 256L+i*8+14+1, 0, &filebyte)) return -1;
				}

				/* Nu de rest: */
//OLD				fseek(fp_ddos,starttrack*18*256, SEEK_SET);
				for (i=0; i<length; i++) {
//OLD					fread(&filebyte, 1, 1, fp_ddos);
					if (bbcim_disk_read(image, fp_ddos, starttrack*18*256+i, s, &filebyte)) {
						if (feof(fp_ddos)) {
							return bbcim_ddos_disk_error(ddos_no + 'A', verbose);
						}
						return -1;
					}

//OLDWRITE					fwrite(&filebyte, 1, 1, fp_dfs);
					if (bbcim_disk_write(&image_dfs, fp_dfs, 256L+i*8+14+2+i, 0, &filebyte)) return -1;

				}
				fclose(fp_dfs);

				/* Now write a .dsk file */
				bbc_archive__write_disk_attributes2(
					dfs_image_name,
					image->attr->sides, image->attr->tracks, image->attr->sectors_per_track, image->attr->bytes_per_sector, BBC_ARCHIVE__DFS_DISK, verbose);
			}
		}
	} /* sides */

	if (verbose) printf("\n");
	fclose(fp_ddos);

	return 0;
}
