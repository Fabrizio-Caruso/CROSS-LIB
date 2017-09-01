#ifndef _APPLE2_INPUT
#define _APPLE2_INPUT

#include <peekpoke.h>

unsigned char GET_CHAR()
{
	unsigned char polledValue = PEEK(0xC000);
	POKE(0xC010,0);
	return PEEK(0xC010)-128;	
}

#endif // _APPLE2_INPUT