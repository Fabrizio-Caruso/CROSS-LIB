/*
 * ImageDisk - A PC based floppy disk archival/restore utility
 *
 * ImageDisk is a program to read entire diskette images into files
 * (type .IMD), and to recreate a copy of the diskette from that image.
 * A detailed analysis is performed on the diskette, and information about
 * the formatting is recorded in the image file. This allows ImageDisk to
 * work with virtually any soft sectored diskette format that is compatible
 * with the PC's type 765 floppy diskette controller and drives.
 *
 * ImageDisk performs direct hardware access to the floppy disk controller
 * chip, and therefore cannot run under operating systems which do not
 * permit application programs such access. These include Windows NT,
 * 2000 and XP. I strongly recommend running the program under pure DOS.
 *
 * This program is compiled using my own development tools, and will not
 * build under mainstream compilers without significant work. This source
 * code is provided for informational purposes only, and I provide no
 * support for it, technical or otherwise.
 *
 * Copyright 2005-2012 Dave Dunfield
 * All rights reserved.
 *
 * For the record: I am retaining copyright on this code, however this is
 * for the purpose of keeping a say in it's disposition. I encourage the
 * use of ideas, algorithms and code fragments contained herein to be used
 * in the creation of compatible programs on other platforms (eg: Linux).
 */
#include <stdio.h>				// Standard I/O definition
#include <file.h>				// File system definitions
#include <window.h>				// Windowing definitions
#include <keys.h>				// BIOS key codes
#include <setjmp.h>				// setjmp/longjmp definitions
#include "version.h"			// Version indicator

// Misc options and parameters
#define	MSEC		256			// Maximum sectors/track supported
#define	CFG_X		45			// Settings X position
#define	CFG_Y		2			// Settings Y position
#define	SFILES		110			// Number of files that can be selected
#define	TIMEOUT		36			// 2 seconds (55ms ticks)
#define	MDEL		100			// Minimum delay

// Status index values
#define	ST_TOTAL	0			// Total sector count
#define	ST_COMP		1			// Number Compressed
#define	ST_DAM		2			// Number Deleted
#define	ST_BAD		3			// Number Bad
#define	ST_UNAVAIL	4			// Number unavailable

// The low-level FDC functions are coded both in 'C' for readability,
// and in hand-optimized assembler to improve performance on PC/XT class
// Machines - define the following "CIO" symbol to enable the C versions.
//#define			CIO			// Define for 'C' versions of I/O functions

// Help file index markers
#define	HINDEX		0			// Main help index
#define	HCMD		1			// Command line options
#define	HMAIN		2			// Main operating window
#define	HSET		3			// Settings
#define	HALIGN		4			// Align/Test
#define	HEXCL		5			// Sector exclusion map
#define	HFILE		6			// File selection
#define	HENTRY		7			// Data entry

// Special PC video characters
#define	BDONE		0xDF		// Bottom side done
#define	TDONE		0xDC		// Top side done
#define	CONE		0xF9		// Cylinder scale - ONE marker
#define	CFIVE		0x04		// Cylinder scale - FIVE marker

// Help system constants
#define	LINK_S		('N'-0x40)	// Start of link marker
#define	LINK_E		('O'-0x40)	// End   of link marker
#define	TAB_SIZE	4			// Spacing of TABs
#define	LINKS		25			// Maximum # links per screen

// Comment editor parameters
#define	LNUM		8			// Number of lines
#define	LSIZE		63			// Size of each line
#define	LS1			64			// Lsize+1 (Total size required = LNUM*LS1)

#define	TICK peekw(0x40,0x6C)	// BIOS Clock Tick

#define	FDCS		(FDC+2)		// Offset to FDC status register
#define	FDCD		(FDC+3)		// Offset to FDC data   register

// Video attribute indexes
#define	AT_STAT		0			// Status window- normal
#define	AR_STAT		1			// Status window- reversed
#define	AH_STAT		2			// Status window- hilited
#define	AT_MAIN		3			// Main window	- normal
#define	AR_MAIN		4			// Main window	- reversed
#define	AH_MAIN		5			// Main window	- hilited
#define	AT_POP		6			// pop-ups		- normal
#define	AR_POP		7			// pop-ups		- reversed
#define	AT_HELP		8			// Help window	- normal
#define	AT_LINK		9			// Help link	- normal
#define	AH_LINK		10			// Help link	- hilited

unsigned char video[] = {		// Color video attributes
	0x47, 0x74, 0x4B, 0x17, 0x71, 0x1E, 0x70, 0x07, 0x70, 0x71, 0x17 };
unsigned char mvideo[] = {		// Mono video attributes
	0x70, 0x07, 0x1F, 0x07, 0x70, 0x07, 0x70, 0x07, 0x07, 0x0A, 0x70 };

// Diskette side specific parameters
struct SIDE {
	unsigned Stop;				// Top of sector list (# sectors)
	unsigned Lmsn;				// Mode/Ssize/Nsectors
	unsigned char Mode;			// Mode (rate/Density)
	unsigned char Ssize;		// Sector size (128 << Ssize)
	unsigned char Eot;			// Last sector in track
	unsigned char Smap[MSEC];	// Sector numbering map
	unsigned char Cmap[MSEC];	// Sector cylinder map
	unsigned char Hmap[MSEC]; }	// Sector head map
		S0,						// Side-1 structure
		S1,						// Side-2 structure
		*S;						// Pointer to current side

unsigned
	Wdata,				// Write data
	FIseg,				// Floppy interrupt segment
	FIoff,				// Floppy interrupt offset
	SEG,				// External segment	(aligned to 64k boundary)
	DMA,				// DMA segment 		(advanced for each read/write)
	FDC = 0x3F2,		// Base address of FDC
	Msn,				// Mode, Sector-size, Nsectors
	Lmsn,				// Last Msn (for change detect)
	Rcount,				// Top of FDC result list (#results)
	Stop,				// Top of track sector list (#sectors)
	Timeout,			// FDC timeout counter
	PPort,				// Parallel port address (0=Disabled)
	SKdel = 55,			// Seek delay
	Retry = 5,			// Maximum retries
	Cpass = 3,			// Number of cleaning passes
	STcount[5],			// Status counts
	Lkey,				// Last keypress detected
// The following must be together and initialized as they are
// save and restored as a FSSIZE-byte block
	Fn=9, Fm=5,			// Format parameters (Mode, Sec/track)
	Fs=2, Fss=1;		// Format parameters (Sec-size, Starting#)
unsigned char
	Ffill = 0xE5,		// Format fill byte
	Intl = 0,			// Interleave factor
#define FSSIZE 10		// Size of saved block
	FMTflag,			// Format saved flag
	FMTsave[11];		// Format save buffer

unsigned char
	*ptr,				// General pointer
	*ptr1,				// General pointer
	TGman,				// Manual TG43 setting
	TGcyl = 43,			// TG43 cylinder number
	PPlow = 0xF0,		// Pport-TG43 low value
	PPhigh = 0x0F,		// Pport-TG43 high value
	Dstep = 0x55,		// Double-Step flag			(55 = As original)
	Dds,				// Active double-step
	Dana = 255,			// Detailed analysis		(0	= Enable)
	Dam = 255,			// Allow Deleted-Data Address-Mark
	Bad,				// Keep Bad sectors
	FmtOK,				// Format parameters set
	Fa0,				// Force side0 analysis
	Fa1,				// Force side1 analysis
	Drive,				// Selected drive			(0=3=A-D)
	Maxdrive = 2,		// Number of drives supported on FDC
	Cylinders = 80,		// Number of cylinders
	Cylinder,			// Current Cylinder
	Head,				// Current Head
	Hflag,				// Optional bits encoded with HEAD
	Sector,				// Current Sector
	Ssize,				// Sector size (128 << Ssize)
	Side1 = 0x55,		// Read side-1				(55 = As original)
	Ss1,				// Active side-1
	Gl = 0x1B,			// Gap length (write) 0x18
	Glf = 0x54,			// Gap length (format) 0x30
	Xgl = 255,			// Override gap length (write)
	Xglf = 255,			// Override gap length (format)
	SPsrt = 8,			// Step rate
	SPhlt = 127,		// Head load time
	SPhut = 15,			// Head unload time
	Nintl,				// Interleave was changed
	Lrate,				// Locked data rate			(Fn = active)
	Eot,				// Last sector in track
	Mode,				// Bitrate/Density selector
	Lc,					// Last Cylinder accessed
	Lh = 255,			// Last Head accessed
	Mfm,				// DoubleDensity indicator
	Xhelp,				// Startup Help flag (0=Enable)
	Resync,				// Resync state counter
	Result[15],			// Results from FDC
	Scale[81],			// Scale characters (for display)
	*Smap,				// Current sector numbering map
	*Cmap,				// Current sector cylinder map
	*Hmap,				// Current sector head map
	Sflag[MSEC],		// Current sector flags
	Sskip[MSEC],		// Sector skip flags
	Imap[MSEC],			// Interleave map
	buffer[512];		// General buffer
	
unsigned Mtext[] = {	// Translate Mode%3 to actual data rate
	500, 300, 250 };

// Mode translation table
unsigned char Mtran[] = { 0, 1, 2, 3, 4, 5 };

// Digital Output Register - Drive seleect (0-3) values
unsigned char DORsel[] = { 0x14, 0x25, 0x46, 0x87 };

struct WINDOW
	*swin,				// Status window
	*pwin,				// Progress window
	*mwin;				// Main output window

jmp_buf
	xmain;				// Exit to main

// Table of Sector size/number to gap lengths
unsigned char gl_table[] = {
	0x00, 0x1A, 0x07, 0x1B,	// 8" FM
	0x01, 0x0F, 0x0E, 0x2A,
	0x02, 0x08, 0x1B, 0x3A,
	0x03, 0x04, 0x47, 0x8A,
	0x04, 0x02, 0xC8, 0xFF,
	0x05, 0x01, 0xC8, 0xFF,
	0x01, 0x1A, 0x0E, 0x36,	// 8" MFM
	0x02, 0x0F, 0x1B, 0x54,
//	0x02, 0x10, 0x18, 0x3C,
	0x03, 0x08, 0x35, 0x74,
	0x04, 0x04, 0x99, 0xFF,
	0x05, 0x02, 0xC8, 0xFF,
	0x06, 0x01, 0xC8, 0xFF,
	0x00, 0x12, 0x07, 0x09,	// 5" FM
	0x00, 0x10, 0x10, 0x19,
	0x01, 0x08, 0x18, 0x30,
	0x02, 0x04, 0x46, 0x87,
	0x03, 0x02, 0xC8, 0xFF,
	0x04, 0x01, 0xC8, 0xFF,
	0x01, 0x12, 0x0A, 0x0C,	// 5" MFM
	0x01, 0x10, 0x20, 0x32,
	0x02, 0x08, 0x2A, 0x50,
	0x02, 0x09, 0x18, 0x40,
	0x02, 0x0A, 0x07, 0x0E,
	0x03, 0x04, 0x8D, 0xF0,
	0x04, 0x02, 0xC8, 0xFF,
	0x05, 0x01, 0xC8, 0xFF,
//	0x02, 0x0F, 0x2A, 0x50,	// 1.2MB 5.25
	0x02, 0x12, 0x1B, 0x54,	// 1.4MB 3.5
	0xFF };

FILE
	*fp;					// General file pointer

unsigned char
	Directory[70],			// Select file: Saved directory
	*Dip;					// Select file: Directory pointer

extern char *ARGV[];		// Help: to access program name

/*
 * Restore format parms if changed
 */
void fmtrest(void)
{
	if(FMTflag == 0x55) {
		memcpy(&Fn, FMTsave, FSSIZE);
		FMTflag = 0; }
}

