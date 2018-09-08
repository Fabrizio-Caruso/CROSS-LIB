/*
 * Program to convert raw binary data into .IMD image format.
 *
 * This program is compiled using my own development tools, and will not
 * build under mainstream compilers without significant work. It is being
 * provided for informational purposes only, and I provide no support for
 * it, technical or otherwise.
 *
 * Copyright 2005-2012 Dave Dunfield
 * All rights reserved.
 *
 * For the record: I am retaining copyright on this code, however this is
 * for the purpose of keeping a say in it's disposition. I encourage the
 * use of ideas, algorithms and code fragments contained herein to be used
 * in the creation of compatible programs on other platforms (eg: Linux).
 */
#include <stdio.h>
#include "version.h"

unsigned char
	*ptr,				// General parse pointer
	*ptr1,				// General pointer
	Twoside,			// Double sided indicator
	Verbose,			// Display detail information
	Fill,				// Fill value
	Compress = 255,		// Compress sectors
	Ifile[65],			// Input filename
	Ofile[65],			// Output filename
	Ffile[200],			// Format file
	Buffer[32768];		// Comment/track buffer
unsigned
	Day,				// Current day
	Month,				// Current month
	Year,				// Current year
	Hour,				// Current hour
	Minute,				// Current minite
	Second,				// Current second
	Fmtnext = -1,		// Next format record
	Cylinder,			// Current cylinder
	Cylinders = -1,		// Number of cylinders
	Cmtsize;			// Comment size

struct SIDE {
	unsigned Mode;				// Data-rate/Density
	unsigned Ssize;				// Sector size (encoded)
	unsigned SSize;				// Sector size (plain)
	unsigned Nsec;				// Number of sectors/track
	unsigned CMsize;			// Cylinder numbering map size
	unsigned HMsize;			// Head numbering map size
	unsigned char Smap[256];	// Sector numbering map
	unsigned char Cmap[256];	// Cylinder numbering map
	unsigned char Hmap[256];	// Head numbering map
	} Side0, Side1;

FILE
	*fpi,			// Binary file in
	*fpo,			// IMD file out
	*fpf;			// Detail record fp

/*
 * Report an error
 */
register error(unsigned args)
{
	unsigned char buffer[100], *p;
	_format_(nargs() * 2 + &args, buffer);
	if(p = ptr1) {
		while(*p) {
			if(isspace(*p)) {
				*p = 0;
				break; }
			++p; }
		fputs(ptr1, stdout);
		fputs(": ", stdout); }
	fputs(buffer, stdout);
	exit(-1);
}

/*
 * Open a file with default extension
 */
void filename(unsigned char *p, unsigned char *name, unsigned char *ext)
{
	unsigned char d;
rd:	d = 255;
	for(;;) switch(*p++ = *name++) {
		case 0 :
			if(d)
				strcpy(p-1, ext);
			return;
		case ':' :
		case '\\':	goto rd;
		case '.' :	d = 0;	}
}

/*
 * Obtain a number from the command line
 */
unsigned char get_num(unsigned *value, unsigned l, unsigned h)
{
	unsigned c, v, b;
	b = 10;
	switch(*ptr) {
	case '$' : b = 16;	goto l1;
	case '@' : b = 8;	goto l1;
	case '%' : b = 1;
	l1: ++ptr; }
	v = 0;
	c = *ptr;
	for(;;) {
		if((c >= '0') && (c <= '9'))
			c -= '0';
		else if((c >= 'A') && (c <= 'F'))
			c -= ('A'-10);
		else if((c >= 'a') && (c <= 'f'))
			c -= ('a'-10);
		else
			c = 65535;
		if(c >= b)
			error("Bad digit '%c'", *ptr);
		v = (v * b) + c;
		switch(c = *++ptr) {
		case ' ' :
		case '\t' :
		case ',' :
		case '-' :
		case '.' : ++ptr;
		case 0 :
			if((v < l) || (v > h))
				error("Value %u out of range %u-%u", v, l, h);
			*value = v;
			return c; } }
}

/*
 * Check a size value and issue error if it exceeds set limit
 */
