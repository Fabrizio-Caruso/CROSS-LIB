#include <tgi.h>

#include "standard_libs.h"

#include "cross_lib.h"

void _XL_CHAR(uint8_t x, uint8_t y,  char ch)
{
    if(ch==' ')
    {
        _XL_DRAW(x,y,_XL_NUMBER_OF_TILES+FONT_SIZE-1,_XL_WHITE);
    }
    else
    {
        if((ch>='A')&&(ch<='Z'))
        {
            _XL_DRAW(x,y,ch-'A'+_XL_NUMBER_OF_TILES,_XL_WHITE);
        }
        else
        {
            _XL_DRAW(x,y,ch-'0'+_XL_NUMBER_OF_TILES+FONT_SIZE,_XL_WHITE);
        }
    }
}


void _XL_PRINT(uint8_t x, uint8_t y, char * str)
{
    uint8_t i;
    
    i=0;
    while(str[i]!='\0')
    {
        // _XL_DRAW(x+i,y,str[i]-'A'+_XL_NUMBER_OF_TILES,_XL_WHITE);
        _XL_CHAR(x+i,y,str[i]);
        ++i;
    }
    
}


void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
    
	uint8_t i;
	uint8_t digit;

	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
        _XL_DRAW((x+length-1-i),y,(uint8_t) (digit+_XL_NUMBER_OF_TILES+26),_XL_WHITE);
	}
    
}



