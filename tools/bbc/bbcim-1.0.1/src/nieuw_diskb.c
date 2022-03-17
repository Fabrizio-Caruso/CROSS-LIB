/* nieuw_diskb.c, Copyright (c) 1996-2014 W.H. Scholten
 *
 * Deel van bbcim.
 */


void new_image(char *disk, unsigned int size) {
	FILE *fpdisk;
	unsigned char byte;
	long i;

	size &=0xFFF;
	/* not allowed in true DFS but might be useful for big disks in beebem. */

	fpdisk=fopen(disk,"wb");
	byte=0;

	for (i=0; i<512; i++)
		fwrite(&byte,1,1,fpdisk);

	/* Set default disk size to 80 track */
	byte=(size>>8);
	fseek(fpdisk,256+6L,SEEK_SET);
	byte |=3<<4;
	fwrite(&byte,1,1,fpdisk);  /*i.e. bootoption=*EXEC, sect on disk='size' */
	byte=(size & 0xFF);
	fwrite(&byte,1,1,fpdisk);

	/* Netjes afsluiten*/
	fclose(fpdisk);
}