/*
 * Check for format changed
 */
int format_changed(void)
{
	Msn = ((Mode * Ssize) << 8) | Stop;
	if(Msn != Lmsn) {
		Lmsn = Msn;
		return 255; }
	return 0;
}

/*
 * Turn off the parallel port TG43 output
 */
void ppoff(void)
{
	if(PPort)
		out(PPort, PPlow);
}

/*
 * Update the Cylinder and Head indicators and select correct side structure
 */
void upCH(void)
{
	w_gotoxy(15, 0, swin);
	w_printf(swin, "T%-3u H%u", Cylinder, Head);

	// Copy the parameters for this side to globals for easy access.
	// (This should just be a struct pointer, however the original
	//  program used a set of globals and this was an easy to way
	//  to enhance it to support side specific data without a lot of
	//  changes.
	S = Head ? S1 : S0;
	Mode	= S->Mode;
	Ssize	= S->Ssize;
	Stop	= S->Stop;
	Eot		= S->Eot;
	Smap	= S->Smap;
	Cmap	= S->Cmap;
	Hmap	= S->Hmap;
}

/*
 * Begin a new status line, with cylinder/head indicator if changed.
 */
register newch(unsigned args)
{
	unsigned char buffer[81];
	_format_(nargs() * 2 + & args, buffer);
	if((Cylinder != Lc) || (Head != Lh))
		wprintf("\n%3u/%u: ", Lc = Cylinder, Lh=Head);
	else
		wputs("\n     : ");
	wputs(buffer);
}

/*
 * Reorder the sectors to the requested interleave.
 * If necessary, calculate optimal interleave.
 */
void interleave(void)
{
	unsigned i, j, k;
	unsigned char D, E, F, G, *map;

	Nintl = 255;

	// First sort sector numbering map to obtain a
	// sequential list of sectors
	map = buffer+256;
	memcpy(Sflag, Smap, Stop);
	for(i=0; i < Stop; ++i)
		map[i] = i;
	for(i=0; i < Stop; ++i) {
		D = Sflag[i];
		F = map[i];
		for(j=i+1; j < Stop; ++j) {
			E = Sflag[j];
			if(E < D) {
				G = map[j];
				Sflag[j] = D;
				Sflag[i] = D = E;
				map[j] = F;
				map[i] = F = G; } } }

	if(Intl == 255) {		// Best guess - calculate interleave
		// Scan sector map to determine interleave occurances
		memset(buffer, 0, Stop);
		for(i=j=k=0; i < Stop; ++i) {
			D = Sflag[i];
			E = Sflag[(i+1)%Stop];
			while(Smap[j] != D)
				j = (j + 1) % Stop;
			while(Smap[k] != E)
				k = (k + 1) % Stop;
			E = (k >= j) ? k-j : Stop-(j-k);
			++buffer[E]; }

		// Locate best interleave value
		D = k = 0;
		for(i=0; i < Stop; ++i) {
			if(buffer[i] > D) {
				k = i;
				D = buffer[i]; } } }
	else
		k = Intl;

	w_gotoxy(58, 0, swin);
	w_printf(swin, "%u:1", k);

	// Regenerate the Sector Numbering Map using
	// the sector list and requested interleave
	memset(buffer, 0, Stop);
	for(i=j=0; i < Stop; ++i) {
		while(buffer[j])
			j = (j + 1) % Stop;
		Imap[j] = map[i];
		buffer[j] = 255;
		j = (j + k) % Stop; }
}

/*
 * Update individual map value
 */
void update_int(unsigned char *map)
{
	unsigned i;
	memcpy(buffer, map, Stop);
	for(i=0; i < Stop; ++i)
		map[i] = buffer[Imap[i]];
}

/*
 * Compute gap length and update format displays
 */
void compute_gl(unsigned char iupdate)
{
	unsigned i, s, gl, glf;

	// First, look to see if Ssize/Nsec combination is in our table
	for(i=0; (s = gl_table[i]) != 0xFF; i += 4) {
		if((s == Ssize) && (gl_table[i+1] == Stop)) {
			gl = gl_table[i+2];
			glf = gl_table[i+3];
			goto exit; } }

	// Rough calculation to approximate usable gaps
	switch(Mtext[Mode % 3]) {
	case 300 :
	case 250 : i = 3125-104;	break;
	case 500 : i = 5208-173; }

	if(Mfm)		// DD: 144+GL2 + Nx(62+SS+GL1)
		glf = (((i*2)-144) / Stop) - 62;
	else		// SD: 73+GL2 + Nx(33+SS+GL1)
		glf = ((i - 73) / Stop) - 33;

	gl = 128 << Ssize;
	if((glf <= gl) || ((glf -= gl) < 13))
		glf = 13;
	gl = (glf * 2) / 3;
	if((gl -= (gl / 5)) < 7)
		gl = 7;
	if(gl > 200)
		gl = 200;
	if(glf > 255)
		glf = 255;

exit:
	// If fixed gaps are not selected, copy in new values
	if(Xgl) Gl = gl;
	if(Xglf) Glf = glf;

	// Update format displays
	w_gotoxy(40, Nintl = 0, swin);
	w_printf(swin, "%2ux%-5u%-4u%-3u", Stop, 128 << Ssize, Gl, Glf);
	Msn = ((Mode * Ssize) << 8) | Stop;
	if(S->Lmsn != Msn) {
		S->Lmsn = Msn;
		newch("%uk %cD - %u sectors of %u bytes - G1:%u G2:%u",
			Mtext[Mode % 3], Mfm ? 'D' : 'S', Stop, 128 << Ssize, Gl, Glf);
		if(Intl) {
			interleave();
			if(iupdate) {
				update_int(Smap);
				if(Hflag & 0x80)	update_int(Cmap);
				if(Hflag & 0x40)	update_int(Hmap); } } }
	w_gotoxy(0, 1, swin);
	for(i=0; i < Stop; ++i)
		w_printf(swin, " %u", Smap[i]);
}

/*
 * Update the cylinder bar graph with status character
 */
void Cstat(unsigned char c)
{
	if(c) {		// Status char - write in head position
		if(Cylinder >= 80) {	// More than 80, restart with highlight
			*pwin  = video[AH_STAT];
			w_gotoxy(Cylinder % 80, Head ? 0 : 2, pwin);
			w_putc(c, pwin);
			*pwin = video[AT_STAT];
			return; }
		w_gotoxy(Cylinder, Head ? 0 : 2, pwin);
		w_putc(c, pwin); }

	// Update status bar position indicator only
	w_gotoxy(Cylinder, 1, pwin);
	*pwin = video[AR_STAT];
	w_putc(Scale[Cylinder], pwin);
	*pwin = video[AT_STAT];
}

/*
 * Update full status bar display for random cylinder
 */
void upCYL(void)
{
	unsigned i;
	w_gotoxy(0, 1, pwin);
	for(i=0; i < 80; ++i) {
		*pwin = video[(i > Cylinder) ? AT_STAT : AR_STAT];
		w_putc(Scale[i], pwin); }
	*pwin = video[AT_STAT];
}

/*
 * Display function prompt text on left-hand side of screen
 */
void prompt(unsigned char *p[], unsigned x)
{
	unsigned y, i, w;
	unsigned char *ip;

	for(y=i=0; y < 17; ++y) {
		wgotoxy(x, y+2);
		if(ip = p[i])
			++i;
		else
			ip = "";
		w = 40;
		if(*ip && (*ip < ' ')) {
			w -= *ip;
			wputf("", *ip++); }
		wputf(ip, w); }
}

/*
 * Draw the blank cylinder bar-graph
 */
void draw_scale(void)
{
	w_gotoxy(0, 1, pwin);
	w_puts(Scale, pwin);
}

/*
 * Close pending windows
 */
void wcloseall(struct WINDOW *level)
{
	while(W_OPEN != level)
		wclose();
}

/*
 * Display a title on a window
 */
void wintitle(unsigned x, char *ptr)
{
	unsigned char buffer[6];
	memcpy(buffer, W_OPEN, 6);
	W_OPEN[1] &= (~WBOX3 >> 8);
	--W_OPEN[3];
	wgotoxy(x,0);
	wprintf(" %s ", ptr);
	memcpy(W_OPEN, buffer, 6);
}

/*
 * Deselect current drive
 */
void deselect_drive(void)
{
	out(0x0A, 6);			// Mask DMA channel
	ppoff();				// Release PPT
	out(FDC, 0x04);			// Disable FDC
}

/*
 * Report an error and return to main screen
 */
register void error(unsigned args)
{
	_format_(nargs() * 2 + &args, Sflag);
	if(fp) {
		fclose(fp);
		fp=0; }
	if(!mwin) {			// Not started yet
		printf("%s : %s\n", ptr1, Sflag);
		exit(-1); }
	out(FDC, 0);		// Reset disk
	delay(MDEL);
	wcloseall(mwin);	// Back to main window
	deselect_drive();
	fmtrest();
	longjmp(xmain, Sflag);
}

/*
 * Process multiple error messages via global ptr
 */
void merror(unsigned char *p)
{
	if(ptr) {		// Already have one pending
		wputc('\n');
		wputs(ptr); }
	ptr = p;		// Queue this one
}

/*
 * Help commnd - Implements "hypertext" screens
 */
void help(unsigned section)
{
	int c, lx, ly, ox, oy;
	unsigned i, j, size, ncount;
	unsigned char *ptr, l;
	unsigned char xs[LINKS], ys[LINKS], names[LINKS][25], link[LINKS];

	ptr = ARGV[i=j=l=0];
	memset(Sflag, Xhelp = 255, sizeof(Sflag));
	while(*ptr) {
		if((buffer[i++] = *ptr++) == '.')
			j = i; }
	strcpy(buffer+j, "HLP");
	if(!(fp = fopen(buffer, "rb")))
		error("%s not found", buffer);
	if((TGcyl&PPlow&PPhigh) == 255) {
		wgotoxy(0, 0);
		wprintf("%04x\n%04x", fp+(10+512), asm" MOV AX,SP"); }

	/* Locate the section in question */
lookup:
	Sflag[l] = section;
lookup1:
	size = getc(fp);
	size |= getc(fp) << 8;
	if(section--) {
		fseek(fp, 0, size, 1);
		goto lookup1; }

	/* Draw the screen */
	wopen(5, 4, 70, 17, (unsigned)video[AT_HELP]|WSAVE|WCOPEN|WBOX1);
	wcursor_off();
	i = ncount = ptr = 0;
	while(buffer[i++] = getc(fp));
	wintitle(0, buffer);

	if(Sflag[(l-1)&255] != 255)
		wintitle(46, "F1=Back ESC=exitHelp");
	else
		wintitle(54, "ESC=exitHelp");

	wgotoxy(0, 0);
	while(i++ < size) switch(c = getc(fp)) {
		case LINK_S :	/* Start link */
			ptr = names[ncount];
			xs[ncount] = W_OPEN->WINcurx;
			ys[ncount] = W_OPEN->WINcury;
			*W_OPEN = video[AT_LINK];
			break;
		case LINK_E :	/* End link */
			link[ncount++] = getc(fp);
			*ptr = ptr = 0;
			*W_OPEN = video[AT_HELP];
			++i;
			break;
		case '\t' :		/* Horizontal TAB */
			do
				wputc(' ');
			while(W_OPEN->WINcurx % TAB_SIZE);
			break;
		case 0 :
			c = '\n';
		default:
			wputc(c);
			if(ptr)
				*ptr++ = c; }

	/* Allow user to select field & link to new screen */
	i = section = 0;
	for(;;) {
		wgotoxy(xs[section], ys[section]);
		*W_OPEN = video[AT_LINK];
		wputs(names[section]);
recmd:	wgotoxy(xs[i], ys[i]);
		*W_OPEN = video[AH_LINK];
		wputs(names[section = i]);
		*W_OPEN = video[AT_HELP];
		switch(c = toupper(wgetc())) {		/* User keystroke */
			case _KLA :				/* Left arrow - previous field */
				i = (i ? i : ncount) - 1;
				continue;
			case _KRA :				/* Right arrow - next field */
				i = (i + 1) % ncount;
				continue;
			case _KHO : i = 0; 			continue;
			case _KEN : i = ncount-1;	continue;
			case _KUA : ox = oy = -1000; goto dofind;
			case _KDA : ox = oy = 1000;
			dofind:
				size = i;
				for(j = 0; j < ncount; ++j) {
					lx = (int)xs[j] - (int)xs[i];
					ly = (int)ys[j] - (int)ys[i];
					if(c == _KUA) {
						if((ly >= 0) || (ly < oy)) continue; }
					else {
						if((ly <= 0) || (ly > oy))	continue; }
					if(abs(lx) > abs(ox)) continue;
					size = j;
					ox = lx;
					oy = ly; }
				i = size;
				continue;
			case 0x1B:				/* Escape exit */
				wclose();
				fclose(fp); *buffer = fp = 0;
				return;
			case '\n' :				/* Enter - chain to menu */
				section = link[i];
				++l;
			go:	rewind(fp);
				wclose();
				goto lookup;
			case _K1 :				// Back
			case _KBS:
				--l;
				if((section = Sflag[l]) != 255)
					goto go;
				++l;
				goto recmd; }
		// Search for next link beginning with entered character
		j = i;
		while((toupper(*names[i=(i+1)%ncount]) != c) && (i != j)); }
}

