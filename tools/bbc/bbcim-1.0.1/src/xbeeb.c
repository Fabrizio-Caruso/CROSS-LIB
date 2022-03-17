/* xbeeb.c, Copyright (C) W.H.Scholten 1996-2014, 2017
 *
 * Laatste verandering: 30-6-96
 * Alles werkt maar code is zeer lelijk (dubbel etc). VERBETER.
 *
 * part of bbcim.
 */

#include "overschrijf.h"

void xbeeb(int argc, char *argv[], int options) {
	FILE *fparchief, *fpinfo, *fpcat, *fp_oude_cat;
	char infofile[256];

	const char* catalogus="__CATALOG__";
	const char* oude_catalogus="__CATALOG__%";


	int archief_bestand, l;
	char bbcfile_naam[15], xbeeb_naam[15];
	unsigned int loadaddress, execaddress;
	char read_line[256];
	char locked;
	char nieuwe_naam[256];
	int antwoord, len_s,i;
	int length;
	unsigned int crc;

/* Conversion without command line list not implemented:
//   if ((argc-options)<3)
//    {
//     #ifdef MESSAGES_NL
//     printf("Naam van het toe te voegen archief bestand?");
//     #else
//     printf("Name of the archive file to be added?");
//     #endif
//     scanf("%50s", addfile);
//     } else strcpy(addfile, argv[2+options]);
*/

	if ((argc-options)<2) {
#ifdef MESSAGES_NL
		printf("Geen lijst van archief bestanden meegegeven\n");
#else
		printf("No archive file list given\n");
#endif
		exit(1);
	}





	for(archief_bestand=2; archief_bestand<argc; archief_bestand++) {
		/* .inf bestanden overslaan: */
		/*int */l=strlen(argv[archief_bestand]);
		if (l>4) {
			if (!strncmp(argv[archief_bestand]+l-4,".inf",4))
				continue;
		}

		strcpy(infofile,argv[archief_bestand]);
		strcat(infofile,".inf");


		/* Bestaat het archief bestand? */
		fparchief=fopen(argv[archief_bestand], "rb");
		if (fparchief==NULL) {
#ifdef MESSAGES_NL
			printf("Archief bestand %s kan niet geopend worden\n", argv[archief_bestand]);
#else
			printf("Archive file %s cannot be opened\n", argv[archief_bestand]);
#endif
			continue;
		}
		fclose(fparchief);

		/* Bestaat het .inf bestand? */
		fpinfo=fopen(infofile, "r");
		if (fpinfo==NULL) {
#ifdef MESSAGES_NL
			printf("Info bestand %s kan niet geopend worden, sla over\n", infofile);
#else
			printf("Info file %s cannot be opened, skipping file\n", infofile);
#endif
			continue;
		}

		/* Uit het inf bestand de bestandsnaam halen en de adressen. */
#if 1
		fscanf(fpinfo,"%12s", bbcfile_naam);
		if (bbcfile_naam[1]!='.') {
			strncpy(bbcfile_naam, argv[archief_bestand], 12);
			fseek(fpinfo, 0L, SEEK_SET);
		}
		bbcfile_naam[9]=0;
		fscanf(fpinfo,"%x %x", &loadaddress, &execaddress);
#else
		fscanf(fpinfo,"%12s %x %x", bbcfile_naam, &loadaddress, &execaddress);
		bbcfile_naam[9]=0;

		if (bbcfile_naam[1]!='.') {
			strcpy(bbcfile_naam, argv[archief_bestand]);
			fseek(fpinfo, 0L, SEEK_SET);
			fscanf(fpinfo," %x %x", &loadaddress, &execaddress);
		}
#endif


		/* 'Locked' wordt nu behandeld: */
		fgets(read_line, 49, fpinfo);
		fclose(fpinfo);
 
		len_s=strlen(read_line);

		/*      NOTE: 'Locked' may be abbreviated to 'L':*/
		locked=' ';
		for (i=0; i<len_s; i++)
			if (read_line[i]=='L')
				{locked='L'; break;}



		/* Hernoemen etc als naam al bestaat (en alleen als de nieuwe naam anders is) */
		strcpy(nieuwe_naam, bbcfile_naam);

		if (strcmp(argv[archief_bestand], bbcfile_naam)) {

			if ((fparchief=fopen(bbcfile_naam, "rb"))!=NULL) {
				fclose(fparchief);
				antwoord=overschrijf_vraag(argv[archief_bestand], nieuwe_naam);
				/* If new name has no bbcdir assume $ */
				/* will work correctly if no new name is given (overwrite/skip)*/
				if (nieuwe_naam[1]!='.') {
					for(i=12; i>1; i--)
						nieuwe_naam[i]=nieuwe_naam[i-2];
					nieuwe_naam[0]='$'; nieuwe_naam[1]='.';
				}
				nieuwe_naam[9]=0; /*maximale naam lengte*/

				switch(antwoord) {
				case OVERSCHRIJVEN:
					rename(catalogus, oude_catalogus);
					fp_oude_cat=fopen(oude_catalogus,"r");
					if (fp_oude_cat != NULL) { /*EXISTS ??*/
						/* verwijder oude catalogus ingang: */
						fpcat=fopen(catalogus,"w");

						while (fgets(read_line, 49, fp_oude_cat)) {
							sscanf(read_line, "%12s", xbeeb_naam);
							if (!strcmp(bbcfile_naam, xbeeb_naam))
								continue;
							fputs(read_line, fpcat);
						}
						fclose(fpcat); fclose(fp_oude_cat);
					} /* if (exists) */
					break;

				case HERNOEM_NIEUW:
					strcpy(bbcfile_naam, nieuwe_naam);
					break;

				case HERNOEM_BESTAAND:
					/* verander oude catalogus ingang: */
					rename(catalogus, oude_catalogus);

					fp_oude_cat=fopen(oude_catalogus,"r");
					if (fp_oude_cat != NULL) { /*exists ??? */
						/* it might not if creating an xbeeb dir and files have xbeeb like names */
						fpcat=fopen(catalogus,"w");
						while(fgets(read_line, 49, fp_oude_cat)) {
							sscanf(read_line, "%9s", xbeeb_naam);
							if (!strcmp(bbcfile_naam, xbeeb_naam)) {
								fprintf(fpcat, "%-9s", nieuwe_naam);
								fputs(read_line+9, fpcat);
							}
							else
								fputs(read_line, fpcat);
						}
						fclose(fpcat); fclose(fp_oude_cat);
					}/*if (exists) */
					rename(bbcfile_naam, nieuwe_naam);
					break;

				case SLA_OVER:
					continue;
				} /*switch*/
			} /*if*/





#ifdef MESSAGES_NL
			printf("bestand %s hernoemd tot %s\n", argv[archief_bestand], bbcfile_naam);
#else
			printf("renamed file %s to %s\n", argv[archief_bestand], bbcfile_naam);
#endif

		} /*einde test of de 'hernoem naam' anders is.*/

		fparchief=fopen(argv[archief_bestand], "rb");
		fseek(fparchief, 0L, SEEK_END);
		length=ftell(fparchief);
		fclose(fparchief);

		rename(argv[archief_bestand], bbcfile_naam);

		remove(infofile);

/* if CRC was bad, should that one be put in the __CATALOG__ ??
   Probably. For now, I recalculate. FIX (if crc is bad say so in __CATALOG__)
*/

		crc=fcrc(bbcfile_naam);


		/* Add filename & information to __CATALOG__ */
		fpcat=fopen(catalogus,"a");
		fprintf(fpcat,"%-9s  %c  %6X %6X %6X     CRC=%04X\n", bbcfile_naam, locked, loadaddress, execaddress, length, (unsigned int) crc);
		fclose(fpcat);
	} /* FOR */
}
