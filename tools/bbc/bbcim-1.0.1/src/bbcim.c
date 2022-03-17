/*
// bbcim.c (bbc disk image manipulation):
//
//    Diskimage manipulation for single density Acorn DFS, Watford DFS 62 file,
//    Double density extensions and much more.
//
// Copyright (c) W.H. Scholten 1996, 1997, 2009, 2014, 2017
//
// Permission to use, copy, modify, distribute, and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear in
// supporting documentation, and that the name of the copyright holder
// not be used in advertising or publicity pertaining to distribution
// of the software without specific, written prior permission. The
// copyright holder makes no representations about the suitability of
// this software for any purpose. It is provided "as is" without express
// or implied warranty.
//
// THE COPYRIGHT HOLDER DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
// SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS, IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
// SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
// RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
// CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
// CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
//
//
// Send comments and bug-reports to:
//
//    swhs-web@xs4all.nl
//
*/

/* Overview of changes in bbcim:
   The first bbcim version was 0.60 (I think) in 1996, a combination of seperate programs I put on my webpages
   in 1995 and 1996 (my website was at that time at: http://www.cistron.nl/~wouters/)

   Changes in 0.83 (from 0.70):
   +  xcrc implemented
   +  -ed may work on a mac (LET ME KNOW): uncomment #define MAC
   +  -e options now in any order
   +  -e, -x and -y: if a file exists, you can choose whether to overwrite or rename.
   c  seeks immediately before read/write in add-loop, otherwise segmentation faults sometimes (bug in GCC?).
   c  fixed small mistake in xcrc.

   Changes in 0.91 (from 0.83):
   +  'use' added.
   +  -s file overwrite choice.
   +  -e file {list}

   c  -x : rename file to itself fixed (no overwrite question)
   c  -xcrc: filenames < 7 char's now OK.
   c  -s : L before load address (as in xbeeb's __CATALOG__) is now acceptable.

   Changes in 0.92:
   +  -ab : adds bare files, otherwise info file is used by default.
   c : -e; b->s, info file is now added by default, b=bare.

   Changes in 0.93-beta (9-3-97):
   + code cleaned up with explicit type conversions
   + added code to replace characters in filenames (usually not needed in linux); e.g. '/' gives problems.
   + Help in english

   Changes in 0.94-beta (16-3-97):
   c w62 conversion now included in bbcim.


   Changes in 0.95-beta (30-6-97 - 4-7-97):
   c w62 conversion: if there are less than 32 files, only one diskimage is made.
   c disknames without a . didn't extract with -ed (as the dir name was equal to the diskimage name). Added a rename.
   c cleaned up 'mkdir' selection. You now have 3 choices.
   + Config additions: bbcim now compiles on other OS'es without changes.
   c bugfix: -s# works again (no idea when it got broken).


   Bugs and 'features':
   - several files in a diskimage can have the same name (can be useful)
   (i.e. -a doesn't overwrite)
   - -e: name checking in diskimage is case dependant.

   If in doubt: it's not a bug, it is a feature!

   0.95-beta6 (20-4-2009):
   c various message/documentation cleanups and references to the new webpages, some fixes for newer compilers, etc.:
   + Makefile added
   c naming scheme as in bbcim 0.105. No more removal of a file extension to be replaced by a new one, which was
     useful for DOS. E.g. 'bbcim -ed alpha.ssd' puts files in a directory 'alpha.ssd.d/'. bbcim -e alpha.ssd gives names
     such as alpha.ssd.$.!BOOT
   c source code indentation cleanups (from old gnu style 2-spaces to 1 tab) and some message changes.

   0.95-beta7 (21-4-2009):
   c -splitds checks for the correct density (sectors/track) argument.

   0.95-beta8 (3-5-2009):
   c small improvements to the man page and cleanup of the source code (all re-indented with tabs).
   c Makefile target 'install' also includes 'all' so 'make install' works if you haven't done a 'make' first.

   0.95-beta9 (6-5-2009):
   c small source cleanups.

   0.95-beta10 (14-5-2009):
   c a few changes using string literals in fprintf as suggested by clang's output.

   0.95-beta11 (21-5-2009):
   c a few changes in the Makefile. Installation locations are now local in $(HOME)/bin/ and $(HOME)/man/man1/

   1.0 (26-1-2014):
   c Put the source code files into a directory src/
   c Added a note about the experimental branch (renamed the latest version at this time from 0.107 to 1.1) in the man page.
   c Changed the define NL to MESSAGES_NL in case you want the text output in Dutch.
   c Licence is now MIT type, so basically you can do what you want with the software. See doc/LICENCE.
   c Cleaned up some of the comments in the code and translated a few from Dutch to English.
   c Before an error print the BBCIM version number and website.
   c Changed the version number from 0.95b11 to 1.0

   1.0.1 (16-4-2017):
   c support long filenames (up to 255 chars)
*/