void checksize(unsigned v, unsigned s)
{
	if(v > s)
		error("Too large (exceeds %u bytes)", s);
}

/*
 * Obtain a map value from the command line
 */
void get_map(unsigned char map[], unsigned *size, unsigned l, unsigned h)
{
	unsigned c, i, j, s;
	s = 0;
top:
	c = get_num(&i, l, h);
	checksize(s, 255);
	map[s++] = i;
next:
	switch(c) {
	case ',' : goto top;
	case '-' :
		c = get_num(&j, l, h);
		if(j > i) {
			while(i < j) {
				checksize(s, 255);
				map[s++] = ++i; } }
		else {
			while(i > j) {
				checksize(s, 255);
				map[s++] = --i; } }
		goto next;
	case '.' :
		c = get_num(&j, 0, 256);
		while(--j) {
			checksize(s, 255);
			map[s++] = i; }
		goto next;
	} *size = s;
}

/*
 * Check a map for duplicate entries
 */
void check_dup(unsigned char map[], unsigned size)
{
	unsigned i, j;
	unsigned char flags[256];
	memset(flags, 0, sizeof(flags));
	for(i=0; i < size; ++i) {
		if(flags[j=map[i]])
			error("Duplicate entry %u", j);
		flags[j] = 255; }
}

/*
 * Parse a (command-line/file) option value
 */
int option(char flag)
{
	unsigned i, j;
	struct SIDE *s;
	unsigned char fname[65], f;
	FILE *fp;
	static unsigned Strans[] = { 128, 256, 512, 1024, 2048, 4096, 8192 };

	ptr1 = ptr;
	switch(i = (toupper(*ptr++) << 8) | toupper(*ptr++)) {
	case '/1' : Twoside = 0;							goto ret0;
	case '/2' : Twoside = 255;							goto ret0;
	case '/V' : switch(*ptr++) {
		case '0' : Verbose = 0;							goto ret0;
		case '1' : Verbose = 255;						goto ret0;
		} --ptr; Verbose = 15;							goto ret0;
	case '/C' : Compress = 255;							goto ret0;
	case '/U' : Compress = 0;							goto ret0;
	case 'F=' : get_num(&i, 1, 255); Fill = i;			goto ret0;
	case 'N=' : get_num(&Cylinders, 1, 256);			goto ret1;
	case 'C=' :
		if(*ptr == '@') {
			filename(fname, ptr+1, ".TXT");
			fp = fopen(fname, "rvqb");
			while((i = getc(fp)) != EOF) {
				checksize(Cmtsize, sizeof(Buffer));
				Buffer[Cmtsize++] = i; }
			fclose(fp);
			while(*ptr && isspace(*ptr))
				++ptr;
			goto ret1; }
		for(;;) switch(i = *ptr) {
			case 0 :
			case ' ': ++ptr;
			case '\t':
				Buffer[Cmtsize++] = 0x0D;
				Buffer[Cmtsize++] = 0x0A;
				goto ret1;
			case '~' : i = ' ';
			default: ++ptr;
				checksize(Cmtsize, sizeof(Buffer));
				Buffer[Cmtsize++] = i; }
	ret1: if(flag)
		error("Not allowed in track records");
	ret0: return 0; }
	s = Side0;
	f = 255;
	switch(*ptr) {
	case '1' : s = Side1; Twoside = 255;
	case '0' : f = 0; ++ptr; }
	if(*ptr++ == '=') switch(i) {
	case 'DM' : get_num(&j, 0, 5);
		s->Mode = j;
		if(f) Side1.Mode = j;
		goto ret0;
	case 'SS' : get_num(&j, 0, 8192);
		for(i=0; i < sizeof(Strans); ++i) {
			if(Strans[i] == j) {
				s->Ssize = i;
				s->SSize = j;
				if(f) {
					Side1.Ssize = Side0.Ssize;
					Side1.SSize = Side0.SSize; }
				goto ret0; } }
		error("Bad sector size");
	case 'SM' : get_map(s->Smap, &s->Nsec, 0, 255);
		if(f) memcpy(Side1.Smap, Side0.Smap, Side1.Nsec = Side0.Nsec);
		goto ret0;
	case 'CM' : get_map(s->Cmap, &s->CMsize, 0, 255);
		if(f) memcpy(Side1.Cmap, Side0.Cmap, Side1.CMsize = Side0.CMsize);
		goto ret0;
	case 'HM' : get_map(s->Hmap, &s->HMsize, 0, 1);
		if(f) memcpy(Side1.Hmap, Side0.Hmap, Side1.HMsize = Side0.HMsize);
		goto ret0;
	} return 255;
}

