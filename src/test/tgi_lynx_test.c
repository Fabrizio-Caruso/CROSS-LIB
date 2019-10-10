#include "display_macros.h"
#include "graphics_settings.h"

#include <tgi.h>

#define CLI() asm("\tcli")

#define _BG_COLOR COLOR_GREY

int main(void)
{
    tgi_install (tgi_static_stddrv);

    tgi_init ();        
    CLI();

    while (tgi_busy())  {  };

    tgi_setpalette(tgi_getdefpalette());
    tgi_setbgcolor(_BG_COLOR);

    tgi_setcolor(COLOR_RED);
    tgi_outtextxy(8*6,8,"hello"); 
    
    tgi_setcolor(COLOR_WHITE);
    tgi_outtextxy(8,8,"hello"); 

    while(1){};
    return 0;
}
