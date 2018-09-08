/*
 * TD02IMD - Convert Teledisk .TD0 images to ImageDisk .IMD format
 *
 * TD02IMD reads a teledisk .TD0 image file, and reformats it into an
 * ImageDisk .IMD file.
 *
 * Note that the Teledisk file format is closed and completely undocumented.
 * TD02IMD relies on information obtained by reverse engineering and may not
 * be able to handle all Teledisk images. I have included my notes on the
 * Teledisk file format in the file TD0NOTES.TXT
 *
 * This program is compiled using my own development tools, and will not
 * build under mainstream compilers without significant work. This source
 * code is provided for informational purposes only, and I provide no
 * support for it, technical or otherwise.
 *
 * Copyright 2007-2012 Dave Dunfield
 * All rights reserved.
 *
 * For the record: I am retaining copyright on this code, however this is
 * for the purpose of keeping a say in it's disposition. I encourage the
 * use of ideas, algorithms and code fragments contained herein to be used
 * in the creation of compatible programs on other platforms (eg: Linux).
 */
#include <stdio.h>
#include <file.h>
#include "version.h"

// Debug flags
#define	DEBUG_DUMPH 0x80		// Dump disk header
#define	DEBUG_DUMPT	0x40		// Dump Track headers
#define	DEBUG_DUMPS	0x20		// Dump Sector headers
#define	DEBUG_SHOWS 0x02		// Show sectors processed
#define	DEBUG_ONE	0x01		// Do only one track

// LZSS parameters
#define SBSIZE		4096				// Size of Ring buffer
#define LASIZE		60					// Size of Look-ahead buffer
#define THRESHOLD	2					// Minimum match for compress

// Huffman coding parameters
#define N_CHAR	(256-THRESHOLD+LASIZE)	// Character code (= 0..N_CHAR-1)
#define TSIZE		(N_CHAR*2-1)		// Size of table
#define ROOT		(TSIZE-1)			// Root position
#define MAX_FREQ	0x8000				// Update when cumulative frequency reaches this value

unsigned
	parent[TSIZE+N_CHAR],	// parent nodes (0..T-1) and leaf positions (rest)
	son[TSIZE],				// pointers to child nodes (son[], son[]+1)
	freq[TSIZE+1];			// frequency table
	Bits, Bitbuff,			// buffered bit count and left-aligned bit buffer
	GBcheck,				// Getbyte check down-counter
	GBr,					// Ring buffer position
	GBi,					// Decoder index
	GBj,					// Decoder index
	GBk;					// Decoder index

unsigned char
	Debug,					// Debug mode
	Warn,					// Warning level
	Wmask,					// Warning mask
	GBstate,				// Decoder state
	Eof,					// End-of-file indicator
	Advcomp,				// Advanced compression enabled
	ring_buff[SBSIZE+LASIZE-1];	// text buffer for match strings

FILE
	*fpi,					// Input file pointer
	*fpo;					// Output file pointer

/*
 * LZSS decoder - based in part on Haruhiko Okumura's LZHUF.C
 */

unsigned char d_code[256] = {		// Huffman decoder tables
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09,
0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B, 0x0B,
0x0C, 0x0C, 0x0C, 0x0C, 0x0D, 0x0D, 0x0D, 0x0D, 0x0E, 0x0E, 0x0E, 0x0E, 0x0F, 0x0F, 0x0F, 0x0F,
0x10, 0x10, 0x10, 0x10, 0x11, 0x11, 0x11, 0x11, 0x12, 0x12, 0x12, 0x12, 0x13, 0x13, 0x13, 0x13,
0x14, 0x14, 0x14, 0x14, 0x15, 0x15, 0x15, 0x15, 0x16, 0x16, 0x16, 0x16, 0x17, 0x17, 0x17, 0x17,
0x18, 0x18, 0x19, 0x19, 0x1A, 0x1A, 0x1B, 0x1B, 0x1C, 0x1C, 0x1D, 0x1D, 0x1E, 0x1E, 0x1F, 0x1F,
0x20, 0x20, 0x21, 0x21, 0x22, 0x22, 0x23, 0x23, 0x24, 0x24, 0x25, 0x25, 0x26, 0x26, 0x27, 0x27,
0x28, 0x28, 0x29, 0x29, 0x2A, 0x2A, 0x2B, 0x2B, 0x2C, 0x2C, 0x2D, 0x2D, 0x2E, 0x2E, 0x2F, 0x2F,
0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F };