#define VERSION "1.0.1 (16 April 2017)"



/*************** CONFIGURATION *********************/
/* Set this define for Dutch messages: */
/*#define MESSAGES_NL*/

#define UNIX_MKDIR

/* Make directory: UNIX_MKDIR, MAC_MKDIR, or SYSTEM_MKDIR (using system("mkdir.."); ) */


/* #define NO_COMMAND_LINE */
/* If you can't give parameters on the command line use this (this was meant, in 1997) for Macs. However
   now that Macs use a unix-like system with terminals available, this may be useless. */


/* Characters to be replaced in filenames (on the host system, not the original BBC filenames in the .inf files):
   Most filing systems can't handle all characters from bbc filenames (esp. '/' and '\'). Replacing them
   and a few more to make it easier seems prudent :)

   Add whatever you need for your system.
*/

char filenamechar_bad[]     = { '\\', '/', ':', '*', '?', '\"', '<', '>', '|', '`', '\'' };
char filenamechar_replace[] = { '_' , '_', '_', '_', '_', '_' , '_', '_', '_', '_', '_'  };



/************ END OF CONFIGURATION ***********************/





#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef MAC_MKDIR
#include <unix.h>
#endif

#ifdef UNIX_MKDIR
#include <sys/stat.h>
// #include <fcntl.h>
// #include <unistd.h>
#endif

#ifdef SYSTEM_MKDIR
#endif

/* Should use this in various places:
   #include <sys/stat.h>
   struct stat file_stat;
   if (stat(filename, file_stat)<0) perror("can't open file\n");
*/



/* Defines */
/* Byte offsets in disk image */
#define DFStitle0_7    0
#define DFStitle8_11   256
#define DFSaccessct    (256+4)
#define DFSentries     (256+5)
#define DFSbootsec     (256+6)
#define DFSsectondiskL (256+7)

/*next offsets +n*8 for each file */
#define DFSfilename    8
#define DFSdirectory   0x0f
#define DFSloadaddrL   (256+8)
#define DFSloadaddrM   (256+9)
#define DFSexecaddrL   (256+0x0a)
#define DFSexecaddrM   (256+0x0b)
#define DFSlengthL     (256+0x0c)
#define DFSlengthM     (256+0x0d)
#define DFSeellddss    (256+0x0e)
#define DFSstartsecL   (256+0x0f)






/* Prototypes: */
int  rename_sequential(char *naam);
int  overschrijf_vraag(char *oude_naam, char *nieuwe_naam);


void cleanup(int argc, char *argv[], int options);
unsigned int fcrc(char *bestand);
void icrc(int argc, char *argv[], int options);
void xcrc(int argc, char *argv[], int options);

void ddos2dfs(int argc, char *argv[], int options);
void add_to_image(char *disk, int options, int argc, char *argv[], int remove_file, int use_infofiles);
void del_from_image(int argc, char *argv[], int options);
void expand_image(char *disk, int expand);
void interss(int argc, char *argv[], int options);
void splitds(int argc, char *argv[], int options);
void minimise_image(char *disk);
void new_image(char *disk, unsigned int size);
void split_cat(int argc, char *argv[],int options, char *optread_line);
int  w62dfs(int argc, char *argv[], int options);
void xbeeb(int argc, char *argv[], int options);


