
#include <stdint.h>
#include <conio.h>

#include "cross_lib.h"

#if !defined(__ANTIC_MODE6_GRAPHICS)
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

    void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
    { 
        _GOTOXY(x,y);
        cprintf(str); 
    };

#else
    extern uint8_t _atari_text_color;


	uint8_t screenCode(uint8_t ch)
	{
		if(ch==32)
		{
			return 0;
		}
		else
		{
			return ch+(_atari_text_color)-0x20u;
		}
	}


    void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
    {
        uint8_t i = 0;

        while(str[i]!='\0')
        {

            DISPLAY_POKE(loc(x+i,y), screenCode(str[i]));
            ++i;
        }
    }
    
    void _XL_CHAR(uint8_t x, uint8_t y, char ch) 
    {
        DISPLAY_POKE(loc(x,y),screenCode(ch));
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
#if defined(__ANTIC_MODE6_GRAPHICS)
		DISPLAY_POKE(loc(x+length-1-i,y), (uint8_t) (digit+(uint8_t) 0x10 + _atari_text_color));
#else
		DISPLAY_POKE(loc(x+length-1-i,y), (uint8_t) (digit+(uint8_t) 0x10 + 64));

#endif
	}
}


