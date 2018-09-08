/*
 * TESTFDC - a program to evaluate a PC floppy disk controller.
 *
 * This program evaluates the floppy disk controller in your system to
 * determine how well it performs at single/double density at various data
 * rates.  It also tests MFM operation with 128 byte sectors.  Many PCs
 * cannot access these disk types.  If you plan to use my ImageDisk program
 * to archive and restore non-PC format disks, this program should help you
 * find a PC thats up to the task.
 *
 * TESTFDC performs direct hardware access to the floppy disk controller
 * chip, and therefore cannot run under operating systems which do not
 * permit application programs such access. These include Windows NT,
 * 2000 and XP. I strongly recommend running the program under pure DOS.
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
#include <setjmp.h>
#include "version.h"

#define	TIMEOUT	36			// 2 seconds (55ms ticks)
#define	MDEL	100			// Minimum delay

#define	FDCS	(FDC+2)		// Offset to FDC status register
#define	FDCD	(FDC+3)		// Offset to FDC data   register

// Report indexes
#define	SD250	0
#define	SD300	1
#define	SD500	2
#define	DD250	3
#define	DD300	4
#define	DD500	5
#define	XD250	6
#define	XD300	7
#define	XD500	8

unsigned
	FIseg,					// Floppy interrupt segment
	FIoff,					// Floppy interrupt offset
	Lkey,					// Last key pressed
	FDC = 0x3F2,			// Base address of FDC
	SEG,					// External segment (aligned to 64k boundary)
	DMA,					// DMA segment
	Timeout,				// FDC timeout counter
	Rcount,					// Top of FDC result list	(#results)
	Stop,					// Top of sector list		(#sectors)
	SKdel = 55;				// Seek delay

unsigned char
	Fdone,					// FDC interrupt done
	Drive = 255,			// Selected drive (0-3 = A-D)
	Dtype = 255,			// Drive type
	Cylinders = 80,			// Number of cylinders
	Cylinder,				// Current cylinder
	Head,					// Current head
	Sector,					// Current sector
	Ssize,					// Sector size
	Mode,					// Data rate / Density
	Mfm,					// Double-density indicator
	Stest = 255,			// Test Single-Density
	Dtest = 255,			// Test Double-Density
	Xtest = 255,			// Test MFM-128 byte sector mode
	*Rfile,					// Report file
	Gl = 0x1B,				// Gap length
	Glf = 0x54,				// Gap length (format)
	Ffill = 0xE5,			// Format fill byte
	SPsrt = 8,				// Step rate
	SPhlt = 127,			// Head load time
	SPhut = 15,				// Head unload time
	Eskip,					// Error skip mode
	Result[15],				// Results from FDC
	Resync,					// Resync state counter
	Texit,					// Test new exit code
	Smap[256],				// Sector numbering map
	Report[9];				// Final report

jmp_buf
	xjump;					// Exit to main

FILE
	*fp;					// General file pointer

unsigned Mtext[] = {		// Translate Mode%3 to actual data rate
	500, 300, 250 };

// Digital Output Register - Drive select (0-3) values
unsigned char DORsel[] = { 0x14, 0x25, 0x46, 0x87 };

unsigned char *drives[] = {		// Drive type table
	"360",
	"720",
	"1.2",
	"1.44",
	0 };

unsigned char *Mdesc[] = {		// Mode descriptions
	"Single-Density",
	"Double-Density",
	"Double/128byte" };

// Mode tables:			  #s  Si  Md  Gl  Glf Mode Report
unsigned char SL300[] = { 12,  0,  2,  7,  9,   0, SD250 };
unsigned char SH300[] = { 15,  0,  0, 16, 25,   0, SD500 };
unsigned char DL300[] = {  9,  2,  5, 27, 28,   1, DD250 };
unsigned char DH300[] = { 18,  2,  3, 27, 28,   1, DD500 };
unsigned char XL300[] = { 23,  0,  5, 27, 28,   2, XD250 };
unsigned char XH300[] = { 50,  0,  3, 27, 60,   2, XD500 };

unsigned char SL360[] = { 12,  0,  1,  7,  9,   0, SD300 };
unsigned char SH360[] = { 26,  0,  0,  7, 27,   0, SD500 };
unsigned char DL360[] = {  9,  2,  4, 27, 28,   1, DD300 };
unsigned char DH360[] = { 15,  2,  3, 42, 80,   1, DD500 };
unsigned char XL360[] = { 23,  0,  4, 27, 28,   2, XD300 };
unsigned char XH360[] = { 41,  0,  3, 27, 28,   2, XD500 };

unsigned char help[] = { "\n\
Evaluates the floppy controller in your system to determine how it performs at\n\
single/double density at various data rates. Also tests MFM operation with 128\n\
byte sectors. If you plan to use ImageDisk to archive and restore non-PC format\n\
disks, this program should help you find a PC thats up to the task.\n\
\nUse: TESTFDC drive [type-override] [options]\n\n\
drive:	Physical floppy drive to test (A: - D:)\n\n\
type		360 = DD 40 track	 1.2 = HD 80-track (5.25)\n\
override:	720 = DD 80 track	1.44 = HD 80-track (3.5)\n\n\
opts:	/D	- skip standard Double-density test (should always work)\n\
	/R	- please Read this!\n\
	R=file	- Write final report to file.\n\
	/S	- skip Single-density test\n\
	/X	- eXclude MFM/128 test\n\
\nYou will be prompted to insert low and/or high density media.   TESTFDC  only\n\
writes to the top 5 tracks of the disk,  allowing you to use a single disk to\n\
boot and to test. TESTFDC should be run from a bare DOS boot (no windows).\n" };

unsigned char help1[] = { "\n\
I am attempting to build a registry of mainboard and add-in floppy controllers\n\
and their capabilities. The latest version of this registry can be found with\n\
the TESTFDC program at: http://www.classiccmp.org/dunfield/img\n\n\
If you test a system/controller not already listed, or exhibiting results\n\
differing from those that I have on file - please use the R=file option to\n\
save the TESTFDC report(s) and send them to me along with:\n\n\
 - The manufacturer and model number of your mainboard or add-in controller\n\
 - The floppy controller chip used.\n\n\
My contact information is available on the site.\n\n\Dave Dunfield.\n" };

/*
 * Deselect current drive
 */
