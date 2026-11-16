
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




struct ParentStruct
{
    int data;
    void (*set_parent_data)(struct ParentStruct*, int);
};

typedef struct ParentStruct parent_t;

struct ChildStruct
{
    parent_t _parent;
    int more_data;
    void (*set_child_data)(struct ChildStruct* this, int val);
};

typedef struct ChildStruct child_t;


void _set_parent_data(struct ParentStruct* this, int val)
{
    this->data = val;
}


void _set_child_data(child_t* this, int val)
{
    this->more_data = val;
}


int main()
{
    parent_t foo;
    child_t bar;
    parent_t *bar_p = (parent_t*) &bar;
    unsigned int vidmem=0xf800;

    setvideobase(vidmem);

    bar._parent.set_parent_data = _set_parent_data;
    foo.set_parent_data = _set_parent_data;
   
    foo.set_parent_data(&foo, 42);
    bar_p->set_parent_data(bar_p, 43);
    
    if(foo.data==42)
    {
        vidstrcpy(vidmem+40, "OK  -  FOO.DATA == 42");
    }
    else
    {
        vidstrcpy(vidmem+40, "KO  -  FOO.DATA != 42");
    }
    
    if(bar_p->data==43)
    {
        vidstrcpy(vidmem+80, "OK  -  BAR_P->DATA == 43");
    }
    else
    {
        vidstrcpy(vidmem+80, "KO  -  BAR_P->DATA != 43");
    }

    return 0;
}
