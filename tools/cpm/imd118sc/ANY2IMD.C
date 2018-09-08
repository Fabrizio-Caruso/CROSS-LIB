/*
 * ANY2IMD - Any format to ImageDisk .IMD via floppy write/read
 *
 * A simple way to convert ANY image type to .IMD format is to simply write
 * the image to a floppy disk using whatever tool was designed for that
 * purpose, and then read it back with ImageDisk. This works, however it
 * gets tedious because you need to run both the disk-creator and ImageDisk
 * separately for each file  you wish to convert, entering the filename twice
 * (once for each program), and noting and re-entering the comment field.
 *
 * ANY2IMD simplifies this process process by building a list of files of
 * the foreign image type and automatically entering the names into the
 * programs, as well as capturing the comment field displayed by the
 * foreign imaging program and entering it into the .IMD file.
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
#include <keys.h>
#include "version.h"

#define	TICK	peekw(0x40,0x6C)

#define	ENTER	0x1C0D				// Scancode for ENTER key
#define	SPACE	0x3920				// Scancode for SPACE key
#define	ESC		0x011B				// Scancode for ESC   key

unsigned
	X,								// Current screen X position
	Y,								// Current screen Y position
	C,								// Input character
	Vseg = 0xB800,					// Video segment (Assumes COLOR)
	Timer,							// Active timer
	Ptop,							// Top of string pool
	Fcount,							// Count of files
	Fcurrent,						// Current file
	scrbuf[25][80];					// Screen save buffer

int
	Cx=0,							// Capture X position
	Cy=0,							// Capture Y position
	Cw=50,							// Capture Width
	Ch=10,							// Capture Height
	Fx = -1,						// File display X position
	Fy = -1;						// File display Y position

FILE
	*fp;

unsigned char
	Imd,							// IMD is active (reading disk)
	Prompt = 255,					// Prompt for operation
	Didcap,							// Capture was performed
	Fflag,							// Filename update flag
	Sactive,						// Script is active flag
	*Sptr,							// Script pointer
	*Fptr,							// Active filename pointer
	*ptr,							// General pointer
	*Iname = "ANY2IMD",				// .INI filename
	Kdel,							// Key delay
	Wcmd[129],						// Write executable command
	Rcmd[129],						// Read  executable command
	Tail[128],						// Command execution tail
	Wscript[129],					// Write script
	Rscript[129],					// Read  script
	pool[4096],						// Filename storage pool
	capbuf[2107];					// Video capture buffer

// Prefix codes for "ALPHA" keys
char alpha_prefix[] = {
	0x1E, 0x30, 0x2E, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24,
	0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1F, 0x14,
	0x16, 0x2F, 0x11, 0x2D, 0x15, 0x2C };

// Codes for misc. ASCII keys
unsigned ascii_prefix[] = {
	0x3920, 0x0231, 0x0332, 0x0433, 0x0534, 0x0635, 0x0736, 0x0837,
	0x0938, 0x0A39, 0x0B30, 0x0C2D, 0x0D3D, 0x2B5C, 0x297E, 0x0221,
	0x0340, 0x0423, 0x0524, 0x0625, 0x0826, 0x092A, 0x0A28, 0x0B29,
	0x0C5F, 0x0D2B, 0x2B7C, 0x1A5B, 0x1B5D, 0x273B, 0x2827, 0x332C,
	0x342E, 0x352F, 0x1A7B, 0x1B7D, 0x273A, 0x2822, 0x333C, 0x343E,
	0x353F, 0x2960, 0x075E,	0 };

// Names for special keys
unsigned key_names[] = {
	'F1', 'F2', 'F3', 'F4', 'F5', 'F6', 'F7', 'F8', 'F9', 'F0',		// 80-89
	'S1', 'S2', 'S3', 'S4', 'S5', 'S6', 'S7', 'S8', 'S9', 'S0',		// 8A-93
	'C1', 'C2', 'C3', 'C4', 'C5', 'C6', 'C7', 'C8', 'C9', 'C0',		// 94-9D
	'A1', 'A2', 'A3', 'A4', 'A5', 'A6', 'A7', 'A8', 'A9', 'A0',		// 9E-A7
	'EN', 'BS', 'TB', 'BT',											// A8-AB
	'ES', 'IN', 'DL', 'K+', 'K-',									// AC-B0
	'LA', 'RA', 'UA', 'DA',											// B1-B4
	'HO', 'EN', 'PU', 'PD',											// B5-B8
	0 };

// Key codes not easily calculated from table position
unsigned misc_code[] = {
	0x1C0D, 0x0E08, 0x0F09, 0x0F00,			// Enter, Backspace, Tab, Backtab
	0x011B, 0x5200, 0x5300, 0x4E2b, 0x4A2D,	// Esc, Ins, Del, Key+, Key-
	0x4B00, 0x4D00, 0x4800, 0x5000,			// Left, Right, Up, Down
	0x4700, 0x4F00, 0x4900, 0x5100 };		// Home, End, PgUp, PgDn
#define	NUM_MISC	17

/*
 * Locate extension for a filename
 */
