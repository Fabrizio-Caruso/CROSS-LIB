#include <nstdlib.h>

#include <rca_vis_video.h>
// #include <rca_printf.h>

#include "standard_libs.h"


void PRINT(uint8_t x, uint8_t y, char * str)
{
    vidstrcpy((uint16_t) 0xF800+x+y*40,str);
    // gotoxy(x,y);
    // printf(str);

}

#if defined(__COMX__)
    #define CHAR_OFFSET 48
#else
    #define CHAR_OFFSET 48
#endif


#define _DISPLAY(x,y,ch) vidchar((uint16_t)0xF800+x+y*40, ch+CHAR_OFFSET)

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
	
	for(i=0;i<6;++i)
	{
		_DISPLAY(x+i,y, (uint8_t) (digits[5-i])+CHAR_OFFSET);
	}
}	

void print_02u(uint8_t x, uint8_t y, uint16_t val)
{
	_DISPLAY(x,y, ((uint8_t) val)/10+CHAR_OFFSET);
	_DISPLAY(1+x,y, ((uint8_t) val)%10+CHAR_OFFSET);
}	


void print_u(uint8_t x, uint8_t y, uint16_t val)
{
	_DISPLAY(x,y, (uint8_t) (val+CHAR_OFFSET));
}


void PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val)
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
}


#include <nstdlib.c>

