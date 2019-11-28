/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * crc.c: check or make 16 bit crc's of files (see BBC user guide p.399)
 *
 * Copyright (C) W.H.Scholten 1996 - 2001
 */


#include "_bbc_crc.h"
int bbcim_archive_crc_check(char *filename, int verbose);


/* CRC check for the archive format. Returns 0 = ok, -1 if a bad crc or no crc
 * was found, 1 if the file is an .inf file accompanying a real file.
 */
int bbcim_archive_crc_check(char *filename, int verbose) {
	FILE *fp, *fp_inf;
	char *filename_inf;
	int  found_crc;
	int  len_s;
	int  i;

	char read_line[100];
	unsigned int crc;
	int  result = 0;

	struct bbc_archive__file_attr bbcfile_attr;
	struct bbc_archive__file bbcfile;

	bbcfile.filename = filename;
	bbcfile.attr = &bbcfile_attr;

	/* .inf bestanden overslaan: */
	if (bbc_archive__is_inf_file(filename)) return 1;

	/* Bestaat het archief bestand? */
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		if (verbose) fprintf(stderr, TEKST_NL("Archief bestand %s kan niet geopend worden\n")
			   TEKST_ENG("Archive file %s cannot be opened\n")
			   , filename);
		return -1;
	}
	fclose(fp);

	if (bbc_archive__read_file_attributes(filename, &bbcfile_attr, 1)) {
		bbc_archive__free_file_attributes(&bbcfile_attr);
		return -1;
	}

	filename_inf = bbc_archive__new_inf_filename(filename, verbose);
	if (filename_inf == NULL) return bbcim_errno;

	fp_inf = fopen(filename_inf, "r");
	if (fp_inf == NULL) { free(filename_inf); return -1; }

	/* CRC zoeken */
	fseek(fp_inf, 0L, SEEK_SET);
	fgets(read_line, 99, fp_inf);

	found_crc = 0;
	len_s = strlen(read_line);
	printf(TEKST_NL("bestand %-9s (%-9s) CRC ")
		   TEKST_ENG("file %-9s (%-9s) CRC ")
		   , filename, (&bbcfile)->attr->name);

	for (i=0; i<len_s-4; i++) {
		if (!strncmp(read_line+i, "CRC=", 4)) {
			found_crc = 1;
			fseek(fp_inf, i+4L, SEEK_SET);
			fscanf(fp_inf, "%x", &crc);
			if (crc != bbc_fcrc(filename, 1)) {
				result = -1;
				printf(TEKST_NL("fout, gevonden: %4X")
					   TEKST_ENG("wrong, found: %4X")
					   , crc);
				crc = bbc_fcrc(filename, 0);
				printf(TEKST_NL(", berekend: %4X\n")
					   TEKST_ENG(", calculated: %4X\n")
					   , crc);
			}
			else {
				printf(TEKST_NL("in orde\n")
					   TEKST_ENG("OK\n"));
			}
			break;
		}
	}
	if (!found_crc)
		printf(TEKST_NL("ontbreekt\n")
			   TEKST_ENG("missing\n"));

	free(filename_inf);
	fclose(fp_inf);

	return result;
}