unsigned char *findext(unsigned char *source)
{
	unsigned char *p;
	unsigned char d;
	d = 0;
	for(;;) switch(*source++) {
		case 0   : return d ? p : 0;
		case '.' : d = 255; p = source; continue;
		case ':' :
		case '\\': d = 0; }
}

/*
 * Get a value element from the command option
 */
unsigned getvalue(unsigned char e)
{
	unsigned v, c;
	unsigned char *p;
	p = ptr;
	v = 0;
	do {
		if(isspace(c = *ptr++))
			continue;
		if((c -= '0') > 9) {
			printf("Bad number: %s\n", p);
			exit(-1); }
		v = (v * 10) + c; }
	while(*ptr != e);
	if(e)
		++ptr;
	return v;
}

/*
 * Process command argument
 */
void command_option(void)
{
	unsigned i, v;
	unsigned char *p, c, name[13];

	v = (toupper(*ptr++) << 8) | toupper(*ptr++);

	// Check for double-character options
	if(*ptr++ == '=') switch(v) {
		default:
			printf("Bad option: %s\n", ptr-3);
			exit(-1);
		case 'CC' :
			Cx = getvalue(',');
			Cy = getvalue(',');
			Cw = getvalue(',');
			Ch = getvalue(0);
			return;
		case 'FD' :
			Fx = getvalue(',');
			Fy = getvalue(0);
			return;
		case 'WE' : memset(Wcmd, 0, sizeof(Wcmd)); strcpy(Wcmd, ptr);	return;
		case 'RE' : memset(Rcmd, 0, sizeof(Rcmd)); strcpy(Rcmd, ptr);	return;
		case 'WS' : p = Wscript;			goto dokey;
		case 'RS' : p = Rscript;			dokey:
			do {
				if((c = *ptr++) == '~') {			// Lead-in
					if((v = *ptr++) != '~') {		// Special command
						switch(v = (toupper(v) << 8) | toupper(*ptr++)) {
						case 'WA' :
							c = getvalue(';') / 55;
							*p++ = 0xF0;
							goto found;
						case 'FN' :	c = 0xF1; goto found;
						case 'CC' : c = 0xF2; goto found;
						case 'ST' : c = 0xF3; goto found; }
						for(i=0; key_names[i]; ++i) {
							if(key_names[i] == v) {
								c = i | 0x80;
								goto found; } }
						printf("Unknown escape: ~%c%c\n", v >> 8, v & 255);
						exit(-1); } }
		found:	*p++ = c; }
			while(c);
			return; }

	// Check for single-character options
	switch(v) {
	case '/I' : Iname = ptr-1; return;
	case '/P' : Prompt = 0;	return; }

	// Load filenames
	if(find_first(ptr-3, 0, name, &i, &i, &i, &i, &i)) {
		printf("Could not find: %s\n", ptr-3);
		exit(-1); }
	do {
		++Fcount;
		p = name;
		do {
			if(*p == '.')		// Trim extension
				*p = 0; }
		while(*p++);
		p = name;
		while(pool[Ptop++] = *p++);
		if(Ptop >= sizeof(pool))
			abort("Memory exhausted"); }
	while !find_next(name, &i, &i, &i, &i, &i);
}

