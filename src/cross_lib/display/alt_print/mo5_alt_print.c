#include "standard_libs.h"
#include "display_macros.h"

void PRINT(uint8_t x, uint8_t y, char * str)
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

void print_05u0(uint8_t x, uint8_t y, uint16_t val)
{
	uint8_t i;
	uint8_t digits[6];
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (uint8_t) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	gotoxy(x,y);
	for(i=0;i<6;++i)
	{
		PUTCH((uint8_t) (digits[5-i])+48);
	}
}	

void print_02u(uint8_t x, uint8_t y, uint16_t val)
{
	gotoxy(x,y);
	PUTCH(((uint8_t) val)/10+48);
	PUTCH(((uint8_t) val)%10+48);
}	


void print_u(uint8_t x, uint8_t y, uint16_t val)
{
	gotoxy(x,y);
	PUTCH((uint8_t) (val+48));	
}


void PRINTF(uint8_t x, uint8_t y, char * str, uint16_t val)
{
	if(strlen(str)==5)
	{	
		print_05u0(x,y,val);
	}
	else if(strlen(str)==4)
	{
		print_02u(x,y,val);		
	}
	else
	{
		print_u(x,y,val);		
	}
	SWITCH_COLOR_BANK_OFF();	
}
