/* diskb_del.c, Copyright (C) W.H.Scholten 1996-2014
 *
 * Remove a file from a disk image
 *
 * 16-4-1996
 * For bbcim version >=0.60
 */


void del_from_image(int argc, char *argv[], int options) {
	FILE *fpdisk;
	char bbcfile_naam[15];
	unsigned char byte;
	unsigned char files;
	int  rfiles;
	long file,k;
	int  locked;
	int  removed;
	int  i,j;
	char testnaam[15];


	/* DEL moet met lijst */
	if ((argc-options)<2) {
#ifdef MESSAGES_NL
		printf("niet voldoende argumenten\n");
#else
		printf("Not enough arguments (interactive mode not finished for -d)\n");
#endif
		exit(1);
	}
/*   #ifdef MESSAGES_NL
	 printf("Naam van het bbc diskbeeld?");
	 #else
	 printf("Name of the bbc disk image?");
	 #endif
	 scanf("%50s", disk);
	 } else {strcpy(disk, argv[1+options]); options++;}
*/

	fpdisk=fopen(argv[1+options],"rb+");


	if (fpdisk==NULL) {
#ifdef MESSAGES_NL
		printf("Bestand %s is niet te openen\n",argv[1+options]);
#else
		printf("File %s cannot be opened\n",argv[1+options]);
#endif
		exit(1);
	}



	/*Doe alleen iets als het diskbeeld meer dan 2 sectoren bevat. */
	fseek(fpdisk,0L, SEEK_END);
	if (ftell(fpdisk)<512) {
#ifdef MESSAGES_NL
		printf("niets op het diskbeeld\n");
#else
		printf("Nothing on the disk image\n");
#endif
		exit(1);
	}



	fseek(fpdisk,256+5L,SEEK_SET);
	fread(&files,1,1,fpdisk);

	rfiles=files/8;
#ifdef MESSAGES_NL
	printf("aantal bestanden: %d\n",rfiles);
#else
	printf("number of files: %d\n",rfiles);
#endif

	removed=0;

	/*Hoofdlus: alle bestanden opzoeken. */
	if (files>0) {

/* EVT om backup te maken:
//    {fclose(fpdisk);
//     strcpy(backupnaam, disk);strcat(backupnaam,"%");

//     remove(backupnaam);
//     rename(disk,backupnaam);

//     fpdisk=fopen(backupnaam,"r");
//     fpdisk2=fopen(disk,"w");
*/



		for(file=0;file<files; file +=8) {
			fseek(fpdisk,file+15L,SEEK_SET);
			fread(&byte,1,1,fpdisk);

			locked=byte >>7;

			bbcfile_naam[0]=(byte & 0x7F);
			bbcfile_naam[1]='.';    

			fseek(fpdisk,file+8L,SEEK_SET);
/*    for(i=0; i<7; i++) {
	  fread(&byte,1,1,fpdisk);
	  bbcfile_naam[i+2]=byte;
      }
*/
			fread(bbcfile_naam+2,1,7,fpdisk);

			bbcfile_naam[9]=0;


			/*spaties aan het einde van extract weghalen: geeft anders bestands namen
			  met spaties aan het einde */
			for(i=8; i>0 && (bbcfile_naam[i]==' '); i--);
			bbcfile_naam[i+1]=0;

			for (j=options+1; j<argc; j++) {
				if (argv[j][1]=='.')
					strncpy(testnaam, argv[j],9);
				else {
					testnaam[0]='$'; testnaam[1]='.';
					strncpy(testnaam+2, argv[j],7);
				}
				testnaam[9]=0;

				if (!strcmp(bbcfile_naam, testnaam)) {
					for (k=(file+8)+8; k<256; k++) {
						fseek(fpdisk,k, SEEK_SET);
						fread(&byte,1,1,fpdisk);
						fseek(fpdisk,k-8L, SEEK_SET);
						fwrite(&byte,1,1,fpdisk);

						fseek(fpdisk,k+256L, SEEK_SET);
						fread(&byte,1,1,fpdisk);
						fseek(fpdisk,k-8L+256L, SEEK_SET);
						fwrite(&byte,1,1,fpdisk);
					}
#ifdef MESSAGES_NL
					printf("Bestand %s verwijderd\n",testnaam);
#else
					printf("File %s removed\n",testnaam);
#endif
					removed++;
/*          continue;*/
					break;
				}
			}
		}/*for*/

		files -=removed*8;
		fseek(fpdisk,256+5L,SEEK_SET);
		fwrite(&files,1,1,fpdisk);

		fclose(fpdisk);

#ifdef MESSAGES_NL
		printf("  %d bestand%s verwijderd\n", removed, (removed!=1) ? "en" : "");
#else
		printf("  %d file%s removed\n", removed, (removed!=1) ? "s" : "");
#endif

	}/*if files>0 */
}