/*
 * INT-16 (BIOS keyboard services) replacement interrupt handler
 */
asm {
SAVAX	DW		0				// Saved AX during interrupt processing
DOS_SS	DW		0				// DOS saved SS when MC active
DOS_SP	DW		0				// DOS saved SP when MC active
MC_SP	DW		06000h			// Stack to use for MC
;
; INT-16 handler
;
INT16:	CMP		AH,1				; 0 or 1 (CHECK/GET)
		JA		doint				; No, pass to original handler
; AH=0 (Get) or AH=1 (Check) - context switch to MC code to process
		CLI
		MOV		CS:DOS_SS,SS		; Save the DOS stack segment
		MOV		CS:DOS_SP,SP		; Save the DOS stack pointer
		MOV		CS:SAVAX,AX			; Save AX
		MOV		AX,CS				; Get CS
		MOV		SS,AX				; Set SP
		MOV		SP,CS:MC_SP			; Get MICRO-C stack pointer
		PUSH	BX
		PUSH	CX
		PUSH	DX
		PUSH	SI
		PUSH	DI
		PUSH	ES
		PUSH	DS
		MOV		DS,AX
		PUSH	CS:SAVAX			; Pass AX as parameter
		STI
		CALL	_keyboard			; Invoke MC handler
		CLI
		POP		BX					; Clear stacked parm
		POP		DS
		POP		ES
		POP		DI
		POP		SI
		POP		DX
		POP		CX
		POP		BX
		MOV		SS,CS:DOS_SS		; Restore DOS stack segment
		MOV		SP,CS:DOS_SP		; Restore DOS SP
		STI
		AND		AX,AX				; Key returned?
		JNZ		goh					; Yes, return to caller
		MOV		AX,CS:SAVAX			; Restore parm & pass to native
; Long jump to interrupt handler
doint:	DB		0EAh				; Far JMP
INT16O:	DW		0					; Original handler segment
INT16S:	DW		0					; Original handler offset
goh:	RETF	2
}

/*
 * Test for character via direct BIOS call to original handller
 */
unsigned tstchar(void) asm
{
		MOV		AH,01h				; Test for char
		PUSHF						; Fake interrupt
		PUSH	CS					; Fake long call
		CALL	doint				; Execute handler
		JNZ		isok				; We have character
		XOR		AX,AX				; Reset to zero
isok:
}

/*
 * Get character via direct BIOS call to original handler
 */
unsigned getchar(void) asm
{
		XOR		AH,AH				; Get char
		PUSHF						; Fake interrupt
		PUSH	CS					; Fake long call
		CALL	doint				; Execute handler
}

/*
 * Allocate INT16 interrupt vector (install out handler)
 */
void getint(void) asm
{
; Read OLD vector & save for later restore
		MOV		AX,3516h			; Get INT 16 vector
		INT		21h					; Ask DOS
		MOV		WORD PTR INT16O,BX	; Save offset
		MOV		WORD PTR INT16S,ES	; Save segment
; Set vectors to our handler
		MOV		DX,OFFSET INT16
		MOV		AX,2516h			; Set INT vector
		INT		21h					; Ask DOS
}

/*
 * Release INT16 interrupt vector
 */
void relint(void) asm
{
		PUSH	DS					; Save DS
		MOV		DX,CS:WORD PTR INT16O	; Read old handler offset
		MOV		AX,CS:WORD PTR INT16S	; Read old handler segment
		MOV		DS,AX				; Most be in DS
		OR		AX,DX				; Did we record it?
		JZ		noint				; No, never installed
		MOV		AX,2516h			; Set INT vector
		INT		21h					; Ask DOS
noint:	POP		DS					; Restore DS
}

