/*
    Hello World for the COMX-35
    by Marcel van Tongoren & Bill Rowe
*/

#include <nstdlib.h>
#define putc(c) putcomx(c,0x3302)

void putcomx(unsigned char c, unsigned int r13val){
    asm(
        " glo r12\n"
        " lbr 0x2cd3\n");
}

char shapes[] =
{
	97, // First byte is the character to shape followed by 9 bytes for the shape
	0x00, 0xcc, 0xde, 0xed, 0xff, 0xf3, 0xed, 0xe1, 0x00,
	98, // next character to shape
	0x00, 0x8c, 0x9e, 0xad, 0xbf, 0xb3, 0xad, 0xa1, 0x00,
	99,
	0x00, 0x00, 0xcc, 0xde, 0xff, 0xde, 0xcc, 0x00, 0x00,
};

void shapechar(char * shapelocation, int number)
{
	asm( //shapelocation pointer is R12, number of shapes is R13
	" ldi 0xf7\n"
	" phi R8\n"
	" ldi 0xfb\n"
	" phi R9\n"
	"$$nextshape:\n"
	" ldi 0xC0 \n"
	" plo R8 ; R8 = charmem pointer\n"
	" plo r9 ; R9 = vidmem pointer\n"
	" ldi 9 \n"
	" plo R10 ; R10.0 = number of lines, we need to somehowe make a NTSC version for 8 lines\n"
	" lda 12\n" 
	" phi R10 ; R10.1 = character\n"
	"$$nextline:\n"
	" ghi R10\n"
	" b1  $	; wait til video is quiet\n"
	" str R9 ; store character in vidmem\n"
	" inc R9\n"
	" lda R12\n"
	" str R8 ; store first shape line in charmem\n"
	" inc R8\n"
	" dec R10\n"
	" glo R10\n"
	" bnz $$nextline ; number of lines - 1 if not zero do next line\n"
	" dec r13\n"
	" glo r13\n"
	" bnz $$nextshape\n");
}

void setvideobase(unsigned int vidmem){
    asm( //vidmem pointer is R12
        " sex R12\n"
        " out 7\n"
        " sex R2\n");
}

void disableinterrupt(){
    asm( 
        " sex 3\n"
        " dis\n"
        " db 0x23\n");
}

void enableinterrupt(){
    asm( 
        " sex 3\n"
        " ret\n"
        " db 0x23\n");
}

void vidstrcpy(unsigned int vidmem,char * text){ //write to video memory
	asm(
	"$$cpy:\n"
	" lda R13 ;pick up input pointer\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n"
	" inc R12 ;++\n"
	" bnz $$cpy\n");
}

void vidclr(unsigned int vidmem, int vidlen){ //write 0's to video memory
	asm( //vidmem pointer is R12, vidlen is R13
	"$$cpy:\n"
	" ldi 0 ;source a 0 for clearing the screen\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n"
	" inc R12 ;++\n"
	" dec R13 ;decrement count\n"
	" glo R13 ; check bottom byte\n"
	" bnz $$cpy\n"
	" ghi R13 ;top byte\n"
	" bnz $$cpy\n");
	return;
}

void vidchar(unsigned int vidmem, int character){ //write character to vidmem location in video memory
	asm(//vidmem pointer is R12, character is R13.0
	" glo r13\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n");
	return;
}

