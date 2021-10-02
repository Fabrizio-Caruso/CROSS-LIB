
#include <stdint.h>
#include <conio.h>

#include "cross_lib.h"

#if !defined(ATARI_MODE_1_COLOR)
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

void _XL_PRINT(uint8_t x, uint8_t y, char * str)
{ 
    _GOTOXY(x,y);
    cprintf(str); 
};
#else
extern uint8_t text_color;

void _XL_PRINT(uint8_t x, uint8_t y, char * str)
{
    uint8_t i = 0;

    while(str[i]!='\0')
    {
        #if defined(_API_VERSION) & _API_VERSION<2
        DISPLAY_POKE(loc(x+i,y), str[i]+(text_color)-0x40u);
        #else
        DISPLAY_POKE(loc(x+i,y), str[i]+(text_color)-0x20u);
        #endif
        ++i;
	}
}
#endif

void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
#if defined(ATARI_MODE_1_COLOR)
		DISPLAY_POKE(loc(x+length-1-i,y), (uint8_t) (digit+(uint8_t) 0x10 + text_color));
#else
		DISPLAY_POKE(loc(x+length-1-i,y), (uint8_t) (digit+(uint8_t) 0x10 + 64));

#endif
	}
}


