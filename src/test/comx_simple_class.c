
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
    int data;
    void (*set_class_data)(struct ClassStruct*, int);
};

typedef struct ClassStruct class_t;


void _set_class_data(struct ClassStruct* this, int val)
{
    this->data = val;
}


int main()
{
    class_t foo;
    
    unsigned int vidmem=0xf800;

    setvideobase(vidmem);

    foo.set_class_data = _set_class_data;
   
    foo.set_class_data(&foo, 42);

    
    if(foo.data==42)
    {
        vidstrcpy(vidmem+40, "OK  -  FOO.DATA == 42");
    }
    else
    {
        vidstrcpy(vidmem+40, "KO  -  FOO.DATA != 42");
    }
    


    return 0;
}
