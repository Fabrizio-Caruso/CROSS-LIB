/*
 * ImageDisk Analyser
 *
 * Analyses a disk image and recommends drive/options to recreate.
 *
 * This program is compiled using my own development tools, and will not
 * build under mainstream compilers without significant work. It is being
 * provided for informational purposes only, and I provide no support for
 * it, technical or otherwise.
 *
 * Copyright 2012 Dave Dunfield
 * All rights reserved.
 *
 * For the record: I am retaining copyright on this code, for the purpose
 * of keeping a say in it's disposition however I encourage the use of ideas,
 * algorithms and code fragments contained herein to be used in the creation
 * of compatible programs on other platforms (eg: Linux).
 */
#include <stdio.h>
#include "version.h"

#define	B500	62500		// 500,000 / 8
#define	B300	37500		// 300,000 / 8
#define	B250	31250		// 200,000 / 8
#define	SOVH	85			// Guesstimate of sector overhead
#define	COVH	85			// Guesstimate of cylinder overhead

unsigned
	Ssize,					// Decoded sector size
	Dsize,					// Data size
	Hsize,					// Highest data size
	Ntop;					// Top of note list
FILE
	*Fp;					// General file pointer
unsigned char
	*Ptr,					// General pointer
	*Fptr,					// Filename pointer
	Notes[8],				// Notes
	Filename[65],			// Input filename
	Verbose = 255,			// Verbose output
	// Statistics
	Modes,					// Modes user
	Hcyl,					// Highest cylinder
	Hhead,					// Highest head
	// Data read from disk
	Mode,					// Track mode
	Cyl,					// Track cylinder
	Head,					// Track head
	Nsec,					// Track #sectors
	Ssiz,					// Sector size
	Nmap[256],				// Sector Numbering map
	Cmap[256],				// Sector Cylinder map
	Hmap[256],				// Sector Head map
	Buffer[8192];			// General buffer

unsigned char Sig[] = { 'I', 'M', 'D' };

unsigned char *Note_text[] = {
"40 track image will use only first 1/2 of 80-track drive.\n",
"77 track image likely from 8\" drive.\n",
"Should fit on 360rpm drive, 300rpm drive will leave long end gap.\n",
""
};

