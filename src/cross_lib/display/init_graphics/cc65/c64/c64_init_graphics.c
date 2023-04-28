
#if !defined(PEEK)
    #include <peekpoke.h>
#endif
#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
	// Select video bank and set the character set pointer to point to $C800
	// where the redefined charset has been placed by the linker
	POKE(56576u,0);
	POKE(53272u,2);
	POKE(648,192);
	
    #if defined(_BACKGROUND_COLOR) && _BACKGROUND_COLOR==_XL_WHITE
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
    
    _setScreenColors();
}