unsigned char d_len[] = { 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7 };

/*
 * Initialise the decompressor trees and state variables
 */
void init_decompress()
{
	unsigned i, j;

	for(i = j = 0; i < N_CHAR; ++i) {		// Walk up
		freq[i] = 1;
		son[i] = i + TSIZE;
		parent[i+TSIZE] = i; }

	while(i <= ROOT) {						// Back down
		freq[i] = freq[j] + freq[j+1];
		son[i] = j;
		parent[j] = parent[j+1] = i++;
		j += 2; }

	memset(ring_buff, ' ', sizeof ring_buff);
	Advcomp = freq[TSIZE] = 0xFFFF;
	parent[ROOT] = Bitbuff = Bits = 0;
	GBr = SBSIZE - LASIZE;
}

/*
 * Increment frequency tree entry for a given code
 */
void update(int c)
{
	unsigned i, j, k, f, l;

	if(freq[ROOT] == MAX_FREQ) {		// Tree is full - rebuild
		// Halve cumulative freq for leaf nodes
		for(i = j = 0; i < TSIZE; ++i) {
			if(son[i] >= TSIZE) {
				freq[j] = (freq[i] + 1) / 2;
				son[j] = son[i];
				++j; } }

		// make a tree - first connect children nodes
		for(i = 0, j = N_CHAR; j < TSIZE; i += 2, ++j) {
			k = i + 1;
			f = freq[j] = freq[i] + freq[k];
			for(k = j - 1; f < freq[k]; --k);
			++k;
			l = (j - k) * sizeof(freq[0]);

			memmove(&freq[k+1], &freq[k], l);
			freq[k] = f;
			memmove(&son[k+1], &son[k], l);
			son[k] = i; }

		// Connect parent nodes
		for(i = 0 ; i < TSIZE ; ++i)
			if((k = son[i]) >= TSIZE)
				parent[k] = i;
			else
				parent[k] = parent[k+1] = i; }

	c = parent[c+TSIZE];
	do {
		k = ++freq[c];
		// Swap nodes if necessary to maintain frequency ordering
		if(k > freq[l = c+1]) {
			while(k > freq[++l]);
			freq[c] = freq[--l];
			freq[l] = k;
			parent[i = son[c]] = l;
			if(i < TSIZE)
				parent[i+1] = l;
			parent[j = son[l]] = c;
			son[l] = i;
			if(j < TSIZE)
				parent[j+1] = c;
			son[c] = j;
			c = l; } }
	while(c = parent[c]);	// Repeat up to root
}

/*
 * Get a byte from the input file and flag Eof at end
 */
unsigned GetChar()
{
	unsigned c;
	if((c = getc(fpi)) == EOF) {
		c = 0;
		Eof = 255; }
	return c;
}

/*
 * Get a single bit from the input stream
 */
unsigned GetBit()
{
	unsigned t;
	if(!Bits--) {
		Bitbuff |= GetChar() << 8;
		Bits = 7; }

	t = Bitbuff >> 15;
	Bitbuff <<= 1;
	return t;
}

/*
 * Get a byte from the input stream - NOT bit-aligned
 */
unsigned GetByte()
{
	unsigned t;
	if(Bits < 8)
		Bitbuff |= GetChar() << (8-Bits);
	else
		Bits -= 8;

	t = Bitbuff >> 8;
	Bitbuff <<= 8;
	return t;
}

/*
 * Decode a character value from table
 */
unsigned DecodeChar()
{
	unsigned c;

	// search the tree from the root to leaves.
	// choose node #(son[]) if input bit == 0
	// choose node #(son[]+1) if input bit == 1
	c = ROOT;
	while((c = son[c]) < TSIZE)
		c += GetBit();

	update(c -= TSIZE);
	return c;
}

/*
 * Decode a compressed string index from the table
 */
