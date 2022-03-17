/* intersplit.c, Copyright (C) W.H.Scholten 1996-2014, 2017
 *
 * make or split an interleaved disk image, single/double density
 * functions: interss & splitds
 *
 * original version 11-3-1996
 *
 * Deel van bbcim.
 */


/* Samenvoegen van 2 enkelzijdige diskbeelden tot een dubbelzijdig om-en-om diskbeeld */
void interss(int argc, char *argv[], int options) {
	char  side0[256],  side1[256], interleaved[256]; 
	char option2[5];
	unsigned char byte;
	FILE *fp0, *fp1, *fp_inter;
	int sec_per_track, track, i;

	int dd=0;

	if ((argc-options)==1) {
#ifdef MESSAGES_NL
		printf("Single density (sd) of double density (dd)?");
#else
		printf("Single density (sd) or double density (dd)?");
#endif
		scanf("%2s", option2);
	} else {strcpy(option2, argv[1+options]); options++;}

	if (!strcmp(option2, "dd")) dd=1;


	if ((argc-options)==1) {
#ifdef MESSAGES_NL
		printf("Naam van het diskbeeld voor zijde 0?");
#else
		printf("Name of the disk image for side 0?");
#endif
		scanf("%50s", side0);
	} else strcpy(side0, argv[1+options]);


	if ((argc-options)<=2) {
#ifdef MESSAGES_NL
		printf("Naam van het diskbeeld voor zijde 1?");
#else
		printf("Name of the disk image for side 1?");
#endif
		scanf("%50s", side1);
	} else strcpy(side1, argv[2+options]);


	if ((argc-options)<=3) {
#ifdef MESSAGES_NL
		printf("Naam van om-en-om beeld?");
#else
		printf("Name of the interleaved disk image?");
#endif
		scanf("%50s", interleaved);
	} else strcpy(interleaved, argv[3+options]);



	fp0=fopen(side0,"rb");
	if (fp0==NULL) {
#ifdef MESSAGES_NL
		printf("Bestand %s is niet te openen\n", side0);
#else
		printf("File %s cannot be opened\n", side0);
#endif
		exit(1);
	}

	if (strcmp(side0,side1)) {
		/*alleen als twee verschillende bestanden */
		fp1=fopen(side1,"rb");
		if (fp1==NULL) {
#ifdef MESSAGES_NL
			printf("Bestand %s is niet te openen\n", side1);
#else
			printf("File %s cannot be opened\n", side1);
#endif
			exit(1);
		}
	} else fp1=fp0;


	fp_inter=fopen(interleaved, "wb");

	printf("  %s + %s > %s\n", side0, side1, interleaved);


	sec_per_track=10;
	if (dd) sec_per_track=18;


	fseek(fp0, 0L, SEEK_SET);
	fseek(fp1, 0L, SEEK_SET);
	fseek(fp_inter,0L, SEEK_SET);
	for (track=0; track<80;track++) {
		fseek(fp0,0L+track*256L*sec_per_track, SEEK_SET);
		for (i=0; i<256*sec_per_track; i++) {
			if (feof(fp0) && feof(fp1)) exit(0);
			if (feof(fp0)) byte=0; else fread(&byte,1,1,fp0);
			fwrite(&byte,1,1,fp_inter);
		}
		fseek(fp1,0L+track*256L*sec_per_track, SEEK_SET);
		for (i=0; i<256*sec_per_track; i++) {
			if (feof(fp1) && feof(fp0)) exit(0);
			if (feof(fp1)) byte=0; else fread(&byte,1,1,fp1);
			fwrite(&byte,1,1,fp_inter);
		}
	}
}
/*einde samenvoegen*/


/* Splits om-en-om diskbeeld: */
void splitds(int argc, char *argv[], int options) {
	char  side0[250],  side1[250], interleaved[250]; 
	char option2[5];
	unsigned char byte;
	FILE *fp0, *fp1, *fp_inter;
	int sec_per_track, track;

	int dd=0;

	if ((argc-options)==1) {
#ifdef MESSAGES_NL
		printf("Single density (sd) of double density 18 sectoren per track (dd)?");
#else
		printf("Single density (sd) or double density 18 sectors per track (dd)?");
#endif
		scanf("%2s", option2);
	} else {
		strncpy(option2, argv[1+options], 4);
		option2[4] = 0;
		if (strcmp(option2, "sd") && strcmp(option2, "dd")) {
			fprintf(stderr, "-splitds {sd|dd} <disk>\n");
			exit(1);
		}
		options++;
	}

	if (!strcmp(option2, "dd")) dd=1;


	if ((argc-options)==1) {
#ifdef MESSAGES_NL
		printf("Naam van de om-en-om diskdump?");
#else
		printf("Name of the interleaved diskdump?");
#endif
		scanf("%50s", interleaved);
	} else strcpy(interleaved, argv[1+options]);

	strcpy(side0, interleaved);
/*  side0[strcspn(side0,".")]=0; */ /* This was useful for DOS, but that's irrelevant these days */
	strcpy(side1, interleaved);
	strcat(side0,".0");
	strcat(side1,".1");

	printf(" %s > %s + %s\n",interleaved, side0, side1);

	fp_inter=fopen(interleaved, "rb");
	if (fp_inter==NULL) {
#ifdef MESSAGES_NL
		printf("Bestand %s is niet te openen\n", interleaved);
#else
		printf("File %s cannot be opened\n", interleaved);
#endif
		exit(1);
	}


	fp0=fopen(side0,"wb");
	fp1=fopen(side1,"wb");


	sec_per_track=10;
	if (dd) sec_per_track=18;

	fseek(fp_inter,0L, SEEK_SET);
	for (track=0; track<80;track++) {
		int i;
		for (i=0; i<256*sec_per_track; i++) {
			if (!fread(&byte,1,1,fp_inter)) exit(0);
			fwrite(&byte,1,1,fp0);
		}
		for (i=0; i<256*sec_per_track; i++) {
			if (!fread(&byte,1,1,fp_inter)) exit(0);
			fwrite(&byte,1,1,fp1);
		}
	}
} /*einde splitsen */