unsigned char Help[] = { "Copyright "#CYEAR" Dave Dunfield - All rights reserved.\n\
\nUse: IMDA image[.IMD] [options]\n\n\
opts:	/Q		- Quiet: inhibit informational output\n" };

// Drive descriptor flags
#define	D38		0x0000	// 3.5" DD 80-track
#define	H38		0x0001	// 3.5" HD 80-track
#define	D54		0x0002	// 5.25" DD 40-track
#define	D58		0x0003	// 5.25" DD 80-track
#define	H58		0x0004	// 5.25" HD 80-track
#define	D8		0x0005	// 8" 77-track
#define	DSTEP	0x0010	// Double-stepping
#define	T32		0x0020	// Translate 300->250
#define	T23		0x0040	// Translate 250->300
#define	NOT1	0x0100	// fits on 40 tracks
#define	NOT2	0x0200	// 77 track drive
#define	NOT3	0x0400	// fits on a 360

// Display drive type and applicable options
void Drive(unsigned f)
{
	unsigned i, j;

	putc('\n', stdout);
	switch(f & 7) {
	case 0 : printf(" 3.5\" DD 80-track");	break;
	case 1 : printf(" 3.5\" HD 80-track");	break;
	case 2 : printf(" 5.25\" DD 40-track");	break;
	case 3 : printf(" 5.25\" QD 80-track");	break;
	case 4 : printf(" 5.25\" HD 80-track");	break;
	case 5 : printf(" 8\" 77-track");			}
	if(f & 0xFF00) {
		printf("   NOTE:", f);
		for(i=1; i < 8; ++i) {
			if((0x80 << i) & f) {
				for(j=0; j < Ntop; ++j) {
					if(Notes[j] == i)
						goto oknote; }
				Notes[Ntop++] = i;
		oknote:	printf(" *%u", j+1); } } }

	putc('\n', stdout);
	printf("   Double-step: %s\n", (f & DSTEP) ? "ON" : "OFF");
	if(f & T32) printf("   300 kbps -> 250 kbps\n");
	if(f & T23) printf("   250 kbps -> 300 kbps\n");
}

// Generate a filename with optional extension
int filename(unsigned char *d, unsigned char *s, unsigned char *e)
{
	unsigned char f;
	f = 0;
	for(;;) switch(*d++ = toupper(*s++)) {
		case '.' : f = 255;				continue;
		case ':' :
		case '\\': f = 0;	Fptr = d;	continue;
		case 0 :
			if(!f)
				strcpy(d-1, e);
			return; }
}

/*
 * Main program
 */
main(int argc, char *argv[])
{
	unsigned i, j, k;

	fputs("IMageDisk Analyzer "#VERSION" / "#__DATE__"\n", stdout);

	// Process command line
	for(i=1; i < argc; ++i) {
		Ptr = argv[i];
		switch((toupper(*Ptr++)<<8) | toupper(*Ptr++)) {
		case '-Q' :
		case '/Q' : Verbose = 0;		continue;
		} if(Fp)
			abort(Help);
		filename(Fptr = Filename, argv[1], ".IMD");
		Fp = fopen(Filename, "rvqb"); }

	if(!Fp)
		abort(Help);

	// Skip & display IMD comment
	j = 0;
	while((i = getc(Fp)) != 0x1A) {
		if(i == EOF)
			abort("EOF on input\n");
		if(j >= sizeof(Buffer))
			abort("Comment too large\n");
		Buffer[j++] = i; }
	Buffer[j] = 0;
	if(Verbose)
		fputs(Buffer, stdout);

	// Process track records
	for(;;) {
		// Read in sector information
		if((Mode = getc(Fp)) == EOF)
			break;
		if(Mode > 0x05)
			abort("Bad MODE value");
		if((Cyl = getc(Fp)) == EOF)
			abort("EOF on Cylinder\n");
		if((Head = getc(Fp)) == EOF)
			abort("EOF on Head\n");
		if((Nsec = getc(Fp)) == EOF)
			abort("EOF on #sectors\n");
		if((Ssiz = getc(Fp)) == EOF)
			abort("EOF on Ssize\n");
		if(Ssiz > 0x06)
			abort("Bad Ssize value\n");
		Ssize = 0x80 << Ssiz;
		Dsize = ((Ssize+SOVH) * Nsec) + COVH;
		if(fget(Nmap, Nsec, Fp) != Nsec)	// Skip numbering map
			abort("EOF on SNmap\n");
		if(Head & 0x80) {					// Skip cylinder map if present
			if(fget(Cmap, Nsec, Fp) != Nsec)
				abort("EOF in SCmap\n"); }
		if(Head & 0x40) {					// Skip head map if present
			if(fget(Hmap, Nsec, Fp) != Nsec)
				abort("EOF in SHmap\n"); }
		for(i=0; i < Nsec; ++i) {			// Skip data records
			if(j = getc(Fp)) {
				k = ((j-1) & 1) ? 1 : Ssize;
				if(fget(Buffer, k, Fp) != k)
					abort("EOF in data\n"); } }
		Modes |= 1 << (Mode % 3);			// Keep all modes
		if(Cyl > Hcyl)						// Track highest cylinder
			Hcyl = Cyl;
		if((Head & 0x3F) > Hhead)			// Track highest head
			Hhead = Head & 0x3F;
		if(Dsize > Hsize)					// Track highest data size
			Hsize = Dsize;
	}
	fclose(Fp);
	++Hcyl;
	if(Verbose) {		// Display summary
		printf("Required cylinders: %u\n", Hcyl);
		printf("Required heads    : %u\n", Hhead+1);
		printf("Data rate         :");
		if(Modes & 4) printf(" 250kbps");
		if(Modes & 2) printf(" 300kbps");
		if(Modes & 1) printf(" 500kbps");
		printf("\nEst. maximum track: %u bytes\n", Hsize); }
	switch(Modes) {
	default: abort("Mixed data rates - unknown drive\n");
	case 0x00: abort("Appears to contain no data\n");
	case 0x04:
	case 0x02:
	case 0x01: }

	// Display drives and options that seem appropriate
	printf("\nPossible drives/options to write %s :\n", Fptr);
	i = (Hcyl <= 40) ? DSTEP : 0;
	switch(Modes) {
	case 0x01:	// 500kbps
		j = (Hsize < (B500/6)) ? NOT3 : 0;
		k = (Hcyl == 77) ? NOT2 : 0;
		Drive(H38|j|k);
		if(Hsize < (B500/6)) {
			Drive(H58|k);
			if(Hcyl <= 77)
				Drive(D8); }
		break;
	case 0x02:	// 300kbps
		j = i ? NOT1 : 0;
		if(!i)
			Drive(D54|T32);
		Drive((D58|T32)|i);
		Drive(H58|i);
		Drive((D38|T32)|j);
		Drive((H38|T32)|j);
		break;
	case 0x04:	// 250kbps
		j = i ? NOT1 : 0;
		if(i)
			Drive(D54);
		Drive(D58|i);
		Drive((H58|i)|T23);
		Drive(D38|j);
		Drive(H38|j);
	}
	if(Ntop)
		putc('\n', stdout);
	for(i=0; i < Ntop; ++i) {
		printf("*%u ", i+1);
		fputs(Note_text[Notes[i]-1], stdout); }
}
