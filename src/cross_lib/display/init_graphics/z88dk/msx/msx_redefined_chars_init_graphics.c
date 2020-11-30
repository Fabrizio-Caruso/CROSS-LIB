
#include <msx/gfx.h>

// 0000 | 07FF | VRAM: Main Tile Patterns (1/3)
// 0800 | 0FFF | VRAM: Extra Tile Patterns (2/3)
// 1000 | 17FF | VRAM: Extra Tile Patterns (3/3)
// 1800 | 1AFF | VRAM: Tilemap
// 1B00 | 1B7F | VRAM: Sprite Attributes
// 1B80 | 1BAF | VRAM: Palette Table
// 2000 | 37FF | VRAM: Colormap
// 3800 | 3FFF | VRAM: Sprite Patterns

#if !defined(USE_MSX_BIOS)
    #define COLOR_DEF 0x1000
    #define CHAR_BASE 0x2000
    #define TEXT_MODE_32x40 mode_0
#else
    #define COLOR_DEF 0x2000
    #define CHAR_BASE 0x0000
    #define TEXT_MODE_32x40 mode_1    
#endif

#include "msx_redefined_chars_settings.h"

#include "8x8_chars.h"

#include "udg_map.h"

#include "memory_mapped_graphics.h"


void set_group_color(uint8_t group, uint8_t color)
{
	msx_vpoke(COLOR_DEF+group, color<<4);
}


void set_udg_colors(void)
{
	uint8_t i;
	
    #if !defined(ALL_COLOR)
        set_group_color(0,9);
        set_group_color(1,7);
        set_group_color(2,9);
        set_group_color(3,11);
        set_group_color(4,10);
        set_group_color(5,2);
        set_group_color(6,7);
        set_group_color(7,7);
        
        for(i=8;i<=11;++i)
        {
            set_group_color(i,8);
        }
	#else
        for(i=0;i<=32;++i)
        {
            set_group_color(i,ALL_COLOR);
        }
    #endif
    
    

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

#include <conio.h>
void INIT_GRAPHICS(void)
{
    
    #if defined(VDP_WAIT_V_SYNC)
        #include <interrupt.h>
        add_raster_int(tick_count_isr);
    #endif

	set_mode(TEXT_MODE_32x40);
    
	set_color(15, 1, 1);	
    
	set_udg_colors();
    
	SET_UDG_IMAGES();
}
