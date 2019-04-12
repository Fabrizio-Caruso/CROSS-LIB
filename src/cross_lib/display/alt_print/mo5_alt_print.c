
#include "display_macros.h"

#define _DISPLAY(x,y,c)

void PRINT(unsigned char x, unsigned char y, char * str)
{
	unsigned char i = 0;
	
	gotoxy(x,y);
	while(str[i]!='\0')
	{
		PUTCH(str[i]);
		++i;
	}
	SWITCH_COLOR_BANK_OFF();
}

void print_05u0(unsigned char x, unsigned char y, unsigned short val)
{
	unsigned char i;
	unsigned char digits[6];
	
	digits[0] = 0;
	for(i=1;i<6;++i)
	{
		digits[i] = (unsigned char) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	for(i=0;i<6;++i)
	{
		_DISPLAY(x+i,y, (unsigned char) (digits[5-i])+48);
	}
}	

void print_02u(unsigned char x, unsigned char y, unsigned short val)
{
	_DISPLAY(x,y, ((unsigned char) val)/10+48);
	_DISPLAY(1+x,y, ((unsigned char) val)%10+48);
}	


void print_u(unsigned char x, unsigned char y, unsigned short val)
{
	_DISPLAY(x,y, (unsigned char) (val+48));
}


void PRINTF(unsigned char x, unsigned char y, char * str, unsigned short val)
{
	// if(strlen(str)==5)
	// {	
		// print_05u0(x,y,val);
	// }
	// else if(strlen(str)==4)
	// {
		// print_02u(x,y,val);		
	// }
	// else
	// {
		// print_u(x,y,val);		
	// }
}