/*
 * Display a map value
 */
void showmap(unsigned char *title, unsigned char Map[], unsigned size)
{
	unsigned i, j;
	printf("%5s:", title);
	for(i=j=0; i < size; ++i) {
		if(++j > 18) {
			printf("\n     ");
			j = 1; }
		printf("%4u", Map[i]); }
	putc('\n', stdout);
}

/*
 * Validate settings before generating image
 */
void validate(unsigned side)
{
	unsigned i, j;
	struct SIDE *s;
	unsigned char flags[256];

	if(side) {
		s = Side1;
		ptr1 = "Side1"; }
	else {
		s = Side0;
		ptr1 = "Side0"; }
	if(Verbose) {
		printf("%3u/%u:  Mode:%u  Sectors:%u  Ssize:%u/%u\n",
			Cylinder, side, s->Mode, s->Nsec, s->Ssize, s->SSize);
		if(Verbose & 0xF0) {
			showmap("Smap", s->Smap, s->Nsec);
			if(s->HMsize) showmap("Hmap", s->Hmap, s->HMsize);
			if(s->CMsize) showmap("Cmap", s->Cmap, s->CMsize); } }

	if(s->Mode > 5)
		error("Data Mode must be defined.");

	if(!s->SSize)
		error("Sector Size must be defined");

	if(!s->Nsec)
		error("At least 1 sector must be defined");

	if(	(s->CMsize && (s->CMsize != s->Nsec)) || (s->HMsize && (s->HMsize != s->Nsec)) )
		error("Cylinder/Head maps must match sector map size");

	memset(flags, 0, sizeof(flags));
	for(i=0; i < s->Nsec; ++i) {
		if(flags[j=s->Smap[i]])
			error("Duplicate sector map entry %u", j);
		flags[j] = 255; }
	ptr1 = 0;
}

/*
 * Skip to next non-blank in input
 */
unsigned char *skip(void)
{
	while(isspace(*ptr))
		++ptr;
	return *ptr;
}

/*
 * Read disk format data from command option file
 */
void read_format(void)
{
again:
	if(!fgets(ptr = Ffile, sizeof(Ffile)-1, fpf)) {
		Fmtnext = -1;
		return; }
	switch(skip()) {
	case ';' :
	case 0 : goto again; }
	get_num(&Fmtnext, 0, 65535);
}

/*
 * High-speed test for compressable sector (all bytes same value)
 */
int issame() asm
{
		MOV		BX,OFFSET DGRP:_Buffer
		MOV		CX,4[BP]			; Get sector size
		MOV		AL,[BX]				; Get byte1
		DEC		CX					; Backup count
iss1:	INC		BX					; Next in source
		CMP		AL,[BX]				; Does it match?
		JNZ		iss2				; No, report different
		LOOP	iss1				; Check entire sector
		MOV		AL,255				; Return !0
		POP		BP					; Restore BP
		RET
iss2:	XOR		AX,AX				; Return 0
}

/*
 * Write a .IMD format track record
 */
