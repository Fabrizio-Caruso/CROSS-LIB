#include "display_macros.h"
#include "graphics_settings.h"

#include <tgi.h>

#define CLI() asm("\tcli")

void _XL_INIT_GRAPHICS(void)
{
	
    tgi_install (tgi_static_stddrv);

	tgi_init ();		
	CLI();

	while (tgi_busy())  {  };

	tgi_setpalette(tgi_getdefpalette());
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(TGI_COLOR_BLACK);

	_XL_CLEAR_SCREEN();
    
    setScreenColors();
}
