/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * catalogue_to_inf.c
 *
 * Copyright (C) W.H.Scholten 1996, 1997, 2000, 2001, 2003
 */

#include <_bbc_filenames.h>
int bbcim_catalogue_to_inf(char *cat_naam, int flags, int verbose);


/* Convert entries from a catalogue file (a file with information as *CAT would
 * give on a BBC computer) to seperate .inf files for each catalogue entry.
 * Useful to convert xbeeb files to archive format for example.
 */
int bbcim_catalogue_to_inf(char *cat_naam, int flags, int verbose) {
	FILE *fpcat, *fpinf;
	char inf_file [256];
	char nieuwe_naam [256];
	char read_line [256];
	char bbcfile_naam [ 15 ];

	if (bbcim_file_sanitycheck(cat_naam, verbose)) return bbcim_errno;

	fpcat = fopen(cat_naam, "r");
	while (fscanf(fpcat, "%12s", bbcfile_naam) != EOF) {
		bbcfile_naam[9] = 0;
		fgets(read_line, 99, fpcat); /* Get remainder of line. */
		if (bbcfile_naam[1] == '.') { /* It's a line with file information */
			unsigned int load, exec, crc;
			int  i, args;

			bbc_replace_bad_chars(bbcfile_naam);

			if (!(flags & (BBCIM__NAME__KEEP_ROOT_DIR | BBCIM__NAME__KEEP_NONROOT_DIR)))
				bbcim_reduce_bbcname__no_dir(inf_file, bbcfile_naam);
			else if (flags & BBCIM__NAME__KEEP_ROOT_DIR)
				bbcim_reduce_bbcname__no_rootdir(inf_file, bbcfile_naam);
			else strcpy(inf_file, bbcfile_naam);

			strcat(inf_file, ".inf");

			/* Check if file exists (can be opened) */
			if ((fpinf = fopen(inf_file, "r")) != NULL) {
				int  antwoord;

				fclose(fpinf);
				antwoord = bbcim_overschrijf_vraag(inf_file, nieuwe_naam);

				switch(antwoord) {
				case BBCIM_OVERSCHRIJVEN:
					break;

				case BBCIM_HERNOEM_NIEUW:
					strcpy(inf_file, nieuwe_naam); break;

				case BBCIM_HERNOEM_BESTAAND:
					rename(inf_file, nieuwe_naam); break;

				case BBCIM_SLA_OVER:
					continue; /* next in while loop */
				} /* switch */
			}
			/* end of name check */

			fpinf = fopen(inf_file, "w");
			fprintf(fpinf, "%s", bbcfile_naam);

			for (i=0; i<99; i++) {
				if (read_line[i] > ' ') break;
			}

			args = sscanf(read_line, "%*[ L\x09]%x %x %*[^=]=%x", &load, &exec, (unsigned int *) &crc);
			fprintf(fpinf, "  %6X  %6X", load, exec);
			if (read_line[i] == 'L') fprintf(fpinf, " Locked");
			if (args == 3) fprintf(fpinf, "  CRC=%4X", (unsigned int) crc);
			fclose(fpinf);
		}
	
	}
	fclose(fpcat);

	return 0;
}
