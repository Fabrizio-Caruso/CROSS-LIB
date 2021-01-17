
#include <system.h>
#include <conio.h>

#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
    set_text();
    charsetlc();
    textcolor(COLOR_WHITE);
    // bgcolor(COLOR_DKBLUE);
    bgcolor(COLOR_BLACK);

    setScreenColors();

}

