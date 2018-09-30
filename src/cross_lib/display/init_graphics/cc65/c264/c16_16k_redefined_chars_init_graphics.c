#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../graphics_settings.h"

// #include "../../../graphics_data/8x8_chars.h"

// #include "../../../redefine_characters/udg_map.h"

//#define UDG_BASE_FACTOR 15

void INIT_GRAPHICS(void)
{
	// unsigned short i;
	

	// POKE(1177,62); // disable switch to RAM in PEEK
	// for(i=0;i<1023;++i)
	// {
		// POKE(UDG_BASE_FACTOR*1024+i,PEEK((unsigned long) (54272ul+(unsigned long) i)));
	// }
	// POKE(1177,63); // re-enable switch to RAM in PEEK
	POKE(65299ul,(PEEK(65299ul)&3)|((15)*4)); // change character base address to 28th Kbyte
	POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
		
	// {
		// unsigned short i;
		// for(i=0;i<256;++i)
		// {
			// POKE((3072+i),i);
		// }
		
		// while(1){};		
	// }
}

