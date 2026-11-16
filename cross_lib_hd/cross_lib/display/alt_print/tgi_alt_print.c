#include <tgi.h>

#include "standard_libs.h"


void _XL_CHAR(uint8_t x, uint8_t y,  char ch)
{
    char str[2];
    
    str[1]='\0';
    str[0] = (uint8_t) ch;
    
    tgi_outtextxy(x*8,y*8,str);
}

void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
	tgi_outtextxy(x*8,y*8,str);
}


void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
    char str[2];
    
    str[1]='\0';
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
        str[0] = (uint8_t) (digit+(uint8_t) 48u);
		tgi_outtextxy((x+length-1-i)*8,y*8, str );   
	}
}