unsigned DecodePosition()
{
	unsigned i, j, c;

	// Decode upper 6 bits from given table
	i = GetByte();
	c = d_code[i] << 6;

	// input lower 6 bits directly
	j = d_len[i >> 4];
	while(--j)
		i = (i << 1) | GetBit();

	return (i & 0x3F) | c;
}

/*
 * Report error and terminate (formatted output)
 */
register error(unsigned args)
{
	unsigned char buffer[100];
	_format_(nargs() * 2 + &args, buffer);
	if(fpo) fclose(fpo);
	if(fpi) fclose(fpi);
	fputs(buffer, stdout);
	exit(-1);
}

/*
 * Get a byte from the input file - decompress if required
 *
 * This implements a state machine to perform the LZSS decompression
 * allowing us to decompress the file "on the fly", without having to
 * have it all in memory.
 */
int getbyte()
{
	unsigned c;

	--GBcheck;
	if(!Advcomp)			// No compression
		return getc(fpi);

	for(;;) {				// Decompressor state machine
		if(Eof)					// End of file has been flagged
			return -1;
		if(!GBstate) {			// Not in the middle of a string
			c = DecodeChar();
			if(c < 256) {		// Direct data extraction
				ring_buff[GBr++] = c;
				GBr &= (SBSIZE-1);
				return c; }
			GBstate = 255;		// Begin extracting a compressed string
			GBi = (GBr - DecodePosition() - 1) & (SBSIZE-1);
			GBj = c - 255 + THRESHOLD;
			GBk = 0; }
		if(GBk < GBj) {			// Extract a compressed string
			ring_buff[GBr++] = c = ring_buff[(GBk++ + GBi) & (SBSIZE-1)];
			GBr &= (SBSIZE-1);
			return c; }
		GBstate = 0; }			// Reset to non-string state
}

/*
 * Get a word from the input file via getbyte (for compression)
 */
unsigned getword()
{
	unsigned w;
	w = getbyte();
	return (getbyte() << 8) | w;
}

/*
 * Get a block from the input file via getbyte (for compression)
 */
int getblock(unsigned char *p, unsigned size, unsigned char *e)
{
	int c;
	char eof;
	eof = 0;
	while(size) {
		--size;
		if((c = getbyte()) == -1) {
			eof = 255;
			if(e)
				error("EOF reading %s", e);
			break; }
		*p++ = c; }
	return eof;
}

// Teledisk sector flag meanings
#define	SEC_DUP		0x01		// Sector was duplicated
#define	SEC_CRC		0x02		// Sector has CRC error
#define	SEC_DAM		0x04		// Sector has Deleted Address Mark
#define	SEC_DOS		0x10		// Sector not allocated
#define	SEC_NODAT	0x20		// Sector has no data field
#define	SEC_NOID	0x40		// Sector has no ID field

// Main image file header
struct td0_header {
	unsigned 		Sig;		// TD Signature
	unsigned char	Sequence;	// Volume sequence number
	unsigned char	Checksig;	// Check signature for multi-volume sets
	unsigned char	TDversion;	// Teledisk version
	unsigned char	Datarate;	// Data rate
	unsigned char	Drivetype;	// Source drive type
	unsigned char	Stepping;	// Stepping type
	unsigned char	DOSmode;	// Read according to DOS allocation
	unsigned char	Sides;		// # of sizes read
	unsigned Hcrc;				// Header crc
} Header;
unsigned char Htemplate[] = {	// Debug output template
"sig:\x81 seq:\x80 cseq:\x80 ver:\x80 Den:\x80 Dtyp:\x80 Tden:\x80 DOS:\x80 Side:\x80 Crc:\x81\n" };

// Individual track header
struct track_header {
	unsigned char	Tsectors;	// number sectors/track
	unsigned char	Tcylinder;	// Physical cylinder
	unsigned char	Tside;		// Physical side
	unsigned char	Tcrc;		// Crc of header
} Thead;
unsigned char Ttemplate[] = {	// Debug output template
"T#sec:\x80 cyl:\x80 side:x80 crc:\x80\n" };

