
// https://www.msx.org/wiki/Category:VDP_Registers

#include "8x8_chars.h"

#include "udg_map.h"

#include "memory_mapped_graphics.h"
#include "display_macros.h"

/*

0 Transparent
1 Black 
2 Green
3 Light Green
4 Dark Blue
5 Light Blue
6 Dark Red
7 Cyan
8 Red
9 Bright Red
10 Yellow
11 Light Yellow
12 Dark Green
13 Magenta
14 Grey
15 White
*/

#define _CREAT_VDP_GREEN 3
#define _CREAT_VDP_BLUE 5
#define _CREAT_VDP_CYAN 7
#define _CREAT_VDP_RED 9
#define _CREAT_VDP_YELLOW 10
#define _CREAT_VDP_WHITE 15
#define _CREAT_VDP_BLACK 0

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define _CREAT_BACKGROUND_COLOR _CREAT_VDP_WHITE
    #define _CREAT_TEXT_COLOR _CREAT_VDP_BLACK
#else
    #define _CREAT_BACKGROUND_COLOR _CREAT_VDP_BLACK
    #define _CREAT_TEXT_COLOR _CREAT_VDP_WHITE
#endif


#define set_group_colorset_group_color(group, color) \
do \
{ \
	DISPLAY_POKE((uint16_t) COLOR_DEF + (uint16_t) group, (((uint16_t) color)<<4)+_CREAT_BACKGROUND_COLOR); \
} while(0)


void set_group_block_color(uint8_t group, uint8_t color)
{
	uint8_t i;
	for(i=0;i<4;++i)
	{
		set_group_colorset_group_color(group+i,color);
	}
}

void set_udg_colors(void)
{

    set_group_block_color(0,_CREAT_VDP_WHITE);
    set_group_block_color(4,_CREAT_VDP_WHITE);
    set_group_block_color(8,_CREAT_VDP_WHITE);
    set_group_block_color(12,_CREAT_VDP_WHITE);
    set_group_block_color(16,_CREAT_VDP_CYAN);
    set_group_block_color(20,_CREAT_VDP_CYAN);
    set_group_block_color(24,_CREAT_VDP_CYAN);
    set_group_block_color(28,_CREAT_VDP_CYAN);

}

void redefine(const uint8_t ch, const uint8_t* image) 
{ 
    uint8_t i; 
    
    for(i=0;i<8;++i) 
    { 
        DISPLAY_POKE(CHAR_BASE +(uint16_t)(ch<<3)+i,image[i]); 
    } 
} 


void SET_UDG_IMAGES(void) 
{ 
    uint8_t i;
    uint8_t j;

    for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
    {
        for(j=0;j<2;++j)
        {
            redefine(redefine_map[i].ascii+128*j, redefine_map[i].bitmap);
        }
    } 
}


void _XL_INIT_GRAPHICS(void)
{
    SET_UDG_IMAGES();
    
    set_udg_colors();

    _setScreenColors();
}


