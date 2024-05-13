
#if !defined(PEEK)
    #include <peekpoke.h>
#endif
#include "display_macros.h"

#define _C64_FOREGROUND 53280u
#define _C64_BACKGROUND 53281u

void _XL_INIT_GRAPHICS(void)
{
	// Select video bank and set the character set pointer to point to $C800
	// where the redefined charset has been placed by the linker
	POKE(56576u,0);
	POKE(53272u,2);
	POKE(648,192);
	
	/*
    #if defined(_BACKGROUND_COLOR) && _BACKGROUND_COLOR==1
        // Frame color = WHITE
        POKE(53280u,1);
        
        // Background color = WHITE
        POKE(53281u,1);
    #else      
        // Frame color = BLACK
        POKE(53280u,0);
        
        // Background color = BLACK
        POKE(53281u,0);
    #endif
	*/
	POKE(_C64_FOREGROUND,_BACKGROUND_COLOR);
	POKE(_C64_BACKGROUND,_BACKGROUND_COLOR);

    _setScreenColors();
}