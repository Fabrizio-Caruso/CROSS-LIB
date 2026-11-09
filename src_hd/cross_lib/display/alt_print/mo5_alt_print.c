#include "standard_libs.h"
#include "display_macros.h"

void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
	uint8_t i = 0;
	
	gotoxy(x,y);
	while(str[i]!='\0')
	{
		PUTCH(str[i]);
		++i;
	}
    
	SWITCH_COLOR_BANK_OFF();
}


void _XL_CHAR(uint8_t x, uint8_t y, char ch)
{
    gotoxy(x,y);
    PUTCH(ch);
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
		gotoxy(x+length-1-i,y);
		PUTCH(48+digit);
	}
	
}	