/* NB invoer en bbcnaam mogen niet zelfde string zijn */
/*
  void invoer_naar_bbcnaam(char *invoer, char *bbcnaam) {
  if (invoer[1]=='.')
  strncpy(bbcnaam, invoer,9);
  else {
  bbcnaam[0]='$'; bbcnaam[1]='.'; strncpy(bbcnaam+2, invoer, 7);
  }
  bbcnaam[9]=0;
  }
*/

void reduceer_unix_naam(char *unixnaam, char *bbcnaam);


void reduceer_unix_naam(char *unixnaam, char *bbcnaam) {
	if (bbcnaam[0]=='$')
		strcpy(unixnaam,bbcnaam+2);
	else {
		unixnaam[0]=bbcnaam[0];
		strcpy(unixnaam+1,bbcnaam+2);
	}
}



#include "overschrijf.c"

#include "nieuw_diskb.c"
#include "diskb_add.c"
#include "diskb_del.c"
#include "expand_im.c"
#include "minimaliseer.c"

#include "intersplits.c"
#include "ddos2dfs.c"
#include "w62.c"

#include "splits_cat.c"
#include "bbc_crc.c"
#include "xbeeb.c"
#include "archief.c"


int  filenamechar_no=sizeof(filenamechar_bad)/sizeof(char);	/* number of char's to be checked for replacement */

void print_version(void);
void use(void);




void print_version(void) {
	printf("\nbbcim "VERSION" (C) W.H.Scholten 1996-2014 (http://wouter.bbcmicro.net/)\n");
}


void use(void) {
	print_version();
#ifdef MESSAGES_NL
	printf("Opties: -e[s/d,#,b,r] -a[b,r] -d -c -crc -min -40/80/max\n" \
		   "        -y -icrc -s[#]\n" \
		   "        -x -xcrc\n" \
		   "        -splitds -interss -ddos2dfs -w62dfs\n" \
		   "        -V -H\n"
		   "Doe bbcim -H voor nadere informatie of zie de 'man' pagina\n"
		);
#else
	printf("Options: -e[s/d,#,b,r] -a[b,r] -d -c -crc -min -40/80/max\n" \
		   "        -y -icrc -s[#]\n" \
		   "        -x -xcrc\n" \
		   "        -splitds -interss -ddos2dfs -w62dfs\n" \
		   "        -V -H\n"
		   "Try bbcim -H for more information or see the man page\n"
		);
#endif
}