/*
 * Floppy disk interrupt handler - send EOI and set 'done' flag
 */
asm {
XFDONE	DB		0				; 'DONE' flag
FINT:	PUSH	AX				; Save AX
		MOV		AL,20h			; Get EOT
		OUT		20h,AL			; Send to PIC
		MOV		CS:XFDONE,255	; Set flag
		POP		AX				; Restore AX
		IRET
}

#ifdef CIO
#define XFreset asm" MOV CS:XFDONE,0"
#define	XFget asm {\
	XOR		AH,AH\
	MOV		AL,CS:XFDONE\
}
#else
/*
 * Stub for ASM callable delay
 */
asm {
XDEL:	PUSH	DX				; Save DS
		PUSH	AX				; Save AX
		CALL	_delay			; Perform delay
		POP		AX				; Restore AX
		POP		DX				; Restore DX
		RET
}
#endif

/*
 * ESC key detected - exit with message
 */
void xabort(void)
{
	error("Abort by operator");
}

#ifdef CIO
/*
 * Test for operator abort and exit if ESC pressed
 */
void tstabt(void)
{
	unsigned i;
	if(i = kbtst()) {
		if((Lkey = i) == 0x1B) {
			xabort(); } }
}
#endif

/*
 * Write command byte to FDC
 */
#ifdef	CIO
void wrfdc(unsigned char b)
{
	unsigned char r;
	static unsigned char c = 0;

	asm {	// Reset BIOS timeout counter
		XOR	AX,AX
		MOV	ES,AX
		MOV	BYTE PTR ES:[440h],255
	}
top:
	do {
		if(!--c)
			tstabt();
		r = in(FDCS); }
	while(!(r & 0x80));

	if(r & 0x40) {
		in(FDCD);
		goto top; }

	out(FDCD, b);
}
#else
void wrfdc(b) asm
{
		XOR		AX,AX				; Begin with zero
		MOV		ES,AX				; Address low memory
		MOV		BYTE PTR ES:[0440h],255	; Reset BIOS timeout
		MOV		CL,AL				; Zero "hung" counter
wrf1:	DEC		CL					; Reduce timeout
		JNZ		wrf2				; Not yet
		CALL	_kbtst				; Test for key
		AND		AX,AX				; Any key?
		JZ		wrf2				; No
		MOV		DGRP:_Lkey,AX		; Save last RX key
		CMP		AL,1Bh				; Abort request?
		JNZ		wrf2				; No
		JMP		_xabort				; Process the abort
wrf2:	MOV		DX,DGRP:_FDC		; Address the FDC
		ADD		DX,2				; Offset to FDCS
		IN		AL,DX				; Read status
		TEST	AL,80h				; Ready?
		JZ		wrf1				; No, wait for it
		INC		DX					; Advance to FDCD
		TEST	AL,40h				; Ready for output?
		JZ		wrf3				; Yes, proceed
		IN		AL,DX				; Mo - read to clear
		JMP	short wrf1				; And go again
wrf3:	MOV		AX,4[BP]			; Get data to write
		OUT		DX,AL				; Write to FDC data
}
#endif

/*
 * Wait for interrupt from the FDC
 * Last byte of command is sent here so that we can enable
 * interrupt before commands begins.
 */
#ifdef	CIO
void waitirq(unsigned char c)
{
	unsigned t;
	XFreset;
	wrfdc(c);
	t = TICK;
	do {
		if(XFget) {
			out(FDC, DORsel[Drive]);
			return; } }
	while((TICK - t) < 75);
	error("No interrupt from FDC");
}
#else
void waitirq(c)
{
	asm {
		MOV		CS:XFDONE,0			; Reset indicator
		MOV		AX,4[BP]			; Get argument
		PUSH	AX					; Pass it
		CALL	_wrfdc				; Write it
		POP		AX					; Cleanup
		XOR		AX,AX				; Get zero
		MOV		ES,AX				; ES = seg0
		MOV		CX,ES:[046ch]		; Read BIOS tick
wai1:	CMP		CS:XFDONE,0			; Done flag set?
		JZ		wai2				; No, keep trying
		MOV		BL,DGRP:_Drive		; Get drive
		XOR		BH,BH				; Zero high
		MOV		AL,DGRP:_DORsel[BX]	; Get index
		MOV		DX,DGRP:_FDC		; Get I/O address
		OUT		DX,AL				; Write it
		POP		BP
		RET
wai2:	MOV		AX,ES:[046Ch]		; Read BIOS tick
		SUB		AX,CX				; Calculate elapsed
		CMP		AX,55				; Over?
		JBE		wai1				; Wait for it
	}
	error("No interrupt from FDC");
}
#endif

/*
 * Initialize data transfer mode (rate and density)
 * and update status display.
 */
void initmode(void)
{
	unsigned x;
	Mfm = (Mode / 3) ? 0x40 : 0x00;
	out(FDC+5, x = Mode % 3);
	w_gotoxy(3, 0, swin);
	w_printf(swin, "%3uk %cD", Mtext[x], Mfm ? 'D' : 'S');
}

/*
 * Initialize DMA channel
 */
void initdma(unsigned char mode, unsigned count)
{
	disable();
	out(0x0A, 6);						// Mask channel
	out(FDC, DORsel[Drive] | 0x08);		// Enable DMA & IRQ
	out(0x0B, mode);					// Set mode
	out(0x0C, 0);						// Clear byte pointer
	out(0x81, DMA >> 12);				// Dma page
	out(0x04, (DMA << 4) & 255);		// Dma address low
	out(0x04, (DMA >> 4) & 255);		// Dma address high
	out(0x05, --count);					// count low
	out(0x05, count >> 8);				// count high
	out(0x0A, 2);						// Unmask channel
	enable();
}

/*
 * Write command sequence to FDC
 */
void wrcmd(unsigned c)
{
	wrfdc(c|Mfm);
	wrfdc(Head ? (Drive|4) : Drive);
}

/*
 * Read byte from FDC
 */
#ifdef	CIO
int rdfdc(void)
{
	unsigned t;
	unsigned char r;
	static unsigned char c = 0;

	t = TICK;
	do {
		if(!--c) {
			tstabt();
			if((TICK - t) > Timeout) {
				Timeout = 10;
				Result[0] = 0x48;
				return -1; } }
		r = in(FDCS); }
	while(!(r & 0x80));

	if(!(r & 0x40))
		return -1;

	return in(FDCD);
}
#else
int rdfdc(void) asm
{
		XOR		AX,AX			; Get zero
		MOV		ES,AX			; Address low memory
		MOV		BX,ES:[046Ch]	; Get BIOS tick
		MOV		CL,AL			; Zero "hung" counter
rdf1:	DEC		CL				; Reduce timeout
		JNZ		rdf2			; Not yet
		CALL	_kbtst			; Test for key
		AND		AX,AX			; Key available?
		JZ		rdf2			; No key
		MOV		DGRP:_Lkey,AX	; Save last
		CMP		AL,01Bh			; Escape?
		JNZ		rdf2			; No
		CALL	_xabort			; Abort
rdf2:	MOV		AX,ES:[046Ch]	; Get BIOS tick
		SUB		AX,BX			; Calculate elapsed
		CMP		AX,DGRP:_Timeout; Timeout expired
		JBE		rdf4			; No, wait for it
		MOV		DGRP:_Timeout,10; Set default timeout
		MOV		AL,48h			; Indicate failed
		MOV		DGRP:_Result,AL	; Set Result[0]
rdf3:	MOV		AX,-1			; Indicate failure
		JMP		rdf5			; And exit
rdf4:	MOV		DX,DGRP:_FDC	; FDC address
		ADD		DX,2			; Offset to FDCS
		IN		AL,DX			; Read it
		TEST	AL,80h			; Data ready?
		JZ		rdf1			; Wait for it
		TEST	AL,40h			; Providing data
		JZ		rdf3			; No - error
		INC		DX				; Advance to FDCD
		IN		AL,DX			; Read data
		XOR		AH,AH			; Zero high
rdf5:
}
#endif

/*
 * Read result stream from FDC
 */
int result(unsigned m)
{
	unsigned d;
	memset(Result, Rcount = 0, sizeof(Result));
	while(m) {
		--m;
		if((d = rdfdc()) & 0xFF00)
			break;
		Result[Rcount++] = d; }
	return Result[0] & 0xC0;	// 0xC0  0xD8
}

/*
 * Recalibrate the FDC head
 */
void recal(void)
{
	unsigned char x;
	x = 2;
	ppoff();
	do {
		wrfdc(0x07);
		out(FDC, DORsel[Drive] | 0x08);		// Enable IRQ
		waitirq(Drive);
		do {
			delay(MDEL);
			wrfdc(0x08);		// Sense interrupt
			result(2); }
		while(in(FDCS) & 0x0F);	// Still seeking
		delay(MDEL); }
	while(--x);
	Cylinder = 0;
}

/*
 * Seek to track
 */
void seek(unsigned c)
{
	Cylinder = c;
	if(c = kbtst()) {
		if((Lkey = c) == 0x1B)
			xabort(); }
	delay(55);
	wrfdc(0x0F);					// Seek command
	wrfdc(Drive);					// Drive ID
	out(FDC, DORsel[Drive] | 0x08);	// Enable IRQ
	waitirq(Dds ? Cylinder+Cylinder : Cylinder); // cylinder
	do {
		delay(55);
		wrfdc(0x08);				// Sense interrupt
		result(2); }
	while(in(FDCS) & 0x0F);			// Still seeking
	if(SKdel)
		delay(SKdel);
}

/*
 * Resync and retry head position
 */
void resync(void)
{
	unsigned c;
	c = Cylinder;
	switch(++Resync) {
	case 1 : Cstat('E');	return;
	case 2 :			// 1st retry - Step in/out
	case 5 :
		Cstat('1');
		if(c) {
			seek(c-1);	delay(200);
			seek(c);	delay(200); }
		return;
	case 3 :			// Second retry - step out/in
	case 6 :
		Cstat('2');
		if(c < Cylinder) {
			seek(c+1);		delay(200);
			seek(c);		delay(200); }
		return;
	case 4 :			// Third retry - recalibrate the head
	case 7 :
		Cstat('3');
		recal(); delay(250);
		seek(c);
		return;
	case 8 :			// Forth retry - deselect/select
		Cstat('4');
		deselect_drive();
		delay(1000);
		out(FDC, DORsel[Drive]);
		delay(500);
		initmode();
		Resync = 1; }
}

