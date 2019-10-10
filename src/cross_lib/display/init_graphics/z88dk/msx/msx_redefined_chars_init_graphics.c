
#include <msx/gfx.h>

#if !defined(USE_MSX_BIOS)
    #define COLOR_DEF 0x1000
    #define CHAR_BASE 0x2000
#else
    #define COLOR_DEF 0x0100
    #define CHAR_BASE 0x2000
#endif

#include "msx_redefined_chars_settings.h"

#include "8x8_chars.h"

#include "udg_map.h"

#include "memory_mapped_graphics.h"


void set_group_color(unsigned char group, unsigned char color)
{
	msx_vpoke(COLOR_DEF+group, color<<4);
}


void set_udg_colors(void)
{
	unsigned char i;
	
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


void redefine(const unsigned char ch, const unsigned char* image) 
{ 
    unsigned char i; 
    
    for(i=0;i<8;++i) 
    { 
        DISPLAY_POKE(CHAR_BASE +(unsigned short)(ch<<3)+i,image[i]); 
    } 
} 


void SET_UDG_IMAGES(void) 
{ 
	unsigned char i;
    
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i) 
	{ 
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap); 
	} 
}

#include <conio.h>
void INIT_GRAPHICS(void)
{
	set_mode(mode_0);
    
	set_color(15, 1, 1);	
    
	set_udg_colors();
    
	SET_UDG_IMAGES();
}
