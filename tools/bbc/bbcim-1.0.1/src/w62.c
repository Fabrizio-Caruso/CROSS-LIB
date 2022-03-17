/* w62.c, Copyright (C) 1996-2014,2017 W.H. Scholten
 *
 * Converts a Watford 62 file diskimage into 2 Acorn DFS diskimages
 *
 * Part of bbcim.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int  w62dfs(int argc, char *argv[], int options) {
	FILE *fpdisk, *fpdisk2;
	unsigned char byte;
	char disk1[256], disk2[256];
	int  i;

	if (argc<3) {
#ifdef MESSAGES_NL
		printf("Diskbeeld naam afwezig\n");
#else
		printf("Diskimage name missing\n");
#endif
		return 1;
	}

  
	strcpy(disk1, argv[2]);
	disk1[strcspn(disk1,".")]=0;
	strcpy(disk2, disk1);
	strcat(disk1,".w1");
	strcat(disk2,".w2");

	fpdisk=fopen(argv[2],"rb");
	if (!fpdisk) {
#ifdef MESSAGES_NL
		printf("Watford 62 bestand diskbeeld niet te openen\n");
#else
		printf("Failed to open watford 62 file image\n");
#endif
		exit(1);
	}

	/* are there files on image 2? */
	fseek(fpdisk,512+DFSentries,SEEK_SET);
	fread(&byte,1,1,fpdisk);
	if (byte) {

		fpdisk2=fopen(disk2,"wb");

		/* Move 2 sectors down */
		for (i=0; i<512; i++) {
			fseek(fpdisk,512+i,SEEK_SET);
			fread(&byte,1,1,fpdisk);
			fseek(fpdisk,0+i,SEEK_SET);
			fwrite(&byte,1,1,fpdisk2);
		}

		fseek(fpdisk,512,SEEK_SET);

		for (i=512; i<200*1024; i++) {
			fread(&byte,1,1,fpdisk);
			if (feof(fpdisk)) break;
			fwrite(&byte,1,1,fpdisk2);
		}
		fclose(fpdisk2);
	}

	/* Netjes afsluiten */
	fclose(fpdisk);
	rename(argv[2], disk1);
	return 0;
}
