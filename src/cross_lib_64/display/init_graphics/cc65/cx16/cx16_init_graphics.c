#include <cx16.h>
#include "standard_libs.h"

#include "8x8_chars.h"

#include "udg_map.h"


#define CHAR_BASE ((long) 0x01F000)


void redefine(const uint8_t ch, const uint8_t* image) 
{ 
    uint8_t i;
    
    for(i=0;i<8;++i) 
    {
        if(ch>63)
        {
            vpoke(image[i], CHAR_BASE +(unsigned long)((((unsigned long) ch-64U)<<3)+i)); 
        }
        else
        {
            vpoke(image[i], CHAR_BASE +(unsigned long)((((unsigned long) ch)<<3)+i)); 
        }
    } 
} 


void SET_UDG_IMAGES(void) 
{ 
	uint8_t i;
    
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
	{ 
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap); 
	} 
}

// ; #define VIDEOMODE_80x60         0x00
// ; #define VIDEOMODE_80x30         0x01
// ; #define VIDEOMODE_40x60         0x02
// ; #define VIDEOMODE_40x30         0x03
// ; #define VIDEOMODE_40x15         0x04
// ; #define VIDEOMODE_20x30         0x05
// ; #define VIDEOMODE_20x15         0x06
// ; #define VIDEOMODE_22x23         0x07
// ; #define VIDEOMODE_64x50         0x08
// ; #define VIDEOMODE_64x25         0x09
// ; #define VIDEOMODE_32x50         0x0A
// ; #define VIDEOMODE_32x25         0x0B
// ; #define VIDEOMODE_80COL         VIDEOMODE_80x60
// ; #define VIDEOMODE_40COL         VIDEOMODE_40x30
// ; #define VIDEOMODE_320x240       0x80
void _XL_INIT_GRAPHICS(void)
{
    videomode(VIDEOMODE_20x30);
    printf("%c%c%c%c",0x90,0x01,0x05,147);
    // printf("%c",0x09);
    // printf("%c",0x01);
    // printf("%c",0x01);

    // printf("%c",0x10);
    // printf("%c",0x01);
    cputc(0x10);
    cputc(0x01);
	SET_UDG_IMAGES();
}