void deselect_drive(void)
{
	out(0x0A, 6);		// Mask channel-2 DMA
	out(FDC, 0x04);		// Disable FDC
}

/*
 * Report error with formatted message and terminate
 */
register error(unsigned args)
{
	unsigned char buffer[100];
	_format_(nargs() * 2 + &args, buffer);
	printf(" %s\n", buffer);
	out(FDC, 0);
	delay(MDEL);
	deselect_drive();
	longjmp(xjump, buffer);
}

/*
 * Floppy disk interrupt handler - send EOI and set Fdone flag
 */
asm {
FINT:	PUSH	AX				; Save AX
		MOV		AL,20h			; Get EOT
		OUT		20h,AL			; Send to PIC
		MOV		CS:_Fdone,255	; Set flag
		POP		AX				; Restore AX
		IRET
}

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

/*
 * ESC key detected - exit with message
 */
void xabort(void)
{
	Eskip = 255;
	error("!Abort by operator");
}

/*
 * Write command byte to FDC
 */
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
		IN		AL,DX				; No - read to clear
		JMP	short wrf1				; And go again
wrf3:	MOV		AX,4[BP]			; Get data to write
		OUT		DX,AL				; Write to FDC data
}

/*
 * Wait for interrupt from the FDC
 * Last byte of command is sent here so that we can enable
 * interrupt before commands begins.
 */
