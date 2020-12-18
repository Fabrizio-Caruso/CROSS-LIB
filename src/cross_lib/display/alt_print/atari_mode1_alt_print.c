
#include <stdint.h>
#include <conio.h>

#include "cross_lib.h"


void _GOTOXY(uint8_t x, uint8_t y)
{ 
	if((y)&1) 
	{ 
		gotoxy(x+20,(y)/2);
	} 
	else 
	{ 
		gotoxy(x, (y)/2);
	} 
};

void PRINT(uint8_t x, uint8_t y, char * str)
{ 
    _GOTOXY(x,y);
    cprintf(str); 
};


void PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
		DISPLAY_POKE(loc(x+length-1-i,y), (uint8_t) (digit+(uint8_t) 80u));
	}
}


