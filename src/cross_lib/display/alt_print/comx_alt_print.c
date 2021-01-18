#include "display_macros.h"
#include <devkit/video/vis_video.h>

#include "standard_libs.h"

#if !defined(NO_STATS)
unsigned int strlen(char *str)
{
    asm(
        " ldi 0xff\n"
        " plo R15\n"
        " phi R15\n"
        "$$loop:\n"
        " inc R15\n"
        " lda R12\n"
        " bnz $$loop\n"
        " Cretn\n");
    return 0;
}
#endif

void _XL_PRINT(uint8_t x, uint8_t y, char * str)
{
#if !defined(__CIDELSA__)
    vidstrcpy((uint16_t) BASE_ADDR+x+y*40,str);
#else
    vidstrcpy((uint16_t) 0xF800+XSize*YSize-YSize -x*40+y,str);
#endif
}

#if defined(__COMX__) && !defined(COMX_COLOR)
    #define CHAR_OFFSET 96
#else
    #define CHAR_OFFSET 48
#endif

#if !defined(__CIDELSA__)
    #define _DISPLAY(x,y,ch) vidchar((uint16_t)BASE_ADDR+(x)+(y)*40, (uint8_t) (ch+CHAR_OFFSET))
#else
    #define _DISPLAY(__x,__y,__ch) vidchar((uint16_t)0xF800+XSize*YSize-YSize -(__x)*40+(__y), (uint8_t) (__ch+CHAR_OFFSET))
#endif


void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
		_DISPLAY(x+length-1-i,y, (uint8_t) (digit));
	}
}	


