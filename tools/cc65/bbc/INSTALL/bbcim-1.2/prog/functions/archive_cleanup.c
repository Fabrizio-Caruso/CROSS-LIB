/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * archive_cleanup.c
 * ENGLISH: cleans up the info file and renames (should rename be optional?)
 *
 * Copyright (C) W.H.Scholten 1996, 2001, 2003
 */

#include <_bbc_filenames.h>
#include <_bbc_crc.h>
int bbcim_archive_cleanup(char *filename, int verbose);


int bbcim_archive_cleanup(char *filename, int verbose) {
	FILE *fpadd, *fpinf;
	char *inf_file = NULL;
	unsigned int crc;
	int  antwoord;
	char bbcfile_naam [ 13 ];
	char host_bbc_filename [ 15 ];
	char nieuwe_inf [ 256 ];
	char nieuwe_naam [ 256 ];
	int  found;

	struct bbc_archive__file bbcfile;
	struct bbc_archive__file_attr attr;
	bbcfile.attr = &attr;

	/*.inf bestanden overslaan:*/
	if (bbc_archive__is_inf_file(filename)) return 1;

	/* Bestaat het archief bestand? */
	fpadd = fopen(filename, "rb");
	if (fpadd == NULL) {
		fprintf(stderr, TEKST_NL("Archief bestand %s kan niet geopend worden") TEKST_ENG("Archive file %s cannot be opened") "\n", filename);
		return -1;
	}
	fclose(fpadd);

	if (bbc_archive__read_file_attributes(filename, &attr, 1)) return -1;

	inf_file = bbc_archive__new_inf_filename(filename, verbose);
	if (inf_file == NULL) return bbcim_errno;

	bbcim_reduce_bbcname__no_rootdir(host_bbc_filename, bbcfile_naam);
#warning "flags to choose between no_dir/no_rootdir?"
	bbc_replace_bad_chars(host_bbc_filename);

	/* Hernoemen etc als naam al bestaat */
	strcpy(nieuwe_naam, host_bbc_filename);

	if (strcmp(filename, host_bbc_filename)) {
		if ((fpadd = fopen(host_bbc_filename, "rb")) != NULL) {
			fclose(fpadd);
			antwoord = bbcim_overschrijf_vraag(filename, nieuwe_naam);

			switch(antwoord) {

			case BBCIM_OVERSCHRIJVEN:
				rename(filename, host_bbc_filename);
				break;

			case BBCIM_HERNOEM_NIEUW:
				strcpy(host_bbc_filename, nieuwe_naam);
				rename(filename, host_bbc_filename);
				break;

			case BBCIM_HERNOEM_BESTAAND:
				rename(host_bbc_filename, nieuwe_naam);
				strcat(nieuwe_naam, ".inf");
				rename(inf_file, nieuwe_naam);
				rename(filename, host_bbc_filename);
				break;

			case BBCIM_SLA_OVER:
				if (inf_file) free(inf_file);
				return -1;
			} /*switch*/

		}
		else
			rename(filename, host_bbc_filename);
	} /*if*/

	strcpy(nieuwe_inf, host_bbc_filename);
	strcat(nieuwe_inf, ".inf");


	found = 0;
	if (bbcfile.attr->crc != -1) {
		int  recalculated_crc = bbc_fcrc(host_bbc_filename, 1);
		found = 1;
		if (bbcfile.attr->crc != recalculated_crc) {
				printf(TEKST_NL("fout, gevonden: %X, berekend: %X")TEKST_ENG("wrong, found: %X, calculated: %X"), (unsigned int) bbcfile.attr->crc, recalculated_crc);
		}
		else {
			printf(TEKST_NL("in orde") TEKST_ENG("ok"));
		}
	}
	else {
		/* Add a CRC only if there wasn´t one yet: */
		printf(TEKST_NL("toegevoegd") TEKST_ENG("added"));
	}
	crc = bbc_fcrc(host_bbc_filename, 0);

	remove(inf_file);

	fpinf = fopen(nieuwe_inf, "w");
	fprintf(fpinf, "%-10s %8X %8X  CRC=%4X", bbcfile_naam, attr.load_address, attr.exec_address, (unsigned int) crc);
	fclose(fpinf);


	/* Did the name actually change? */
	if (strcmp(filename, host_bbc_filename)) {
		printf(TEKST_NL(", hernoemd tot %s\n") TEKST_ENG(", renamed to %s\n"), host_bbc_filename);
	}
	else printf("\n");

	if (inf_file) free(inf_file);

	return 0;
}
