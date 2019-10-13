
#include <tgi.h>
#include <conio.h>

#define CLI() asm("\tcli")

int main(void)
{
    tgi_install (tgi_static_stddrv);

    tgi_init ();        
    CLI();

    tgi_clear();

    while (tgi_busy())  {  };

    tgi_setpalette(tgi_getdefpalette());
    tgi_setbgcolor(TGI_COLOR_GREY);

    tgi_setcolor(TGI_COLOR_RED);

    tgi_outtextxy(8,8*0,"step 0"); 
    
    cgetc();   
    tgi_outtextxy(8,8*1,"step 1"); 

    cgetc();   
    tgi_outtextxy(8,8*2,"step 2"); 
    
    cgetc();   
    tgi_outtextxy(8,8*3,"step 3"); 
    
    while(1){};
    return 0;
}
