
#include "8x8_chars.h"

#include "udg_map.h"

#include "memory_mapped_graphics.h"
#include "display_macros.h"


void set_group_color(uint8_t group, uint8_t color)
{
	DISPLAY_POKE((uint16_t) COLOR_DEF + (uint16_t) group, ((uint16_t) color)<<4);
}

void set_udg_colors(void)
{
	uint8_t i;
	
	set_group_color(0,9);
	set_group_color(1,7);
	set_group_color(2,9);
	set_group_color(3,11);
	set_group_color(4,10);
	set_group_color(5,2);
	set_group_color(6,4);
	set_group_color(7,4);
	
	for(i=8;i<=11;++i)
	{
		set_group_color(i,8);
	}
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
    
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
	{ 
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap); 
	} 
}


void _XL_INIT_GRAPHICS(void)
{
    
    SET_UDG_IMAGES();
    
    set_udg_colors();
    
    _setScreenColors();
}