/*
 * Compute scancode from ASCII/Encoded byte value
 */
unsigned scancode(unsigned char c)
{
	unsigned i, k;

	if(isalpha(c))								// A-Z
		return (alpha_prefix[(c & 0x1F) - 1] << 8) | c;

	for(i=0; k = ascii_prefix[i]; ++i) {		// Other ASCII
		if((k & 255) == c)
			return k; }

	// Handle special cases
	if((c >= 0x80) && (c < 0x8A)) {				// F1-F10
		k = 0x3B;
		c -= 0x80;
		goto dofunc; }
	if((c >= 0x8A) & (c < 0x94)) {				// Shift F1-F10
		k = 0x54;
		c -= 0x8A;
		goto dofunc; }
	if((c >= 0x94) && (c < 0x9E)) {				// Ctrl F1-F10
		k = 0x5E;
		c -= 0x94;
		goto dofunc; }
	if((c >= 0x9E) && (c < 0xA8)) {				// Alt F1-F10
		k = 0x68;
		c -= 0x9E;
dofunc:	return (k + c) << 8; }

	if((c >= 0xA8) && (c < (0xA8+NUM_MISC)))	// Misc. function
		return misc_code[c - 0xA8];

	return 0;									// Unknown
}

/*
 * Local video functions
 */

// Write character to video screen
void vputc(unsigned x, unsigned y, unsigned char c)
{
	if((x > 79) || (y > 24))
		abort("Bad offset");
	poke(Vseg, (x*2) + (y*160), c);
}

// Write string to video screen
void vputs(unsigned char *s)
{
	while(*s) {
		vputc(X++, Y, *s++);
		if(X >= 80) {
			X = 0;
			++Y; } }
}

// Formatted print to video screen
register void vprintf(unsigned args)
{
	unsigned char buffer[81];
	_format_(nargs() * 2 + &args, buffer);
	vputs(buffer);
}

/*
 * Clear screen & home physical cursor
 */
void vcls(void)
{
	unsigned i;

	for(i=X=Y=0; i < 4000; i += 2)
		poke(Vseg, i, 0x0720);
	asm {
		MOV		AH,02h			// Set cursor
		XOR		DX,DX			// Row=0 Column=0
		MOV		BH,DL			// Page = 0
		INT		10h				// Ask BIOS
	}
}

/*
 * Capture applicaton screen to 'scrbuf'
 */
void capture_screen(void)
{
	unsigned i, j, o;
	for(i=o=0; i < 25; ++i) {
		for(j=0; j < 80; ++j)
			scrbuf[i][j] = peekw(Vseg, (j*2)+o);
		o += 160; }
}

/*
 * Restore applicaton screen from 'scrbuf'
 */
void restore_screen(void)
{
	unsigned i, j, o;
	for(i=o=0; i < 25; ++i) {
		for(j=0; j < 80; ++j)
			pokew(Vseg, (j*2)+o, scrbuf[i][j]);
		o += 160; }
}

/*
 * Capture text from application screen at Cx,Cy,Cw,Ch
 */
unsigned capture(unsigned char *ptr)
{
	unsigned i, j, o;
	unsigned char *p, *p1;
	p = ptr;
	o = (Cy * 160) + (Cx*2);
	for(i=0; i < Ch; ++i) {
		p1 = p;
		for(j=0; j < Cw; ++j)
			*p++ = peek(Vseg, (j*2)+o);
		while(p1 < p) {
			if((*p1 < ' ') || (*p1 > 0x7E))
				*p1 = ' ';
			++p1; }
		o += 160;
trim:	if(j) switch(*(p-1)) {
		case ' ':
		case 0 : --j; --p; goto trim; }
		*p++ = '\n'; }
	*p++ = 0;
	return p - ptr;
}

/*
 * Test for and buffer keystroke (part of setup_capture)
 */
void xtstc(void)
{
	unsigned i;
	if(!C) {
		for(i=0; i < 5; ++i) {
			delay(55);
			if(tstchar()) {
				C = getchar();
				break; } } }
}

