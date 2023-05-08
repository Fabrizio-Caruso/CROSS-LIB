#include "display_macros.h"
#include <devkit/video/vis_video.h>

#include "standard_libs.h"


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


// TODO: This is broken with a const parameter
void _XL_PRINT(uint8_t x, uint8_t y, const char * str)
{
#if !defined(__CIDELSA__)
    vidstrcpy((uint16_t) BASE_ADDR+x+y*40,(char *)str);
#else
    vidstrcpy((uint16_t) 0xF800+REAL_XSIZE*40-40 -(uint16_t)(x+X_OFFSET)*(uint16_t)40+(y+Y_OFFSET),(char *)str);
#endif
}

#if defined(__COMX__) && !defined(__COMX_COLOR_GRAPHICS)
    #define CHAR_OFFSET 96
#else
    #define CHAR_OFFSET 48
#endif


#if !defined(__CIDELSA__)
    #if defined(__LCC1802_UNBUFFERED) 
        #define _DISPLAY(x,y,ch) vidcharnobufxy(x,y, (uint8_t) (ch+CHAR_OFFSET))
    #else
        #define _DISPLAY(x,y,ch) vidcharxy(x,y, (uint8_t) (ch+CHAR_OFFSET))
    #endif
#else
        #define _DISPLAY(__x,__y,__ch) vidcharxy(__x+X_OFFSET,__y+Y_OFFSET, (uint8_t) (__ch+CHAR_OFFSET))
#endif

#define COLOR_BIT 0

#define _XL_CHAR(x,y,ch) _DISPLAY(x,y,(ch+80+COLOR_BIT))

void _XL_PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digit;
	
	for(i=0;i<length;++i)
	{
		digit = (uint8_t) ((val)%10);
		val-= digit;
		val/=10;
		_DISPLAY(x+length-1-i,y, (uint8_t) (digit+128));
	}
}	


