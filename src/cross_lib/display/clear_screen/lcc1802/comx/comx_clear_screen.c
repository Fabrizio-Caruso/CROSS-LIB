


// Code by Marcel van Tongeren
void vidclr(unsigned char * vidmem, int vidlen){ //write 0's to video memory
	asm( //vidmem pointer is R12, vidlen is R13
#if defined(__PECOM__)
	" sex R3\n"
	" out 1\n"
	" db  2\n"
    " sex R2\n"
#endif
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

void CLEAR_SCREEN(void)
{
    vidclr((unsigned char*)0xf800, 40*24);
}