/*
 * Setup capture window
 */
int setup_capture(void)
{
	unsigned i, w, h;
	char f;

	C = 0;
	f = 0xDB;
	for(;;) {
		// Force within 80x25 screen limits
		if(Cx < 0) Cx = 0;
		if(Cx > 79) Cx = 79;
		if(Cy < 0) Cy = 0;
		if(Cy > 24) Cy = 24;
		if(Cw < 1) Cw = 1;
		if(Ch < 1) Ch = 1;
		if((Cx+Cw) > 80) Cw = 80 - Cx;
		if((Cy+Ch) > 25) Ch = 25 - Cy;
		// Draw box
		w = Cw - 1;
		h = Ch - 1;
		for(i=1; i < w; ++i) {
			vputc(Cx+i, Cy, 0xC4);
			vputc(Cx+i, Cy+h, 0xC4); }
		for(i=1; i < h; ++i) {
			vputc(Cx, Cy+i, 0xB3);
			vputc(Cx+w, Cy+i, 0xB3); }
		vputc(Cx, Cy, f);
		vputc(Cx+w, Cy, f);
		vputc(Cx, Cy+h, f);
		vputc(Cx+w, Cy+h, 0xDB);
		xtstc();			// Check for character
		// Clear box (restore underlying text)
		for(i=0; i < w; ++i) {
			vputc(Cx+i, Cy, scrbuf[Cy][Cx+i]);
			vputc(Cx+i, Cy+h, scrbuf[Cy+h][Cx+i]); }
		for(i=0; i < h; ++i) {
			vputc(Cx, Cy+i, scrbuf[Cy+i][Cx]);
			vputc(Cx+w, Cy+i, scrbuf[Cy+i][Cx+w]); }
		vputc(Cx, Cy, scrbuf[Cy][Cx]);
		vputc(Cx+w, Cy, scrbuf[Cy][Cx+w]);
		vputc(Cx, Cy+h, scrbuf[Cy+h][Cx]);
		vputc(Cx+w, Cy+h, scrbuf[Cy+h][Cx+w]);
		xtstc();			// Check for character
		// Act on any received command keys
		i = C;
		C = 0;
		switch(i) {
		case ESC	: return 0;			// Escape (cancel)
		case ENTER	: return 255;		// Enter  (accept)
		case _F1	: f ^= 0x1E; }
		if(f == 0xC5) switch(i) {		// Move - adjust Cw,Ch
		case _UA	: --Ch;			continue;
		case _DA	: ++Ch;			continue;
		case _LA	: --Cw;			continue;
		case _RA	: ++Cw;			continue; }
		else switch(i) {				// Resize - Adjust Cx,Cy
		case _UA	: --Cy;			continue;
		case _DA	: ++Cy;			continue;
		case _LA	: --Cx;			continue;
		case _RA	: ++Cx;			continue; } }
}

/*
 * Keyboard interrupt handler
 */
