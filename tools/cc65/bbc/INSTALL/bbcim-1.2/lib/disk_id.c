/* Try to identify a disk image type */

#include <_bbc_archive.h>

int verify_watford_id(FILE *fp, int offset);

int bbcim_disk_identify(char *filename, int *possible_disktypes, int verbose) {
//	int i;
	FILE * fp;
	unsigned char mybyte;
	int  st;
	unsigned int side__type_bits;
	unsigned int side0__type_bits;
	unsigned int side1__type_bits;
	unsigned int sides__bits;

	if (bbcim_file_sanitycheck (filename, verbose)) return bbcim_errno;

	fp = fopen(filename, "rb");
	side__type_bits = 0xff; /* all bits set */
	side0__type_bits = 0xff; /* all bits set */
	side1__type_bits = 0xff; /* all bits set */
	sides__bits = 0x3;

//	bbcim_disk_read(image, fp, 256L+5L, side, &mybte);
	fseek(fp, 256L+5L, SEEK_SET);
	st = fread(&mybyte, 1, 1, fp);
	if (st == 1) {
		if ((mybyte % 8) != 0) {
			side0__type_bits &= ~((1<<BBC_ARCHIVE__DFS_DISK) | (1<<BBC_ARCHIVE__DDOS_DISK) | (1<<BBC_ARCHIVE__W62_DISK));
		}
	}
//	bbcim_disk_read(image, fp, 256*10+256L+5L, side, &mybte);
	fseek(fp, 256*10+256L+5L, SEEK_SET);
	st = fread(&mybyte, 1, 1, fp);
	if (st == 1) {
		if ((mybyte % 8) != 0) {
			side1__type_bits &= ~((1<<BBC_ARCHIVE__DFS_DISK) | (1<<BBC_ARCHIVE__DDOS_DISK) | (1<<BBC_ARCHIVE__W62_DISK));
		}
	}


	/* Verify it's a Watford 62 file image */
	if (verify_watford_id(fp, 512L)) {
		side__type_bits = (1<<BBC_ARCHIVE__W62_DISK); side0__type_bits = (1<<BBC_ARCHIVE__W62_DISK);
	}
	else {
		side__type_bits &= ~(1<<BBC_ARCHIVE__W62_DISK); side0__type_bits &= ~(1<<BBC_ARCHIVE__W62_DISK);
	}
	if (verify_watford_id(fp, 10*256+512L)) {
		side1__type_bits = (1<<BBC_ARCHIVE__W62_DISK);
	}
	else {
		side1__type_bits &= ~(1<<BBC_ARCHIVE__W62_DISK);
	}



	if (verbose) {
		int t;

		printf("It looks like one of these possiblities:\n");
		if (sides__bits & 1) {
			printf(" 1 side: it can be: ");
			for (t=0; t<BBC_ARCHIVE__HDFS_DISK+1; t++) {
				if (side0__type_bits & (1<<t)) printf("%s ", bbc_archive__dfs_names[t]);
			}
			printf("\n");
		}
		if (sides__bits & 2) {
			printf(" 2 sides: side 0 can be: ");
			for (t=0; t<BBC_ARCHIVE__HDFS_DISK+1; t++) {
				if (side0__type_bits & (1<<t)) printf("%s ", bbc_archive__dfs_names[t]);
			}
			printf("\n");
			printf("          side 1 can be: ");
			for (t=0; t<BBC_ARCHIVE__HDFS_DISK+1; t++) {
				if (side1__type_bits & (1<<t)) printf("%s ", bbc_archive__dfs_names[t]);
			}
			printf("\n");
		}
	}

	return bbcim_errno;
}


int verify_watford_id(FILE *fp, int offset) {
	int i, st;
	unsigned char mybyte;

	/* Verify it's a Watford 62 file image */
	fseek(fp, offset, SEEK_SET);
	for (i=0; i<7; i++) {
//		if (bbcim_disk_read(image, fp, 512 + OFFSET__DFS_entries, s, &mybyte)) return -1;
		st = fread(&mybyte, 1, 1, fp);
		if (st != 1) break;
		if (mybyte != 0xAA) break;
	}

	if (i != 8) return 0;

	fseek(fp, offset+OFFSET__DFS_entries, SEEK_SET);
	st = fread(&mybyte, 1, 1, fp);
	if (st != 1) return 0;
	if ((mybyte % 8) != 0) return 0;

	return 1;
}
