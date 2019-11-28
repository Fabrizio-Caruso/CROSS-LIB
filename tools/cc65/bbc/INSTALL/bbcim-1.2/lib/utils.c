/* BBCIM: Acorn BBC computer disk image manipulation and more
 *
 * utils.c:
 *
 * Copyright (C) W.H. Scholten 1996, 1997, 2001, 2003
 */



/* Prototypes: */
int bbcim_rename_diskimage(char *from, char *to, int verbose);
int bbcim_file_sanitycheck(char *filename, int verbose);
int bbcim_diskimage_sanitycheck(struct bbc_archive__diskimage *image, int verbose);


/* Functions: */
int bbcim_rename_diskimage(char *from, char *to, int verbose) {
	char *dsk_from, *dsk_to;
	int  st;

	bbcim_errno = 0;

	st = rename(from, to);
	if (st < 0) {
		bbcim_errno = -1;
		bbcim_error = "Rename failed";
		if (verbose) fprintf(stderr, "%s: %s (to %s)\n", from, bbcim_error, to);
		return bbcim_errno;
	}


	dsk_from = bbc_archive__new_dsk_filename(from, verbose);
	if (dsk_from == NULL) return bbcim_errno;


	dsk_to = bbc_archive__new_dsk_filename(to, verbose);
	if (dsk_to == NULL) { free (dsk_from); return bbcim_errno; }


	if (bbcim_file_sanitycheck(dsk_from, verbose) == 0) {

		st = rename(dsk_from, dsk_to);
		if (st < 0) {
			bbcim_errno = -1;
			bbcim_error = "Rename failed";
			if (verbose) fprintf(stderr, "%s: %s (to %s)\n", dsk_from, bbcim_error, dsk_to);
		}
	} else bbcim_errno = 0; /* ignore non-existing dsk files, as they are not compulsory */

	return bbcim_errno;
}




void bbcim_reduce_bbcname__no_dir(char *unix_naam, char *bbc_naam) {
//	printf("no dir\n");
	if (bbc_naam[0] == '$')
		strcpy(unix_naam, bbc_naam+2);
	else {
		unix_naam[0] = bbc_naam[0];
		strcpy(unix_naam+1, bbc_naam+2);
	}
}
void bbcim_reduce_bbcname__no_rootdir(char *unix_naam, char *bbc_naam) {
//	printf("no rootdir\n");
	if (bbc_naam[0] == '$')
		strcpy(unix_naam, bbc_naam+2);
	else {
		strcpy(unix_naam, bbc_naam);
	}
}



/* Reading numbers from a buffer */
int bbcim_read4lsb(unsigned char *data) {
	return (int) data[0] | ( ((int) data[1]) <<8) | ( ((int) data[2]) <<16) | ( ((int) data[3]) <<24);
}

int bbcim_read3lsb(unsigned char *data) {
	return (int) data[0] | ( ((int) data[1]) <<8) | ( ((int) data[2]) <<16);
}

int bbcim_read2lsb(unsigned char *data) {
	return (int) data[0] | ( ((int) data[1]) <<8);
}



int bbcim_file_sanitycheck(char *filename, int verbose) {
	FILE *fp;

	bbcim_errno = 0;

	fp = fopen(filename, "rb");
	if (fp == NULL) {
		bbcim_error =
			TEKST_NL("Kan bestand niet openen")
			TEKST_ENG("Cannot open file")
			;
		if (verbose) fprintf(stderr, "%s: %s\n", filename, bbcim_error);
		bbcim_errno = BBCIM_ERROR__OPEN_FAILED;
		return bbcim_errno;
	}
	fclose(fp);

	return 0;
}


