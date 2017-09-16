#include <peekpoke.h>
#include "atmos_input.h"

unsigned char GET_CHAR(void)
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