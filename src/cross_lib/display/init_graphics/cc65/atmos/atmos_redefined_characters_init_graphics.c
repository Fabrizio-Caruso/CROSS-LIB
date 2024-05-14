#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graphics_settings.h"
#include "display_macros.h"

#include "6x8_chars.h"

#define UDG_BASE_ADDR 0xb400

#include "udg_map.h"


#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define ORIC_BACKGROUND_COLOR 23
    #define ORIC_INK_COLOR 0
#else
    #define ORIC_BACKGROUND_COLOR 16
    #define ORIC_INK_COLOR 3
#endif

void init_colors(void)
{
	uint8_t i;
	
	// Initialize colors 	
	for(i=0;i<28;++i)
	{
		// yellow on black (inverted: blue on white)
		POKE(0xBB80+i*40,ORIC_BACKGROUND_COLOR);
		POKE(0xBB81+i*40,ORIC_INK_COLOR); 
	}
}

void _XL_INIT_GRAPHICS(void)
{
    REDEFINE_AT(((uint8_t *)UDG_BASE_ADDR));

    init_colors();
    
    _setScreenColors();
}
