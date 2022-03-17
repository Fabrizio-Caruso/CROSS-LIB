/* overschrijf.c, Copyright (C) W.H.Scholten 1996,1997,2014, 2017
 *
 * Part of bbcim.
 */


#include "overschrijf.h"

int  rename_sequential(char *naam) {
	FILE *fp;
	static char new_name[256];
	int i=0;

	strcpy(new_name, naam);
	for(;;) {
		fp=fopen(new_name, "r");
		if (fp == NULL) break;
		fclose(fp);
		strcpy(new_name, naam);
		sprintf(new_name+strlen(new_name), "%d", i);
		i++;
	}
	strcpy(naam,new_name);
	return 1;
}

int  overschrijf_vraag(char *oude_naam, char *nieuwe_naam) {
	char string;
	int antwoord=0;

	printf("bestand %s -> %s:\n", oude_naam, nieuwe_naam);

	while (!antwoord) {
#ifdef MESSAGES_NL
		printf("  overschrijven/hernoem nieuw/hernoem bestaand/sla over?(o/n/b/s)");
		scanf(" %c",&string);
		switch(string) {
		case 'o': antwoord=OVERSCHRIJVEN;break;
		case 'n': antwoord=HERNOEM_NIEUW;break;
		case 'b': antwoord=HERNOEM_BESTAAND;break;
		case 's': antwoord=SLA_OVER;}
#else
		printf("  overwrite/rename new file/rename existing file/skip?(o/n/e/s)");
		scanf(" %c",&string);
		switch(string) {
		case 'o': antwoord=OVERSCHRIJVEN;break;
		case 'n': antwoord=HERNOEM_NIEUW;break;
		case 'b': antwoord=HERNOEM_BESTAAND;break;
		case 's': antwoord=SLA_OVER;}
#endif
	}

	if (antwoord==HERNOEM_NIEUW || antwoord==HERNOEM_BESTAAND) {
#ifdef MESSAGES_NL
		printf("nieuwe naam?");
#else
		printf("new name?");
#endif
		scanf("%49s", nieuwe_naam);
	}

	return antwoord;
}
