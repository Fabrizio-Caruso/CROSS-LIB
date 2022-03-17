/* splits_cat.c, Copyright (C) W.H.Scholten 1996-2014, 2017
 *
 * Part of bbcim
 */


void split_cat(int argc, char *argv[],int options, char *optread_line) {
	FILE *fpcat, *fpinfo;
	char infofile[256];
	char read_line[256], bbcfile_naam[15];
	char cat_naam[256], nieuwe_naam[256];
	int i, args;

	int no_bbc_dir=0;
	int antwoord;

	unsigned int load, exec, crc;

	if ((argc-options)<2) {
#ifdef MESSAGES_NL
		printf("Naam van de te splitsen catalogus?");
#else
		printf("Name of the catalogue to be split?");
#endif
		scanf("%50s", cat_naam);
	} else strcpy(cat_naam, argv[1+options]);


	if (optread_line[2]=='#') no_bbc_dir=1;

	fpcat=fopen(cat_naam,"r");
	if (fpcat == NULL) {
#ifdef MESSAGES_NL
		fprintf(stderr, "Bestand niet gevonden\n");
#else
        fprintf(stderr, "File not found\n");
#endif
		return; }

	while (fscanf(fpcat,"%12s", bbcfile_naam)!=EOF) {
		bbcfile_naam[9]=0;

#if DEBUG
		printf("bbcfile=%s", bbcfile_naam);
#endif

		fgets(read_line, 99, fpcat); /* Get remainder of line */


		if (bbcfile_naam[1]=='.') {
			if (no_bbc_dir)
/*         {if (bbcfile_naam[0]=='$') strcpy(infofile,bbcfile_naam+2);
		   else {infofile[0]=bbcfile_naam[0];
		   strcpy(infofile+1,bbcfile_naam+2);
		   }
		   }
*/
				reduceer_unix_naam(infofile, bbcfile_naam);

			else strcpy(infofile,bbcfile_naam);
			strcat(infofile,".inf");

			/* Check if file exists (can be opened) */
			if ((fpinfo=fopen(infofile, "r"))!=NULL) {
				fclose(fpinfo);
				antwoord=overschrijf_vraag(bbcfile_naam, infofile);

				switch(antwoord) {
				case OVERSCHRIJVEN:
					break;

				case HERNOEM_NIEUW:
					strcpy(infofile, nieuwe_naam); break;

				case HERNOEM_BESTAAND:
					rename(infofile, nieuwe_naam); break;

				case SLA_OVER:
					continue; /*next in while loop*/
				} /* switch */
			}
			/* End name check */



			fpinfo=fopen(infofile, "w");
			fprintf(fpinfo, "%s", bbcfile_naam);

			for (i=0; i<99; i++) {
				if (read_line[i]>' ') break;
			}

			args =sscanf(read_line,"%*[ L\x09]%x %x %*[^=]=%x", &load, &exec, (unsigned int *) &crc);
			fprintf(fpinfo, "  %6X  %6X", load, exec);
			if (read_line[i]=='L') fprintf(fpinfo, " Locked");
			if (args==3) fprintf(fpinfo, "  CRC=%4X", (unsigned int) crc);
			fclose(fpinfo);
		}
    
	}
	/* netjes afsluiten */
	fclose(fpcat);
}
