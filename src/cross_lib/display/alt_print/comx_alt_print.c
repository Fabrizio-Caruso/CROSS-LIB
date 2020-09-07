#include "display_macros.h"
#include <devkit/video/vis_video.h>

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
#if !defined(__CIDELSA__)
    vidstrcpy((uint16_t) BASE_ADDR+x+y*40,str);
#else
    vidstrcpy((uint16_t) 0xF800+XSize*YSize-YSize -x*40+y,str);
#endif
}

#define CHAR_OFFSET 48

#if !defined(__CIDELSA__)
    #define _DISPLAY(x,y,ch) vidchar((uint16_t)BASE_ADDR+(x)+(y)*40, (uint8_t) (ch+CHAR_OFFSET))
#else
    #define _DISPLAY(x,y,ch) vidchar((uint16_t)0xF800+XSize*YSize-YSize -x*40+y, (uint8_t) (ch+CHAR_OFFSET))
#endif


void PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digits[5];
	
	for(i=0;i<length;++i)
	{
		digits[i] = (uint8_t) ((val)%10);
		val-= digits[i];
		val/=10;
	}
	
	for(i=0;i<length;++i)
	{
        #if defined(__COMX__) 
		_DISPLAY(x+length-1-i,y, (uint8_t) (digits[i]+CHAR_OFFSET));
        #elif defined(__PECOM__) || defined(__TMC600__)
        vidchar((uint16_t)BASE_ADDR+x+length-1-i+y*40, (uint8_t) (digits[i]+CHAR_OFFSET));
        #elif defined(__CIDELSA__)
        vidchar((uint16_t)0xF800+XSize*YSize-YSize-(x+length-1-i)*40+y, (uint8_t) (digits[i])+CHAR_OFFSET);
        #endif
	}
}	


