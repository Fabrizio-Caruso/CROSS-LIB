
#include <system.h>
#include <conio.h>

#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
    set_graphics(VDP_SPR_16x16);                        // set video mode
    charsetlc();                                        // load character set with lowercase
    vdpmemset(gImage, 32, 768);                         // clear screen
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_MEDGREEN);      // set screen color
    vdpmemset(gColor, 0x10, 32);                        // set colors to black on transparent
    
    
    // charsetlc();
    // cursor(0);
    // textcolor(COLOR_WHITE);
    // bgcolor(COLOR_BLACK);

    setScreenColors();
}