void waitirq(c)
{
	asm {
		MOV		DGRP:_Fdone,0		; Reset indicator
		MOV		AX,4[BP]			; Get argument
		PUSH	AX					; Pass it
		CALL	_wrfdc				; Write it
		POP		AX					; Cleanup
		XOR		AX,AX				; Get zero
		MOV		ES,AX				; ES = seg0
		MOV		CX,ES:[046ch]		; Read BIOS tick
wai1:	CMP		DGRP:_Fdone,0		; Done flag set?
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
	Eskip = 255;
	error("!No FDC interrupt");
}

/*
 * Initialize data transfer mode (rate and density)
 */
void initmode(void)
{
	unsigned x;
	Mfm = (Mode / 3) ? 0x40 : 0x00;
	out(FDC+5, x = Mode % 3);
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
	char x;
	x = 2;
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
	delay(55);
	if(c = kbtst()) {
		if((Lkey = c) == 0x1B)
			xabort(); }
	wrfdc(0x0F);					// Seek command
	wrfdc(Drive);					// Drive ID
	out(FDC, DORsel[Drive] | 0x08);	// Enable IRQ
	waitirq(Cylinder); 				// cylinder
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
int resync(void)
{
	unsigned c;
	if(Result[1] & 0x02)	// Write protect
		return 255;
	c = Cylinder;
	switch(++Resync) {
	case 2 :
	case 4 :
		recal(); delay(250); seek(c);
	case 1 :
	case 3 :
		return 0; }
	return 255;
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
void diskerror()
{
	unsigned char r, *p;
	p = 0;
	printf(" !error (%u)", Sector);
	if(Result[0] & 0x08) printf(p=" EC");
	r = Result[1];
	if(r & 0x80) printf(p=" EndCyl");
	if(r & 0x20) printf(p=" Data");
	if(r & 0x10) printf(p=" Overun");
	if(r & 0x04) printf(p=" NoSector");
	if(r & 0x02) { Eskip = 255; printf(p=" WriteProt"); }
	if(r & 0x01) printf(p=" NoAdrMark");
	if(!p) {
		printf(" %02x %02x %02x",
			Result[0], Result[1], Result[2]); }
	error("");
}

/*
 * Read a sector from the disk
 */
int read_sector(unsigned index)
{
	unsigned r;
	r = Resync = 0;
retry:
	wrcmd(0x06);
	wrfdc(Cylinder);				// Cylinder
	wrfdc(Head);					// Head
	wrfdc(index+1);					// Sector
	wrfdc(Ssize);					// N
	wrfdc(Sector);					// EOT
	wrfdc(Gl);						// GL
	initdma(0x46, 128 << Ssize);
	waitirq(Ssize ? 255 : 128);
	result(7);
	if(Result[0] & 0xC0) {
		if(resync()) {
			diskerror();
			return 255; }
		goto retry; }
	return 0;
}

/*
 * Write a sector to the disk
 */
int write_sector(unsigned index)
{
	unsigned r;
	r = Resync = 0;
retry:
	wrcmd(0x05);
	wrfdc(Cylinder);				// Cylinder
	wrfdc(Head);					// Head
	wrfdc(index+1);					// Sector
	wrfdc(Ssize);					// N
	wrfdc(Sector);					// EOT
	wrfdc(Gl);						// GL
	initdma(0x4A, 128 << Ssize);
	waitirq(Ssize ? 255 : 128);
	result(7);
	if(Result[0] & 0xC0) {
		if(resync()) {
			diskerror();
			return 255; }
		goto retry; }
	return 0;
}

/*
 * Advance DMA address by sector size
 */
void bumpdma(void)
{
	DMA +=  (((128 << Ssize) + 31) / 16);
}

/*
 * Reset FDC subsystem to known state (start of function)
 */
void reset(void)
{
//	Stop = Mfm = Ssize =
	Rcount = Cylinder = Head = Sector = Resync = Lkey = 0;
	Timeout = TIMEOUT;
	DMA = SEG;

	asm {	// Reset BIOS timeout counter
		XOR	AX,AX
		MOV	ES,AX
		MOV	BYTE PTR ES:[440h],255
	}
	out(0x0A, 6);	// Mask channel-2 DMA
	out(FDC, 0); delay(MDEL); out(FDC, 4); delay(MDEL);
	out(0x3F2, 4);	// Insure primary is clear
	out(0x372, 4);	// Insure secondary is clear
	wrfdc(0x03);	// Specify
	wrfdc((SPsrt << 4) | SPhut);	// Step, head unload
	wrfdc(SPhlt << 1);				// Head load, DMA
	result(0);
	out(FDC, DORsel[Drive]);
	delay(500);
	initmode();
	recal();
	delay(500);
}

/*
 * Format a track
 */
int format_track(void)
{
	unsigned i, o, r;

	Timeout = TIMEOUT;
	initmode();
	r = Resync = 0;

retry:
	DMA = SEG;
	for(i=o=0; i < Stop; ++i) {
		poke(DMA, o++, Cylinder);
		poke(DMA, o++, Head);
		poke(DMA, o++, Smap[i]);
		poke(DMA, o++, Ssize); }

	wrcmd(0x0D);
	wrfdc(Ssize);
	wrfdc(Stop);
	wrfdc(Glf);
	initdma(0x4A, o);
	waitirq(Ffill);

	result(7);
	if(Result[0] & 0xC0) {
		if(resync()) {
			diskerror();
			return 255; }
		goto retry; }
	return 0;
}

/*
 * Perform test for a given mode table entry
 */
int test_mode(unsigned char *mtable)
{
	unsigned i, s, r;

	if(setjmp(xjump))
		return 255;

	Stop	= mtable[0];
	Ssize	= mtable[1];
	Mode	= mtable[2];
	Gl		= mtable[3];
	Glf		= mtable[4];
	r		= mtable[6];


	printf("%uk %s:", Mtext[Mode%3], Mdesc[mtable[5]]);

	if(Eskip) {						// Error/Exit - skip test
		printf(" !Skipped\n");
		return 255; }

	Report[r] = 1;					// Assume failed (guilty until proven innocent)
	// Build sector interleave table
	for(i=s=0; i < Stop; ++i) {
		Smap[s] = i+1;
		if((s += 2) >= Stop)
			s = 1; }

	// Initialize a recognizable data buffer
	s = (128 << Ssize) * Stop;
	for(i=0; i < s; ++s)
		poke(SEG, i, i);

	// Format the disk to the specified mode
	printf(" Format");
	reset();
	for(Cylinder = Cylinders - 5; Cylinder < Cylinders; ++Cylinder) {
		Head = 0;
		seek(Cylinder);
		format_track(); }

	// Write sectors with data
	printf(" Write");
	for(Cylinder = Cylinders - 5; Cylinder < Cylinders; ++Cylinder) {
		seek(Cylinder);
		DMA = SEG;
		for(i = 0; i < Stop; ++i)
			write_sector(i); }

	for(i=0; i < s; ++i)		// Clear the data buffer
		poke(SEG, i, 0);

	// Read content back and verify
	printf(" Verify");
	for(Cylinder = Cylinders - 5; Cylinder < Cylinders; ++Cylinder) {
		seek(Cylinder);
		DMA = SEG;
		for(i = 0; i < Stop; ++i)
			read_sector(i);
		for(i=0; i < s; ++i) {
			if(peek(SEG, i) != (i & 255)) {
				printf(" Failed\n");
				return 255; } } }

	// If we get here - test passed.
	printf(" Ok\n");
	Report[r] = 2;		// Correct our assumption - this one worked!
	return 0;
}

/*
 * Prompt for media change
 */
int prompt(unsigned char *p)
{
	unsigned r;
	unsigned char buffer[100];

	deselect_drive();
	Eskip = 0;
	sprintf(buffer, "Insert %smedia - Press ENTER or ESC to skip:", p);
	fputs(p=buffer, stdout);
	for(;;) switch(kbget()) {
	case 0x1B: r = 0; goto exit;
	case '\r':
	case '\n': r = 255; goto exit; }
exit:
	putc('\r', stdout);
	while(*p) {
		++p;
		putc(' ', stdout); }
	putc('\r', stdout);
	return r;
}

/*
 * Get drive type from BIOS
 */
unsigned get_drive_type(drive) asm
{
	MOV		DL,4[BP]		; Get drive index
	MOV		AH,08h			; BIOS function
	INT		13h				; Call BIOS
	MOV		AX,BX			; Gesult
}

/*
 * Generate the final report
 */
void final_report(FILE *fp)
{
	unsigned i, l, d, m, y, h, mi, s;
	static unsigned char *rtext[] = {
		"Single-Density at %u kbps ",
		"Double-Density at %u kbps ",
		"Double-Density at %u kbps / 128 byte sectors " };
	static unsigned char *rpass[] = {
		"Not tested",
		"Failed",
		"Passed" };
	static unsigned rrate[] = {
		250, 300, 500,  250, 300, 500,  250, 300, 500 };

	get_date(&d, &m, &y);
	get_time(&h, &mi, &s);

	fprintf(fp,"Report on FDC capabilities, issued %u/%02u/%04u %u:%02u:%02u:\n",
		d, m, y, h, mi, s);
	for(i=0; i < 9; ++i) {
		l = fprintf(fp, rtext[i/3], rrate[i]);
		while(l < 55) {
			putc('.', fp);
			++l; }
		fprintf(fp, " %s\n", rpass[Report[i]]); }
}

/*
 * Main program
 */
main(int argc, char *argv[])
{
	unsigned i, j;
	unsigned char *p;

	printf("TESTFDC "#VERSION" / "#__DATE__"\nCopyright 2007-"#CYEAR" Dave Dunfield - All rights reserved.\n");

	// Parse command line arguments
	for(i=1; i < argc; ++i) {
		p = argv[i];
		switch((toupper(*p++) << 8) | toupper(*p++)) {
		case 'A:' : Drive = 0;			continue;
		case 'B:' : Drive = 1;			continue;
		case 'C:' : Drive = 2;			continue;
		case 'D:' : Drive = 3;			continue;
		case '/D' : Dtest = 0;			continue;
		case '/R' : abort(help1);		continue;
		case '/S' : Stest = 0;			continue;
		case '/E' : Texit = atox(p);	continue;
		case '/X' : Xtest = 0;			continue;
		case 'R=' : Rfile = p;			continue; }
		for(j=0; drives[j]; ++j) {
			if(!strcmp(p-2, drives[j])) {
				Dtype = j;
				goto found; } }
dohelp:
		fputs(help, stdout);
		return;
found: }

	if(Drive & 0xF0)				// No drive specified
		goto dohelp;

	SEG = alloc_seg(4096);			// 64k segment
	SEG = (SEG+0x7FF) & 0xF800;		// Advance to nearest 32k boundary

	// Determine and display BIOS drive type
	p = "Unknown"; i = 255;
	switch(get_drive_type(Drive)) {
	case 0 : p="Not installed";	break;
	case 1 : p="360K DD, 40-track"; i = 0;	break;
	case 2 : p="1.2M HD, 80-track"; i = 2;	break;
	case 3 : p="720k DD, 80-track"; i = 1;	break;
	case 4 : p="1.44M HD, 80-track";i = 3;	break;
	default: p="Unknown"; }
	printf("BIOS reports drive %c: as %s.\n", Drive+'A', p);

	// Determine drive type to use based on BIOS/override
	if(Dtype & 0xF0) {				// No override
		if((Dtype = i) &  0xF0)		// Set to BIOS - error if n/a
			abort("You must specify a drive type for this drive.\n"); }
	else {
		switch(Dtype) {
		default: goto dohelp;
		case 0 : p = "360K DD, 40-track";	break;
		case 1 : p = "720k DD, 80-track";	break;
		case 2 : p = "1.2M HD, 80-track";	break;
		case 3 : p = "1.44M HD, 80-track";	}
		printf("Testing as: %s\n", p); }

	// Take over floppy hardware interrupt
	asm {
		MOV		AX,350Eh		; Get vector 14 (FDC interrupt)
		INT		21h				; Ask DOS
		MOV		DGRP:_FIseg,ES	; Save segment
		MOV		DGRP:_FIoff,BX	; Save offset
		MOV		DX,OFFSET FINT	; New offset
		MOV		AX,250Eh		; Set vector 14
		INT		21h				; Ask DOS
	}

	// Perform tests apropriate to the drive
	switch(Dtype) {
	case 0 :		// 360
		Cylinders = 40;
	case 1 :		// 720
		if(prompt("")) {
			if(Stest)
				test_mode(SL300);
			if(Dtest)
				test_mode(DL300);
			if(Xtest)
				test_mode(XL300); }
		break;
	case 2 :		// 1.2
		if(prompt("low density ")) {
			if(Stest)
				test_mode(SL360);
			if(Dtest)
				test_mode(DL360);
			if(Xtest)
				test_mode(XL360); }
		if(prompt("high density ")) {
			if(Stest)
				test_mode(SH360);
			if(Dtest)
				test_mode(DH360);
			if(Xtest)
				test_mode(XH360); }
		break;
	case 3 :		// 1.4
		if(prompt("low density ")) {
			if(Stest)
				test_mode(SL300);
			if(Dtest)
				test_mode(DL300);
			if(Xtest)
				test_mode(XL300); }
		if(prompt("high density ")) {
			if(Stest)
				test_mode(SH300);
			if(Dtest)
				test_mode(DH300);
			if(Xtest)
				test_mode(XH300); }
	}

	// Release FDC
	deselect_drive();
	asm {
		MOV		DX,DGRP:_FIoff	; Get offset
		PUSH	DS				; Save DS
		MOV		DS,DGRP:_FIseg	; Get segment
		MOV		AX,250Eh		; Set vector
		INT		21h				; Ask DOS
		POP		DS				; Restore DS
	}

	if(Texit) {
		if(Texit & 1) {					// Reset FDC
			out(FDC, 0);
			delay(MDEL);
			out(FDC, 4);
			delay(MDEL); }
		if(Texit & 2)
			poke(0x40, 0x3E, 0x0F);		// All drives recalibrate
		if(Texit & 4) {
			out(FDC+5, 0);				// Reset data rate
			poke(0x40, 0x8B, 0); }		// Indicate 500khz
		if(Texit & 8) {
			poke(0x40, 0x90, 0);		// Unknown media A:
			poke(0x40, 0x92, 0); } }	// Unknown media B:

	asm {
		XOR		AH,AH			; 00 = Reset
		MOV		DL,AH			; Drive A
		INT		13h				; BIOS: Reset diskette
	}

	// Display final report
	putc('\n', stdout);
	final_report(stdout);
	if(Rfile) {				// Output to file requested
		fp = fopen(Rfile, "wvq");
		final_report(fp);
		fclose(fp);
		printf("Written to: %s\n", Rfile); }
}
