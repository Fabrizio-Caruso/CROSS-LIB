
void setvideobase(unsigned int vidmem){
    asm( //vidmem pointer is R12
        " sex R12\n"
        " out 7\n"
        " sex R2\n");
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


struct ClassStruct
{
    void (*method_t)(void);
};

typedef struct ClassStruct class_t;

unsigned int vidmem=0xf800;
    
void _print_42(void)
{
    vidstrcpy(vidmem+40, "42");
}


int main()
{
    class_t foo;

    setvideobase(vidmem);

    foo.method_t = _print_42;
    
    foo.method_t();
    


    return 0;
}