int bbcim_diskimage_sanitycheck(struct bbc_archive__diskimage *image, int verbose) {
	int i,s;
	FILE *fpdisk;
	unsigned char files = 0;
	unsigned char filebyte = 0;

	bbcim_errno = 0;

	fpdisk = fopen(image->filename, "rb");
	if (fpdisk == NULL) {
		bbcim_error =
			TEKST_NL("Kan diskbeeld niet openen\n\n")
			TEKST_ENG("Cannot open disk image\n\n")
			;
		if (verbose) fprintf(stderr, "%s: %s\n", image->filename, bbcim_error);
		bbcim_errno = BBCIM_ERROR__OPEN_FAILED;
		return bbcim_errno;
	}


	switch(image->attr->type) {
	case BBC_ARCHIVE__DFS_DISK:
		//for (side=0; side<image.sides; side++) {
		/* Doe alleen iets als het diskbeeld meer dan 2 sectoren bevat. */
		fseek(fpdisk, 0L, SEEK_END);
		if (ftell(fpdisk) < 512 * image->attr->sides - 1) {
			/* Minimum must be (sides*catalogue size) */
			bbcim_error =
				TEKST_NL("niets op het diskbeeld\n\n")
				TEKST_ENG("nothing on the disk image\n\n")
				;
			bbcim_errno = BBCIM_ERROR__NO_ROOT_DIR;
			break;
		}

		for (s=0; s<image->attr->sides; s++) {
			if (bbcim_disk_read(image, fpdisk, OFFSET__DFS_entries, s, &files)) return -1;
//OLD		fseek(fpdisk, 256+5L, SEEK_SET);
//OLD		fread(&files, 1, 1, fpdisk);
			if (files % 8) {
				bbcim_error =
					TEKST_NL("corrupt diskbeeld (bestand-aantal byte)\n")
					TEKST_ENG("bad disk image (fileno byte)\n")
					;
				bbcim_errno = BBCIM_ERROR__BAD_DISK;
				break;
			}
		}
		/* SANITY CHECK startsec>1 ? */
		break;

	case BBC_ARCHIVE__W62_DISK:
		/* Verify it's a Watford 62 file image */
		for (s=0; s<image->attr->sides; s++) {
			for (i=0; i<7; i++) {
				if (bbcim_disk_read(image, fpdisk, 512 + OFFSET__DFS_entries, s, &filebyte)) return -1;
				if (filebyte != 0xAA) {
					//if (verbose) fprintf(stderr, "%s is not a Watford 62 file image (there should be 8 bytes AA at positions 512-519)\n", image->filename);
					return -1;
				}
			}

			/* are there files on image 2? */
//OLD	fseek(fp_w62, 512+DFSentries, SEEK_SET);
//OLD	fread(&filebyte, 1, 1, fp_w62);
			if (bbcim_disk_read(image, fpdisk, OFFSET__DFS_entries, s, &filebyte)) return -1;
			if ((filebyte %8) != 0) {
				/* Not a valid catalogue */
				//if (verbose) fprintf(stderr, "%s: not a valid 2nd catalogue\n", image->filename);
				return -1;
			}
			if (bbcim_disk_read(image, fpdisk, 512 + OFFSET__DFS_entries, s, &filebyte)) return -1;
			if ((filebyte %8) != 0) {
				/* Not a valid 2nd catalogue */
				//if (verbose) fprintf(stderr, "%s: not a valid 2nd catalogue\n", image->filename);
				return -1;
			}
		}
		break;

	case BBC_ARCHIVE__DDOS_DISK:
		break;

	case BBC_ARCHIVE__ADFS_DISK:
		break;

	default:
		bbcim_error = 
			TEKST_NL("slecht disk type\n")
			TEKST_ENG("bad disktype\n")
			;
		bbcim_errno = BBCIM_ERROR__BAD_DISK_TYPE_ARGUMENT;
		break;
	}

	fclose(fpdisk);

	if (verbose && bbcim_errno) fprintf(stderr, "%s\n", bbcim_error);

	return bbcim_errno;
}


/* Write a byte to a disk image, this will take into account double sided
 * (interleaved) images! */
