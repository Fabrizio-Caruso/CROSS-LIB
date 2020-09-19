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

#if defined(__CIDELSA__) || defined(__PECOM__) || defined(__TMC600__)
    #define CHAR_OFFSET 48 
#elif defined(__COMX__)
    #define CHAR_OFFSET 96
#else
    #define CHAR_OFFSET 0
#endif

#if !defined(__CIDELSA__)
    #define _DISPLAY(x,y,ch) vidchar((uint16_t)BASE_ADDR+(x)+(y)*40, (uint8_t) (ch+CHAR_OFFSET))
#else
    #define _DISPLAY(__x,__y,__ch) vidchar((uint16_t)0xF800+XSize*YSize-YSize -(__x)*40+(__y), (uint8_t) (__ch+CHAR_OFFSET))
#endif


#if defined(__COMX__)
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
		_DISPLAY(x+length-1-i,y, (uint8_t) (digits[i]));
	}
}	


