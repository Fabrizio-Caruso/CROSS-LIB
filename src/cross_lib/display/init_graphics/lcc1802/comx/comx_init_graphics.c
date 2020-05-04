
// Code by Marcel van Tongeren
void vidchar(int vidmem, int character){ //write character to vidmem location in video memory
	asm(//vidmem pointer is R12, character is R13.0
	" glo r13\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n");
	return;
}


void shapechar(const unsigned char * shapelocation, int number)
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


void redefine_char(const unsigned char * shapelocation, int color)
{
    unsigned char colored_shape[9];
    unsigned char i;
    
    for(i=1;i<9;++i)
    {
       colored_shape[i]=shapelocation[i]+color*64;
    }
    shapechar(colored_shape, 1);
}

#define PLAYER_DOWN_UDG              12,18,12,51,45,12,18,51
#define PLAYER_UP_UDG                12,30,12,51,45,12,18,51
#define PLAYER_LEFT_UDG              12,22,12,62,13,12,20,20
#define PLAYER_RIGHT_UDG             12,26,12,31,44,12,10,10
const unsigned char player_down[10] = {  97, PLAYER_DOWN_UDG, 0 };
const unsigned char player_up[10] = { 98 , PLAYER_UP_UDG, 0 };
const unsigned char player_right[10] = { 99 , PLAYER_LEFT_UDG, 0 };
const unsigned char player_left[10] = { 100 , PLAYER_RIGHT_UDG, 0};

void INIT_GRAPHICS(void)
{
    redefine_char(player_down, 1);
    redefine_char(player_up, 1);
    redefine_char(player_right, 2);
    redefine_char(player_left, 3);
    
	asm(" ldiReg R8,0xF800\n"
	    " sex R8\n"
	    " out 7\n"
	    " sex R2\n");
}