int bbcim_disk_write(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *byte) {
	int st;
	int new_offset = bbcim_disk_offset(image, offset, side);
	if (new_offset >= 0) {
		fseek(fp, new_offset, SEEK_SET);
		st = fwrite(byte, 1, 1, fp);
		if (st != 1) return -1;
	}
	else return -1;

	return 0;
}
int bbcim_disk_read(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *byte) {
	int st;
	int new_offset = bbcim_disk_offset(image, offset, side);
	if (new_offset >= 0) {
		fseek(fp, new_offset, SEEK_SET);
		st = fread(byte, 1, 1, fp);
		if (st != 1) return -1;
	}
	else return -1;

	return 0;
}

/* These are somewhat ugly and should take into account track size to reduce the number of reads/writes */
int bbcim_disk_write_multi(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *bytes, int nbytes) {
	int i;
	int st;
	int new_offset;
	int bytes_written = 0;

	for (i=0; i<nbytes; i++) {
		new_offset = bbcim_disk_offset(image, offset+i, side);
		if (new_offset >= 0) {
			fseek(fp, new_offset, SEEK_SET);
			st = fwrite(bytes+bytes_written, 1, 1, fp);
			if (st != 1) return bytes_written; /* -1; */
		}
		else return bytes_written; /* -1; */
	}

	return bytes_written; /* 0; */
}


int bbcim_disk_read_multi(struct bbc_archive__diskimage *image, FILE *fp, int offset, int side, unsigned char *bytes, int nbytes) {
	int i;
	int st;
	int new_offset;
	int bytes_read = 0;

	for (i=0; i<nbytes; i++) {
		new_offset = bbcim_disk_offset(image, offset+i, side);
		if (new_offset >= 0) {
			fseek(fp, new_offset, SEEK_SET);
			st = fread(bytes+bytes_read, 1, 1, fp);
			if (st != 1) return bytes_read; /*-1;*/
			bytes_read++;
		}
		else return bytes_read; /*-1;*/
	}

	return bytes_read;
}


/* Returns value > 0 if ok, -1 on error (side 1 of a single sided disk image) */
int bbcim_disk_offset(struct bbc_archive__diskimage *image, int side_offset, int side) {
	int  track_size;
	int  track_no;
	int  new_offset;

	track_size = image->attr->sectors_per_track * image->attr->bytes_per_sector;

	if (image->attr->type != BBC_ARCHIVE__ADFS_DISK) {
		if (side < 0 || side > 1) return -1;

		if (side_offset >= track_size*image->attr->tracks) return -1; /* offset out of range */

//		if (side == 1 && image->attr->sides == 1) return -1; /* can't write to second side of a single sided disk image */
		if (side >= image->attr->sides) return -1; /* can't write to second side of a single sided disk image */

		track_no = side_offset / track_size;

		new_offset = (image->attr->sides * track_no * track_size) + (side * track_size) + (side_offset % track_size);
	}
	else return -1;

	return new_offset;
}


int bbcim_print_error_bad_side(char *function, struct bbc_archive__diskimage *image, int side, int verbose) {
	bbcim_errno = BBCIM_ERROR__BAD_DISK_SIDE;
	bbcim_error = "Bad side";
	if (verbose) fprintf(stderr, "%s: %s: %s %d\n", function, image->filename, bbcim_error, side);

	return bbcim_errno;
}


int bbcim_print_error_unsupported_disktype(char *function, struct bbc_archive__diskimage *image, int verbose) {
	bbcim_errno = BBCIM_ERROR__NOT_IMPLEMENTED;
	bbcim_error = "Unsupported disk type";
	if (verbose) fprintf(stderr, "%s: %s: %s\n", function, image->filename, bbcim_error);

	return bbcim_errno;
}


int bbcim_print_error_diskimage_truncated(char *function, struct bbc_archive__diskimage *image, char *file, int verbose) {
	bbcim_errno = BBCIM_ERROR__DISK_TRUNCATED;
	bbcim_error = "Unexpected end of the disk image";
	if (verbose) {
		fprintf(stderr, "%s: %s: %s", function, image->filename, bbcim_error);
		if (file == NULL) {
			fprintf(stderr, "\n");
		}
		else {
			fprintf(stderr, " at file %s\n", file);
		}
	}

	return bbcim_errno;
}
