#ifndef _ATMOS_INPUT
#define _ATMOS_INPUT

#include <peekpoke.h>

unsigned char GET_CHAR()
{
	unsigned char polledValue = PEEK(0x208);

	switch(polledValue)
	{
		case 174:
			return 'A';
		break;
		case 190:
			return 'W';
		break;
		case 182:
			return 'S';
		break;
		case 185:
			return 'D';
		break;
		case 132:
			return ' ';
		break;
	}
	return '\0';
}

#endif