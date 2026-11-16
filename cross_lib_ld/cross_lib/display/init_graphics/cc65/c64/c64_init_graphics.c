
#if !defined(PEEK)
    #include <peekpoke.h>
#endif
#include "display_macros.h"

#define _C64_FOREGROUND 53280u
#define _C64_BORDER 53281u

void _XL_INIT_GRAPHICS(void)
{
	// Select video bank and set the character set pointer to point to $C800
	// where the redefined charset has been placed by the linker
	POKE(56576u,0);
	POKE(53272u,2);
	POKE(648,192);
	

	POKE(_C64_FOREGROUND,_XL_BACKGROUND_COLOR);
	POKE(_C64_BORDER,_XL_BACKGROUND_COLOR);

    _setScreenColors();
}