/*
 * Read sector ID passing under head
 */
int readid(void)
{
	wrcmd(0x0A);
	return result(7);
}

/*
 * Report a disk error
 */
void diskerror(char *t)
{
	unsigned char r, f, *p;
	f = p = 0;
	newch("%s error <%u>", t, Sector);
	if(Result[0] & 0x08) wputs(p=" EC");
	r = Result[1];
	if(r & 0x80) wputs(p=" EndCyl");
	if(r & 0x20) wputs(p=" Data");
	if(r & 0x10) wputs(p=" Overun");
	if(r & 0x04) wputs(p=" NoData");
	if(r & 0x02) { wputs(p=" WriteProt"); f = 255; }
	if(r & 0x01) wputs(p=" NoAdrMark");
	if(!p) {
		wprintf(" %02x %02x %02x",
			Result[0], Result[1], Result[2]); }
	if(f)
		error("Fatal error");
}

/*
 * Read a sector from the disk
 */
int read_sector(unsigned index)
{
	unsigned r;
	r = Resync;
retry:
	wrcmd(0x06);
	wrfdc(Cmap[index]);				// Cylinder
	wrfdc(Hmap[index]);				// Head
	wrfdc(Sector = Smap[index]);	// Sector
	wrfdc(Ssize);					// N
	wrfdc(Sector);					// EOT (Was Eot)
	wrfdc(Gl);						// GL
	initdma(0x46, 128 << Ssize);
	waitirq(Ssize ? 255 : 128);
	result(7);
	if(Result[0] & 0xC0) {
		if(++r >= Retry) {
			diskerror("Read");
			return 255; }
		resync();
		goto retry; }
	return 0;
}

/*
 * Write a sector to the drive
 */
int write_sector(unsigned index, unsigned char df)
{
	unsigned r;
	r = Resync = 0;
retry:
	wrcmd((df & 2 ) ? 0x09 : 0x05);
	wrfdc(Cmap[index]);				// Cylinder
	wrfdc(Hmap[index]);				// Head
	wrfdc(Sector = Smap[index]);	// Sector
	wrfdc(Ssize);					// N
	wrfdc(Sector);					// EOT (Was Eot)
	wrfdc(Gl);						// GL
	initdma(0x4A, 128 << Ssize);
	waitirq(Ssize ? 255 : 128);
	result(7);
	if(Result[0] & 0xC0) {
		if(++r >= Retry) {
			diskerror("Write");
			return 255; }
		resync();
		goto retry; }
	return 0;
}

/*
 * Adjust the data rate up or down
 */
void adjmode(unsigned char dec)
{
	if(Lrate & 0xF0) {		// Locked rate
		dec = Lrate & 7;
		S->Mode = Mode = (Mode == dec) ? (dec+3) : dec;
		return; }

	if(dec)
		S->Mode = Mode = Mode ? Mode-1 : 5;
	else
		S->Mode = Mode = (Mode + 1) % 6;
}

/*
 * Analyze disk track
 */
int analyze_track(unsigned char xf)
{
	unsigned i, s, n, h, l, h1, l1, c;
	unsigned char hd, cy, *f;

	DMA = SEG;
	upCH();
	Hflag = Nintl = 0; Resync = 1;
	Timeout = TIMEOUT;

	// First time through - always do full analysis
	if(Head) {
		if(Fa1) { Fa1 = 0; goto X0; } }
	else {
		if(Fa0) { Fa0 = 0; goto X0; } }

	// If not full analysis - check to see if still reading valid id's
	if(xf && Dana) {
		initmode();
		if(!readid()) {
			hd= Result[4];
			s = Result[5];
			if(Result[6] == Ssize) for(i=0; i < Stop; ++i) {
				if((Smap[i] == s) && (Hmap[i] == hd)) {
					if(format_changed())
						compute_gl(255);
					w_gotoxy(0, 1, swin);
					for(i=0; i < Stop; ++i) {
						if(Hmap[i] != Head) Hflag |= 0x40;
						w_printf(swin, " %u", Smap[i]); }
					w_cleol(swin);
					return 0; } } } }

X0:	// Full analysis required
	w_gotoxy(0, 1, swin); w_cleol(swin);
	memset(Smap, Stop = i = h = c = 0, MSEC);
	memset(Sflag, 0, MSEC);
	adjmode(255);		// Backup to insure quick fail on re-analyze

	l = l1 = 65535;

	// Find bad access mode to line up on sector boundary
	for(;;) {
		initmode();
		if(readid())
			break;
		adjmode(0); }

	// We should be at index hole after failed read-id
	// Read sector id's to determine interleave
	i = 0;
X1:	adjmode(0);
	initmode();
	Resync = f = 0;

	for(;;) {
		if(readid()) {		// Failed to read ID
			if(Stop)			// We already have sectors
				return 255;		// So report failure (should not happen)
			if(++i < 12) { goto X1; }
				return 255; }	// No ID after timeout (should not happen)
		cy= Result[3];			// Cylinder
		hd= Result[4];			// Head
		s = Result[5];			// Size
		n = Result[6];			// Number
		if(Sflag[s]++) {
			switch(Sflag[s]) {
			case  4 :
				if(((h-l)+1) == c)
					goto X2;
				continue;
			case 20 : f = "Missing sectors"; goto X2;
			case 8 :
			case 12 :
			case 16 : resync(); }
			continue; }
		if((Cmap[Stop] = cy) != Cylinder)
			Hflag |= 0x80;
		if((Hmap[Stop] = hd) != Head)
			Hflag |= 0x40;
		Smap[Stop++] = s;
		if(s < l) l = s;
		if(s > h) h = s;
		++c; }

X2:
	S->Ssize = Ssize = n;
	S->Eot = Eot = h;
	S->Stop = Stop;

	for(i=h1=0; i < Stop; ++i) {
		s = Sflag[Smap[i]];
		if(s > h1) h1 = s;
		if(s < l1) l1 = s; }
	if((h1 - l1) > 2)
		f = "Unable to determine interleave";
	if(f)
		newch(f);
	compute_gl(255);
	return 0;
}

/*
 * Write analasis information to file
 */
void write_ana(void)
{
	putc(Mtran[Mode], fp);
	putc(Cylinder, fp);
	putc(Head|Hflag, fp);
	putc(Stop, fp);
	putc(Ssize, fp);
	if(Stop) {
		fput(Smap, Stop, fp);
		if(Hflag & 0x80)
			fput(Cmap, Stop, fp);
		if(Hflag & 0x40)
			fput(Hmap, Stop, fp); }
}

/*
 * Write null_track record to output file
 */
void write_null(void)
{
	putc(0, fp);
	putc(Cylinder, fp);
	putc(Head, fp);
	putc(0, fp);
	putc(0, fp);
}

/*
 * High-speed test for compressable sector (all bytes same value)
 */
int issame() asm
{
		MOV		ES,DGRP:_DMA		; Get DMA segment
		XOR		BX,BX				; Zero offset
		MOV		CX,4[BP]			; Get sector size
		MOV		AL,ES:[BX]			; Get byte1
		DEC		CX					; Backup count
iss1:	INC		BX					; Next in source
		CMP		AL,ES:[BX]			; Does it match?
		JNZ		iss2				; No, report different
		LOOP	iss1				; Check entire sector
		MOV		AL,255				; Return !0
		POP		BP					; Restore BP
		RET
iss2:	XOR		AX,AX				; Return 0
}

/*
 * Write sector data to file
 */
void write_data(unsigned char v)
{
	unsigned s, o;
	++STcount[ST_TOTAL];
	if(!v) {
		++STcount[ST_UNAVAIL];
		putc(0, fp);
		return; }
	if(v & 2) ++STcount[ST_DAM];
	if(v & 4) ++STcount[ST_BAD];
	if(issame(s = 128 << Ssize)) {
		++STcount[ST_COMP];
		putc(v+1, fp);
		putc(peek(DMA, 0), fp);
		return; }
	putc(v, fp);
	o = 0;
	do {
		putc(peek(DMA, o++), fp); }
	while(--s);
}

/*
 * Advance DMA address by sector size
 */
void bumpdma(void)
{
	DMA += ((unsigned)(128 << Ssize) + 31) >> 4;
}

/*
 * Reset FDC subsystem to known state (start of function)
 */
void reset(void)
{
	Rcount = Stop = Mfm = Cylinder = Head = Sector = Ssize =
	Eot = Resync = Lkey = 0;
	Fa0 = Fa1 = Lh = 255;
	Lmsn = -1;
	Timeout = TIMEOUT;
	Dds = Dstep;
	Ss1 = Side1;
	DMA = SEG;
	if(Dam) Dam = 255;	// Reset DAM indicator
	memset(S0, 0, sizeof(S0));
	memset(S1, 0, sizeof(S1));
	memset(STcount, 0, sizeof(STcount));
	TGman &= 0xF0;

	S0.Mode = S1.Mode = Mode = Lrate & 7;

	asm {	// Reset BIOS timeout counter
		XOR	AX,AX
		MOV	ES,AX
		MOV	BYTE PTR ES:[440h],255
	}
	out(FDC, 0); delay(MDEL); out(FDC, 4); delay(MDEL);
	out(0x3F2, 4);	// Insure primary is clear
	out(0x372, 4);	// Insure secondary is clear
	wrfdc(0x03);	// Specify
	wrfdc((SPsrt << 4) | SPhut);	// Step, head unload
	wrfdc(SPhlt << 1);				// Head load, DMA
	result(0);
	ppoff();
	w_clwin(swin);
	w_printf(swin, "%c: ", Drive+'A');
	w_clwin(pwin);
	out(FDC, DORsel[Drive]);
	delay(500);
	upCH();
	initmode();
	recal(); delay(500);
}

void showstat(void)
{
	unsigned i, v;
	unsigned char f;
	static void *st_names[] = {
		"Compressed", "Deleted", "Bad", "Unavail" };

	wprintf("\n%u sectors", STcount[ST_TOTAL]);
	for(f=i=0; i < 4; ++i) {
		if(v = STcount[i+1]) {
			wprintf("%s%u %s", f ? ", " : " (", v, st_names[i]);
			f = 255; } }
	if(f)
		wputc(')');
}

/*
 * Prompt for disk to be inserted
 */
void pwait(unsigned char *t)
{
	unsigned w;
	w = strlen(t) + 58;
	wopen(40-(w/2), 20, w, 3, (unsigned)video[AT_POP]|WSAVE|WBOX1|WCOPEN);
	wprintf("Insert disk to %s - press ENTER to proceed, ESC to cancel.", t);
	for(;;) switch(wgetc()) {
		case 0x1B:
			wclose();
			error("Abort by operator");
		case '\n' :
			wclose();
			return; }
}

/*
 * Test and report disk RPM
 */
