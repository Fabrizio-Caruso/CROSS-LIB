/*
    Hello World for the COMX-35
*/

void setvideobase(){
	asm(" ldiReg R8,0xF800\n"
	    " sex R8\n"
	    " out 7\n"
	    " sex R2\n");
}

void vidstrcpy(char * vidmem,char * text){ //write to video memory
	asm(
	"$$cpy:\n"
	" lda R13 ;pick up input pointer\n"
	" b1  $	;wait til video is quiet\n"
	" str R12 ;move the byte\n"
	" inc R12 ;++\n"
	" bnz $$cpy\n");
}

void vidclr(char * vidmem, int vidlen){ //write 0's to video memory
	asm( //vidmem pointer is R12, vidlen is R13
	"$$cpy:\n"
	" ldi 0 ;;source a 0 for clearing the screen\n"
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

void main(){
	char* vidmem=(char *)0xf800;
	char key;
	int loop, vidmemaddress;

	setvideobase();
	vidclr(vidmem,24*80);
	vidstrcpy(vidmem, "HELLO WORLD");

	loop = 1;
	vidmem = (char *)0xf828;
	vidmemaddress = 0xf828;
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


