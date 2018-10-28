#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../../graphics_settings.h"

#define UDG_BASE_FACTOR 15


void INIT_GRAPHICS(void)
{
	unsigned short i;

	POKE(65299ul,(PEEK(65299ul)&3)|((15)*4)); // change character base address to 28th Kbyte
	POKE(65298ul,PEEK(65298ul)&251); // make graphics chip get characters from RAM
	
	#if defined(DEBUG)
	{
		unsigned char i;
		for(i=0;i<255;++i)
		{
			POKE((3072+i),i+0x40);
		}
		
		while(1){};
	}
	#endif
	
	#if defined(FORCE_NO_CONIO_LIB)
		POKE(65301u,0);
		POKE(65305u,0);
	#endif

	for(i=0;i<1000;++i)
	{
		POKE(2048+i,0x71);
	}
	
}

