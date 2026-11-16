#include <tgi.h>
#define CLI() asm("\tcli")
int main(void)
{
    tgi_install (tgi_static_stddrv);
    tgi_init ();        
    CLI();
    while (tgi_busy())  {  };
    tgi_setpalette(tgi_getdefpalette());
    tgi_setbgcolor(TGI_COLOR_WHITE);
    tgi_outtextxy(8,8,"    "); 
    tgi_setcolor(TGI_COLOR_BLACK);
    // tgi_setcolor(TGI_COLOR_RED);
    tgi_outtextxy(8,8,"hello");
        
    while(1){};
    return 0;
}
