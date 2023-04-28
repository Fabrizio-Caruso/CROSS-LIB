#include <supervision.h>
#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
    SV_LCD.height = 160;
    SV_LCD.width = 160;
    
    _setScreenColors();
}