// Individual sector header
struct sector_header {
	unsigned char	Scylinder;	// Cylinder number in ID field
	unsigned char	Sside;		// Side number in ID field
	unsigned char	Ssector;	// Sector number in ID field
	unsigned char	Ssize;		// Size of sector
	unsigned char	Sflags;		// Sector control flags
	unsigned char	Scrc;		// Sector header CRC
} Shead;
unsigned char Stemplate[] = {	// Debug output template
"Scyl:\x80 side:\x80 num:\x80 size:\x80 flag:\x80 crc:\x80" };

// Data block header - follows sector header only if data present
struct data_header {
	unsigned		Doffset;	// Offset to next data block
	unsigned char	Dmethod;	// Method of compression
} Dhead;
unsigned char Dtemplate[] = {	// Debug output template
" offset:\x81 method:\x80\n" };

unsigned
	Tracks,			// Total # of tracks processed
	Sectors,		// Total number of sectors processed
	Mode,			// Current track mode
	Size,			// Current sector size
	Cylinder,		// Current cylinder
	Head,			// Current head
	Stop,			// Top of sector numbering map
	Dlow,			// Debug output - track low
	Dhigh = 255,	// Debug output - track high
	Maxsec = -1,	// Maxumum sector number
	Sremove,		// Removed sector count
	Dseg,			// Sector data segment
	Dtop,			// Segment top
	Doffset[256];	// Data offset

unsigned char
	FM = 255,		// Assume FM if disk marked FM
	Verbose = 255,	// Verbose output
	Lowden = 255,	// Convert low-density
	*ptr,			// General pointer
	Rflags,			// Remove flags
	Rdup,			// Remove duplicates
	Nmap[256],		// Sector numbering map
	Cmap[256],		// Sector cylinder map
	Hmap[256],		// Sector head map
	Smap[256],		// Sector size map
	Fmap[256],		// Sector flag bytes
	Stype[256];		// Sector type flags

// Various header value to text tables
unsigned char *dt_text[] = { "5.25-96/48", "5.25", "5.25-96", "3.5", "3.5", "8\"", "3.5" };
unsigned char *dr_txt[] = { "LD", "LD", "HD" };
unsigned char *dr_step[] = { "S", "D", "E" };

static char CR[] = { "\r\n" };

register warn(unsigned args)
{
	unsigned char buffer[65];
	_format_(nargs() * 2 + &args, buffer);
	if(!(*buffer & Wmask)) {		
		printf("%02u/%u: %s\n", Cylinder, Head, buffer+1);
		Wmask |= *buffer; }
}

/*
 * Lookup value index in table and return static string
 */
unsigned char *txt(unsigned v, unsigned char *t[], unsigned s)
{
	static unsigned char buf[20];
	if(v >= (s/2))
		sprintf(buf, "<unknown %02x>", v);
	else
		strcpy(buf, t[v]);
	return buf;
}
#define	TXT(v, t) txt(v, t, sizeof(t))

/*
 * Output character only if debug output enabled
 */
void Dputc(int c)
{
	unsigned t;
	t = Thead.Tcylinder;
	if((t >= Dlow) && (t <= Dhigh))
		putc(c, stdout);
}

/*
 * Print formatted string via debug output
 */
register Dprintf(unsigned args)
{
	unsigned char buffer[100], *p;
	_format_(nargs() * 2 + &args, p = buffer);
	while(*p)
		Dputc(*p++);
}

/*
 * Dump header information for debug
 */
void dump(unsigned char *p, unsigned char *t, unsigned char Mask)
{
	unsigned char c;

	if(Debug & Mask)
		for(;;) switch(c = *t++) {
		case 0 : return;
		case 0x80 : Dprintf("%02x", *p++);					continue;
		case 0x81 : Dprintf("%04x", *(unsigned*)p); p += 2;	continue;
		case 0x82 : Dprintf("%d", *p++);					continue;
		case 0x83 : Dprintf("%d", *(unsigned*)p);	p += 2;	continue;
		default: Dputc(c); }
}

/*
 * Compute Cyclic Redundancy Check (CRC) over block of data
 */
