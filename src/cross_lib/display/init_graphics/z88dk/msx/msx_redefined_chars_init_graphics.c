
#include <msx/gfx.h>

#define CHAR_BASE 0x0100
#define COLOR_DEF 0x2000

#include "../../../image_settings/msx_redefined_chars_settings.h"

#include "../../../graphics_data/8x8_chars.h"

#include "../../../redefine_characters/udg_map.h"

void redefine(unsigned short offset, const char *new_char)
{
	unsigned char i;
	
	for(i=0;i<8;++i)
	{
		msx_vpoke(CHAR_BASE+(offset<<3)+i-32*8,new_char[i]);
	}
}

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


void set_udg_images(void)
{
	unsigned char i;
	
	for (i = 0; i < sizeof(redefine_map) / sizeof(*redefine_map); ++i)
	{
	   redefine(redefine_map[i].ascii, redefine_map[i].bitmap);
	}
}

void INIT_GRAPHICS(void)
{
	set_color(15, 1, 1);	
	set_mode(mode_1);
	
	set_udg_colors();
	set_udg_images();
}
