#include "foo.h"


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



unsigned char _foo = 43;

extern Foo FOO;

int main(void)
{
	unsigned char* vidmem=(unsigned char *)0xf800;


    
	setvideobase();
	vidclr(vidmem,24*40);
	vidstrcpy(vidmem, "HELLO WORLD");
    
    // if(FOO==42)
        // vidstrcpy(vidmem+80, "FOO IS 42");
    // else
        // vidstrcpy(vidmem+80, "FOO IS NOT 42!");
    
    
    // if(_FOO==43)
        // vidstrcpy(vidmem+120, "_FOO IS 43");
    // else
        // vidstrcpy(vidmem+120, "_FOO IS NOT 43!");
    
    FOO.bar = _FOO;
    
    while(1){};
    return 0;
}
