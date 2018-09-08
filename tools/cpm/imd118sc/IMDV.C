/*
 * ImageDisk Viewer
 *
 * This program allows examination of the content of a .IMD image file.
 *
 * This program is compiled using my own development tools, and will not
 * build under mainstream compilers without significant work. It is being
 * provided for informational purposes only, and I provide no support for
 * it, technical or otherwise.
 *
 * Copyright 2007-2012 Dave Dunfield
 * All rights reserved.
 *
 * For the record: I am retaining copyright on this code, for the purpose
 * of keeping a say in it's disposition however I encourage the use of ideas,
 * algorithms and code fragments contained herein to be used in the creation
 * of compatible programs on other platforms (eg: Linux).
 */
#include <stdio.h>
#include <window.h>
#include <file.h>
#include "version.h"

struct WINDOW
	*swin,			// Status window
	*iwin,			// Informational window
	*mwin;			// Main window

HANDLE
	*fh;			// Open file handle

FILE
	*fp,			// General file pointer
	*fps;			// Output file

int
	Edit,			// Edit position
	Hl, Hh;			// Highlight portion

unsigned
	Nsect,			// Current # sectors
	Secsize,		// Sector size
	Sector,			// Current logical sector
	Psector,		// Current physical sector
	Track,			// Current track
	Tracks,			// Total number of track records
	Mstring = 5,	// Minimum scan-string length
	Width = 80,		// scan-string output width
	H, L,			// File position
	Ft, Fs, Fp,		// Find (search) Track/Sector/Position
	T[512][2],		// Track offsets
	S[256][2];		// Sector offsets (within current track)

unsigned char
	*File,			// File name pointer
	*Custom,		// Custom character set name
	*Ptr,			// General pointer
	Ileave = 255,	// Interleave option
	RDonly = 255,	// Read-only option
	Csearch = 255,	// Case insensitive search option
	Xor,			// Data XOR value
	Charset,		// Selected character set
	Mode,			// Current track mode
	Smode,			// Sector record type
	Sflag,			// Status update flag
	Fflag,			// Find-flag
	Fcs,			// Find case-convert
	Cylinder,		// Current cylinder
	Head,			// Current head
	Ssize,			// Current sector size
	Finput[65],		// Search input buffer
	Fstring[65],	// Search string
	Smap[256],		// Sector numbering map
	Cmap[256],		// Cylinder numbering map
	Hmap[256],		// Head numbering map
	Omap[256],		// Sector ordering map (for interleave)
	Cset[256],		// Custom character set
	Secdata[8192];	// Sector data buffer

unsigned			// Sector size translate table
	Sector_size[] = { 128, 256, 512, 1024, 2048, 4096, 8192 };

unsigned char *Sector_mode[] = {	// Data mode translate table
	"500Khz  FM",
	"300Khz  FM",
	"250Khz  FM",
	"500Khz MFM",
	"300Khz MFM",
	"250Khz MFM" };