void rpm(void)
{
	unsigned t, t1, c, l1[2], l2[2];
	unsigned char id, id1, p;
	static unsigned L1091[] = { 1091, 0 };
	static unsigned L200[] = { 200, 0 };
	static char *rpm_prompt[] = {
		"RPM:",
		"",
		"Please insure that the drive",
		"contains a formatted diskette",
		"",
		"Press SPACE to recalculate",
		"      ESC to end test.",
		0 };

	wclwin();
	prompt(rpm_prompt, 5);
	pwait("TEST-RPM");

top:
	wgotoxy(0, 9);
	reset();

	if(analyze_track(0))
		return;

	id = Smap[p = 0];
restart:
	c = 0;
	for(;;) {
		if(readid())
			goto top;
		id1 = Result[5];
		if(id1 == id) {
			t = TICK;
			break; } }

	for(;;) {
		if(readid())
			goto top;
		id1 = Result[5];
		if(id1 == id) {
			t1 = TICK - t;
			++c;
			longset(l1, c);
			longmul(l1, L1091);
			longadd(l1, L200);
			longset(l2, t1);
			longdiv(l1, l2);
			w_gotoxy(0, 1, pwin);
			w_printf(pwin, "%-5u %-5u %-3u RPM: %u", c, t1, id, t1 = l1[0]);
			if(t1 >= 500) {
				if(++p >= Stop)
					error("Cannot lock onto disk");
				id = Smap[p]; } }
		else {
			if(Lkey == ' ') {
				Lkey = 0;
				goto restart; } } }
}

/*
 * Write image file header
 */
void write_header(void)
{
	unsigned day, month, year, hour, minute, second;

	get_date(&day, &month, &year);
	get_time(&hour, &minute, &second);
	fprintf(fp, "IMD "#VERSION": %2u/%02u/%04u %2u:%02u:%02u\r\n",
		day, month, year,
		hour, minute, second);
}

/*
 * Edit comment
 */
void edit(void)
{
	unsigned i, t;
	static char *form[] = {
		(LSIZE+4)<<8|(LNUM+2),
		"\x00\x00\x3F",
		"\x00\x01\x3F",
		"\x00\x02\x3F",
		"\x00\x03\x3F",
		"\x00\x04\x3F",
		"\x00\x05\x3F",
		"\x00\x06\x3F",
		"\x00\x07\x3F",
		0 };
	wputs("\nEnter comment (ESC to exit):");
	wform(37-(LSIZE/2), 12-(LNUM/2), WSAVE|WCOPEN|WBOX1|video[AT_POP], form,
		buffer, &buffer[LS1], &buffer[LS1*2], &buffer[LS1*3], &buffer[LS1*4],
		&buffer[LS1*5], &buffer[LS1*6], &buffer[LS1*7]);
	for(t=(LS1*LNUM); t; t -= LS1) {
		if(buffer[t-LS1])
			break; }
	write_header();
	wputc('\r'); wcleow();
	for(i=0; i < t; i += LS1) {
		if(i) wputc('\n');
		wputs(buffer+i);
		fputs(buffer+i, fp);
		putc('\r', fp);
		putc('\n', fp); }
	putc(0x1A, fp);
}

/*
 * Perform TG43 processing
 * - Update parallel port if required
 * - Pause if manual control enabled
 */
void tg43(unsigned char wflag)
{
	unsigned char c;

	c = (Cylinder > TGcyl) ? 0x0F : 0x00;
	if(PPort)
		out(PPort, c ? PPhigh : PPlow);

	if(TGman & wflag) {		// Manual control enabled
		if((TGman & 0x0F) != c) {		// Crossed boundary
			TGman = c | 0xF0;
			wopen(15, 10, 50, 4, (unsigned)video[AT_POP]|WSAVE|WBOX1|WCOPEN);
			wprintf("Head has crossed the cylinder %u boundary.", TGcyl);
			wputs("\nPress ENTER to proceed, ESC to cancel.");
			for(;;) switch(wgetc()) {
				case 0x1B:
					wclose();
					error("Abort by operator");
				case '\n' :
					wclose();
					return; } } }
}

/*
 * Calculate the correct sector data flag value for this sector
 */
unsigned sector_data_flag(char err, char msg)
{
	unsigned char s;
	s = 0;
	if(err) {			// Read error occured
		if(Bad) {		// Do we keep bad sectors?
			if(!(Result[1] & 0x96)) {
				if((Result[2] & 0x21) == 0x20) {
					s |= 4;
					wputs(msg ? " - Recording bad sector" : "b");
					goto keep; } } }
		wputs(msg ? " - Unavailable" : "u");
		return 0; }
keep:
	if(Result[2] & 0x40) {
		if(Dam) {
			if(Dam == 255) {
				Dam = 15;
				if(msg)
					newch("Deleted-Data");
				else
					wputc('d'); }
			s |= 2; } }
	return s+1;
}

/*
 * Read entire disk into file
 */
int read_disk(void)
{
	unsigned i, e;
	unsigned char f, sf[MSEC];

//	memset(buffer, 0, sizeof(buffer));
	if(fp = fopen(Directory, "r")) {
		fgets(Sflag, sizeof(Sflag), fp);
		wprintf("\n%s", Sflag);
		ptr = buffer;
		for(i=0; i < LNUM; ++i) {
			if(!fgets(ptr, LS1, fp))
				break;
			wprintf("\n%s", ptr);
			ptr += LS1; }
		fclose(fp);
		ptr = 0;
		wprintf("\nProceed (Y/N)?");
		getkey: switch(i = toupper(wgetc())) {
			default: goto getkey;
			case 'N' :
				return 255;
			case 'Y' : }
		wgotoxy(0, 2); wcleow(); wgotoxy(79, 0); }

	if(!(fp = fopen(Directory, "wb")))
		error("Cannot write file");
	edit();
	pwait("READ");
	reset();
	draw_scale();
	for(Cylinder = 0; Cylinder < Cylinders; ++Cylinder) {
		f = 255;
		Head = 0;
		Cstat(0);
		seek(Cylinder);
		tg43(0);
ra1:	if(Sskip[Cylinder] & 1) {
			Cstat('X');
			goto ra1a; }
		Cstat('A');
		if(analyze_track(f)) {
			if(Dana)
				break;
			Cstat('N');
			write_null();
			goto ra1a; }
		if(!(Hflag & 0x80))
			memset(Cmap, Cylinder, Stop);
		Cstat('R');
		for(i=0; i < Stop; ++i) {
			if(e=read_sector(i)) {
				if(Retry&&f) { f = 0; wputs(" - Reanalyzing"); goto ra1; } }
			sf[i] = sector_data_flag(e, 255);
			bumpdma(); }
		write_ana();
		DMA = SEG;
		for(i=0; i < Stop; ++i) {
			write_data(sf[i]);
			bumpdma(); }
		Cstat(BDONE);
ra1a:	if(Ss1) {
			f = 255;
			Head = 1;
ra2:		if(Sskip[Cylinder] & 2) {
				Cstat('X');
				goto ra3; }
			Cstat('A');
			i = analyze_track(f);
			if(Ss1 == 0x55) {
				if(i) {
					Ss1 = 0;
					newch("Single-sided");
					goto ra3; }
				else {
					Ss1 = 0xAA;
					newch("Double-sided"); } }
			else if(i) {
				if(Dana)
					break;
				Cstat('N');
				write_null();
				goto ra3; }
			if(!(Hflag & 0x80))
				memset(Cmap, Cylinder, Stop);
			Cstat('R');
			for(i=0; i < Stop; ++i) {
				if(e=read_sector(i)) {
					if(Retry&&f) { f = 0; wputs(" - Reanalyzing"); goto ra2; } }
				sf[i] = sector_data_flag(e, 255);
				bumpdma(); }
			write_ana();
			DMA = SEG;
			for(i=0; i < Stop; ++i) {
				write_data(sf[i]);
				bumpdma(); }
			Cstat(TDONE); }
ra3:	if(Dds == 0x55) {
			Dds = Head = 0;
			seek(4);
			if(analyze_track(255))
				error("Cannot read disk");
			if(readid())
				error("Cannot read disk");
			switch(Result[3]) {
			default: error("Seek error %u Result[3]");
			case 4 : Dds = 0;	break;
			case 2 : Dds = 0xAA; }
			recal(); delay(500); }
		if(!Cylinder)
			newch("%s-step", Dds ? "Double" : "Single"); }

	fclose(fp);
	showstat();
	return fp = 0;
}

/*
 * Format a track
 */
void format_track(void)
{
	unsigned i, o, r;
	unsigned char s;

//wprintf("\nF T%-2u H%u St%u Ss%02x", Cylinder, Head, Stop, Ssize);
	Timeout = TIMEOUT;
	initmode();
	r = Nintl = Resync = 0;
	if(format_changed())
		compute_gl(0);
	else if(Intl)
		interleave();
	if(!Nintl) {	// Default 1:1 map
		for(i=0; i < Stop; ++i)
			Imap[i] = i; }
retry:
	DMA = SEG;
	w_gotoxy(0, 1, swin); w_cleol(swin);
	for(i=o=0; i < Stop; ++i) {
		w_printf(swin, " %u", s=Smap[Imap[i]]);
		poke(DMA, o++, Cmap[Imap[i]]);
		poke(DMA, o++, Hmap[Imap[i]]);
		poke(DMA, o++, s);
		poke(DMA, o++, Ssize); }

	wrcmd(0x0D);
	wrfdc(Ssize);
	wrfdc(Stop);
	wrfdc(Glf);
	initdma(0x4A, o);
	waitirq(Ffill);

	result(7);
	if(Result[0] & 0xC0) {
		if(++r >= Retry) {
			diskerror("Format");
			return 255; }
		resync();
		goto retry; }
	return 0;
}

/*
 * Write an entire disk
 */
void write_disk(void)
{
	unsigned i, o, s, m;
	unsigned char c, d, h, hf, xf, lc, bs;

	h = hf = 255;
	lc = 0;
	if(!(fp = fopen(Directory, "rb")))
		error("Cannot read file");
	wputc('\n');
txt: switch(i = getc(fp)) {
		default:
			if((i < ' ') || (i > '~'))
				goto txt;
		case 0x0A :
			wputc(i);
			goto txt;
		case EOF :
			fclose(fp); fp = 0;
			error("File format error");
		case 0x1A : }
	pwait("WRITE");
	reset();
	draw_scale();

	while((i = getc(fp)) != EOF) {
		m = Mtran[i];
		c = getc(fp);		// Cylinder
		if(c >= Cylinders) {
			wputs("\nImage exceeds configured Cylinders");
			break; }
		d = getc(fp);		// Head
		if(Head = d & 0x0F)
			h = 0x55;
		if(hf && c) {
			if(h == 0x55)
				newch("Double-sided");
			else
				newch("Single-sided");
			newch("%s-step", Dds ? "Double" : "Single");
			hf = 0; }
		if(c != Cylinder) {
			while(lc < c) {
				Cylinder = lc++;
				Cstat(0); }
			Cylinder = c;
			Cstat(0);
			seek(c); }
		upCH();
		S->Mode = Mode = m;
		tg43(255);
		S->Stop = Stop = getc(fp);
		S->Ssize = Ssize = getc(fp);
		S->Eot = Eot = Stop; // ?
		if(!Stop) {
			Cstat('N');
			continue; }
		fget(Smap, Stop, fp);
		if(d & 0x80)		// Cylinder ID override
			fget(Cmap, Stop, fp);
		else
			memset(Cmap, Cylinder, Stop);
		if(d & 0x40)		// Head ID override
			fget(Hmap, Stop, fp);
		else
			memset(Hmap, Head, Stop);
//		delay(250);
		xf = ((Head ? 0x02 : 0x01)) & Sskip[Cylinder];
		if(!xf) {
			Cstat('F');
			format_track();
			Cstat('W'); }
		w_gotoxy(0, 1, swin); w_cleol(swin);
		for(bs=i=0; i < Stop; ++i) {
			w_printf(swin, " %u", Smap[i]);
			o = 0;
			s = 128 << Ssize;
			++STcount[ST_TOTAL];
			if(c = getc(fp)) {		// Data available
				if(--c & 1) {			// Compressed
					++STcount[ST_COMP];
					d = getc(fp);
					do {
						poke(DMA, o++, d); }
					while(--s); }
				else {					// Normal
					do {
						poke(DMA, o++, getc(fp)); }
					while(--s); }
				if(c & 2) {				// deleted
					if(Dam) {
						++STcount[ST_DAM];
						if(Dam == 255) {
							Dam = 15;
							newch("Deleted-Data"); } } }
				if(c & 4) {				// Bad
					++STcount[ST_BAD];
					if(!bs) {
						newch("Image track contains bad sectors");
						bs = 255; } }
				if(!xf)
					write_sector(i, c); }
			else
				++STcount[ST_UNAVAIL]; }
		if(xf)
			Cstat('X');
		else
			Cstat(Head ? TDONE : BDONE); }
	fclose(fp); fp=0;
	showstat();
}

