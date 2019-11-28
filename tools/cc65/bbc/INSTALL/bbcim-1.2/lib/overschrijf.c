/* BBCIM: Acorn BBC computer disk image manipulation
 *
 * overschrijf.c
 *
 * Copyright (C) W.H.Scholten 1996, 1997, 2001
 */

#include "overschrijf.h"


int bbcim_overschrijf_vraag(char *oude_naam, char *nieuwe_naam) {
	char string;
	int antwoord = 0;

	printf("bestand %s -> %s:\n", oude_naam, nieuwe_naam);

	while (!antwoord) {
		printf(TEKST_NL("  overschrijven/hernoem nieuw/hernoem bestaand/sla over?(o/n/b/s)")
			   TEKST_ENG("  overwrite/rename new file/rename existing file/skip?(o/n/e/s)")
			);
		scanf(" %c", &string);
		switch(string) {
		case 'o': antwoord = BBCIM_OVERSCHRIJVEN; break;
		case 'n': antwoord = BBCIM_HERNOEM_NIEUW; break;

#ifdef LANGUAGE_DUTCH
		case 'b': antwoord = BBCIM_HERNOEM_BESTAAND; break;
#else
		case 'e': antwoord = BBCIM_HERNOEM_BESTAAND; break;
#endif
		case 's': antwoord = BBCIM_SLA_OVER; break;
		}
	}

	if (antwoord == BBCIM_HERNOEM_NIEUW || antwoord == BBCIM_HERNOEM_BESTAAND) {
		printf(
			TEKST_NL("nieuwe naam?")
			TEKST_ENG("new name?")
			);
		scanf("%49s", nieuwe_naam);
	}

	return antwoord;
}
