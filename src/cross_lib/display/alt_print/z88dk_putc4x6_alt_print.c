
extern uint8_t XSize;
extern uint8_t YSize;

#include "cross_lib.h"

# if defined(Z88DK_PUTC4X6)
	void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
	{
		uint8_t i = 0;
		while(str[i]!='\0')
		{
			gotoxy(x+i,y);
			cputc(str[i++]);
		}
	}


	void _XL_PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val)
	{

	}
#endif
