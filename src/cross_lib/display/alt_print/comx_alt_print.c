// #include <nstdlib.h>
#include "display_macros.h"
#include <comx/rca_vis_video.h>
// #include <comx/rca_printf.h>

#include "standard_libs.h"

#include "memory_mapped_graphics.h"

#if !defined(NO_STATS)
unsigned char strlen(const char *s)
{
    unsigned char count = 0;
    
    while(*s)
    {
        ++count;
        ++s;
    };
    return count;
}
#endif

void PRINT(uint8_t x, uint8_t y, char * str)
{
    vidstrcpy((uint16_t) BASE_ADDR+x+y*40,str);
}

#define CHAR_OFFSET 48

#define _DISPLAY(x,y,ch) vidchar((uint16_t)BASE_ADDR+(x)+(y)*40, (uint8_t) (ch+CHAR_OFFSET))

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
        #if defined(__COMX__)
		_DISPLAY(x+i,y, (uint8_t) (digits[5-i])+CHAR_OFFSET);
        #elif defined(__PECOM__) || defined(__TMC600__)
        vidchar((uint16_t)BASE_ADDR+x+i+y*40, (uint8_t) (digits[5-i])+CHAR_OFFSET);
        #elif defined(__DRACO__)
        vidchar((uint16_t)0xFC10-(x+i)*40+y, (uint8_t) (digits[5-i])+CHAR_OFFSET);
        #endif
	}
}	

#if !defined(NO_STATS)
void print_02u(uint8_t x, uint8_t y, uint16_t val)
{
     #if defined(__COMX__)
	_DISPLAY(x,y, ((uint8_t) val)/10+CHAR_OFFSET);
	_DISPLAY(1+x,y, ((uint8_t) val)%10+CHAR_OFFSET);
    #elif defined(__PECOM__) || defined(__TMC600__)
    vidchar((uint16_t)BASE_ADDR+x+y*40, ((uint8_t) val)/10+CHAR_OFFSET);
    vidchar((uint16_t)BASE_ADDR+1+x+y*40, ((uint8_t) val)%10+CHAR_OFFSET);
    #elif defined(__DRACO__)
    vidchar((uint16_t)0XFC10-x*40+y, ((uint8_t) val)/10+CHAR_OFFSET);
    vidchar((uint16_t)0XFBE8-x*40+y, ((uint8_t) val)%10+CHAR_OFFSET);
    #endif
}	


void print_u(uint8_t x, uint8_t y, uint16_t val)
{

     #if defined(__COMX__)
	_DISPLAY(x,y, (uint8_t) (val+CHAR_OFFSET));
    #elif defined(__PECOM__) || defined(__TMC600__)
    vidchar((uint16_t)BASE_ADDR+x+y*40, ((uint8_t) val)+CHAR_OFFSET);
    #elif defined(__DRACO__)
    vidchar((uint16_t)BASE_ADDR-x*40+y, ((uint8_t) val)+CHAR_OFFSET);
    #endif
}
#endif

void PRINTD(uint8_t x, uint8_t y, char * str, uint16_t val)
{
    #if !defined(NO_STATS)
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
    #else
		print_05u0(x,y,val);
    #endif
}


// #include <nstdlib.c>