unsigned compute_crc(const unsigned char *p, unsigned len, unsigned crc)
{
	unsigned i;
	while(len) {
		--len;
        crc ^= (*p++ << 8);
        for(i = 0; i < 8 ; ++i)
            crc = (crc << 1) ^ ((crc & 0x8000) ? 0xA097 : 0); }
    return crc;
}

/*
 * Display TD0 comment record
 */
void show_comment(void)
{
	unsigned c, i, j, nl, crc, len;
	unsigned char y, mo, d, h, mi, s;

	crc = getword();		// Comment block CRC
	len = getword();		// Comemnt block length
	y	= getbyte();		// Year
	mo	= getbyte();		// Month
	d	= getbyte();		// Day
	h	= getbyte();		// Hour
	mi	= getbyte();		// Minute
	s	= getbyte();		// Second

	c = compute_crc(&len, 8, j = nl = 0);	// Relies on MC allocation!

	sprintf(Cmap, " %u/%02u/%04u %u:%02u:%02u", d, mo+1, (unsigned)y+1900, h, mi, s);
	fputs(Cmap, stdout); putc('\n', stdout);

	if(fpo) {		// Write to output file if enabled
		fputs(Cmap, fpo);
		fputs(CR, fpo); }

	while(j < len) {				// Process each character
		++j;
		i = getbyte();
		c = compute_crc(&i, 1, c);	// Update CRC
		if(!i) {					// Pending new line
			++nl;
			continue; }
		while(nl) {					// Flush pending NLs before output
			--nl;
			putc('\n', stdout);
			if(fpo)
				fputs(CR, fpo); }
		putc(i, stdout);			// Output character
		if(fpo) {
			if(i == '\n')
				putc('\r', fpo);
			putc(i, fpo); } }
	if(crc != c)					// Validate
		error("\nComment CRC error!");
}

/*
 * Read sector from TD0 image and buffer.
 */
void process_sector(void)
{
	unsigned c, i, j, End;
	unsigned char b1, b2;
	unsigned char sbuf[8192];

	++Sectors;

	// Read and decode sector header
	getblock(Shead, sizeof(Shead), "sector header");
	dump(Shead, Stemplate, DEBUG_DUMPS);

	Nmap[Stop] = Shead.Ssector;
	Cmap[Stop] = Shead.Scylinder;
	if((Hmap[Stop] = Shead.Sside) & 0xFE)
		warn("\x01Head not 0 or 1");
	if((Smap[Stop] = Shead.Ssize) > 6)
		error("Unknown sector size");
	Fmap[Stop] = Shead.Sflags;
	Size = 128 << Shead.Ssize;
	Stype[Stop] = (Shead.Sflags & SEC_DAM) ? 3 : 1;	// IMD sector type
	if(Shead.Sflags & SEC_CRC)
		Stype[Stop] += 4;							// Indicate data-error
	End = Dtop + Size;
	Doffset[Stop] = Dtop;

	if(Shead.Sflags & SEC_NODAT) {					// No data
		Stype[Stop] = 0; }
	else if(Shead.Sflags & SEC_DOS) {				// DOS (no data)
		while(Dtop < End)
			poke(Dseg, Dtop++, 0xE5); }
	else {											// DATA available
		getblock(Dhead, sizeof(Dhead), "data header");
		dump(Dhead, Dtemplate, DEBUG_DUMPS);
		GBcheck = Dhead.Doffset - 1;
		while(Dtop < End) switch(Dhead.Dmethod) {
			case 0 :								// Raw sector data
				while(Dtop < End)
					poke(Dseg, Dtop++, getbyte());
				continue;
			case 1 :								// Repeated 2-byte pattern
				c = getword();							// Count
				b1 = getbyte();							// First byte
				b2 = getbyte();							// Second byte
				while(c) {
					--c;
					poke(Dseg, Dtop++, b1);
					poke(Dseg, Dtop++, b2); }
				continue;
			case 2 :							// RLE block
				if(!(c = getbyte())) {				// Literal data block
					c = getbyte();					// Count
					while(c) {
						--c;
						poke(Dseg, Dtop++, getbyte()); }
					continue; }
				// Repeated fragment
				i = 1 << c;							// Length
				c = getbyte();						// Count
				getblock(sbuf, i, "sector data");	// Fragment
				while(c) {							// # fragments
					--c;
					for(j=0; j < i; ++j)			// Copy in one
						poke(Dseg, Dtop++, sbuf[j]); }
				continue;
			default:
				printf("Unknown data mode: %02x\n", Dhead.Dmethod); } }
	if(Debug & DEBUG_SHOWS)
		Dprintf(" %u %x %u\n", Stop, Stype[Stop], Dtop);
	++Stop;
	if(GBcheck)
		error("Sector data size/offset mismatch");
}