int main(int argc, char *argv[]) {
	FILE *fpdisk, *fpcat, *fpextract, *fpinfo;
	char disk[256], catalogus[256], extract[256], info_naam[256], basisnaam[256];
	char bbcdisk_naam[15], bbcfile_naam[15];
	char unix_bbcfile_naam[15];
	char testnaam[15];
	char optstring[20];

	unsigned char byte,H;

	long filesizesum=0;

	int extr=0, extr_dir=0, cat=0, expand=0, min=0, bcrc=0, short_name=0;
	int add=0, info=0 /* don't use info files */, no_bbc_dir=0, remove_file=0, remove_disk=0;
	int extr_list=0;

	int bad_option=1;
	int options=1;

	int i, j, rfiles, bootoption, antwoord, k;
	unsigned char files=0;
	unsigned int sectorsondisk;
	long file;
	int locked;
	unsigned char eellddss; 

	long startsector;
	unsigned long loadaddress, execaddress, length;

	unsigned int crc;

#ifdef UNIX_MKDIR
	mode_t dir_mode;
#endif

#if defined(MAC_MKDIR) || defined(SYSTEM_MKDIR)
	char actie[256];
#endif

#ifdef NO_COMMAND_LINE
	argc=1;
#endif


	if (argc < 2) {
		char optstring2 [ 21 ];
		print_version();

#ifdef MESSAGES_NL
		printf("Interactive modus (meer informatie over de opties: bbcim -H of zie de man pagina)\n");
		printf("Welke optie?");
#else
		printf("Interactive mode (for more information on the options: bbcim -H or see the man page)\n");
		printf("Which option?");
#endif
		scanf("%20s", optstring2);
		if (optstring2[0]=='-') strcpy(optstring, optstring2);
		else { strcpy(optstring, "-"); strcat(optstring, optstring2); }

		options=0;
	}
	else strcpy(optstring, argv[1]);



	/* 1e selectie */
	if (!strcmp(optstring,"-V")){
		print_version();
		exit(0);
	}

	if (!strcmp(optstring,"-H")) {
#define CONSOLE_INVERT        "\033[7m"
#define CONSOLE_RESTORE       "\033[m"
#ifdef MESSAGES_NL
		printf("bbcim "VERSION"\nCommando overzicht:\n" \
			   " -c <diskbeeld> : toon bestanden in diskbeeld\n" \

			   " -e[s/d,#,b,r] <diskbeeld> [{lijst}]: bestanden uit diskbeeld halen\n" \
			   " -a[b,r] <diskbeeld> {lijst} : bestanden in diskbeeld plaatsen\n" \
			   " -d <diskbeeld> {lijst} : verwijder bestanden uit diskbeeld\n" \
			   " -40/-80/-max <diskbeeld> : wijzig grootte van diskbeeld\n" \
			   " -min <diskbeeld> : minimaliseer diskbeeld\n" \
			   " -crc <diskbeeld> : bereken CRC's van bestanden in diskbeeld\n" \

			   " -y {lijst} : archief bestanden bijwerken\n" \
			   " -s[#] <bestand> : splits DFS catalogue-achtig tekst bestand in .inf bestanden\n" \
			   " -x {lijst} : archief bestanden omzetten naar xbeeb formaat\n" \

			   " -icrc {lijst} : controleer CRC's van archief bestanden\n"\
			   " -xcrc : controleer CRC's in xbeeb dir (de huidige dir)\n"\

			   " -interss (sd/dd) <B0> <B1> <B2> : combineer 2 enkelzijdige diskbeelden to 1 dubbelzijdig diskbeeld (B0+B1 -> B2)\n"\
			   " -splitds (sd/dd) <B> : splits een dubbelzijdig diskbeeld naar 2 enkelzijdige diskbeelden\n"\

			   " -ddos2dfs <ddos diskbeeld> : splits enkelzijdig DDOS diskbeeld in DFS diskbeelden\n"\
			   " -w62dfs <watford 62 bestands beeld> : splits een watford 62 bestanden diskbeeld in 2 DFS diskbeelden\n"\

			   " -V : toon versie\n"
			   CONSOLE_INVERT"Zie http://wouter.bbcmicro.net/ voor de nieuwste versie van bbcim.\n"CONSOLE_RESTORE
			);
#else
		printf("bbcim "VERSION"\nCommand overview:\n" \
			   " -c <disk image> : show files in disk image\n" \

			   " -e[s/d,#,b,r] <disk image> [{list}]: extract files from disk image\n" \
			   " -a[b,r] <disk image> {list} : put files in disk image\n" \
			   " -d <disk image> {list} : remove files from disk image\n" \
			   " -40/-80/-max <disk image> : change size of disk image\n" \
			   " -min <disk image> : minimise disk image\n" \
			   " -crc <disk image> : calculate CRC's of files in disk image\n" \

			   " -y {list} : cleanup archive file information\n" \
			   " -s[#] <file> : split a DFS catalogue-like text file in .inf files\n" \
			   " -x {list} : convert archive files to xbeeb format\n" \

			   " -icrc {list} : check CRC's of archive files\n"\
			   " -xcrc : check CRC's in xbeeb dir (the current dir)\n"\

			   " -interss (sd/dd) B0 B1 B2 : combine 2 single sided disk images to a double sided disk image (B0+B1 -> B2)\n"\
			   " -splitds (sd/dd) B : split a double sided disk image into 2 single sided disk images\n"\

			   " -ddos2dfs <ddos image> : split a single sided DDOS disk image into DFS disk images\n"\
			   " -w62dfs <watford 62 file image> : split a watford 62 file disk image into 2 DFS disk images\n"\

			   " -V : show version\n"
			   CONSOLE_INVERT"See http://wouter.bbcmicro.net/ for the latest version of bbcim.\n"CONSOLE_RESTORE
	 );
#endif
		exit(0);
	}

	if (!strcmp(optstring,"-interss")) {interss(argc, argv, options); exit(0);}
	if (!strcmp(optstring,"-splitds")) {splitds(argc, argv, options); exit(0);}
	if (!strcmp(optstring,"-ddos2dfs")) {ddos2dfs(argc, argv, options); exit(0);}
	if (!strcmp(optstring,"-w62dfs")) {w62dfs(argc, argv, options); exit(0);}
	if (!strcmp(optstring,"-d")) {del_from_image(argc, argv, options); exit(0);}
	if (!strncmp(optstring,"-s",2)) {split_cat(argc, argv, options, optstring); exit(0);}
	if (!strcmp(optstring,"-x")) {xbeeb(argc, argv, options); exit(0);}
	if (!strcmp(optstring,"-y")) {cleanup(argc, argv, options); exit(0);}
	if (!strcmp(optstring,"-icrc")) {icrc(argc, argv, options); exit(0);}
	if (!strcmp(optstring,"-xcrc")) {xcrc(argc, argv, options); exit(0);}




	/* Bekijk nu de resterende opties */
	if (!strcmp(optstring,"-c")) {
		cat=1;
		bad_option=0;
	}

	if (!strncmp(optstring,"-e", 2)) {
		extr=1;
		bad_option=0;
		info=1;
		for (i=2; i<strlen(optstring); i++) {
			switch (optstring[i]) {
			case 'd' : extr_dir=1; break;
			case 's' : short_name=1; break;
			case 'b' : info=0; break;
			case '#' : no_bbc_dir=1; break;
			case 'r' : remove_disk=1; break;
			default : bad_option=1;
			}
			if (short_name & extr_dir) {
#ifdef MESSAGES_NL
				fprintf(stderr, "sd = slechte optie\n");
#else
				fprintf(stderr, "sd = bad option\n");
#endif
				exit(1);
			}
		} /*for*/
	}


	if (!strcmp(optstring,"-crc")) {bcrc=1; cat=1; bad_option=0;}
	if (!strcmp(optstring,"-40"))  {expand=40*10; bad_option=0;}
	if (!strcmp(optstring,"-80"))  {expand=80*10; bad_option=0;}
	if (!strcmp(optstring,"-max")) {expand=1023;  bad_option=0;}
	if (!strcmp(optstring,"-min")) {min=1; 	bad_option=0;}

	if (!strncmp(optstring,"-a", 2)) {
		add=1;
		bad_option=0;
		info=1;
		for (i=2; i<strlen(optstring); i++) {
			switch (optstring[i]) {
			case 'r' : remove_file=1; break;
			case 'b' : info=0; break;
			default  : bad_option=1;
			}
		}
	}


	if (!strcmp(optstring,"-new")) {
		bad_option=0;
	}

	/* Einde opties */

	if (bad_option) {
#ifdef MESSAGES_NL
		printf("Slechte optie\n");
#else
		printf("Bad option\n");
#endif
		use();
		exit(1);
	}

	if ((argc-options)<2) {
#ifdef MESSAGES_NL
		printf("Naam van het bbc diskbeeld?");
#else
		printf("Name of the bbc disk image?");
#endif
		scanf("%250s", disk);
	} else {
		strncpy(disk, argv[1+options], 250);
		disk[250]=0;
	}

	fprintf(stderr, "Disk=%s\n", disk);
	/* NU UITVOEREN VAN DE OPTIE:*/

	/* Make empty disk image.................. */
	if (!strcmp(optstring,"-new")) {
		new_image(disk, 800);
		exit(0);
	}


	strcpy(basisnaam, disk);
/*	basisnaam[strcspn(disk,".")]=0; */ /* At some time this was useful for DOS, irrrelevant in 2009. */
	strcpy(catalogus, basisnaam);
	strcat(catalogus,".cat");
 


	fpdisk=fopen(disk,"rb");



	/* Add files to disk image...............
	   Only for add the disk image doesn't need to exist yet */
	if (add) {
		if (fpdisk==NULL) {
			new_image(disk, 800); /*DEFAULT size=80 track*/
#ifdef MESSAGES_NL
			printf("Nieuw diskbeeld gemaakt\n");
#else
			printf("New disk image made\n");
#endif
		}
		else fclose(fpdisk);

		add_to_image(disk, options, argc,  argv, remove_file, info);
		exit(0);
	}
	/* end of add-files.............. */



	/* For other options the disk image must exist */
	if (fpdisk==NULL) {
#ifdef MESSAGES_NL
		printf("Bestand %s is niet te openen\n",disk);
#else
		printf("File %s cannot be opened\n",disk);
#endif
		exit(1);
	}

	if (extr_dir) {
		strcat(basisnaam, ".d"); /* as in experimental branch of bbcim, 0.105 and later */

		rename_sequential(basisnaam); /* in case a file (dir too?)  already exists named basis_naam */
#ifdef MAC_MKDIR
		strcpy(actie, ":"); /* Might need to be "volume:" where volume is ? */
		strcat(actie, basisnaam);
		mkdir(actie,0);
#endif
#ifdef UNIX_MKDIR
		dir_mode=0777; /* CHANGE */
		mkdir(basisnaam, dir_mode);
#endif
#ifdef SYSTEM_MKDIR
		sprintf(actie, "mkdir %s", basisnaam);
		system(actie);
#endif
	}

#ifdef DEBUG
	if (no_bbc_dir) {
#ifdef MESSAGES_NL
		printf("geen bbc dir\n");
#else
		printf("no bbcdir\n");
#endif
	}
#endif


	/* Expand diskbeeld ................. */
	if (expand) {
		fclose(fpdisk);
		expand_image(disk,expand);
		exit(0);
	}

 


	/* CRC, CAT, Extract, Minimise ................*/

	/* Only do something if the disk image has more than 2 sectors */
	fseek(fpdisk,0L, SEEK_END);
	if (ftell(fpdisk)<512) {
#ifdef MESSAGES_NL
		printf("niets op het diskbeeld\n");
#else
		printf("nothing on the disk image\n");
#endif

		exit(1);
	}

	/* Minimise */
	if (min) {
		fclose(fpdisk); minimise_image(disk);
		exit(0);
	}


	/* Determine disk name (voor cat, crc, extract) */
	fseek(fpdisk,0L,SEEK_SET);
	for (i=0; i<8; i++) {
		fread (&byte,1,1,fpdisk);
		bbcdisk_naam[i]=byte;
		if (byte==0) break;
	}
	fseek(fpdisk,256L,SEEK_SET);
	for (i=0; i<4; i++) {
		fread (&byte,1,1,fpdisk);
		bbcdisk_naam[i+8]=byte;
		if (byte==0) break;
	}






	fseek(fpdisk,256+5L,SEEK_SET);
	fread(&files,1,1,fpdisk);


	/* Sanity check on disk image: */
	if (files % 8) {
#ifdef MESSAGES_NL
		printf("corrupt diskbeeld (bestand-aantal byte)\n");
#else
		printf("bad disk image (fileno byte)\n");
#endif
		exit(1);
	}


	rfiles=files/8;
	extr_list=(extr && (argc-options>2));


	fread(&byte,1,1,fpdisk);
	bootoption=byte >> 4;
	H=byte & 3;
	fread(&byte,1,1,fpdisk);
	sectorsondisk=byte+H*256L;


	if (extr && !extr_list) fpcat=fopen(catalogus,"w"); else fpcat=stdout;

	/*if (extr_list) fpcat=NULL;*/

	if (extr_list) goto skip_diskinfo;

#ifdef MESSAGES_NL
	fprintf(fpcat,"\nDiskette :%s",bbcdisk_naam);
	if (strlen(bbcdisk_naam)==0) fprintf(fpcat, "(geen naam)");
	fprintf(fpcat,"\n%d sectoren op de diskette\n",sectorsondisk);
#else
	fprintf(fpcat,"\nDisk :%s",bbcdisk_naam);
	if (strlen(bbcdisk_naam)==0) fprintf(fpcat, "(no name)");
	fprintf(fpcat,"\n%d sectors on disk\n",sectorsondisk);
#endif

#ifdef MESSAGES_NL
	fprintf(fpcat, "bootoptie: ");
#else
	fprintf(fpcat, "bootoption: ");
#endif

	switch(bootoption) {
	case 0 :
#ifdef MESSAGES_NL
		fprintf(fpcat, "geen");
#else
		fprintf(fpcat, "none");
#endif
		break;
	case 1 : fprintf(fpcat, "*LOAD !BOOT");break;
	case 2 : fprintf(fpcat, "*RUN !BOOT");break;
	case 3 : fprintf(fpcat, "*EXEC !BOOT");
	}
	fprintf(fpcat,"\n");

#ifdef MESSAGES_NL
	fprintf(fpcat, "Bestand    Laad  Start  Lengte Toegang startsector\n");
#else
	fprintf(fpcat, "File       Load   Exec  Length Access  startsector\n");
#endif


  skip_diskinfo:

	/* Main loop: find all files */
	if (files>0) {

		int extr_list_no=0;

		for(file=0;file<files; file +=8) {

			strcpy(extract, basisnaam);
			if (extr_dir) strcat(extract,"/"); else strcat(extract,".");

			if (short_name) extract[0]=0;

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


			/* spaties aan het einde van bbc naam weghalen: geeft anders bestands namen
			   met spaties (in linux). */
			for (i=8;i>0 && (bbcfile_naam[i]==' '); i--);
			bbcfile_naam[i+1]=0;

#if DEBUG
			printf(bbcfile_naam);
#endif

			/* If a file list is given, see if the name is in the disk image */
			if (extr_list) {
				int found=0;
				for (i=options+2; i<argc;i++) {
					if (argv[i][1]=='.')
						strncpy(testnaam, argv[i],9);
					else {
						testnaam[0]='$'; testnaam[1]='.';
						strncpy(testnaam+2, argv[i],7);
					}
					testnaam[9]=0;
					if (!strcmp(bbcfile_naam, testnaam)) found=1;
				}
				if (!found) continue;
				extr_list_no++;
			}








			/* load/exec  adressen + lengte bepalen */
			fseek(fpdisk,256+file+14,SEEK_SET);

			fread(&eellddss,1,1,fpdisk);

			/* Start sector */
			fread(&byte,1,1,fpdisk);
			startsector=byte+(eellddss & 3)*256L;

			/* Sanity check on diskimage? (check that startsec>1, but this disallows viewing dos cat of
			   vol 0A with standard dfs routines. */


			/* Load address */
			fseek(fpdisk,file+256L+8L,0);
			fread(&byte,1,1,fpdisk);
			fread(&H,1,1,fpdisk);
			loadaddress=H*256L+byte+(eellddss & 0x0C)*16384L;
			if (loadaddress & 0x30000) loadaddress |=0xFF0000;

			/* Exec address */
			fread(&byte,1,1,fpdisk);
			fread(&H,1,1,fpdisk);
			execaddress=H*256L+byte+(eellddss & 0xC0)*4L*256L;
			if (execaddress & 0x30000) execaddress |=0xFF0000;

			/* File length */
			fread(&byte,1,1,fpdisk);
			fread(&H,1,1,fpdisk);
			length=H*256L+byte+(eellddss & 0x30)*16L*256L;

			filesizesum +=length;


			/* Uitvoer van deze gegevens naar catalogus */
			if (!extr_list) {
				fprintf(fpcat,"%-9s %6lX %6lX %6lX", bbcfile_naam, loadaddress, execaddress, length);
				if (locked) fprintf(fpcat," Locked");
				else        fprintf(fpcat,"       "); /* Voor crc uitlijnen */
			}

			strcpy(unix_bbcfile_naam, bbcfile_naam);

			/* Name check: replace certain characters in filenames (most filesystems can't
			   handle all characters from bbc filenames) */
			for (i=0; i<filenamechar_no; i++) {
				for (j=0; j<strlen(unix_bbcfile_naam); j++) {
					if (unix_bbcfile_naam[j]==filenamechar_bad[i])
						unix_bbcfile_naam[j]=filenamechar_replace[i];
				}
			}



			if (no_bbc_dir){
/*      if (bbcfile_naam[0]=='$') strcat(extract, unix_bbcfile_naam+2);
		else {
		bbcfile_naam[1]=bbcfile_naam[0]; strcat(extract, unix_bbcfile_naam+1);
		}
*/
				reduceer_unix_naam(extract+strlen(extract), unix_bbcfile_naam);
			}
			else strcat(extract, unix_bbcfile_naam);

			if (extr) {

				/* Name check: does it already exist? */
				char nieuwe_naam[55];

				strcpy(nieuwe_naam, extract);
				if ((fpextract=fopen(extract, "rb"))!=NULL) {
					fclose(fpextract);
					antwoord=overschrijf_vraag(extract, nieuwe_naam);

					switch(antwoord) {
					case OVERSCHRIJVEN:
						break;
					case HERNOEM_NIEUW:
						strcpy(extract, nieuwe_naam); break;
					case HERNOEM_BESTAAND:
						rename(extract, nieuwe_naam);
						/* altijd ook info bestand hernoemen: */
						strcpy(info_naam, extract); strcat(info_naam, ".inf");
						strcat(nieuwe_naam, ".inf"); rename(info_naam, nieuwe_naam);
						break;
					case SLA_OVER:
						continue; /*to for*/
					} /*switch */
				}
				/* End of name check */


				fpextract=fopen(extract, "wb");

				fseek(fpdisk,startsector*256, SEEK_SET);
				for (i=0; i<length; i++) {
					fread(&byte,1,1,fpdisk);
					if (feof(fpdisk)) {
#ifdef MESSAGES_NL
						printf("onverwacht einde van het diskbeeld" \
							   " bij bestand %s\n", bbcfile_naam);
#else
						printf("unexpected end of the disk image" \
							   " at file %s\n", bbcfile_naam);
#endif
						exit(1);
					}
					fwrite(&byte,1,1,fpextract);
				}
				fclose(fpextract);
			}


			if (info) {
				strcpy(info_naam, extract);
				strcat(info_naam, ".inf");

				fpinfo=fopen(info_naam, "w");
				if (fpinfo ==NULL) {
					fprintf(stderr, "can't write to %s\n", info_naam);
					exit(1);
				}

				/* Evt volgende regel weg. (FILENAAM, bv $.ELITE) */
				fprintf(fpinfo, "%-9s", bbcfile_naam);

				/* I no longer include the (superfluous) file length in the INFO file. */
				fprintf(fpinfo," %6lX %6lX", loadaddress, execaddress);

				if (locked) fprintf(fpinfo," Locked");

			} /*N.B. info bestand wordt in crc deel gesloten*/


			crc=0;
			if (bcrc || info) {
				fseek(fpdisk,startsector*256, SEEK_SET);
				for (i=0; i<length; i++) {
					fread(&byte,1,1,fpdisk);
					if (feof(fpdisk)) {
#ifdef MESSAGES_NL
						printf("onverwacht einde van het diskbeeld" \
							   " bij bestand %s\n", bbcfile_naam);
#else
						printf("unexpected end of the disk image" \
							   " at file %s\n", bbcfile_naam);
#endif
						exit(1);
					}

					crc ^=(byte << 8);
					for(k=0;k<8;k++) {
						if (crc & 32768)
							crc=(((crc ^ 0x0810) & 32767) << 1)+1;
						else
							crc =crc <<1;
					}
				}

				if (info) {
					fprintf(fpinfo, " CRC=%04X", crc); fclose(fpinfo);
				}

			}

			if (extr_list)
				printf(" %s\n",extract);
			else
				fprintf(fpcat,"   %4d", (int) startsector);

			if (bcrc)
				fprintf(fpcat, "       CRC= %04X\n", crc);
			else
				fprintf(fpcat, "\n");
		}

		/* Show no of (extracted) files */
		if (extr_list){
			rfiles=extr_list_no;
#ifdef MESSAGES_NL
			printf("aantal bestanden: %d\n",rfiles);
#else
			printf("number of files: %d\n",rfiles);
#endif
		}
#ifdef MESSAGES_NL
		fprintf(fpcat,"\n%d bestand%s\n", rfiles, (rfiles !=1) ? "en" : "");
#else
		fprintf(fpcat,"\n%d file%s\n", rfiles, (rfiles !=1) ? "s" : "");
#endif


		fclose(fpdisk);
		if (remove_disk && !extr_list) remove(disk);
	}
#ifdef MESSAGES_NL
	fprintf(fpcat,"Totaal %ld bytes\n", filesizesum);
#else
	fprintf(fpcat,"Total %ld bytes\n",  filesizesum);
#endif

	return 0;
}