/*
 * Perform head cleaning "scrub"
 */
void scrub(unsigned c)
{
	if(c >= Cylinders)
		c = Cylinders-1;
	upCYL();
	seek(c);
	delay(MDEL);
}

/*
 * Show numeric field in config display
 */
unsigned show_num(unsigned v, unsigned b)
{
	unsigned i, s;
	unsigned char stack[5];
	s = 0;
	do {
		i = (v % b);
		stack[s++] = (i > 9) ? i+('0'+7) : i + '0'; }
	while(v /= b);
	i = s;
	do {
		wputc(stack[--s]); }
	while(s);
	return i;
}

/*
 * Get numeric field for config display
 */
unsigned get_num(unsigned x, unsigned y, unsigned b, unsigned d, unsigned l, unsigned h)
{
	unsigned w, i, c, v, x1;
	unsigned char f;
	static unsigned char *gn_prompt[] = {
		"Enter value:",
		"",
		"Press: ENTER to accept new value.",
		"\x07ESC   to cancel with no change.",
		"\x07SPACE to restore original value.",
		"\x07BKSP  to remove last digit.",
		0 };

	prompt(gn_prompt, 5);
	wgotoxy(x, y);
	show_num(l, b);
	wputc('-');
	w = show_num(h, b);
	wputs(" : ");
	x1 = W_OPEN->WINcurx;
redef:
	f = 255;
	v = d;
	for(;;) {
		wgotoxy(x1, y);
		*mwin = video[AR_MAIN];
		i = show_num(v, b);
		while(i < w) {
			wputc(' ');
			++i; }
		*mwin = video[AR_MAIN];
		switch(c = wgetc()) {
		case ' ' : goto redef;
		case 0x1B: v = -1; goto doexit;
		case '\n' :
			if((v < l) || (v > h)) {
				beep(750, 200);
				continue; }
		doexit:
			*mwin = video[AT_MAIN];
			wgotoxy(x, y);
			wcleol();
			return v;
		case '\b' :
		case _KBS :
			*mwin = video[AT_MAIN];
			v /= b;
			f = 0;
			continue; }
		if(isdigit(c))
			c -= '0';
		else if((c >= 'A') && (c <= 'F'))
			c -= ('A'-10);
		else if((c >= 'a') && (c <= 'f'))
			c -= ('a'-10);
		else
			continue;
		if(c >= b)
			continue;
		if(f)
			v = f = 0;
		i = (v * b) + c;
		if(i <= h)
			v = i; }
}

/*
 * Prompt for format specific parameters
 */
int fmtprompt(char test)
{
	unsigned i;
	static char *mmenu[] = {
		"500 kbps FM",
		"300 kbps FM",
		"250 kbps FM",
		"500 kbps MFM",
		"300 kbps MFM",
		"250 kbps MFM",
		0 };
	static char *smenu[] = {
		"128",
		"256",
		"512",
		"1024",
		"2048",
		"4096",
		"8192",
		0 };

	wgotoxy(CFG_X, CFG_Y); wputs("Sectors/Track?");
	if((i = get_num(CFG_X+15, CFG_Y, 10, Fn, 1, 255)) == -1)
		return 0;
	wprintf("%u", Fn = i);
	wgotoxy(CFG_X, CFG_Y+1); wputs("Start sector?");
	if((i = get_num(CFG_X+15, CFG_Y+1, 10, Fss, 1, 255)) == -1)
		return 0;
	Fss = i;
	wclwin();
	wprintf("%s sided, %s step",
		Side1 ? "Double" : "Single",
		Dstep ? "Double" : "Single");
	if((Intl == 255) || !(i = Intl))
		i = 1;
	wprintf(", %u sectors/track, starting at %u, interleave: %u:1",
		Fn, Fss, i);
	wputs("\nData rate/type: ");
	if(wmenu(50, 7, WSAVE|WCOPEN|WBOX1|video[AT_POP], mmenu, &Fm))
		return 0;
	wprintf("%s, Sector size: ", mmenu[Fm]);
	if(wmenu(50, 7, WSAVE|WCOPEN|WBOX1|video[AT_POP], smenu, &Fs))
		return 0;
	wprintf("%s", smenu[Fs]);
	if(test) {
		wgotoxy(CFG_X, CFG_Y+2); wputs("Write pattern?");
		Wdata = get_num(CFG_X+15, CFG_Y+2, 16, Wdata, 0, 0xFFFF); }
	return FmtOK = 255;
}

/*
 * Setup maps for format
 */
void fmtsetup(void)
{
	unsigned i;

	S0.Mode = Fm;
	S0.Eot = S0.Stop = Fn;
	S0.Ssize = Fs;
	for(i=0; i < Fn; ++i)
		S0.Smap[i] = Fss+i;
	memcpy(S1, S0, sizeof(S1));
	memset(S0.Hmap, 0, Fn);
	memset(S1.Hmap, 1, Fn);
}

/*
 * Low-Level format function
 */
int format(unsigned char fmt)
{
	unsigned char f, d;

	wclwin();
	ptr = 0;
	if(Side1 == 0x55)
		ptr = "You MUST pick one/two sides";
	if(Dstep == 0x55)
		merror("You MUST pick single/double step");
	if(fmt) {
		if((Intl == 255) || !Intl)
			merror("You MUST specify interleave"); }
	if(ptr)
		error(ptr);

	if(fmt) {
		d = 'F';
		if(!fmtprompt(0))
			return;
		pwait("FORMAT"); }
	else {
		d = 'E';
		pwait("ERASE");
		memcpy(FMTsave, &Fn, FSSIZE);
		FMTflag = 0x55;
		Fn = 1; Fm = 5; Fs = 6; Fss = 1; Intl = 1; Ffill = 0xFF; }

	reset();
	fmtsetup();
	draw_scale();

	f = 255;
	for(Cylinder = 0; Cylinder < Cylinders; ++Cylinder) {
		Head = 0;
		Cstat(0);
		seek(Cylinder);
		upCH();
		tg43(255);
		if(Sskip[Cylinder] & 1)
			Cstat('X');
		else {
			Cstat(d);
			memset(S0.Cmap, Cylinder, Fn);
			format_track();
			if(f) { f=0; memcpy(S1.Smap, S0.Smap, Fn); }
			Cstat(BDONE); }
		if(Ss1) {
			Head = 1;
			upCH();
			if(Sskip[Cylinder] & 2)
				Cstat('X');
			else {
				Cstat(d);
				memset(S1.Cmap, Cylinder, Fn);
				format_track();
				if(f) { f=0; memcpy(S0.Smap, S1.Smap, Fn); }
				Cstat(TDONE); } } }
	fmtrest();
}

/*
 * Alignment/Test function
 */
void align(void)
{
	unsigned i, s, t, y, n, ct, df, sf[MSEC];
	static unsigned char bf, sm[32];
	static unsigned char *align_prompt[] = {
		"Insert formatted",
		"diskette",
		"",
		"A   = Analyze",
		"B   = Beep on/off",
		"D   = read Data",
		"F   = Format",
		"H   = Head 0/1",
		"I   = track Image",
		"P   = fmt Parms",
		"R   = Recal/seek",
		"S   = Step S/D",
		"W   = Write data",
		"Z   = Zero track",
		"0-9 = seek 0-90",
		"+/- = Seek +/-1",
		"X   = eXit",
		0 };
	static unsigned char sflag[] = { 'F', 'W', 'R', 'Z', 'D' };

	memset(sm, y = n = ct = i = 0, sizeof(sm));
rewin:
	wclwin();
	prompt(align_prompt, 1);
	if(!i)
		pwait("ALIGN/TEST");
	reset();
	if(Dds = 0x55) Dds = 0;
	wopen(20, 5, 60, 20, (unsigned)video[AT_MAIN]|WCOPEN|WSCROLL|WWRAP);
	analyze_track(0);
	wputc('\n');

restep:
	w_gotoxy(65, 0, swin);
	w_puts(Dds ? "Double" : "Single", swin);
	w_puts(" step", swin);
reshow:
	df = 0;
reshow1:
	upCYL();
	for(;;) {
		if(readid()) {
			wprintf("\n?");
			memset(sm, y = n = df = 0, sizeof(sm)); }
		else {
			t = Result[3];
			s = Result[5] & 0x1F;
			wprintf("\r%-3u%3u", t, s);
			if(sm[s] < 2) {
				++sm[s];
				if(t == ct)
					++y;
				else
					++n; }
			else {
				wprintf(" %u %u\n", y, n);
				if(!bf)
					beep(500+(y*100), 100);
				memset(sm, y=n=0, sizeof(sm)); } }

		wrfdc(0x04);
		wrfdc(Head ? (Drive|4) : Drive);
		result(1);
		t = Result[0];
		w_gotoxy(25, i=0, swin);
		do {
			s = sflag[i++];
			if(!(t & 0x80))
				s += ('a'-'A');
			w_putc(s, swin);
			t <<= 1; }
		while(i < sizeof(sflag));

		if(i = Lkey) {
			Lkey = 0;
			switch(toupper(i)) {
			case '?' :
			case _F1 : help(HALIGN);	continue;
			case ' ' : switch(i=wgetc()) {
				case 0x1B : xabort();
				case ' ' : i = 0; }
				Lkey = i; goto reshow;
			case '+' :
			case '=' :
			case _UA: ++ct; goto doseek;
			case '-' :
			case '_' :
			case _DA: if(ct) --ct; goto doseek;
			case 'X' : wclose(); return;
			case 'H' : Head = Head ? 0 : 1; upCH(); df=0;	continue;
			case 'B' : bf = bf ? 0 : 255;					continue;
			case 'S' : Dds = Dds ? 0 : 255;					goto restep;
			case 'R' : recal();
				delay(500);
				if(ct) {
					seek(ct);
					delay(MDEL); }
				goto reshow;
			case 'D' :
				analyze_track(0);
				wputs("\nData:");
				DMA = SEG;
				for(i=0; i < Stop; ++i) {
					wprintf(" %u", Smap[i]);
					sf[i] = sector_data_flag(read_sector(i), 0);
					bumpdma(); }
				wputc('\n');
				df = 255;
				goto reshow1;
			case 'I' :
				if(df) {
					sprintf(buffer, "$C%uH%u.IMD", Cylinder, Head);
					if(fp = fopen(buffer, "wb")) {
						write_header();
						putc(0x1A, fp);
						write_ana();
						DMA = SEG;
						for(i=0; i < Stop; ++i) {
							write_data(sf[i]);
							bumpdma(); }
						fclose(fp);
						wprintf("\n%s written\n", buffer);
						goto reshow1; } }
				beep(750, 200);
				continue;
			case 'W' :
				analyze_track(0);
				wputs("\nWrite:");
				for(i=0; i < 16384; ++i)
					pokew(SEG, i+i, Wdata);
				for(i=0; i < Stop; ++i) {
					wprintf(" %u", Smap[i]);
					DMA = SEG;
					write_sector(i, 0); }
				wputc('\n');
				goto reshow;
			case 'P' :
				wclose();
				wclwin();
				fmtprompt(255);
				goto rewin;
			case 'F' :
				wputs("\nFormat");
				if(!FmtOK) {
					wputs(" - Must set P)aramaters\n");
					continue; }
				s = Intl;
				if((Intl == 255) || !Intl)
					Intl = 1;
				fmtsetup();
				upCH();
				S->Lmsn = Lmsn = -1;
				memset(Cmap, Cylinder, Fn);
				format_track();
				Intl = s;
			case 'A' : analyze_track(0); wputc('\n');	goto reshow;
			case 'Z' : recal(); ct = 0; delay(500);		goto reshow; }
			if(isdigit(i)) {
				ct = (i-'0') * 10;
			doseek:
				seek(ct);
				upCH();
				delay(MDEL);
				goto reshow; } } }
}

