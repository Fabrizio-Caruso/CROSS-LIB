#include "../../display_macros.h"

#include <tgi.h>

#define CLI() asm("\tcli")

#define _BG_COLOR COLOR_GREY

void INIT_GRAPHICS(void)
{
	
    tgi_install (tgi_static_stddrv);

	tgi_init ();		
	CLI();

	while (tgi_busy())  {  };

	tgi_setpalette(tgi_getdefpalette());
	tgi_setcolor(COLOR_WHITE);
	tgi_setbgcolor(_BG_COLOR);


	CLEAR_SCREEN();
}