void keyboard(unsigned ax)
{
	unsigned r;

//	static unsigned c;				// Debug only
//	X=Y=0; vprintf("%-5u %04x[%02x%02x] %04x", ++c, Sptr, Sptr[0], Sptr[1], ptr);
//	if(ptr) vprintf("[%02x %02x]", ptr[0], ptr[1]);

	// Update application screen with filename
	// when it is likely to have changed.
	if(!Fflag) {
		if(Fx >= 0) {
			X=Fx; Y = Fy;
			vprintf("%s ", Fptr); }
		Fflag = 255; }


doinput:
	if(ptr) {			// Key entry string active - perform it
		if(*ptr) {
			if(ax & 0xFF00) {		// Test for character
				r = TICK;
				if(Timer == r)
					return 0;
				Timer = r;
				return scancode(*ptr); }
			return scancode(*ptr++); }
		ptr = 0; }

xscript:	// If a script is active, continue execution
	if(Sactive && *Sptr) {
		if(ax & 0xFF00) {				// Test for character
			r = TICK;
			if(Timer == r)				// Always wait 1 tick
				return 0;
			Timer = r;
			if(Kdel) {					// Delay requested
				--Kdel;
				return 0; }
			switch(*Sptr++) {
			case 0xF0 :					// Delay
				Kdel = *Sptr++;
				return 0;
			case 0xF1 :					// Enter filename (full)
				ptr = Fptr;
				goto doinput;
			case 0xF2 :					// Capture input
				capture_screen();
				if(setup_capture()) {	// Display/update window
					Didcap = 255;		// Indicate we proceeded
				capture(capbuf); }		// Perform the actual capture
				restore_screen();
				Fflag = 0;				// Refresh filename
				goto xscript;			// continue script
			case 0xF3 :					// Pause script
				Sactive = Fflag = 0;
				goto dokey; }
			return scancode(*--Sptr); }

		switch(r = *Sptr++) {
		case 0xF0 :						// Ignore delay for GET
			++Sptr;						// Skip operand
			goto xscript;
		case 0xF1 :						// Enter filename (full)
			ptr = Fptr;
			goto doinput;
		case 0xF2 :						// Capture input
			capture_screen();
			if(setup_capture()) {		// Display/update window
				Didcap = 255;			// Indicate we proceeded
			capture(capbuf); }			// Perform the actual capture
			restore_screen();
			Fflag = 0;
			goto xscript;
		case 0xF3 :						// Pause script
			Sactive = Fflag = 0;
			goto dokey; }
		return scancode(r); }

dokey:
	// No special processing - test for real-key
	if(ax & 0xFF00) {					// Test for character
		r = tstchar();					// Test key via BIOS
		if(r == 0x7500) {				// Special key (CTRL-END)
			if(!Sactive) {				// Is script paused?
				getchar();				// Clear from buffer
				Timer = TICK;			// Init timer
				Sactive = 255;			// Restart script
				Fflag = 0;				// Redisplay name
				goto xscript; }			// And proceed
			r = 0; }
		return r; }

	r = getchar();						// Get key via BIOS
	if(r == 0x7500) {					// Special key (CTRL-END)
		if(!Sactive) {					// Is script paused?
			Timer = TICK;				// Init timer
			Sactive = 255;				// Restart script
			Fflag = 0;					// Redisplay name
			goto xscript; }				// And proceed
		goto dokey; }

	return r;
}

/*
 * Set script string to execute & command tail
 */
void setcmd(unsigned char *p, unsigned char *t)
{
	unsigned char *p1;

	Sptr = p;
	Sactive = 255;
	Timer = TICK;
	Kdel = 5;
	Fflag = ptr = 0;

	p = Tail;
	for(;;) switch(*p = *t++) {
		case 0 :
			return;
		case '~' :			// Filename pointer
			if((toupper(*t) == 'F') && (toupper(t[1]) == 'N')) {
				p1 = Fptr;
				while(*p = *p1++)
					++p;
				t += 2;
				continue; }
		default:
			++p; }
}

/*
 * Display next-up command & prompt for action
 */
int showcmd(unsigned char op)
{
	vcls();
	if(Prompt) {
		printf("File %u of %u\n", Fcurrent, Fcount);
		printf("Exec: %s\n", op ? Rcmd : Wcmd);
		printf("Opts: %s\n", Tail);
		if(Didcap)
			printf("Comment captured: Cx=%u Cy=%u Cw=%u Ch=%u\n", Cx, Cy, Cw, Ch);
		printf("%sing '%s' %s floppy- ENTER to proceed, SPACE to skip, ESC to stop.\n",
			op ? "READ" : "WRITE", Fptr, op ? "from" : "to");

		for(;;) switch(getchar()) {
			case ESC:
				relint();
				exit(-1);
			case SPACE :
				return 255;
			case ENTER :
				vcls();
				return 0; } }
	return 0;
}