/*
 * Configuration (Settings) item-name/prompt text
 */
unsigned char *Cprompt[] = {
	"Drive",			// 0
	"Cylinders",		// 1
	"Sides",			// 2
	"Double-step",		// 3
	"R/W    gap",		// 4
	"Format gap",		// 5
	"Format fill",		// 6
	"Full analysis",	// 7
	"Interleave",		// 8
	"Retries    ",		// 9
	"Keep bad sector",	// 10
	"500 kbps ->",		// 11
	"300 kbps ->",		// 12
	"250 kbps ->",		// 13
	0 };

/*
 * Display current configuration
 */
void show_config(unsigned n)
{
	unsigned char *p, v;
	wgotoxy(CFG_X, n+CFG_Y);
	wputf(Cprompt[n], 15);
	*mwin = video[AT_MAIN];
	wputs(": ");
	switch(n) {
	case 0 : wputc(Drive+'A');	return;
	case 1 : wprintf("%-3u", Cylinders);	return;
	case 2 : switch(Side1) {
		case 0x00 : p = "One";	break;
		case 0xFF : p = "Two";	break;
		default: p = "As original"; Side1 = 0x55; }
showp:	wputf(p, 15);
		return;
	case 3 : switch(Dstep) {
		case 0x00 : p = "Off";	break;
		case 0xFF : p = "On";	break;
		default: p = "As original"; Dstep = 0x55; }
		goto showp;
	case 4 :	p = Xgl; v = Gl;
	showgap:
		if(!p) { wprintf("%-3u", v); return; }
		wputs("Calculated");
		return;
	case 5 :	p = Xglf; v = Glf; goto showgap;
	case 6 :	wprintf("%02x", Ffill);			return;
	case 7 :	wputs(Dana ? "No " : "Yes");	return;
	case 9 :	wprintf("%-3u", Retry);			return;
	case 8 :
		switch(Intl) {
		case  0 : wputs("As read");		break;
		case 255: wputs("BestGuess");	break;
		default: wprintf("%u:1", Intl); }
		wcleol();
		return;
	case 10: wputs(Bad ? "Yes" : "No ");		return;
	case 11:
	case 12:
	case 13:	wprintf("%3u kbps", Mtext[Mtran[n-11]]); }
}

/*
 * Edit configuration
 */
void config(void)
{
	unsigned n;
	unsigned char *ip, *ip1;
	static unsigned p;
	static char *config_prompt[] = {
		"Select item to configure",
		"with the Up/Down arrows.",
		"",
		"Press: SPACE to change value",
		"\x07ESC   to exit",
		0 };
	wclwin();
reprompt:
	prompt(config_prompt, 5);
redraw:
	for(n=0; Cprompt[n]; ++n) {
		if(p == n) *mwin = video[AR_MAIN];
		show_config(n); }
	for(;;) {
		switch(wgetc()) {
		case '?' :
		case _K1  : help(HSET);				continue;
		case _KDA : if(++p >= n) p = 0;		goto redraw;
		case _KUA : p = (p ? p : n) - 1;	goto redraw;
		case 0x1B : wclwin(); return;
		case ' ' :
//			show_config(p);
			switch(p) {
			case 0 :
				if(++Drive >= Maxdrive)
					Drive = 0;
				goto redraw;
			case 1 :
				if((n = get_num(CFG_X+17, p+CFG_Y, 10, Cylinders, 1, 255)) != -1)
					Cylinders = n;
				goto reprompt;
			case 2 : ip = &Side1;
T0055FF:		switch(*ip) {
					case 0x00 : *ip = 0xFF;	goto redraw;
					case 0x55 : *ip = 0x00; goto redraw; }
				*ip = 0x55; goto redraw;
			case 3 : ip = &Dstep;	goto T0055FF;
			case 4 : ip = &Xgl; ip1 = &Gl;
OPTNUM:			if(!*ip) { *ip = 255; goto redraw; }
				if((n = get_num(CFG_X+17, p+CFG_Y, 10, *ip1, 1, 255)) != -1) {
					*ip = 0;
					*ip1 = n; }
				goto reprompt;
			case 5 : ip = &Xglf; ip1 = &Glf; goto OPTNUM;
			case 6 :
				if((n = get_num(CFG_X+17, p+CFG_Y, 16, Ffill, 0, 255)) != -1)
					Ffill = n;
				goto reprompt;
			case 7 : Dana = Dana ? 0 : 255;		goto redraw;
			case 8 :
				switch(Intl) {
				case 0  : Intl = 255;	goto redraw;
				default : Intl = 0;		goto redraw;
				case 255: wcleol();
					if((n = get_num(CFG_X+17, p+CFG_Y, 10, 1, 1, 99)) != -1)
						Intl = n;
					goto reprompt; }
			case 9 :
				if((n = get_num(CFG_X+17, p+CFG_Y, 10, Retry, 0, 255)) != -1)
					Retry = n;
				goto reprompt;
			case 10 :	Bad = Bad ? 0 : 255;	goto redraw;
			case 11 :
			case 12:
			case 13:
				if(++Mtran[n=p-11] > 2) Mtran[n] = 0;
				Mtran[n+3] = Mtran[n] + 3;
				goto redraw;
	} } }
}

/*
 * Get current directory name
 */
void getcurrent(void)
{
	Directory[Dip=0] = get_drive() + 'A';
	Directory[1] = ':';
	Directory[2] = '\\';
	getdir(Directory+3);
}

/*
 * Select a file via window panel
 */
int selectfile(unsigned char *match, char new)
{
	unsigned a, i, Nt, Pt;
	unsigned char name[13], *n[SFILES], pool[SFILES*9], *p;

	// Find end of directory index
	if(Dip)
		*Dip = 0;
newdir:
	Dip = Directory; while(*Dip) ++Dip;
	if(*(Dip-1) == '\\')
		--Dip;
	*Dip = '\\';
	// Scan for all files & build name list
	stpcpy(Dip+1, "*.*");
	Nt = Pt = 0;
	if(!find_first(Directory, DIRECTORY, p = name, &i, &i, &a, &i, &i)) do {
		if(Nt >= SFILES)
			break;
		if(a & DIRECTORY) {
			if(!strcmp(p, "."))
				continue;
			n[Nt++] = &pool[Pt];
			pool[Pt++] = 1;	// Insure sorts first!
			while(pool[Pt++] = *p++);
			continue; }
		while(*p) {
			if(!strcmp(p, match)) {
				*p = 0;	p = name;
				n[Nt++] = &pool[Pt];
				while(pool[Pt++] = *p++);
				break; }
			++p; } }
	while !find_next(p=name, &i, &i, &a, &i, &i);

	*Dip = 0;
	if(new || !Nt) goto manent;

	// Sort the name list
	for(i=0; i < Nt; ++i) {
		for(a=i+1; a < Nt; ++a) {
			if(strcmp(n[a], n[i]) < 0) {
				p = n[i];
				n[i] = n[a];
				n[a] = p; } } }
	// Convert directory pref
	for(i=0; i < Nt; ++i) {
		if(*(p = n[i]) != 1)
			break;
		while(p[1]) {
			*p = p[1];
			++p; }
		*p = '\\'; }

	i = ((Nt + 4) / 5) + 2;
	wopen(4, 12-(i/2), 71, i, REVERSE|WSAVE|WBOX1|WCOPEN);
	wintitle(0, Directory);
	a = 0;
	wcursor_off();
draw:
	for(i=0; i < Nt; ++i) {
		wgotoxy((i % 5) * 14, i / 5);
		if(i == a) *W_OPEN = NORMAL;
		wputs(n[i]);
		*W_OPEN = REVERSE; }
	for(;;) switch(wgetc()) {
		case _KLA : if(a) { --a; goto draw; }
		case _KEN : a = Nt-1; goto draw;
		case _KRA : if(++a < Nt) goto draw;
		case _KHO : a = 0;	goto draw;
		case _KUA : if(a > 4) a -= 5; goto draw;
		case _KDA : if((a+5) < Nt) a += 5; goto draw;
		case '\n' :
			*Dip = '\\';
			p = n[a]; while(p[1]) ++p;
			if(*p == '\\') {		// Directory
				*p = 0;
				if(strcmp(p = n[a], "..")) {	// Not UP
					stpcpy(Dip+1, p);
		newdir1:	wclose();
					goto newdir; }
				// Go up one directory
				while(Dip > Directory) {
					if(*--Dip == '\\') {
						*Dip = 0;
						goto newdir1; } }
				getcurrent();
				goto newdir1; }
			concat(Dip+1, n[a], match);
			wclose();
			return 255;
		case '?' :
		case _K1 : help(HFILE);		continue;
		case _K2 :
			wclose();
		manent:
			stpcpy(pool, Directory); Pt = Dip;
			wopen(4, 10, 71, 4, REVERSE|WSAVE|WBOX1|WCOPEN);
			wintitle(0, Directory);
			wclwin(); wputs("Enter filename (or directory ending with '\\'):");
			*Dip = '\\';
			*(Dip+1) = a = 0;
		manent1:
			switch(wgets(0x100, 1, p = Dip = Directory, sizeof(Directory)-1)) {
			case _K1 : help(HENTRY);
			default : goto manent1;
			case '\n' :
				new = 0;
				while(i = *p++) switch(i) {
					case ':' :
					case '\\': Dip = p-1; a = 0; continue;
					case '.' : a = 255; }
				wclose();
				if(*(p-2) == '\\') goto newdir;
				if(!a)
					stpcpy(p-1, match);
				return 255;
			case 0x1B:
				stpcpy(Directory, pool);
				Dip = Pt;
				if(Nt && !new) {
					wclose();
					goto newdir; } }
		case 0x1B :
		case _K10 :
			wclose();
			return 0; }
}

/*
 * Skip to next non-blank
 */
int skip(void)
{
	while(isspace(*ptr))
		++ptr;
	return *ptr;
}

/*
 * Obtain a numeric value from command input
 */
unsigned number(unsigned base, unsigned low, unsigned high)
{
	unsigned c, v;
	unsigned char f;
	v = f = 0;
	skip();
	for(;;) {
		if(isdigit(c = *ptr))
			c -= '0';
		else if((c >= 'A') && (c <= 'F'))
			c -= ('A'-10);
		else if((c >= 'a') && (c <= 'f'))
			c -= ('a'-10);
		else switch(c) {
			case 0 :
			case ',':
				if(f) {
					if((v < low) || (v > high))
						error(((base == 16) ?
							"Value (%x) out of range: %x-%x\n" :
							"Value (%u) out of range: %u-%u\n" ),
							v, low, high);
					return v; }
			default: error("Bad number"); }
		if(c >= base)
			error("Bad numeric value");
		f = 255;
		v = (v * base) + c;
		++ptr; }
}

/*
 * Test for additional command option value (comma separated)
 */