/*
 * Test for block of data being the same
 */
int issame(unsigned char *d, unsigned char v, unsigned s)
{
	while(s) {
		--s;
		if(*d++ != v)
			return 0; }
	return 255;
}

/*
 * Write sector data to output file
 */
void write_data(unsigned index)
{
	unsigned t, s, d, p, Size, End;
	unsigned char b;

	if(t = Stype[index]) {		// Data available
		s = Smap[index];
		Size = 128 << s;
		End = (p = d = Doffset[index]) + Size;
		b = peek(Dseg, p++);
		while(p < End) {
			if(peek(Dseg, p++) != b)
				goto full; }
		putc(t+1, fpo);
		putc(b, fpo);
		return;
full:	putc(t, fpo);
		while(d < End)
			putc(peek(Dseg, d++), fpo);
		return; }
}

/*
 * Remove any sectors from a track according to configured options.
 */
void remove_sectors(void)
{
	unsigned i, j, n;
	unsigned char sdup[256];
	if(Rdup)
		memset(sdup, 0, sizeof(sdup));
	for(i=j=0; i < Stop; ++i) {
		if(Fmap[i] & Rflags) {					// Remove by flags
remove:		++Sremove;
			continue; }
		if(Nmap[i] > Maxsec) goto remove;		// Out of range
		if(Rdup) {								// Remove duplicates
			n = Nmap[i];
			if(sdup[n]) {						// Sector is duplicated
				if(Rdup & 0xF0) {				// Keep last
					n = sdup[n]-1;
					Cmap[n] = Cmap[i];
					Hmap[n] = Hmap[i];
					Smap[n] = Smap[i];
					Fmap[n] = Fmap[i];
					Stype[n] = Stype[i];
					Doffset[n] = Doffset[i]; }
				continue; }
			sdup[n] = i+1; }
		// Keep this sector
		Nmap[j] = Nmap[i];
		Cmap[j] = Cmap[i];
		Hmap[j] = Hmap[i];
		Smap[j] = Smap[i];
		Fmap[j] = Fmap[i];
		Stype[j] = Stype[i];
		Doffset[j] = Doffset[i];
		++j; }
	Stop = j;
}

/*
 * Get number from command line
 */
unsigned getnum(unsigned b)
{
	unsigned v, c;
	char f;
	v = f = 0;
	for(;;) {
		if(isdigit(c = *ptr))
			c -= '0';
		else if((c >= 'a') && (c <= 'f'))
			c -= ('a'-10);
		else if((c >= 'A') && (c <= 'F'))
			c -= ('A'-10);
		else {
			if(!f) switch(c) {
				case '$' : b = 16; ++ptr; continue;
				case '.' : b = 10; ++ptr; continue; }
			switch(c) {
			case 0 :
			case ',' :
				if(!f)
					error("Bad numeric argument");
				return v; }
			error("Bad numeric argument"); }
		if(c >= b)
			error("Bad numeric argument");
		v = (v * b) + c;
		f = 255;
		++ptr; }
}

/*
 * Copy filename & append extension if required
 */
void filename(unsigned char *file, unsigned char *ext, char dropext)
{
	unsigned char ef, *dest, *p;
	dest = Smap;
	ef = p = 0;
	for(;;) switch(*dest++ = *file++) {
		case 0 :						// End of string
			if(dropext && p)			// Remove extension from source
				*(p-1) = 0;
			if(!ef)						// No extension - add
				strcpy(dest-1, ext);
			return;
		case ':' :						// Disk specifier
		case '\\' : p = ef = 0;			// Directory specifier
			continue;
		case '.' :						// Extension specifier
			p = file;
			ef = 255; }
}

