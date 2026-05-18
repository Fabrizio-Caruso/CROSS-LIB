
#if !defined(PEEK)
    #include <peekpoke.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "graphics_settings.h"
#include "display_macros.h"

#if !defined(__UDG_BASE_FACTOR)
    #define __UDG_BASE_FACTOR 15
#endif

// #define DEBUG

void _XL_INIT_GRAPHICS(void)
{

    // __asm__ ("STA $FF3F");
    // __asm__ ("SEI");

	POKE(65299ul,(PEEK(65299ul)&3)|((__UDG_BASE_FACTOR)*4)); // change character base address to 28th Kbyte
	POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
	
	#if defined(DEBUG)
	{
		uint8_t i;
		for(i=0;i<255;++i)
		{
			POKE((3072+i),i+0x40);
		}
		
		while(1){};
	}
	#endif
	
    POKE(65301u,0);
    POKE(65305u,0);

    // clear screen and color ram
    __asm__("jsr $D88B");
    
    _setScreenColors();
}

