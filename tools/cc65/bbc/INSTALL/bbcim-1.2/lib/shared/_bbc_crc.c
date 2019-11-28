/* bbc_crc.h, Copyright (C) W.H.Scholten 2009-2014
 *
 * Calculates CRCs using the algorithm from the BBC microcomputer user guide p.399
 */

#include <stdio.h>

#include "_bbc_crc.h"

int bbc_fcrc(char *bestand, int verbose) {
	FILE *fp;
	unsigned long length;
	unsigned int crc;


	length = 0;
	crc = 0;

	fp = fopen(bestand, "rb");
	if (fp == NULL) {
		if (verbose) printf("Unable to open %s\n", bestand);
		return -1;
	}

	while (1) {
		unsigned char filebyte;
		int  k;

		if (!fread(&filebyte, 1, 1, fp)) break;
		/* if (feof(fp)) break; */
		length++;
		crc ^= (filebyte << 8);
		for(k=0; k<8; k++) {
			if (crc & 32768) crc = (((crc ^ 0x0810) & 32767) << 1)+1;
			else crc <<= 1;
		}
	}
	fclose(fp);

	return crc;
}


int bbc_data_crc(void *data, int length) {
	int  i;
	unsigned char *d = (unsigned char *) data;
	unsigned int crc;

	crc = 0;

	for (i=0; i<length; i++) {
		int  k;

		crc ^= ((*d++) << 8);
		for(k=0; k<8; k++) {
			if (crc & 32768) crc = (((crc ^ 0x0810) & 32767) << 1)+1;
			else crc <<= 1;
		}
	}

	return crc;
}
