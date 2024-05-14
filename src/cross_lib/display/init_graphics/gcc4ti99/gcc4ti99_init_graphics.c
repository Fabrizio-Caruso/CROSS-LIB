
#include <system.h>
#include <conio.h>
#include <vdp.h>

#include "8x8_chars.h"

#include "udg_map.h"

#include "display_macros.h"
// #include <stdint.h>

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define set_group_color(group, color) \
        vdpmemset(gColor+group,COLOR_WHITE+16*color,1)

#else
    #define set_group_color(group, color) \
        vdpmemset(gColor+group,COLOR_BLACK+16*color,1)
#endif

#define _GROUP_COLOR_OFFSET 8

// COLOR_LTGREEN
// COLOR_LTYELLOW

void set_udg_colors(void)
{

// 0
	set_group_color(0,COLOR_LTBLUE);
	set_group_color(1,COLOR_LTBLUE);
	set_group_color(2,COLOR_LTBLUE);
	set_group_color(3,COLOR_LTBLUE);

// 32 - Digits
	set_group_color(4,COLOR_WHITE);
	set_group_color(5,COLOR_WHITE);
	set_group_color(6,COLOR_WHITE);
	set_group_color(7,COLOR_WHITE);

// 64 - Alphabet
	set_group_color(8,COLOR_WHITE);
	set_group_color(9,COLOR_WHITE);
	set_group_color(10,COLOR_WHITE);
	set_group_color(11,COLOR_WHITE);

// 96
    #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
	set_group_color(12,COLOR_BLACK);
	set_group_color(13,COLOR_BLACK);
	set_group_color(14,COLOR_BLACK);
	set_group_color(15,COLOR_BLACK);
    #else
	set_group_color(12,COLOR_WHITE);
	set_group_color(13,COLOR_WHITE);
	set_group_color(14,COLOR_WHITE);
	set_group_color(15,COLOR_WHITE);
    #endif

// 128
	set_group_color(16,COLOR_CYAN);
	set_group_color(17,COLOR_CYAN);
	set_group_color(18,COLOR_CYAN);
	set_group_color(19,COLOR_CYAN);

// 160
	set_group_color(20,COLOR_LTRED);
	set_group_color(21,COLOR_LTRED);
	set_group_color(22,COLOR_LTRED);
	set_group_color(23,COLOR_LTRED);

// 192
	set_group_color(24,COLOR_LTGREEN);
	set_group_color(25,COLOR_LTGREEN);
	set_group_color(26,COLOR_LTGREEN);
	set_group_color(27,COLOR_LTGREEN);

// 224
	set_group_color(28,COLOR_LTYELLOW);
	set_group_color(29,COLOR_LTYELLOW);
	set_group_color(30,COLOR_LTYELLOW);
	set_group_color(31,COLOR_LTYELLOW);
}


void redefine(const uint8_t ch, const uint8_t* image) 
{ 

    vdpmemcpy(gPattern +(uint16_t)(ch<<3),image,8); 
} 


void SET_UDG_IMAGES(void) 
{ 
    uint8_t i;
    uint8_t j;

    for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
    {
        for(j=0;j<5;++j)
        {
            redefine(redefine_map[i].ascii+32*j, redefine_map[i].bitmap);
        }
    } 
}


void _XL_INIT_GRAPHICS(void)
{
    uint8_t i;
    
    set_graphics(VDP_SPR_16x16);                        // set video mode
    charsetlc();                                        // load character set with lowercase
    vdpmemset(gImage, 32, 768);                         // clear screen
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_BLACK);      // set screen color
    
    set_udg_colors();
    
    SET_UDG_IMAGES();

    _setScreenColors();
}

