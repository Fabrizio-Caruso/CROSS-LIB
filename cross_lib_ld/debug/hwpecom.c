/*
    Hello World for the PECOM 64
    by Marcel van Tongoren & Bill Rowe
*/

#include <nstdlib.h>
#define putc(c) putpecom(c,0xB102)

void putpecom(unsigned char c, unsigned int r13val){
    asm(
		" sex R2\n"
		" glo r12\n"
        " lbr 0xAAB8\n");
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
	" sex R3\n"
	" out 1\n"
	" db  2\n"
    " sex R2\n"
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
		" ldireg R8, 0x7cc8\n"
		" ghi R12\n"
		" str R8\n"
		" inc R8\n"
		" glo R12\n"
		" str R8\n"
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

void gotoxypecom(unsigned int vidmem){
	asm(
	" ldireg R8, 0x7CC6\n"
	" sex R8\n"
	" glo R12\n"
	" stxd\n"
	" ghi R12\n"
	" adi 0xf8\n"
	" stxd\n"
	" sex R2\n");
}

void gotoxy(unsigned char x, unsigned char y){
    gotoxypecom(y*40 + x);
}

void vidstrcpy(unsigned int vidmem,char * text){ //write to video memory
	asm(
	" sex R3\n"
	" out 1\n"
	" db  2\n"
    " sex R2\n"
	"$$cpy:\n"
	" lda R13 ;pick up input pointer\n"
	" bz $$end\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n"
	" inc R12 ;++\n"
	" br $$cpy\n"
	"$$end:\n");
}

void vidclr(unsigned int vidmem, int vidlen){ //write 0's to video memory
	asm( //vidmem pointer is R12, vidlen is R13
	" sex R3\n"
	" out 1\n"
	" db  2\n"
    " sex R2\n"
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
	" sex R3\n"
	" out 1\n"
	" db  2\n"
    " sex R2\n"
	" glo r13\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n");
	return;
}

unsigned char getkey(){ //get key value if pressed, otherwise return 0
	asm(
	" ldireg R8, 0x7CCA\n"
	" sex R8\n"				//Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
	"$$checknext:\n"
	" inp 3\n"				// bit 0 and 1 indicate key press
	" ani 3\n"
	" bnz $$keypressed\n"	// bit 0 or 1 are 1 so a key is pressed
	" glo R8\n"
	" inc R8\n"
	" xri 0xe3\n"
	" bnz $$checknext\n"	// R8++ if not 0xE3 check next key
	" ldi 0\n"
	" br  $$exit\n"			// all keys checked - return 0
	"$$keypressed:\n"
	" shr\n"	
	" shr\n"				// move bit 1 into DF
	" glo R8\n"				// get value 'keyset'
	" shlc\n"				// shift left and move DF to b0 so key value is unique
	" adi 0x98\n"			// convert to ASCII
	"$$exit:\n"
	" plo R15\n"
	" ldi 0\n"
	" phi R15\n"
    " sex R2\n"
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

unsigned char cgetc(){ //wait for key press
	asm(
	"$$cgetc:\n"
	" ldireg R8, 0x7CCA\n"
	" sex R8\n"				//Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
	"$$checknext:\n"
	" inp 3\n"				// bit 0 and 1 indicate key press
	" ani 3\n"
	" bnz $$keypressed\n"	// bit 0 or 1 are 1 so a key is pressed
	" glo R8\n"
	" inc R8\n"
	" xri 0xe3\n"
	" bnz $$checknext\n"	// R8++ if not 0xE3 check next key
	" br  $$cgetc\n"		// all keys checked - check again
	"$$keypressed:\n"
	" shr\n"	
	" shr\n"				// move bit 1 into DF
	" glo R8\n"				// get value 'keyset'
	" shlc\n"				// shift left and move DF to b0 so key value is unique
	" adi 0x98\n"			// convert to ASCII
	" plo R15\n"
	" ldi 0\n"
	" phi R15\n"
    " sex R2\n"
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

int kbhit(){ //return 1 if a key is pressed, 0 if not
	asm(
	" ldireg R8, 0x7CCA\n"
	" sex R8\n"				//Set stack to R8 which counts up from 0x7CCA to 0x7CE3 to check all keys
	"$$checknext:\n"
	" inp 3\n"				// bit 0 and 1 indicate key press
	" ani 3\n"
	" bnz $$keypressed\n"	// bit 0 or 1 are 1 so a key is pressed
	" glo R8\n"
	" inc R8\n"
	" xri 0xe3\n"
	" bnz $$checknext\n"	// R8++ if not 0xE3 check next key
	" ldi 0\n"				// all keys checked - return 0
	" lskp\n"
	"$$keypressed:\n"
	" ldi 1\n"				// key pressed - return 1
	" plo R15\n"
	" ldi 0\n"
	" phi R15\n"
    " sex R2\n"
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
	" ldi 0x80\n"		    
	" plo R12 ; load current OUT 5 (low byte) video setting 0x80\n"	  
	" sex R12 ; Noise  generation value\n"
	" out 5\n"
	" sex R2\n");
}

void generatenoise(int range, int volume){
	int tonevalue;
	tonevalue = ((volume & 0xf) + ((range &0x7) << 4) + 0x8) << 8;
	setnoise(tonevalue);
}

unsigned char bgcolor(unsigned char color){
	asm( //color is in R12.0
		 //0: black
		 //1: green
		 //2: blue
		 //3: cyan
		 //4: red
		 //5: yellow
		 //6: magenta
		 //7: white
		" ldireg R8, 0x7cc4\n"
		" ldn R8\n"				//get latest OUT 3 value
		" ani 7\n"				//get old background color
		" plo R15\n"
		" ldi 0\n"
		" phi R15\n"			//return old background in R15
		" ldn R8\n"				//get latest OUT 3 value
		" ani 0xf8\n"			//clear background color
		" str R2\n"				//store value on stack
		" glo R12\n"			//get new color
		" ani 7\n"				//limit to 3 bits
		" or\n"					//new color OR latest OUT3 value
		" str R8\n"				//store new value 
		" sex R8\n"
		" out 3\n"				//set new color value
		" sex R2\n"
		" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

void textcolordefinition(unsigned char definition){
	asm( //definition is in R12.0
		 // b1	b0	RED	 BLUE	GREEN
		 //  0	 0	CB0	 CB1	PCB
		 //  0	 1  CCB0 PCB	CCB1
		 //  1  0/1 PCB	 CCB0	CCB1
		" ldireg R8, 0x7cc4\n"
		" ldn R8\n"				//get latest OUT 3 value
		" ani 0x9f\n"			//clear text color definition
		" str R2\n"				//store value on stack
		" glo R12\n"			//get new color
		" ani 3\n"				//limit to 2 bits
		" shrc\n"
		" shrc\n"
		" shrc\n"
		" shrc\n"
		" or\n"					//new text color definition OR latest OUT3 value
		" str R8\n"				//store new value 
		" sex R8\n"
		" out 3\n"				//set new color definition value
		" sex R2\n");
}

void monochrome(unsigned char mono){
	asm( //mono/cfc is in R12.0, 0=color, 1=mono
		" ldireg R8, 0x7cc4\n"
		" ldn R8\n"				//get latest OUT 3 value
		" ani 0xf7\n"			//clear cfc
		" str R2\n"				//store value on stack
		" glo R12\n"			//get new cfc
		" ani 1\n"				//limit to 1 bits
		" shl\n"
		" shl\n"
		" shl\n"
		" or\n"					//new cfc OR latest OUT3 value
		" str R8\n"				//store new value 
		" sex R8\n"
		" out 3\n"				//set new cfc value
		" sex R2\n");
}

void main(){
	unsigned int vidmem=0xf800;
	unsigned char key;
	unsigned int loop;
	unsigned int oldbkcolor;

    setvideobase(vidmem);

	oldbkcolor = bgcolor(0);
	textcolordefinition(3);
	monochrome(0);

	vidclr(vidmem,24*40);
	vidstrcpy(vidmem, "HELLO WORLD, abc \xe1\xe2\xe3");
	shapechar(shapes, 3);

	loop = 1;
	vidmem = 0xf828;

	while (1){
		key = cgetc();
		switch (key){
			case 0x5E:
				vidchar(vidmem,'U');
				vidchar(vidmem+1,'P');
				vidchar(vidmem+2,' ');
				vidchar(vidmem+3,' ');
				vidchar(vidmem+4,' ');
				generatetone(80,5,7);
			break;
			case 0x5D:
				vidstrcpy(vidmem,"RIGHT");
				generatenoise(2,7);
			break;
			case 0x5C:
				vidstrcpy(vidmem,"LEFT ");
				generatenoise(6,7);
			break;
			case 0x5B:
				vidstrcpy(vidmem,"DOWN ");
				generatetone(80,7,7);
			break;
			case 0x51:  // q
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
			case 0x5E:
				vidchar(vidmem,' ');
				vidmem = vidmem - 40;
				vidchar(vidmem,'X');
			break;
			case 0x5D:
				vidchar(vidmem,' ');
				vidmem++;
				vidchar(vidmem,'X');
			break;
			case 0x5C:
				vidchar(vidmem,' ');
				vidmem--;
				vidchar(vidmem,'X');
			break;
			case 0x5B:
				vidchar(vidmem,' ');
				vidmem = vidmem + 40;
				vidchar(vidmem,'X');
			break;
			case 0x51:  // q
				loop = 0;
			break;	
			default:
			break;			
		}
		if (loop == 0)  break;
	}

    vidclr(vidmem,24*40);
//	gotoxy(0,0);
 //   printf("HELLO WORLD!");
//	gotoxy(20,0);
//    printf("20,0");
//	gotoxy(0,20);
//    printf("0,20");
//	gotoxy(10,10);
//    printf("10,10");
}

#include <nstdlib.c>
