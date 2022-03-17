/* minimaliseer.c, Copyright (C) W.H.Scholten 1996-2014, 2017
 *
 * minimse a bbc disk image (single density)
 * first version: 17-4-1996 for bbcim versie >=0.61
 */

void minimise_image(char *disk) {
	FILE *fpdisk, *fpdisk2;
	char backupnaam[256];
	char bbcfile_naam[12];
	unsigned char byte, H, eellddss;


	long startsector, file;
	long startsector2=2;
	unsigned char files;
	long length;
	int  i;




	fpdisk=fopen(disk, "rb");

	fseek(fpdisk,256L+5L,SEEK_SET);

	fread(&files,1,1,fpdisk);

/*
// int rfiles=files/8;
// #ifdef MESSAGES_NL
//  printf("aantal bestanden: %d\n",rfiles);
// #else
//  printf("number of files: %d\n",rfiles);
// #endif
*/

	if (files>0){
		fclose(fpdisk);
		strcpy(backupnaam, disk);strcat(backupnaam,"~");

		remove(backupnaam);
		rename(disk,backupnaam);

		fpdisk=fopen(backupnaam,"rb");
		fpdisk2=fopen(disk,"wb");


		/* Catalogus kopieeren */
		for(i=0;i<512;i++){
			fread (&byte,1,1,fpdisk);
			fwrite (&byte,1,1,fpdisk2);
		}


		/* Hoofdlus: alle bestanden opzoeken. */
		for(file=files;file>0; file -=8){

			/* The following is only needed for filename printing */
			fseek(fpdisk,file+15L,SEEK_SET);
			fread(&byte,1,1,fpdisk);
			bbcfile_naam[0]=(byte & 0x7F);
			bbcfile_naam[1]='.';
    
			fseek(fpdisk,file,SEEK_SET);
			for(i=0; i<7; i++){
				fread(&byte,1,1,fpdisk);
				bbcfile_naam[i+2]=byte;
			}

/*          fread(bbcfile_naam+2,1,7,fpdisk);*/

			bbcfile_naam[9]=0;


			/* File lengte */
			fseek(fpdisk,file+256L+4L,0);
			fread(&byte,1,1,fpdisk);
			fread(&H,1,1,fpdisk);
			fread(&eellddss,1,1,fpdisk);
			length=H*256L+byte+(eellddss & 0x30)*16L*256L;

			/* Start sector */
			fread(&byte,1,1,fpdisk);
			startsector=byte+(eellddss & 3)*256L;




			/* nieuwe startsector */
			fseek(fpdisk2,256+file+6,SEEK_SET);
			eellddss =(eellddss & 0xFC)+(startsector2>>8);
			fwrite(&eellddss,1,1,fpdisk2);
			byte=startsector2 & 0xFF;
			fwrite(&byte,1,1,fpdisk2);

			printf("startsector %3d > %3d\n", (int) startsector, (int) startsector2);


			fseek(fpdisk,startsector*256, SEEK_SET);
			fseek(fpdisk2,startsector2*256, SEEK_SET);
			for (i=0; i<((length+255)>>8)*256; i++){
				fread(&byte,1,1,fpdisk);
				if (feof(fpdisk) && (i<length)){
#ifdef MESSAGES_NL
					printf("onverwacht einde van het diskbeeld" \
						   " bij bestand %s\n", bbcfile_naam);
#else
					printf("unexpected end of the disk image" \
						   " at file %s\n", bbcfile_naam);
#endif
					exit(1);
				}
				fwrite(&byte,1,1,fpdisk2);
			}
			startsector2=startsector2+((length+255)>>8);

		} /* FOR file */

	}/* if */
}
