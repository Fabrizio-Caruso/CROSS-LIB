

// Code by Marcel van Tongeren
void vidchar(int vidmem, int character){ //write character to vidmem location in video memory
	asm(//vidmem pointer is R12, character is R13.0
#if defined(__PECOM__)
	" sex R3\n"
	" out 1\n"
	" db  2\n"
    " sex R2\n"
#endif
	" glo r13\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n");
	return;
}



void shapechar(const unsigned char * shapelocation, int number)
{
	asm( //shapelocation pointer is R12, number of shapes is R13
#if defined(__PECOM__)
	" sex R3\n" 
	" out 1\n" 
	" db  2\n" 
    " sex R2\n"
#endif
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
	" lbnz $$nextline ; number of lines - 1 if not zero do next line\n"
	" dec r13\n"
	" glo r13\n"
	" lbnz $$nextshape\n");
}


void redefine_char(const unsigned char * shapelocation, int color)
{
    unsigned char colored_shape[10];
    unsigned char i;
    
    colored_shape[0] = *shapelocation;
    for(i=1;i<9;++i)
    {
       colored_shape[i]=shapelocation[i]+color*64;
    }
    colored_shape[9]=0;
    shapechar(colored_shape, 1);
}

#define __PLAYER_DOWN_UDG              12,18,12,51,45,12,18,51
#define __PLAYER_UP_UDG                12,30,12,51,45,12,18,51
#define __PLAYER_LEFT_UDG              12,22,12,62,13,12,20,20
#define __PLAYER_RIGHT_UDG             12,26,12,31,44,12,10,10


#define __GHOST_UDG                    33,30,33,51,33,45,33,30
#define __SKULL_UDG                    30,33,51,33,33,18,18,12
#define __GUN_UDG                       0,32,31,40,56,32, 0, 0
#define __POWERUP_UDG                   0,30,51,55,51,26,18,12
#define __BULLET_UDG                    0, 0, 4,28,14, 8, 0, 0
#define __BOMB_UDG                     30,33,51,45,45,51,33,30

const unsigned char player_down[10] = {  107, __PLAYER_DOWN_UDG, 0 };
const unsigned char player_up[10] = { 108 , __PLAYER_UP_UDG, 0 };
const unsigned char player_right[10] = { 109 , __PLAYER_LEFT_UDG, 0 };
const unsigned char player_left[10] = { 110 , __PLAYER_RIGHT_UDG, 0};

const unsigned char ghost[10] = { 112 , __GHOST_UDG, 0 };
const unsigned char bomb[10] =  { 111 , __BOMB_UDG, 0};



// COMX and PECOM 
void disableinterrupt(){
    asm( 
        " sex 3\n"
        " dis\n"
        " db 0x23\n");
}

// COMX and PECOM
void enableinterrupt(){
    asm( 
        " sex 3\n"
        " ret\n"
        " db 0x23\n");
}

void setvideobase(unsigned int vidmem){
    asm( //vidmem pointer is R12
#if defined(__PECOM__)
		" ldireg R8, 0x7cc8\n"
		" ghi R12\n"
		" str R8\n"
		" inc R8\n"
		" glo R12\n"
		" str R8\n"
#endif
		" sex R12\n"
        " out 7\n"
        " sex R2\n");
}



void textcolordefinition(unsigned char definition){
	asm( //definition is in R12.0
		 // b1	b0	RED	 BLUE	GREEN
		 //  0	 0	CB0	 CB1	PCB
		 //  0	 1  CCB0 PCB	CCB1
		 //  1  0/1 PCB	 CCB0	CCB1
#if defined(__PECOM__)
		" ldireg R8, 0x7cc4\n"
#elif defined(__COMX__)
		" ldireg R8, 0x41C0\n"
#endif
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
#if defined(__PERCOM__)
		" ldireg R8, 0x7cc4\n"
#elif defined(__COMX__)
		" ldireg R8, 0x41C0\n"
#endif
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

// void monochrome(unsigned char mono){
	// asm( //mono/cfc is in R12.0, 0=color, 1=mono
		// " ldireg R8, 0x41C0\n"
		// " ldn R8\n"				//get latest OUT 3 value
		// " ani 0xf7\n"			//clear cfc
		// " str R2\n"				//store value on stack
		// " glo R12\n"			//get new cfc
		// " ani 1\n"				//limit to 1 bits
		// " shl\n"
		// " shl\n"
		// " shl\n"
		// " or\n"					//new cfc OR latest OUT3 value
		// " str R8\n"				//store new value 
		// " sex R8\n"
		// " out 3\n"				//set new cfc value
		// " sex R2\n");
// }


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
#if defined(__PECOM__)
		" ldireg R8, 0x7cc4\n"
#elif defined(__COMX__)
		" ldireg R8, 0x41C0\n"
#endif
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



void INIT_GRAPHICS(void)
{
    setvideobase(0x7800);

    redefine_char(player_down, 3);
    redefine_char(player_up, 3);
    redefine_char(player_right, 3);
    redefine_char(player_left, 3);
    redefine_char(ghost, 3);
    redefine_char(bomb, 0);

    
    #if defined(__COMX__)
        disableinterrupt();
    #endif
    (void) bgcolor(0);
    #if defined(__COMX__)
    	textcolordefinition(3);
        monochrome(0);
    #elif defined(__PECOM__)
    	textcolordefinition(3);
    #endif

}
