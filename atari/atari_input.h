#ifndef _ATARI_INPUT
#define _ATARI_INPUT

#include <peekpoke.h>

unsigned char GET_CHAR()
{
	unsigned char polledValue = PEEK(764);

	switch(polledValue)
	{
		case 193:
			POKE(764,255);
			return 'A';
		break;
		case 215:
			POKE(764,255);
			return 'W';
		break;
		case 211:
			POKE(764,255);
			return 'S';
		break;
		case 196:
			POKE(764,255);
			return 'D';
		break;
		case 160:
			POKE(764,255);
			return ' ';
		break;
	}
	return '\0';
}

#endif // _ATARI_INPUT