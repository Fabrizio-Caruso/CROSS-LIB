#include <peekpoke.h>


#define _RED 6
#define _BROWN 24
#define _WHITE 32
#define _ORANGE 23
#define _GREEN_BROWN 25
#define _CYAN 33
#define _DARK_YELLOW 40
#define _LIGHT_YELLOW 56

void INIT_GRAPHICS(void)
{
	POKE(0x2006,0x3F);
	POKE(0x2006,0x02);
	POKE(0x2007,_DARK_YELLOW);
	
	
	POKE(0x2006,0x3F);
	POKE(0x2006,0x01);
	POKE(0x2007,_RED);
	
	
	POKE(0x2006,0x3F);
	POKE(0x2006,0x03);
	POKE(0x2007,_WHITE);
}