void write_track(unsigned char head)
{
	unsigned i, j, size;
	struct SIDE *s;

	s = head ? Side0 : Side1;
	if(s->CMsize) head |= 0x80;
	if(s->HMsize) head |= 0x40;

	putc(s->Mode, fpo);
	putc(Cylinder, fpo);
	putc(head, fpo);
	putc(s->Nsec, fpo);
	putc(s->Ssize, fpo);

	fput(s->Smap, s->Nsec, fpo);
	if(s->CMsize) fput(s->Cmap, s->CMsize, fpo);
	if(s->HMsize) fput(s->Hmap, s->HMsize, fpo);

	size = s->SSize;
	for(i=0; i < s->Nsec; ++i) {
		if(fpi) {
			j = fget(Buffer, size, fpi);
			if(size != j) {
				memset(Buffer+j, Fill, size-j);
				printf("Input file is smaller than output image (ends at CHS %u/%u/%u)\n", Cylinder, head, i);
				fclose(fpi);
				fpi = 0; } }
		else
			memset(Buffer, Fill, size);
		if(Compress) {
			if(issame(size)) {
				putc(0x02, fpo);
				putc(Buffer[0], fpo);
				continue; } }
		putc(0x01, fpo);
		fput(Buffer, s->SSize, fpo); }
}

unsigned char help[] = { "Copyright 2005-"#CYEAR" Dave Dunfield - All rights reserved.\n\
\nUse: BIN2IMD binary-input-file IMD-output-file [option-file] [options]\n\n\
opts:	/1			- 1-sided output\n\
	/2			- 2-sided output\n\
	/C			- write Compressed sectors\n\
	/U			- write Uncompressed sectors\n\
	/V[0|1]			- Verbose output\n\
	C=text | @file		- image Comment\n\
	N=#cylinders		- set Number of output cylinders\n\
	DM[s]=0-5		- track Data Mode\n\
	SS[s]=128-8192		- track Sector Size\n\
	SM[s]=n[,n-n][n.#]	- track Sector numbering Map\n\
	CM[s]=n[,n-n][n.#]	- track/sector Cylinder	 Map\n\
	HM[s]=n[,n-n][n.#]	- track/sector Head	 Map\n" };

/*
 * Main program
 */
main(int argc, char *argv[])
{
	unsigned i;

	fputs("BINary-2-IMageDisk "#VERSION" / "#__DATE__"\n", stdout);

	Side0.Mode = Side1.Mode = 255;

	for(i=1; i < argc; ++i) {
		ptr = argv[i];
		if(!option(0)) continue;		// Option was recognized
		if(!*Ifile) {					// 1st file = input
			filename(Ifile, argv[i], ".BIN");
			continue; }
		if(!*Ofile) {					// 2nd file = output
			filename(Ofile, argv[i], ".IMD");
			continue; }
		if(*Ffile)						// 3rd file = option
			error("Bad option");
		filename(Ffile, argv[i], ".B2I"); }
	ptr1 = 0;

	if(!(*Ifile && *Ofile))				// Insufficent files
		abort(help);

	// If option file, open it and read first set of parameters
	// Apply them if they apply to track-0
	if(*Ffile) {
		fpf = fopen(Ffile, "rvq");
		read_format();
		if(!Fmtnext) {			// Initial parameters
			while(skip())
				option(0);
			read_format(); } }

	validate(0);				// Validate side-0 settings
	if(Twoside)
		validate(1);			// Validate side-1 settings

	if(Cylinders == -1)
		error("N= (cylinders) not specified");

	fpi = fopen(Ifile, "rvqb");
	fpo = fopen(Ofile, "wvqb");

	get_date(&Day, &Month, &Year);
	get_time(&Hour, &Minute, &Second);
	fprintf(fpo, "IMD "#VERSION": %2u/%02u/%04u %2u:%02u:%02u\r\n",
		Day, Month, Year,
		Hour, Minute, Second);

	if(Cmtsize)					// Output comment if enabled
		fput(Buffer, Cmtsize, fpo);
	putc(0x1A, fpo);

	// For each cylinder write side0 and side1(if enabled) records
	for(Cylinder = 0; Cylinder < Cylinders; ++Cylinder) {
		if(Cylinder == Fmtnext) {
			while(skip())
				option(255);
			validate(0);
			if(Twoside)
				validate(1);
			read_format(); }
		write_track(0);
		if(Twoside)
			write_track(1); }

	// Close files + test for excess data in input file
	fclose(fpo);
	if(fpi)	{
		if(fget(Buffer, sizeof(Buffer), fpi))
			printf("Input file is larger than output image.\n");
		fclose(fpi); }
	if(fpf) fclose(fpf);
}
