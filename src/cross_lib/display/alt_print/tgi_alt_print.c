#include <tgi.h>

#include "standard_libs.h"


void PRINT(uint8_t x, uint8_t y, char * str)
{
	tgi_outtextxy(x*8,y*8,str);
}


void PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digits[5];
    unsigned char str[2];
    
    str[1]='\0';
	
	for(i=0;i<length;++i)
	{
		digits[i] = (uint8_t) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	for(i=0;i<length;++i)
	{
        str[0] = (uint8_t) (digits[i])+48;
		tgi_outtextxy((x+length-1-i)*8,y*8, str );
        
	}
}



