/* bbc_crc.c,  Copyright (C) W.H.Scholten 1996-2014
 *
 * check/make 16 bit CRCs of files, see the BBC micro user guide p.399
 *
 * Laatste verandering 30-6-96
 *
 * Part of bbcim.
 */

unsigned int fcrc(char *bestand) {
	unsigned long length=0;
	unsigned char byte;
	FILE *fp;

	unsigned int crc=0;
	int k;


	length=0; crc=0;

	fp=fopen(bestand,"rb");

	if (fp==NULL) {
#ifdef MESSAGES_NL
		printf("Bestand %s kan niet geopend worden\n", bestand);
#else
		printf("File %s couldn't be opened\n", bestand);
#endif

		return crc;
	}

	while (1) {
		if (!fread(&byte,1,1,fp)) break;
		/* if (feof(fp)) break; */
		length++;
		crc ^=(byte << 8);
		for(k=0; k<8; k++) {
			if (crc & 32768) crc=(((crc ^ 0x0810) & 32767) << 1)+1;
			else crc <<= 1;
		}
	}
	fclose(fp);

	return crc;
}







/* CRC controle voor archief formaat */
void icrc(int argc, char *argv[], int options) {
	FILE *fparchive, *fpinfo;
	char infofile[100];
	int  archivefile,l, found, len_s, i;
	char bbc_naam[12];

	char read_line[100];
	unsigned int crc;



/* Niet gebruikt: (CRC zonder lijst bestanden niet geimplementeerd)
   if ((argc-options)<3){
   #ifdef MESSAGES_NL
   printf("Naam van het te controleren archief bestand?");
   #else
   printf("Name of the archive file to be checked?");
   #endif
   scanf("%50s", archivefile);
   } else strcpy(archivefile, argv[2+options]);
*/


	if ((argc-options)<2) {
#ifdef MESSAGES_NL
		printf("Geen lijst van archief bestanden meegegeven\n");
#else
		printf("No archive file list given\n");
#endif
		exit(1);
	}



	for(archivefile=2; archivefile<argc; archivefile++) {


		/* .inf bestanden overslaan: */
		l=strlen(argv[archivefile]);
		if (l>4)
			if (!strncmp(argv[archivefile]+l-4,".inf",4)) continue;

		strcpy(infofile,argv[archivefile]);
		strcat(infofile,".inf");



		/* Bestaat het info bestand? */
		fpinfo=fopen(infofile, "r");
		if (fpinfo==NULL) {
#ifdef MESSAGES_NL
			printf("Info bestand %s kan niet geopend worden, sla over\n", infofile);
#else
			printf("Info file %s cannot be opened, skipping file\n", infofile);
#endif
			continue;
		}


		/* Bestaat het archief bestand? */
		fparchive=fopen(argv[archivefile], "rb");
		if (fparchive==NULL) {
#ifdef MESSAGES_NL
			printf("Archief bestand %s kan niet geopend worden\n", argv[archivefile]);
#else
			printf("Archive file %s cannot be opened\n", argv[archivefile]);
#endif
			continue;
		}
		fclose(fparchive);


		/* uit het .inf bestand de bestandsnaam halen. */
		fscanf(fpinfo,"%12s", bbc_naam);
		if (bbc_naam[1]!='.') strncpy(bbc_naam, argv[archivefile],12);
		bbc_naam[9]=0;

		/* CRC zoeken */
		fseek(fpinfo,0L,0);
		fgets(read_line, 99, fpinfo);

		found=0;
		len_s=strlen(read_line);
		printf("bestand %-9s (%-9s) CRC ", argv[archivefile],bbc_naam);
		for (i=0; i<len_s-4; i++) {
			if (!strncmp(read_line+i,"CRC=",4)) {
				found=1;
				fseek(fpinfo, i+4L,0); fscanf(fpinfo, "%x", &crc);
				if (crc!=fcrc(argv[archivefile])) {
#ifdef MESSAGES_NL
					printf("fout, gevonden: %4X", crc);
#else
					printf("wrong, found: %4X", crc);
#endif
					crc=fcrc(argv[archivefile]);

#ifdef MESSAGES_NL
					printf(", berekend: %4X\n", crc);
#else
					printf(", calculated: %4X\n", crc);
#endif
				}
				else printf("in orde\n");
				break;
			}
		}
		if (!found) printf("ontbreekt\n");

		fclose(fpinfo);
	} /*FOR */
}






/* Controleer CRC's in __CATALOG__ ($.ELITE 00E00 FF8023 ... CRC=xxxx) */
void xcrc(int argc, char *argv[], int options) {
	FILE *fpcat;
	unsigned int crc;
	int found, len_s, i;

	char bbc_file[15], read_line[100];


	fpcat=fopen("__CATALOG__", "r");
	if (fpcat==NULL) {
#ifdef MESSAGES_NL
		printf("__CATALOG__ kon niet geopend worden\n");
#else
		printf("__CATALOG__ couldn't be opened\n");
#endif
		exit (1);
	}

	while (1) {
		if (fgets(read_line, 99, fpcat)==NULL) break;
		if (read_line[1]=='.') {
			strncpy(bbc_file, read_line, 9); bbc_file[9]=0;
			/* Spaties aan einde weghalen */
			for (i=8;i>0;i--) {
				if (bbc_file[i]!=' ') break;
				bbc_file[i]=0;
			}

			found=0;
			len_s=strlen(read_line);
#ifdef MESSAGES_NL
			printf("bestand %-9s CRC ", bbc_file);
			for (i=0; i<len_s-4; i++) {
				if (!strncmp(read_line+i,"CRC=",4)) {
					found=1;
					sscanf(read_line+i+4L, "%x", &crc);
					if (crc!=fcrc(bbc_file)) {
						printf("fout, gevonden: %4X", crc);
						crc=fcrc(bbc_file);
						printf(", berekend: %4X\n", crc);
					}
					else printf("in orde\n");
					break;
				}
			}
			if (!found) printf("ontbreekt\n");
#else
			printf("file %-9s CRC ", bbc_file);
			for (i=0; i<len_s-4; i++) {
				if (!strncmp(read_line+i,"CRC=",4)) {
					found=1;
					sscanf(read_line+i+4L, "%x", &crc);
					if (crc!=fcrc(bbc_file)) {
						printf("wrong, found: %4X", crc);
						crc=fcrc(bbc_file);
						printf(", calculated: %4X\n", crc);
					}
					else printf("all right\n");
					break;
				}
			}
			if (!found) printf("missing\n");
#endif
		}
	} /*while*/   
}