unsigned char help[] = { "\
ANY-2-IMageDisk "#VERSION" / "#__DATE__"\n\
Copyright 2007-"#CYEAR" Dave Dunfield - All rights reserved.\n\n\
Use:	ANY2IMD filespec... [options]\n\n\
opts:	/Ifile		- Process file.INI		[ANY2IMD]\n\
	/P		- do not Pause before commands\n\
	CC=x,y,w,h	- Comment Capture window	[0,0,50,10]\n\
	RE=execfile [*1]- Read Executable file\n\
	WE=execfile [*1]- Write Executable file\n\
	RS=string [*2]	- Read Script			[none]\n\
	WS=string [*2]	- Write Script			[none]\n\
\nOptions may also be placed in .INI - processed at startup.\n\
[*1] Can give command options in .INI\n\
[*2] See documentation for control codes.\n" };

/*
 * Main program
 */
main(int argc, char *argv[])
{
	unsigned i, t;
	unsigned char *Wt, *Rt;

	// Process command line arguments
	for(i=1; i < argc; ++i) {
		ptr = argv[i];
		command_option(); }

	// Look for .INI file in current directory
	sprintf(Tail, "%s.INI", Iname);
	if(fp = fopen(Tail, "r"))		// From current dir
		goto doini;
	// Not found - look for .INI in home directory
	Wt = Rt = argv[0];
	while(*Wt) switch(*Wt++) {
		case ':' :
		case '\\' : Rt = Wt; }
	*Rt = 0;
	sprintf(Tail, "%s%s.INI", argv[0], Iname);
	if(fp = fopen(Tail, "r")) {
doini:	while(fgets(ptr = capbuf+256, 128, fp)) {
			while(isspace(*ptr))
				++ptr;
			switch(*ptr) {
			case ';' :
			case 0 : continue; }
			for(t=0;  ptr[t]; ++t);
			while(t && isspace(ptr[t-1]))
				--t;
			ptr[t] = 0;
			command_option(); } }

	if(!Ptop)			// No images - display help
		abort(help);

	if(!(*Wcmd && *Rcmd))
		abort("No Read/Write program defined.");

	// Setup Read/Write command/tail buffers for exec()
	Wt = Wcmd;
	while(*Wt) {
		if(isspace(*Wt++)) {
			*(Wt-1) = 0;
			break; } }
	Rt = Rcmd;
	while(*Rt) {
		if(isspace(*Rt++)) {
			*(Rt-1) = 0;
			break; } }

	asm {		// Drop stack by 2k for interrupt
		MOV		AX,SP
		SUB		AX,2048
		MOV		CS:MC_SP,AX
	}

	// Walk list of files, running Read program, then Write program
	t = 0;
	while(t < Ptop) {
		// Get next .TD0 file to process
		Fptr = &pool[t];			// Get filename
		while(pool[t++]);			// Advance to next
		++Fcurrent;

		// Execute Read program
		memset(capbuf, 0, sizeof(capbuf));	// Clear capture
		setcmd(Wscript, Wt);				// Set for read script
		getint();							// Allocate interrupts
		if(showcmd(Imd = Didcap = 0)) {
			relint();
			continue; }
		i = exec(Wcmd, Tail);				// Run read program
		if(i)
			printf("%s failed(%u)\n", Wcmd, i);

		// Trim comment
		if(*(ptr = capbuf)) {
			while(*ptr && (*ptr == '\n'))
				++ptr;
			for(i=0; ptr[i]; ++i);
			while(i) {
				if(ptr[i-1] != '\n')
					break;
				--i; }
			ptr[i++] = '\n';
			ptr[i] = 0; }

		// If we captured - write empty .IMD file with comment
		if(Didcap) {
			sprintf(scrbuf, "%s.IMD", Fptr);
			fp = fopen(scrbuf, "wvq");
			fprintf(fp, "IMD from %s\n", Fptr);
			fputs(ptr, fp);
			fclose(fp); }

		// Execute ImageDisk to write file (read floppy)
		setcmd(Rscript, Rt);
		if(!showcmd(Imd = 255)) {
			i = exec(Rcmd, Tail);
			if(i)
				printf("%s failed(%u)\n", Wcmd, i); }
		relint(); }
}