unsigned char getkey(){ //get key value if pressed, otherwise return 0
	asm(
	" b3 $$keypressed\n" // Check if a key is pressed for the 'first time'
	" bn2 $$nokey\n"     // Branch to nokey if no key was pressed previously
	"$$keypressed:\n"
	" inp 3\n"
	" lskp\n"
	"$$nokey:\n"
	" ldi 0\n"
	" plo R15\n"
	" ldi 0\n"
	" phi R15\n"
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

unsigned char cgetc(){ //wait for key press
	asm(
	"$$loop:\n"
	" b3 $$keypressed\n" // Check if a key is pressed for the 'first time'
	" bn2 $$loop\n"		 // Loop if no key was pressed previously
	"$$keypressed:\n"
	" inp 3\n"
	" plo R15\n"
	" ldi 0\n"
	" phi R15\n"
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

int kbhit(){ //return 1 if a key is pressed, 0 if not
	asm(
    " b3 $$keypressed\n" // Check if a key is pressed for the 'first time'
    " bn2 $$nokey\n"     // Branch to nokey if no key was pressed previously
    "$$keypressed:\n"
	" ldi 1\n"
	" lskp\n"
	"$$nokey:\n"
	" ldi 0\n"
	" plo R15\n"
	" ldi 0\n"
	" phi R15\n"
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

void settone(int value){
	asm(
	" sex R12 ;Tone generation value\n"
	" out 4\n"
	" sex R2\n");
}

void generatetone(int tone, int range, int volume){
	int tonevalue;
	tonevalue = (volume & 0xf) + ((range &0x7) << 4) + 0x8 + ((tone & 0x7f) << 8);
	settone(tonevalue);
}

void setnoise(int value){
	asm(
	" ldireg R8,0x41C3\n"
	" ldn R8\n"		    
	" plo R12 ; load current OUT 5 (low byte) video setting from 0x41C3 and store in R12.0\n"	  
	" sex R12 ; Noise  generation value\n"
	" out 5\n"
	" sex R2\n");
}

void generatenoise(int range, int volume){
	int tonevalue;
	tonevalue = ((volume & 0xf) + ((range &0x7) << 4) + 0x8) << 8;
	setnoise(tonevalue);
}

void main(){
	unsigned int vidmem=0xf800;
	unsigned char key;
	unsigned int loop;

    setvideobase(vidmem);
	disableinterrupt();
    vidclr(vidmem,24*40);
	vidstrcpy(vidmem, "HELLO WORLD, abc \xe1\xe2\xe3");
	shapechar(shapes, 3);

	loop = 1;
	vidmem = 0xf828;
	while (1){
		key = getkey();
		switch (key){
			case 0x82:
				vidchar(vidmem,'U');
				vidchar(vidmem+1,'P');
				vidchar(vidmem+2,' ');
				vidchar(vidmem+3,' ');
				vidchar(vidmem+4,' ');
				generatetone(80,5,7);
			break;
			case 0x83:
				vidstrcpy(vidmem,"RIGHT");
				generatenoise(2,7);
			break;
			case 0x84:
				vidstrcpy(vidmem,"LEFT ");
				generatenoise(6,7);
			break;
			case 0x85:
				vidstrcpy(vidmem,"DOWN ");
				generatetone(80,7,7);
			break;
			case 0x71:  // Q
				loop = 0;
			break;	
			default:
			break;			
		}
		if (kbhit() == 0)
		{
			settone(0);
			setnoise(0);
		}
		if (loop == 0)  break;
	}

	while (kbhit() == 1)
	{
	}

	loop = 1;
	vidmem = 0xfa00;
	vidchar(vidmem,'X');
	while (1){
		key = getkey();
		switch (key){
			case 0x82:
				vidchar(vidmem,' ');
				vidmem = vidmem - 40;
				vidchar(vidmem,'X');
			break;
			case 0x83:
				vidchar(vidmem,' ');
				vidmem++;
				vidchar(vidmem,'X');
			break;
			case 0x84:
				vidchar(vidmem,' ');
				vidmem--;
				vidchar(vidmem,'X');
			break;
			case 0x85:
				vidchar(vidmem,' ');
				vidmem = vidmem + 40;
				vidchar(vidmem,'X');
			break;
			case 0x71:  // Q
				loop = 0;
			break;	
			default:
			break;			
		}
		if (loop == 0)  break;
	}

	enableinterrupt();
    printf("HELLO WORLD! %i %d", 42, 43);
}

#include <nstdlib.c>
