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

void init_colors(void)
{
	uint8_t i;
	
	
	// Initialize colors 	
	#if !defined(FORCE_NARROW)
    for(i=0;i<2;++i)
    {
        // red on black (inverted: cyan on white)		
        POKE(0xBB80+i*40,16);
        POKE(0xBB81+i*40,1); 
    }

    for(i=2;i<28;++i)
	#else
	for(i=0;i<28;++i)
	#endif
	{
		// yellow on black (inverted: blue on white)
		POKE(0xBB80+i*40,16);
		POKE(0xBB81+i*40,3); 
	}
}

void _XL_INIT_GRAPHICS(void)
{
    REDEFINE_AT(((uint8_t *)UDG_BASE_ADDR));

    init_colors();
    
    setScreenColors();
}
