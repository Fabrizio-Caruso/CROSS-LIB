
#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
    #if !defined(Z88DK_SPRITES)
	clg();
    #endif
    _setScreenColors();
}


