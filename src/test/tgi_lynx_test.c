#include <tgi.h>

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
    tgi_outtextxy(8*6,8,"** ##"); 
    
    tgi_setcolor(TGI_COLOR_WHITE);
    tgi_outtextxy(8,8,"** ##"); 

    while(1){};
    return 0;
}
