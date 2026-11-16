/*
    Hello World for the COMX-35
    by Marcel van Tongoren
    modified by Fabrizio Caruso
*/



#define _PLAYER_DOWN_UDG              12,18,12,51,45,12,18,51,0
#define _PLAYER_UP_UDG                12,30,12,51,45,12,18,51,0
#define _PLAYER_LEFT_UDG              12,22,12,62,13,12,20,20,0
#define _PLAYER_RIGHT_UDG             12,26,12,31,44,12,10,10,0

#define _PLAYER_DOWN_UDG_1              12+64,18+64,12+64,51+64,45+64,12+64,18+64,51+64,0+64
#define _PLAYER_UP_UDG_1                12+64,30+64,12+64,51+64,45+64,12+64,18+64,51+64,0+64
#define _PLAYER_LEFT_UDG_1              12+64,22+64,12+64,62+64,13+64,12+64,20+64,20+64,0+64
#define _PLAYER_RIGHT_UDG_1             12+64,26+64,12+64,31+64,44+64,12+64,10+64,10+64,0+64

#define _PLAYER_DOWN_UDG_2              12+128,18+128,12+128,51+128,45+128,12+128,18+128,51+128,0+128
#define _PLAYER_UP_UDG_2                12+128,30+128,12+128,51+128,45+128,12+128,18+128,51+128,0+128
#define _PLAYER_LEFT_UDG_2              12+128,22+128,12+128,62+128,13+128,12+128,20+128,20+128,0+128
#define _PLAYER_RIGHT_UDG_2             12+128,26+128,12+128,31+128,44+128,12+128,10+128,10+128,0+128

#define _PLAYER_DOWN_UDG_3              12+192,18+192,12+192,51+192,45+192,12+192,18+192,51+192,0+192
#define _PLAYER_UP_UDG_3                12+192,30+192,12+192,51+192,45+192,12+192,18+192,51+192,0+192
#define _PLAYER_LEFT_UDG_3              12+192,22+192,12+192,62+192,13+192,12+192,20+192,20+192,0+192
#define _PLAYER_RIGHT_UDG_3             12+192,26+192,12+192,31+192,44+192,12+192,10+192,10+192,0+192

unsigned char shapes[] =
{
	97, // First byte is the character to shape followed by 9 bytes for the shape
	_PLAYER_DOWN_UDG,
	98, // next character to shape
	_PLAYER_UP_UDG,
	99,
	_PLAYER_RIGHT_UDG,
	100,
	_PLAYER_LEFT_UDG,
	101, // First byte is the character to shape followed by 9 bytes for the shape
	_PLAYER_DOWN_UDG_1,
	102, // next character to shape
	_PLAYER_UP_UDG_1,
	103,
	_PLAYER_RIGHT_UDG_1,
	104,
	_PLAYER_LEFT_UDG_1,
	105, // First byte is the character to shape followed by 9 bytes for the shape
	_PLAYER_DOWN_UDG_2,
	106, // next character to shape
	_PLAYER_UP_UDG_2,
	107,
	_PLAYER_RIGHT_UDG_2,
	108,
	_PLAYER_LEFT_UDG_2,
	109, // First byte is the character to shape followed by 9 bytes for the shape
	_PLAYER_DOWN_UDG_3,
	110, // next character to shape
	_PLAYER_UP_UDG_3,
	111,
	_PLAYER_RIGHT_UDG_3,
	112,
	_PLAYER_LEFT_UDG_3
};

void shapechar(unsigned char * shapelocation, int number)
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

void setvideobase(){
	asm(" ldiReg R8,0xF800\n"
	    " sex R8\n"
	    " out 7\n"
	    " sex R2\n");
}

void vidstrcpy(unsigned char * vidmem,char * text){ //write to video memory
	asm(
	"$$cpy:\n"
	" lda R13 ;pick up input pointer\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n"
	" inc R12 ;++\n"
	" bnz $$cpy\n");
}

void vidclr(unsigned char * vidmem, int vidlen){ //write 0's to video memory
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

void vidchar(int vidmem, int character){ //write character to vidmem location in video memory
	asm(//vidmem pointer is R12, character is R13.0
	" glo r13\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n");
	return;
}

int getlastkey(){ //get last key
	asm(
	" ldiReg R8,0x41A3\n"
	" ldn R8\n"
	" plo R15\n"
	" ldi 0\n"
	" str R8\n"
	" phi R15\n"
	" Cretn\n");
	return 0; //this statement will never be executed but it keeps the compiler happy
}

int getkey(){ //get key value if currently pressed
	asm(
	" inp 3\n"
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

#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))


void main(){
	unsigned char* vidmem=(unsigned char *)0xf800;
	char key;
	unsigned int loop, vidmemaddress;
    int i;

	setvideobase();
	vidclr(vidmem,24*40);
	vidstrcpy(vidmem, "HELLO WORLD");
    vidstrcpy(vidmem+40, "a b c d  e f g h  i j k l  m n o p");
	vidmemaddress = (unsigned int) 0xf800+120;
	// vidchar(vidmemaddress+21, 0xe1); // print character a in second color scheme
	for(i = 0;i<16;++i)
	{
        vidchar(vidmemaddress+i*2,0xe1+i);
	}
	shapechar(shapes, 16);
    
    POKE(0xf800+200,0xe1);    
    POKE(0xf800+201,0xe2);
    POKE(0xf800+202,0xe3);    
    POKE(0xf800+203,0xe4);

	loop = 1;
	vidmem = (unsigned char *)0xf828;
	vidmemaddress = (unsigned int) 0xf828;
	while (1){
		key = getkey();
		switch (key){
			case 0x82:
				vidchar(vidmemaddress,'U');
				vidchar(vidmemaddress+1,'P');
				vidchar(vidmemaddress+2,' ');
				vidchar(vidmemaddress+3,' ');
				vidchar(vidmemaddress+4,' ');
				generatetone(80,5,7);
			break;
			case 0x83:
				vidstrcpy(vidmem,"RIGHT");
				generatetone(80,6,7);
			break;
			case 0x84:
				vidstrcpy(vidmem,"LEFT ");
				generatetone(80,4,7);
			break;
			case 0x85:
				vidstrcpy(vidmem,"DOWN ");
				generatetone(80,7,7);
			break;
			case 0x71:  // Q
				settone(0);
				loop = 0;
			break;	
			default:
			break;			
		}
		if (loop == 0)  break;
	}

}


