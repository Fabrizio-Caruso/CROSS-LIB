#include <c128.h>
#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
    fast();
    videomode(80);
    _setScreenColors();
}