int tnext(void)
{
	if(*ptr == ',') {
		++ptr;
		return 255; }
	if(*ptr)
		error("Syntax error");
	return 0;
}

/*
 * Process a single command line option
 */
void cmdparm(unsigned char *fm)
{
	unsigned c, i;
	unsigned char d;
	static unsigned char nest = 5;
	switch(skip()) {
	case '$' :
		ptr1 = ARGV[c=i=0];
		while(*ptr1) {
			if((buffer[i++] = *ptr1++) == '\\')
				c = i; }
		strcpy(buffer+c, ptr+1);
		ptr1 = buffer;
		goto dofile;
	case ';' :
	case 0:	return; }

	ptr1 = ptr;
	switch(c = (toupper(*ptr++) << 8) | toupper(*ptr++)) {
	case '/A' : Drive = 0;								return;
	case '/B' : Drive = 1;								return;
	case '/C' : Drive = 2;								goto set4;
	case '/D' : Drive = 3;								set4:
	case '/4' :	Maxdrive = 4;							return;
	case '/S' :
			FDC = 0x372;
			if(*ptr)
				FDC = number(16, 0, 65535);
			return;
	case '/F' : Dana = 0;								return;
	case '/H' : Xhelp = 255;							return;
	case '/M' : memcpy(video, mvideo, sizeof(video));	return;
	case '/N' : Dam = 0;								return;
	case '/K' : Bad = 255;								return;
	case 'C=' : Cylinders = number(10, 1, 255);			return;
	case 'D=' : stpcpy(Directory, ptr);					return;
	case 'X=' :	d = 3;									doexc:
		c = i = number(10, 0, 255);
		if(tnext())
			c = number(10, 0, 255);
		while(i <= c)
			Sskip[i++] |= d;
		return;
	case 'R=' : Retry = number(10, 0, 255);					return;
	case 'S=' :	Side1 = (number(10, 1, 2)-1) && 0xFF;		return;
	case '?'<<8:
	case '/?' :
	case '-?' :	help(HCMD);	exit(0); }
	if(*ptr++ == '=') switch(c) {
		case 'T5' : c = 0;		goto dotran;
		case 'T3' : c = 1;		goto dotran;
		case 'T1' : c = 2;		dotran:
			switch(number(10, 0, 65535)) {
			default: error("Bad transfer rate");
			case 500: i = 0;	break;
			case 300: i = 1;	break;
			case 250: i = 2; }
			Mtran[c] = i; Mtran[c+3] = i+3;
			return;
		case 'LR': switch(number(10, 0, 65535)) {
			default: error("Bad rate");
			case 500: Lrate = 0xF0;	break;
			case 300: Lrate = 0xF1;	break;
			case 250: Lrate = 0xF2; }
			return;
		case 'IL' :	Intl = *ptr ? number(10, 1, 99) : 255;	return;
		case 'X0' : d=1;									goto doexc;
		case 'X1' : d=2;									goto doexc;
		case 'FF' : Ffill = number(16, 0, 255);				return;
		case 'DS' : Dstep = number(10, 0, 1) && 0xFF;		return;
		case 'CP' : Cpass = number(10, 1, 9);				return;
		case 'GW' : Xgl = 0; Gl = number(10, 0, 255);		return;
		case 'GF' : Xglf =0;Glf = number(10, 0, 255);		return;
		case 'SD' : SKdel= number(10, 0, 2000);				return;
		case 'SR' : SPsrt = 16 - number(10, 1, 16);			return;
		case 'HL' : SPhlt = number(10, 2, 254) / 2;			return;
		case 'HU' : SPhut = number(10, 16, 240) / 16;		return;
		case 'TG' :
			switch(toupper(*ptr++)) {
			case 'P' :
				if((PPort = number(16, 1, 65535)) < 4)
					PPort = peekw(0x40, (PPort*2)+6);
				switch(PPort) {
				default:sprintf(Scale, "\nWarning: non-standard LPT address: %04x\n", PPort);
				case 0x378:
				case 0x278:
				case 0x3BC: }
				if(tnext()) {
					TGcyl = number(10, 0, 255);
					if(tnext()) {
						PPlow = number(16, 0, 255);
						if(tnext())
							PPhigh = number(16, 0, 255); } }
				return;
			case 'M' :
				TGman = 0xF0;
				if(tnext())
					TGcyl = number(10, 0, 255);
				return; }
			error("Bad TG= selection"); }

dofile:
	concat(S1.Smap, ptr1, ".CMD");
	if(i = fopen(S1.Smap, fm)) {
		if(!--nest)
			error("CMD nested too deep");
		while(fgets(ptr = S1.Smap, MSEC-1, i))
			cmdparm("rvq");
		fclose(i);
		++nest; }
}

/*
 * Sector exclusion mapping function
 */
void ssmap(void)
{
	unsigned i;
	static unsigned char p;
	unsigned char c, d;
	wclwin(); wgotoxy(30, d=0); wputs("Track Exclusion map");
	wgotoxy(10, 19);
	wputs("\x1B\x18\x19\x1A=select, SPACE=toggle, F1=Help, F2=Toggle-ALL, ESC=exit");
	for(;;) {
		for(i=0; i < MSEC; ++i) {
			c = Sskip[i];
			if(i == p) *W_OPEN = video[AR_MAIN];
			wgotoxy((i/16)*5, (i%16)+2); wprintf("%3u", i);
			*W_OPEN = video[AH_MAIN]; wputc(" \x19\x18\x12"[c&3]);
			*W_OPEN = video[AT_MAIN]; }
		switch(wgetc()) {
		case '?':
		case _K1: help(HEXCL);	continue;
		case _K2:
			d = (d+1) & 3;
			for(i=0; i < MSEC; ++i)
				Sskip[i] = d;
			continue;
		case _KUA: --p;		continue;
		case _KDA: ++p;		continue;
		case _KRA: p += 16;	continue;
		case _KLA: p -= 16; continue;
		case ' ' : Sskip[p] = (Sskip[p] + 1) & 0x03;	continue;
		case 0x1B:	return; } }
}

/*
 * Main program
 */
main(int argc, char *argv[])
{
	unsigned i, j;
	static unsigned char *main_prompt[] = {
		"S)ettings",
		"R)ead disk  into file",
		"W)rite disk from file",
		"F)ormat disk",
		"E)rase disk",
		"A)lignment/test",
		"C)lean head",
		"T)est RPM",
		"X)clusion map",
		"",
		"F1  = Help",
		"ESC = exit",
		0 };
	static unsigned char *scrub_prompt[] = {
		"Make sure CYLINDERS matches drive",
		"",
		"Insert cleaning diskette",
		"",
		"Press: ENTER to proceed",
		"\x07ESC  to cancel",
		0 };

	// Determine video type
	wopen(0, 0, 10, 10, 0); wclose();
	if(W_BASE == 0xB000)		// Monochrome - switch attributes
		memcpy(video, mvideo, sizeof(video));

	// Get default directory and process command line arguments
	getcurrent();
	ptr = "$IMD";
	cmdparm("r");

	for(i=1; i < argc; ++i) {
		ptr = argv[i];
		cmdparm("rvq"); }

	// Establish 32k DMA segment aligned on 32k boundary
	if(!(SEG = alloc_seg(4096)))		// 64k segment
		abort("Out of memory");
	SEG = (SEG + 0x7FF) & 0xF800;		// Advance to nearest 32k boundary

	// Open windows & display startup messages
	swin = wopen(0, 0, 80, 2, (unsigned)video[AT_STAT]|WSAVE|WCOPEN);
	wgotoxy(23, 1); wputs("** ImageDisk "#VERSION" / "#__DATE__" **");
	pwin = wopen(0, 2, 80, 3, (unsigned)video[AT_STAT]|WSAVE|WCOPEN);
	wgotoxy(13, 1); wputs("Copyright 2005-"#CYEAR" Dave Dunfield - All rights reserved.");
	if(*Scale) wputs(Scale);
	mwin = wopen(0, 5, 80, 20, (unsigned)video[AT_MAIN]|WSAVE|WCOPEN|WWRAP|WSCROLL);

	wcursor_off();

	// Build scale for display
	for(i=0; i < 80; ++i) {
		if(!(i%5))
			j = (i%10) ? CFIVE : (i/10)+'0';
		else
			j = CONE;
		Scale[i] = j; }

	// Take over floppy hardware interrupt
	asm {
		MOV		AX,350Eh		; Get vector 14 (FDC interrupt)
		INT		21h				; Ask DOS
		MOV		DGRP:_FIseg,ES	; Save segment
		MOV		DGRP:_FIoff,BX	; Save offset
		MOV		DX,OFFSET FINT	; New offset
		PUSH	DS
		PUSH	CS
		POP		DS
		MOV		AX,250Eh		; Set vector 14
		INT		21h				; Ask DOS
		POP		DS
	}

	// We return here in error/abort conditions
	ptr = setjmp(xmain);
	if(!Xhelp)					// Show help first time only
		help(HINDEX);

top:
	deselect_drive();
	if(ptr) {					// Error message to display
		wputc('\n');
		wcleol();
		wputs(ptr);
		wputs(" - press ENTER for main menu");
		ptr = 0;
		while(wgetc() != '\n'); }
	wclwin();
	prompt(main_prompt, 10);
	for(i=0; Cprompt[i]; ++i)	// Display current settings
		show_config(i);

	// Prompt for and execute top-level command
recmd:
	switch(toupper(wgetc())) {
	case 'S' : config(); goto top;
	case 'R' :
		memset(buffer, 0, sizeof(buffer));
	rdagain:
		wclwin(); wputs("Read disk into file");
		if(selectfile(".IMD", 255)) {
			wclwin(); wprintf("Read into %s", Directory);
			if(read_disk()) goto rdagain;
			ptr = "Read complete"; }
		goto top;
	case 'W' :
		wclwin();
		if(Dstep == 0x55)
			error("You MUST pick single/double step");
		wputs("Write disk from file");
		if(selectfile(".IMD", 0)) {
			wclwin(); wprintf("Write from %s", Directory);
			write_disk();
			ptr = "Write complete"; }
		goto top;
	case 'F' :
		format(255);
		goto top;
	case 'E' :
		format(0);
		goto top;
	case 'A' : align();				goto top;
	case 'C' :
		wgotoxy(5, 0); wputs("Number of passes");
		if((i = get_num(25, 0, 10, Cpass, 1, 9)) != -1) {
			wprintf("%u", Cpass = i);
			prompt(scrub_prompt, 5);

			pwait("CLEAN-HEADS");
			wclwin(); wputs("Cleaning disk heads, pass:");
			reset(); draw_scale();
			if(Dds == 0x55) Dds = 0;
			j = (Cylinders < 16) ? 2 : (Cylinders / 8);
			for(Head = 0; Head < Cpass;) {
				wprintf(" %u", ++Head);
				for(i=0; i < Cylinders; i += j) {
					scrub(i+(j-1));
					scrub(i); }
				draw_scale();
				recal(); } }
		goto top;
	case 'T' :	rpm();				goto top;
	case 'X' :	ssmap();			goto top;
	case '?' :
	case _K1 :	help(HMAIN);		goto top;
	case 0x1B:
		deselect_drive();
		asm {
			MOV		DX,DGRP:_FIoff	; Get offset
			PUSH	DS				; Save DS
			MOV		DS,DGRP:_FIseg	; Get segment
			MOV		AX,250Eh		; Set vector
			INT		21h				; Ask DOS
			POP		DS				; Restore DS
			}
		wcloseall(0);
		asm {
			XOR		AH,AH			; 00 = Reset
			MOV		DL,AH			; Drive A
			INT		13h				; BIOS: Reset diskette
		}
		return; }
	beep(750, 200);
	goto recmd;
}