unsigned char ebcdic[] = {	// ASCII to Ebcdic translation table
0x00,0x01,0x02,0x03,0x9C,0x09,0x86,0x7F,0x97,0x8D,0x8E,0x0B,0x0C,0x0D,0x0E,0x0F,
0x10,0x11,0x12,0x13,0x9D,0x85,0x08,0x87,0x18,0x19,0x92,0x8F,0x1C,0x1D,0x1E,0x1F,
0x80,0x81,0x82,0x83,0x84,0x0A,0x17,0x1B,0x88,0x89,0x8A,0x8B,0x8C,0x05,0x06,0x07,
0x90,0x91,0x16,0x93,0x94,0x95,0x96,0x04,0x98,0x99,0x9A,0x9B,0x14,0x15,0x9E,0x1A,
' ', 0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,'[', '.', '<', '(', '+', '!',
'&', 0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,0xB0,0xB1,']', '$', '*', ')', ';', '^',
'-', '/', 0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,'|', ',', '%', '_', '>', '?',
0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,0xC0,0xC1,0xC2,'`', ':', '#', '@', 0x27,'=', '"',
0xC3,'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,
0xCA,'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 0xCB,0xCC,0xCD,0xCE,0xCF,0xD0,
0xD1,'~', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,
0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,
'{', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 0xE8,0xE9,0xEA,0xEB,0xEC,0xED,
'}', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 0xEE,0xEF,0xF0,0xF1,0xF2,0xF3,
0x5C,0x9F,'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0xFA,0xFB,0xFC,0xFD,0xFE,0xFF };

unsigned char *search_text[] = {		// Text entry form for search-text
	76<<8|3,
	"\x01\x00\x40Text?",
	0 };
unsigned char *search_byte[] = {		// Text entry form for search-Hex
	76<<8|3,
	"\x01\x00\x40Bytes?",
	0 };

// Command line help
unsigned char help[] = { "Copyright 2007-"#CYEAR" Dave Dunfield - All rights reserved.\n\
\nUse: IMDV image[.IMD] [options]\n\n\
Opts:	/C		- Case-sensitive searches\n\
	/E		- use Ebcdic displays\n\
	/I		- ignore Interleave in sector ordering\n\
	/W		- allow Write to file (edit)\n\
	C=file[.CHR]	- use Custom character-set		[none]\n\
	E=file[.CHR]	- Edit custom character-set		[no]\n\
	M=n		- Minimum scan-string length		[5]\n\
	S=file[.TXT]	- extract scan-Strings to file		[no]\n\
	W=n		- Width of scan-string output		[80]\n\
	X=xx		- Xor sector data with hex value	[00]\n" };

// Main menu help
unsigned char mhelp[] = { "\n\n\n\
   Up/Dn	= Select track\n\
 Left/Right	= Select sector\n\
 Home/End	= Start/End of track sector list\n\
^Home/^End	= First/Last sector on disk\n\
 PgUp/PgDn	= Page data display\n\
   ENTER	= Edit sector\n\
	F1		= Display help\n\
	F2		= Toggle character-set display\n\
	F3		= Search for string from current track\n\
	F4		= Search again\n\
	F5		= Display all strings from current track onward\n\
  ESC/F10	= Exit" };

// Sector editor help
unsigned char ehelp[] = { "\n\n\n\
 Left/Right	= Move by one position\n\
   Up/Dn	= Move by one line\n\
 PgUp/PgDn	= Move by one screen\n\
 Home/End	= Move to start/end of line\n\
^Home/^End	= Move to start/end of sector\n\
	F1		= Display help\n\
	F2		= Toggle character-set display\n\
	F3		= Toggle HEX/ASCII edit mode\n\
   F10		= Exit" };

// Character-set editor help
unsigned char chelp[] = { "\n\n\n\n\n\n\n\
   Up/Dn	= Move by -/+ 1 position\n\
 Left/Right	= Move by -/+ 16 positions\n\
	F1		= Display help\n\
	F2		= Set character unprintable\n\
	F4		= Load ASCII  character set\n\
	F5		= Load EBCDIC chraacter set\n\
   F10		= Exit\n\
	c		= Set printable character translation" };

// Index values for video attributes
#define	INORM	0		// Informational window - normal
#define	MNORM	1		// Main window - normal
#define	ENORM	2		// Edit window - normal
#define	IHILITE	3		// Informational window - hilight
#define	MHILITE	4		// Main window - hilight
#define	EHILITE	5		// Edit window - hilight
#define	MWEAK	6		// Main window - de-emphasis

unsigned char
	*Attrs,				// Active attributes
	Cattrs[]  = {		// Color attributes
		0x47,			// Informational window
		0x17,			// Main window
		0x67,			// Edit window
		0x4E,			// Informational hilight
		0x1E,			// Main hilight
		0x6E,			// Edit hilight
		0x13 },			// Main de-emphasis
	Mattrs[] = {		// Monochrome attributes
		0x70,			// Informational window
		0x07,			// Main window
		0x70,			// Edit window
		0x07,			// Informational hilight
		0x70,			// Main hilight
		0x07,			// Edit hilight
		0x07 };			// Main de-emphasis

/*
 * Display formatted error message & exit
 */
register error(unsigned args)
{
	unsigned char buffer[81];

	_format_(nargs() * 2 + &args, buffer);
	if(swin) {
		wclose();
		wclose();
		wclose(); }
	fputs(buffer, stderr);
	exit(-1);
}

/*
 * Update status line with formatted message
 */
register status(unsigned args)
{
	unsigned char buffer[81];

	_format_(nargs() * 2 + &args, buffer);
	if(!swin) {
		fputs(buffer, stderr);
		putc('\n', stderr);
		exit(-1); }
	w_clwin(swin);
	w_puts(buffer, swin);
	Sflag = 255;
}

/*
 * Get charcter from file & record position
 */
int Getc()
{
	unsigned char c;

	if(!read(&c, 1, fh))
		return -1;
	if(!++L)
		++H;
	return c;
}

/*
 * Get character from file & record position, error on EOF
 */
int Gete()
{
	unsigned char c;

	if(!read(&c, 1, fh))
		error("Corrupt image file");
	if(!++L)
		++H;
	return c;
}

/*
 * Get block of data from file  & update position counter
 */
void Getb(unsigned char *dest, unsigned size)
{
	unsigned i;

	if(size) {
		if(read(dest, size, fh) != size)
			error("Corrupt image file");
		i = L;
		L += size;
		if(L < i)
			++H; }
}

/*
 * Get Y/N response
 */
int yn(void)
{
	switch(wgetc()) {
	case 'y' :
	case 'Y' : return 255;
	case 'n' :
	case 'N' :
	case 0x1B: return 0; }
}

/*
 * Wait for SPACE or ESC
 */
int space(void)
{
	for(;;) switch(wgetc()) {
		case ' ' : return 0;
		case _K10:
		case 0x1B: return 255; }
}

/*
 * Display help screen
 */
void display_help(unsigned char *screen)
{
	unsigned char c;

	wclwin();
	while(c = *screen++) {
		if(c == '\t') {						// Expand tabs  at 4-char intervals
			do
				wputc(' ');
			while(mwin->WINcurx & 3); }
		else
			wputc(c); }

	wgotoxy(0, mwin->WINheight-1);
	wputs("Press SPACE to continue.");
	space();
}

/*
 * Invert bits in sector data according to XOR mask
 */
void invert(void)
{
	unsigned i;
	if(Xor) {
		for(i=0; i < Secsize; ++i)
			Secdata[i] ^= Xor; }
}

/*
 * Read a track from the input file
 */
int read_track(void)
{
	unsigned i;
	unsigned char s;

	// Read trach header & validate
	if((i = Getc()) == EOF)			// Mode byte if another track
		return -1;

	if((Mode = i) > 5)					// Date mode
		error("Bad mode value");
	Cylinder = Gete();					// Cylinder number
	if(((Head = Gete()) & 0x3F) > 1)	// Head number
		error("Bad head value");
	Nsect = Gete();						// # sectors/track
	if((Ssize = Gete()) > 6)			// Encoded sector size
		error("Bad sector-size value");
	Secsize = Sector_size[Ssize];		// Actual sector size

	Getb(Smap, Nsect);					// Sector numbering map

	if(Head & 0x80)
		Getb(Cmap, Nsect);							// Custom cylinder map
	else
		memset(Cmap, Cylinder, sizeof(Cmap));		// Otherwise, all same

	if(Head & 0x40)
		Getb(Hmap, Nsect);							// Custom head map
	else
		memset(Hmap, Head & 0x3F, sizeof(Hmap));	// Otherwise, all same

	// Scan sectors & record offsets (this track only)
	for(i=0; i < Nsect; ++i) {
		S[i][0] = L; S[i][1] = H;
		if(s = Gete()) {
			if((s-1) & 0x01)
				Gete();			// Compressed
			else
				Getb(Secdata, Secsize); } }

	return 0;
}

/*
 * Read current physical sector from input file
 */
void read_sector(void)
{
	lseek(fh, H=S[Psector][1], L=S[Psector][0], 0);		// Position
	if(Smode = Gete()) {
		if((Smode-1) & 1)
			memset(Secdata, Gete(), Secsize);
		else
			Getb(Secdata, Secsize);
		invert(); }
}

/*
 * Build sector re-numbering map for current track interleave
 */
void map_sectors(void)
{
	unsigned i, j;
	unsigned char map[256], c;

	// Begin with unsorted list
	for(i=0; i < Nsect; ++i)
		Omap[i] = i;

	// If interleave is enabled, build index by sorting Sector
	// numbering map and re-order our list to match.
	if(Ileave) {
		memcpy(map, Smap, Nsect);			// Copy to preserve original
		for(i=0; i < Nsect; ++i) {
			for(j=i+1; j < Nsect; ++j) {
				if(map[j] < map[i]) {
					c = map[j];				// Sort sector map
					map[j] = map[i];
					map[i] = c;
					c = Omap[j];			// Copy movement to interleave
					Omap[j] = Omap[i];
					Omap[i] = c; } } } }
}

/*
 * Display printable character with character-set translation (if enabled)
 */
void showchar(unsigned char c)
{
	if(Charset)
		c = ebcdic[c];
	wputc( ((c >= ' ') && (c < 0x7F)) ? c : '.');
}

/*
 * Draw the edit screen
 */
void draw_edit(unsigned char Na, unsigned char Ha)
{
	unsigned a, i, j, p;
	a = Edit;
	*mwin = Na;
	wgotoxy(0, 0); wcleol();
	for(i=0; i < 16; ++i) {
		wgotoxy(0, i+1);
		wcleol();
		if(a >= Secsize)
			continue;
		wprintf(" %04x ", p=a);
		for(j=0; j < 16; ++j) {
			if(!(j & 3))
				wputc(' ');
			*mwin = ((p >= Hl) && (p < Hh)) ? Ha : Na;
			wprintf(" %02x", Secdata[p++]); }
		wgotoxy(62, i+1);
		for(j=0; j < 16; ++j) {
			*mwin = ((a >= Hl) && (a < Hh)) ? Ha : Na;
			showchar(Secdata[a++]); }
		*mwin = Na; }
	wgotoxy(0, 17); wcleol();
}

/*
 * Convert input character to HEX nibble
 */
int ctoh(unsigned c)
{
	if(isdigit(c))
		return c - '0';
	if((c >= 'a') && (c <= 'f'))
		return c - ('a'-10);
	if((c >= 'A') && (c <= 'F'))
		return c - ('A'-10);
	return -1;
}

/*
 * Cycle through character set options
 */
void charset(unsigned step)
{
	unsigned i;
	unsigned char c, *p;

	if(step) switch(++Charset) {
		case 2 :				// Ebcdic -> Custom
			if(Custom)
				goto swapch;
			Charset = 0;
			break;
		case 3 :				// Custom -> ASCII
			Charset = 0;
		swapch:
			for(i=0; i < 256; ++i) {
				c = Cset[i];
				Cset[i] = ebcdic[i];
				ebcdic[i] = c; } }

	// Update display with currently selected character set indicator
	w_gotoxy(72, 5, iwin);
	switch(Charset) {
	default: Charset = 0;
	case 0 : p = "ASCII";	break;
	case 1 : p = "EBCDIC";	break;
	case 2 : p = Custom; }
	w_printf(iwin, "%8s", p);
}

/*
 * Sector editor
 */
void edit(void)
{
	int c, d;
	unsigned x, y, ss;
	unsigned char so, rd, cf, *p;
	static int Offset;
	static unsigned char Emode;

	if(RDonly) {			// Do not allow edit of file read-only
		p = "READ ONLY";
	eabort:
		status("Cannot edit: %s", p);
		return; }
	if(!(c = Smode)) {
		p = "No data"; goto eabort; }
	if((Smode-1) & 1) {
		p = "Compressed"; goto eabort; }

	cf = Hl = Hh = 0;
	wcursor_line();
	ss = (Secsize < 256) ? 128 : 256;
	rd = 255;
	status("SECTOR EDIT:  F1=Help  F2=Charset  F3=Mode  F10=Exit");

	for(;;) {
		while(Offset < 0) {		// Normalize offset to screen
			Offset += 16;
			Edit -= 16;
			rd = 255; }
		while(Offset >= ss) {	// Normalize offset to screen
			Offset -= 16;
			Edit += 16;
			rd = 255; }
		if((Edit+ss) > Secsize) {	// Handle over-top case
			Edit = (Secsize < 256) ? 0 : Secsize - 256;
			Offset = ss - 1; }
		if(Edit < 0)			// Reset if under
			Edit = Offset = 0;
		if(rd) {				// Redraw has been requested
			charset(rd = 0);
			draw_edit(Attrs[ENORM], Attrs[EHILITE]); }
		// Calculate screen location & position cursor
		y = (Offset >> 4) + 1;
		x = Offset & 15;
		if(Emode)
			wgotoxy(62+x, y);
		else
			wgotoxy((x*3)+(x>>2)+8, y);
		so = Offset;
		switch(c=wgetc()) {			// Process command keys
		case _KRA : ++Offset;		continue;					// Move right
		case _KLA : --Offset; 		continue;					// Move left
		case _KDA : Offset += 16;	continue;					// Move down
		case _KUA : Offset -= 16;	continue;					// Move up
		case _KPD : Edit += 256;	goto redraw;				// Page down
		case _KPU : Edit -= 256;	goto redraw;				// Page up
		case _KHO : Offset = (Offset - 1) & 0xFFF0;	continue;	// Start of line
		case _KEN : Offset = (Offset + 1) | 0x0F;	continue;	// End of line
		case _CHO : Edit = Offset = 0;	goto redraw;			// Go to start
		case _CEN :												// Go to end
			Edit = (Secsize < 256) ? 0 : Secsize-256;
			Offset = ss - 1;
	redraw:	rd = 255;			// Request screen redraw
			continue;
		case _K1 : display_help(ehelp); rd=255;			continue;	// Help
		case _K2 : charset(255); rd = 255;				continue;	// Charset
		case _K3 : Emode = Emode ? 0 : 255;				continue;	// Mode
		case _K10 :													// Exit
			if(cf) {
				status("Write changed sector (Y/N)?");
				if(yn()) {
					lseek(fh, H=S[Psector][1], L=S[Psector][0], 0);
					invert();
					write(&Smode, 1, fh);
					write(Secdata, Secsize, fh);
					p = "Sector written"; }
				else
					p = "Sector discarded"; }
			else
				p = "No changes";
			status(p);
			wcursor_off();
			*mwin = Attrs[MNORM];
			return; }
		// Not a control key - attempt data entry
		if(!Emode) {		// HEX data entry
			if((d = ctoh(c)) == -1)
				continue;
			wputc(c);
			if((c = ctoh(wgetc())) != -1) {
				Secdata[Edit+Offset] = c = (d << 4) | c; } }
		else {				// ASCII data entry
			if(c & 0xFF80)
				continue;
			if(Charset) {	// Alternate character set - reverse lookup
				for(d = 0; d < 256; ++d)
					if(ebcdic[d] == c)
						break;
				if((c = d) & 0xFF00)	// Skip if not found
					continue; }
			Secdata[Edit+Offset] = c; }
		wgotoxy((x*3)+(x>>2)+8, y);		// Update HEX
		wprintf("%02x", c);
		wgotoxy(62+x, y);				// Update printable
		showchar(c);
		++Offset;						// Advance position
		cf = 255; }						// Note changed
}

/*
 * Parse a filename and append extension if none supplied
 */
void filename(unsigned char *name, unsigned char *ext)
{
	unsigned char d, *p;
	p = Omap;
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
 * Display line of output with page pauses
 */
int Pline;		// Pause line counter
int pause(unsigned char *buffer)
{
	if(fps) {
		fputs(buffer, fps);
		putc('\n', fps);
		return 0; }

	if(Pline >= 17) {
		status("SPACE to continue, ESC to stop");
		if(space()) {
			status("Aborted");
			return 255; }
		status("");
		wclwin();
		wputs(buffer);
		return Pline = 0; }
	wgotoxy(0, ++Pline);
	wputs(buffer);
	return 0;
}

/*
 * Extract and display printable strings from image
 */
void find_strings(void)
{
	unsigned l, p, t, s;
	unsigned char buffer[1025], c, nl;

	nl = l = 0;
	Pline = -1;
	for(t=Track; t < Tracks; ++t) {
		lseek(fh, H=T[t][1], L=T[t][0], 0);
		read_track();
		map_sectors();
		for(s = 0; s < Nsect; ++s) {
			Psector = Omap[s];
			read_sector();
			for(p=0; p < Secsize; ++p) {
				c = Secdata[p];
				if(Charset)
					c = ebcdic[c];
				if((c < ' ') || (c > 0x7E)) {
					nl = 255;
					continue; }
				if(l >= Width)
					nl = 255;
				if(nl) {
					if(l >= Mstring) {
						buffer[l] = 0;
						if(pause(buffer)) return; }
					nl = l = 0; }
				buffer[l++] = c; } } }
	if(l >= Mstring) {
		buffer[l] = 0;
		if(pause(buffer)) return; }
	if(mwin) {
		status("All strings found");
		space(); }
}

/*
 * Compare using circular buffer
 */
unsigned Slen, Spos;			// Scan length & Sosition
unsigned char Sbuffer[65];		// Scan buffer
int Fcompare(unsigned char *s)
{
	unsigned p;
	p = Spos;
	do {
		if(Sbuffer[p] != *s++)
			return 0;
		if(++p >= Slen)
			p = 0; }
	while(p != Spos);
	return 255;
}

/*
 * Find string in image data
 */
void find(char again)
{
	unsigned char c;

	status("Search: %s", Finput);
	if(again)		// Repeated search
		goto findagain;

	// Initial search, clear the circular buffer
	memset(Sbuffer, Spos = 0, sizeof(Sbuffer));

	// Read each track/sector and scan
	for(Ft=Track; Ft < Tracks; ++Ft) {			// Each track
		lseek(fh, H=T[Ft][1], L=T[Ft][0], 0);
		read_track();
		map_sectors();
		for(Fs = 0; Fs < Nsect; ++Fs) {			// Each sector
			Psector = Omap[Fs];
			read_sector();
			for(Fp=0; Fp < Secsize; ++Fp) {		// Each byte
				c = Secdata[Fp];
				if(Charset)						// Charcter set translation
					c = ebcdic[c];
				Sbuffer[Spos] = Fcs ? toupper(c) : c;
				if(++Spos >= Slen)
					Spos = 0;
				if(Fcompare(Fstring)) {			// Found match
					Track = Ft;
					Sector = Fs;
					Hl = (Hh = Fp + 1) - Slen;
					if(Hl < 0) Hl = 0;
					while(Hl < Edit)
						Edit -= 16;
					while((Edit+256) < Hl)
						Edit += 16;
					status("Found '%s' [%04x]", Finput, Fp);
					Fflag = 255;
					return; }
// To repeat a search, we jump to the end of the loop, allowing it
// to continue with the existing Ft, Fs and Fp values.
findagain: } } }
	status("Not found: '%s'", Finput);
	Hl = Hh = 0;
}

/*
 * Character set editor
 */
void edit_charset(unsigned char *file)
{
	int c;
	unsigned i;
	unsigned char p;

	// Add extension (if necessary) & read file into buffer
	filename(file, ".CHR");
	if(fp = fopen(Omap, "rb")) {
		if(fget(Cset, 256, fp) != 256)		// Too small
			abort("Bad character set");
		if(getc(fp) != EOF)					// Too big
			abort("Bad character set");
		fclose(fp); }

	// Open windows & display title
	swin = wopen(0, 0, 80, 1, WSAVE|WCOPEN|0x70);
	Attrs = (W_BASE == 0xB000) ? Mattrs : Cattrs;
	mwin = wopen(0, 1, 80, 24, WSAVE|WCOPEN|Attrs[MNORM]);
	wcursor_off();
	status("ImageDisk Viewer "#VERSION" / "#__DATE__" - Copyright 2007-"#CYEAR" Dave Dunfield");

retitle:
	wgotoxy(0, 0);
	wprintf(fp ? "Editing: %s" : "New: %s", Omap);

	p = 0;
redraw:
	// Display character set showing current settings
	for(i=0; i < 256; ++i) {
		wgotoxy((i >> 4) * 5, (i & 15) + 4);
		*mwin = Attrs[(i == p) ?  MHILITE : MWEAK];
		wprintf("%02x=", i);
		*mwin = Attrs[MNORM];
		c = Cset[i];
		wputc( ((c >= ' ') && (c < 0x7F)) ? c : 0xA8); }
	for(;;) {
		i = p;
		c = wgetc();
		if(Sflag) {		// Status changed - reshow default
			status(".CHR Editor:  F1=Help  F2=Non-print  F5=LoadASCII  F6=LoadEBCDIC  F10=EXIT");
			Sflag = 0; }
		switch(c) {
		case 0x1B :				// Exit
		case _K10 :
			status("Resave file (Y/N)?");
			c = yn();
			wclose();
			wclose();
			if(c) {
				fp = fopen(Omap, "wvqb");
				fput(Cset, 256, fp);
				fclose(fp); }
			return;
		case _K1 : display_help(chelp); wclwin();	goto retitle;			// Help
		case _K2 : Cset[p] = 0;	goto redraw;								// Non-print
		case _K5 : for(i=0; i < 256; ++i) Cset[i] = i; goto redraw;			// ASCII
		case _K6 : for(i=0; i < 256; ++i) Cset[i] = ebcdic[i]; goto redraw;	// EBCDIC
		case _KLA :								// Move left
			if(p)			p -= 16;
			if(i > 0x0F)	goto redraw;
		case _KUA : --p;	goto redraw;		// Move up
		case _KRA :								// Move right
			if(p != 0xFF)	p += 16;
			if(i < 0xF0)	goto redraw;
		case _KDA : ++p;	goto redraw;		// Move down
		default:
			if((c >= ' ') && (c < 0x7F)) {		// Enter display character
				Cset[p++] = c;
				goto redraw; } } }
}

int skip()
{
	while(isspace(*Ptr))
		++Ptr;
	return *Ptr;
}

/*
 * Get value
 */
int value(unsigned *d, unsigned b, unsigned l, unsigned h, unsigned char *n)
{
	unsigned c, v;
	unsigned char f;

	switch(*Ptr) {
	case '%' : b = 2;	goto newb;
	case '@' : b = 8;	goto newb;
	case '.' : b = 10;	goto newb;
	case '$' : b = 16;	newb:
		++Ptr; }

	v = f = 0;
	for(;;) {
		c = *Ptr;
		if((c >= '0') && (c <= '9'))
			c -= '0';
		else if((c >= 'a') && (c <= 'f'))
			c -= ('a' - 10);
		else if((c >= 'A') && (c <= 'F'))
			c -= ('A' - 10);
		else
			c = 0xFFFF;
		if(c >= b) {
			switch(*Ptr) {
			case ' ' :
			case 0 :
				if(f) {
					if((v < l) || (v > h)) {
						status("%s value out of range", n);
						return 255; }
					*d = v;
					return 0 ; } }
			status("Bad %s value", n);
			return 255; }
		v = (v * b) + c;
		++Ptr;
		f = 255; }
}

/*
 * Main program
 */
main(int argc, char *argv[])
{
	int c;
	unsigned i;

	fputs("IMageDisk Viewer "#VERSION" / "#__DATE__"\n", stdout);

	for(i=1; i < argc; ++i) {		// Parse command line arguments
		Ptr = argv[i];
		switch((toupper(*Ptr++) << 8) | toupper(*Ptr++)) {
		case '-C' :
		case '/C' : Csearch = 0;		continue;	// Case sensitive search
		case '-E' :
		case '/E' : Charset = 1;		continue;	// Ebcdic display
		case '-I' :
		case '/I' : Ileave = 0;			continue;	// Disable interleave
		case '-W' :
		case '/W' : RDonly = 0;			continue;	// Allow update
		case 'C=' :									// Custom character set
			filename(Custom = Ptr, ".CHR");
			fp = fopen(Omap, "rvqb");
			memcpy(Cset, ebcdic, 256);
			if(fget(ebcdic, 256, fp) != 256)
				abort("Bad character set");
			if(getc(fp) != EOF)
				abort("Bad character set");
			fclose(fp);
			Charset = 2;
			while(*Ptr = toupper(c = *Ptr)) {
				if(c == '.') {
					*Ptr = 0;
					continue; }
				++Ptr;
				if(c == '\\') {
					Custom = Ptr;
					continue; } }
			continue;
		case 'E=' : edit_charset(Ptr);	return;		// Edit character set
		case 'M=' :									// Minimum string length
			if(value(&Mstring, 10, 1, 65535, "M=")) return;
			continue;
		case 'S=' :									// Scan strings to file
			filename(Ptr, ".TXT");
			fps = fopen(Omap, "wvq");
			continue;
		case 'W=' :									// Output width
			if(value(&Width, 10, 1, 65535, "W=")) return;
			continue;
		case 'X=' : 								// Data XOR value
			if(value(&c, 16, 0, 255, "X=")) return;
			Xor = c;
			continue;
		} if(File)
			abort(help);
		File = Ptr-2; }

	if(!File)
		abort(help);

	filename(File, ".IMD");
	if(!(fh = open(Omap, (RDonly) ? F_READ|F_BINARY : F_READ|F_WRITE|F_BINARY)))
		error("Cannot open: %s", Omap);

	// Read and echo comment
	while((c = Gete()) != 0x1A) {
		if(c == EOF)
			abort("Corrupt image file");
		putc(c, stdout); }

	// Record position of each track
	for(;;) {
		T[Tracks][0] = L; T[Tracks][1] = H;
		if(read_track())
			break;
		++Tracks; }

	// If scan-output file specified, extract strings & exit
	if(fps) {
		find_strings();
		fclose(fps);
		return; }

	// Open user-interface windows
	swin = wopen(0, 6, 80, 1, WSAVE|WCOPEN|0x70);
	Attrs = (W_BASE == 0xB000) ? Mattrs : Cattrs;
	iwin = wopen(0, 0, 80, 6, WSAVE|WCOPEN|Attrs[INORM]);
	mwin = wopen(0, 7, 80, 18, WSAVE|WCOPEN|Attrs[MNORM]);
	status("ImageDisk Viewer "#VERSION" / "#__DATE__" - Copyright 2007-"#CYEAR" Dave Dunfield");
	wcursor_off();

	Track = Sector = 0;

new_track:		// Read track & setup corresponding globals
	lseek(fh, H=T[Track][1], L=T[Track][0], 0);
	read_track();
	map_sectors();
	if(!(Head & 0x80)) memset(Cmap, Cylinder, sizeof(Cmap));
	if(!(Head & 0x40)) memset(Hmap, Head & 0x3F, sizeof(Hmap));
	if(Sector >= Nsect)
		Sector = Nsect-1;
	if(Edit >= Secsize)
		Edit = 0;

new_sect:		// Read sector into buffer & display information
	Psector = Omap[Sector];
	read_sector();
	w_clwin(iwin);
	w_printf(iwin, "Physical C/H/S:%3u/%u/%u", Cylinder, Head & 0x3F, Psector+1);
	w_gotoxy(50, 0, iwin);
	w_printf(iwin, "Logical C/H/S:%3u/%u/%u\n\n", Cmap[Psector], Hmap[Psector], Smap[Psector]);
	for(i=0; i < Nsect; ++i) {
		*iwin = Attrs[(i == Psector) ? IHILITE : INORM];
		w_printf(iwin, " %u", Smap[i]); }
	*iwin = Attrs[INORM];
	w_printf(iwin, "\n\n%s, %u sectors of %u bytes, ", Sector_mode[Mode], Nsect, Secsize);
	if(!(c = Smode))
		w_puts("No data");
	else {
		--c;
		if(c & 4) w_puts("Bad ", iwin);
		w_puts(c & 2 ? "Deleted " : "Normal ", iwin);
		w_puts(c & 1 ? "Compressed" : "Data", iwin); }

	if(Fflag)
		Fflag = 0;			// Found, leave hilight on, reset flag
	else
		Hh = Hl = 0;		// Not found, hilight off

redraw:			// Redraw screen
	charset(0);
	if(Edit < 0)
		Edit = 0;
	draw_edit(Attrs[MNORM], Attrs[MHILITE]);
recmd:
	for(;;) {	// Command loop - interpret keypresses
		c = wgetc();
		if(Sflag) {		// Status changed - reshow default
			status("IMDV: F1=Help  F2=Charset  F3=SearchT  F4=SearchB F5=Again  F6=Strings  F10=Exit");
			Sflag = 0; }
		switch(c) {
		case _KRA :		// Move to next sector
		nxtsect:
			if((Sector+1) < Nsect) { ++Sector; goto new_sect; }
			if((Track+1) < Tracks) Sector = 0;
		case _KDA :		// Move to next track
		fwdtrack:
			if((Track + 1) >= Tracks) continue;
			++Track; goto new_track;
		case _KLA :		// Move to previous sector
		prevsec:
			if(Sector) { --Sector; goto new_sect; }
			if(Track) Sector = 255;
		case _KUA :		// Move to previous track
		backtrack:
			if(!Track) continue;
			--Track; goto new_track;
		case _KPU :		// Scroll data display backward
			if(!Edit) {
				if(Sector | Track) {
					Edit = Secsize - 256;
					goto prevsec; } }
			Edit -= 256;
			goto redraw;
		case _KPD :		// Scroll data display forward
			if((Edit + 512) < Secsize) {
				Edit += 256;
				goto redraw; }
			if(((Edit + 256) == Secsize) || (Secsize < 256)) {
				if( ((Sector+1) < Nsect) || ((Track+1) < Tracks) ) {
					Edit = 0;
					goto nxtsect; } }
			Edit = Secsize - 256;
			goto redraw;
		case _CHO :						// Beginning
			Track = Sector = Edit = 0;
			goto new_track;
		case _CEN :						// End
			Track = Tracks-1;
			Sector = Nsect - 1;
			Edit = Secsize - 256;
			goto new_track;
		case _KHO :		// Back to start sectors in tracks
			if(!Sector) goto backtrack;
			Sector = 0; goto new_sect;
		case _KEN :		// Fwd to end sectors in tracks
			if((Sector+1) == Nsect) goto fwdtrack;
			Sector = Nsect - 1; goto new_track;
		case '\n' : edit();	goto new_sect;					// Edit sector
		case _K1 :
		case '?' : display_help(mhelp);	goto redraw;		// Help display
		case _K2 : charset(255); goto redraw;				// ASCII/EBCDIC
		case _K3 :											// Find ASCII
			if(wform(2, 10, WSAVE|WCOPEN|WBOX1|0x70, search_text, Finput))
				continue;
			strcpy(Fstring, Finput);
			Slen = strlen(Fstring);
			if(Fcs = Csearch)
				strupr(Fstring);
			c = 0;
			goto gofind;
		case _K4 :											// Find HEX
			if(wform(2, 10, WSAVE|WCOPEN|WBOX1|0x70, search_byte, Ptr = Finput))
				continue;
			i = Slen = 0;
			while(skip()) {
				if(value(&c, 16, 0, 255, "byte"))
					goto recmd;
				Fstring[i++] = c; }
			Slen = i;
			Fcs = c = 0;
		case _K5 :											// Repeat search
gofind:		if(Slen) {
				find(c);
				goto new_track; }
			continue;
		case _K6 : wclwin(); find_strings(); goto new_track;	// Strings
		case _K10:
		case 0x1B:											// Exit
			close(fh);
			wclose();
			wclose();
			wclose();
			return; } }
}