char help[] = { "\
TD0-2-IMageDisk "#VERSION" / "#__DATE__"\n\
Copyright 2007-"#CYEAR" Dave Dunfield - All rights reserved.\n\n\
Use:	TD02IMD filename[.TD0] [options]\n\n\
Opts:	/Dx,l,h	- enable Debug options\n\
	/F	- don't assume whole disk is FM if file FM indicator set\n\
	/L	- don't convert 300kbps (LD in HD drive) to 250 (standard LD)\n\
	/Q	- Quiet: less informational output.\n\
	/RF	- remove Repeated (duplicate) sectors, keep First\n\
	/RL	- remove Repeated (duplicate) sectors, keep Last\n\
	F=ACDIN	- remove sectors based on status Flags		[none]\n\
		  A=deletedaddressmark C=crcerror D=dos I=noid N=nodata\n\
	M=n	- Maximum sector number (remove any higher)	[255]\n\
	O=file	- specify Output filename			[filename.IMD]\n\
	W=CDHIN	- inhibit Warnings:\n\
		C: CRC error (bad sector)\n\
		D: Duplicate sector numbers\n\
		H: Head not 0 or 1\n\
		I: No ID field - possible bogus sector\n\
		N: No DATA field\n\
" };

/*
 * Main program
 */
main(int argc, char *argv[])
{
	int c;
	unsigned i, s;
	unsigned char f;
	static char *File, *Ofile;
	static char RF[] = { "|CA|DNI|" };	// F= option bits
	static char WN[] = { "HDINC" };		// Warning disable

	// Parse command line arguments
	for(s = 1; s < argc; ++s) {
		ptr = argv[s];
		switch((toupper(*ptr++) << 8) | toupper(*ptr++)) {
		case '/D' :
			Debug = getnum(16);
			if(*ptr++ == ',') Dlow = Dhigh = getnum(10);
			if(*ptr++ == ',') Dhigh = getnum(10);
			continue;
		case '/F' :	FM = 0;					continue;
		case '/L' : Lowden = 0;				continue;
		case '/Q' :	Verbose = 0;			continue;
		case '/R' : switch(toupper(*ptr++)) {
			case 'F' : Rdup = 0x0F;			continue;
			case 'L' : Rdup = 255;			continue; }
			abort(help);
		case 'M=' : Maxsec = getnum(10);	continue;
		case 'O=' : Ofile = ptr;			continue;
		case 'F=' :
			while(c = toupper(*ptr++)) {
				i = 0;
				while(RF[i] != c) {
					if(!RF[i++])
						abort(help); }
				Rflags |= 1 << i; }
				continue;
		case 'W=' :
			do {
				c = toupper(*ptr);
				for(i=0; f = WN[i]; ++i) {
					if(f == c) {
						Warn |= (1 << i);
						goto nxt; } }
				abort(help); nxt: }
			while(*++ptr);
			continue;
		} if(File)
			abort(help);
		File = ptr-2; }

	if(!File)
		abort(help);

	if(!(Dseg = alloc_seg(4096)))
		abort("Out of memory");

	// Open .TD0 file, read and validate header
	IOB_size = 4096;
	filename(File, ".TD0", 255);
	fpi = fopen(Smap, "rvqb");
	fget(Header, sizeof(Header), fpi);
	dump(Header, Htemplate, DEBUG_DUMPH);
	c = 0;
	if(compute_crc(Header, 10, 0) != Header.Hcrc) {
		printf("Header CRC failure");
		c = 255; }
	switch(Header.Sig) {
	default:
		printf("No TD signature");
		c = 255;
		break;
	case 'dt' :
		init_decompress();
	case 'DT' : }
	if(c) exit(-1);

	// Generate initial comment, then display comment record
	ptr += sprintf(ptr = Cmap, "TD %u.%u %s", Header.TDversion >> 4,
		Header.TDversion & 15, TXT(Header.Drivetype, dt_text));
	c = Header.Datarate;
	ptr += sprintf(ptr, " %s %sFM", TXT(c & 0x7F, dr_txt), (c & 0x80) ? "" : "M");
	ptr += sprintf(ptr, " %s-step, %u sides", TXT(Header.Stepping & 0x7F, dr_step), Header.Sides);
	if(Advcomp) ptr += sprintf(ptr, " ADV");
	if(Header.DOSmode) ptr += sprintf(ptr, ", DOS");
	if(Verbose)
		fputs(Cmap, stdout);
	filename(Ofile || File, ".IMD", 0);
	fpo = fopen(Smap, "wvqb");
	if(fpo) {
		fputs("IMD ", fpo);
		fputs(Cmap, fpo); }
	if(Header.Stepping & 0x80) show_comment();
	if(Verbose)
		putc('\n', stdout);
	if(fpo) {
		fputs(CR, fpo);
		putc(0x1A, fpo); }		// EOF marker

	if((Header.Datarate & 0x7F) > 2)
		error("Unknown data rate");

	// Process each track and build .IMD track record
	while(!getblock(Thead, sizeof(Thead), 0)) {
		dump(Thead, Ttemplate, DEBUG_DUMPT);
		if(Thead.Tsectors == 0xFF)			// EOF marker
			break;
		if((compute_crc(Thead, 3, 0) & 0xFF) != Thead.Tcrc)
			error("Track header CRC error");
		if(Thead.Tside & 0x7E)
			error("Unknown track-side indicator");

		// Compute IMD mode byte
		Mode = (2 - (Header.Datarate & 0x0F)) + 3;	// Inital mode based on rate
		if((Mode == 4) && Lowden)					// 300->250
			Mode = 3;
		if(Thead.Tside & 0x80)						// Sector FM indicator
			Mode -= 3;
		else if((Header.Datarate & 0x80) && FM)		// Disk FM indicator
			Mode -= 3;

		Cylinder = Thead.Tcylinder;
		Head = Thead.Tside & 1;
		Dtop = Stop = 0;							// Reset buffers
		Wmask = Warn;
		++Tracks;

		// Read each sector, buffer & build maps
		for(s=0; s < Thead.Tsectors; ++s)
			process_sector();
		if(!issame(Smap, Size = Smap[0], Stop))	// 765 cannot do mixed sector sizes
			error("Cannot do mixed sector size within track");
		if(!issame(Cmap, Cylinder, Stop))		// Cylinder map required
			Head |= 0x80;
		if(!issame(Hmap, Head & 0x0F, Stop))	// Head map required
			Head |= 0x40;

		// Remove sectors as requested
		remove_sectors();

		for(i=0; i < Stop; ++i) {
			f = Fmap[i];
			if(f & 0x40)
				warn("\x04No ID field - possible bogus sector");
			if(f & 0x20)
				warn("\x08No DATA field");
			if(f & 0x02)
				warn("\x10CRC error (bad sector)");
			for(s=i+1; s < Stop; ++s) {
				if(Nmap[i] == Nmap[s]) {
					warn("\x02Duplicate sector numbers");
					break; } } }

		// Write out the IMD track record
		if(fpo && Stop) {
			putc(Mode, fpo);						// Sector mode/type
			putc(Cylinder, fpo);					// Physical cylinder
			putc(Head, fpo);						// Physical head
			putc(Stop, fpo);						// # sectors
			putc(Size, fpo);						// Sector size
			for(s=0; s < Stop; ++s)					// Sector numbering map
				putc(Nmap[s], fpo);
			if(Head & 0x80) {						// Cylinder numbering map
				for(s=0; s < Stop; ++s)
					putc(Cmap[s], fpo); }
			if(Head & 0x40) {						// Head numbering map
				for(s=0; s < Stop; ++s)
					putc(Hmap[s], fpo); }
			for(s=Dtop=0; s < Stop; ++s)			// Output sector data
				write_data(s);
			if(Debug & DEBUG_ONE)
				break; } }

	if(fpo)
		fclose(fpo);

	fclose(fpi);

	if(Verbose) {
		printf("%u tracks, %u sectors converted", Tracks, Sectors);
		if(Sremove) printf(", %u sectors removed", Sremove);
		printf(".\n"); }
}
