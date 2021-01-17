#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graphics_settings.h"
#include "display_macros.h"

#include "8x8_chars.h"

#include "udg_map.h"

#include <stdint.h>

#if !defined(UDG_BASE_FACTOR)
	#define UDG_BASE_FACTOR 29
#else
	#define UDG_BASE_FACTOR 15
#endif


void _XL_INIT_GRAPHICS(void)
{
	uint16_t i;
	
	#if defined(FORCE_NO_CONIO_LIB) || defined(INIT_SET_COLOR)
		POKE(65301u,0);
		POKE(65305u,0);
	#endif
    
	POKE(1177,62); // disable switch to RAM in PEEK
	for(i=0;i<1023;++i)
	{
		POKE(UDG_BASE_FACTOR*1024+i,PEEK((uint16_t) (54272ul+(uint16_t) i)));
	}
	POKE(1177,63); // re-enable switch to RAM in PEEK
	POKE(65299ul,(PEEK(65299ul)&3)|((UDG_BASE_FACTOR)*4)); // change character base address to 28th Kbyte
	POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
		
	REDEFINE_AT(UDG_BASE_FACTOR*1024);
    
    setScreenColors();

}

