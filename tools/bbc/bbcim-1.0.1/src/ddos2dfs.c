/* ddos2dfs.c, Copyright (c) W.H. Scholten 1996, 2014, 2017
 *
 * Split DDOS disk images into separate Acorn DFS disk images
 *
 * 14-5-1996, Deel van bbcim >=0.70
 */


#ifdef MESSAGES_NL
#define fout byte=ddos_no+65; printf("onverwacht einde van het diskbeeld" \
									 " bij ddos drive %c ",byte);
#else
#define fout byte=ddos_no+65; printf("Unexpected end of DDOS disk image" \
									 " at ddos drive %c ",byte);
#endif



void ddos2dfs(int argc, char *argv[], int options) {
	FILE *fpdfs, *fpdisk;
	char  dfs_image[256], disk[256], basisnaam[256];
	unsigned char byte=0,H=0;
	long ddos_no, i, sectors_on_volume, length;
	int  starttrack, startsector, l;

	if (argc-options==1) {
#ifdef MESSAGES_NL
		printf("Naam van het DD bbc diskbeeld?");
#else
		printf("Name of the DD bbc disk image?");
#endif
		scanf("%50s", disk);
	}
	else strcpy(disk, argv[2]);


	strcpy(basisnaam, disk);
	l=strlen(disk);
	if (!strcmp(disk+l-3,".dd"))
		basisnaam[l-3]=0;


	fpdisk=fopen(disk,"rb");

	if (fpdisk==NULL) {
#ifdef MESSAGES_NL
		printf("Bestand  %s is niet te openen\n", disk);
#else
		printf("File %s cannot be opened\n", disk);
#endif
		exit(1);
	}
	/* bestand_open_fout(disk); */


	/* Hoofdlus: alle catalogi */
	for (ddos_no=0; ddos_no<8; ddos_no++) {
		fseek(fpdisk,16*256L+8L+2*ddos_no,SEEK_SET);
		fread(&byte,1,1,fpdisk);
		starttrack=byte;

		if (starttrack !=0) {
			strcpy(dfs_image, basisnaam);
			byte=ddos_no+65;
			l=strlen(basisnaam);
			dfs_image[l]=byte;
			strcpy(dfs_image+l+1, ".bbc");

			/* Nu een test of er ueberhaupt iets op de DDOS catalogus staat: */
			fseek(fpdisk, (ddos_no*2+1)*256+5, SEEK_SET);
			fread(&byte,1,1,fpdisk);
			if (byte==0) {
				byte=65+ddos_no;
				printf(" Catalogus %c is leeg\n", byte);
				continue;
			}
			/* (dus geen lege diskbeelden) */

			printf("%s\n",dfs_image);

			/* Uitvoer bestand openen. */
			fpdfs=fopen(dfs_image, "wb");

			fseek(fpdisk, (ddos_no*2+1)*256+6, SEEK_SET);
			fread(&byte,1,1,fpdisk);
			H=byte & 0x03;
			fread(&byte,1,1,fpdisk);
			/*long*/ sectors_on_volume=byte+H*256;

			/*long*/ length=sectors_on_volume*256;

			/* First sector & second sector: */
			fseek(fpdisk, (ddos_no*2+0)*256, SEEK_SET);
			for(i=0; i<512; i++) {
				fread(&byte,1,1,fpdisk);
				if (feof(fpdisk)) {
					fout
						exit(1);
				}
				fwrite(&byte,1,1,fpdfs);
			}

			/* In the second sector, adjust the sector start addresses. */
			/* NB: mag voor allemaal?? (ook als bv 2 best. gebruikt?) Denk wel. */
			for(i=0; i<31; i++) {
				fseek(fpdisk, (ddos_no*2+1)*256+14L+i*8, SEEK_SET);
				fseek(fpdfs, 256L+i*8+14, SEEK_SET);
				fread(&byte,1,1,fpdisk);
				H=byte;
/*  printf("H=%X",H); */
				fread(&byte,1,1,fpdisk);
/* printf("byte=%X",byte); */
				startsector=byte+H*256+2;
				H=startsector >>8;
				byte=startsector & 0xFF;
				fwrite(&H,1,1,fpdfs);
				fwrite(&byte,1,1,fpdfs);
			}

			/* Nu de rest: */
			fseek(fpdisk,starttrack*18*256, SEEK_SET);
			for (i=0; i<length; i++) {
				fread(&byte,1,1,fpdisk);
				if (feof(fpdisk)) {
					fout
						exit(1);
				}

				fwrite(&byte,1,1,fpdfs);
			}
			fclose(fpdfs);
		}
	}
	printf("\n");
}
