
#include <msx/gfx.h>
#include "display_macros.h"

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

/*

$00 	Transparent 	#000000
$01 	Black 	#000000
$02 	Medium Green 	#21C842
$03 	Light Green 	#5EDC78
$04 	Dark Blue 	#5455ED
$05 	Light Blue 	#7D76FC
$06 	Dark Red 	#D4524D
$07 	Cyan 	#42EBF5
$08 	Medium Red 	#FC5554
$09 	Light Red 	#FF7978
$0a 	Dark Yellow 	#D4C154
$0b 	Light Yellow 	#E6CE80
$0c 	Dark Green 	#21B03B
$0d 	Magenta 	#C95BBA
$0e 	Gray 	#CCCCCC
$0f 	White 	#FFFFFF 

*/

#define MSX_COLOR_MEDIUM_GREEN 2
#define MSX_COLOR_DARK_RED 6
#define MSX_COLOR_CYAN 7
#define MSX_COLOR_MEDIUM_RED 8
#define MSX_COLOR_LIGHT_RED 9
#define MSX_COLOR_DARK_YELLOW 10
#define MSX_COLOR_LIGHT_YELLOW 11
#define MSX_COLOR_WHITE 15

void set_udg_colors(void)
{
	uint8_t i;
	
    #if !defined(ALL_COLOR)
        #if defined(__ALTERNATIVE_COLOR)
            set_group_color(0,MSX_COLOR_LIGHT_RED);
            set_group_color(1,MSX_COLOR_WHITE);
            set_group_color(2,MSX_COLOR_LIGHT_RED);
            set_group_color(3,MSX_COLOR_LIGHT_YELLOW);
            set_group_color(4,MSX_COLOR_DARK_YELLOW);
            set_group_color(5,MSX_COLOR_MEDIUM_GREEN);
            set_group_color(6,MSX_COLOR_WHITE);
            set_group_color(7,MSX_COLOR_WHITE);
            for(i=8;i<=11;++i)
            {
                set_group_color(i,MSX_COLOR_WHITE);
            }
            for(i=12;i<=20;++i)
            {
                set_group_color(i,MSX_COLOR_DARK_RED);
            }
            for(i=21;i<=28;++i)
            {
                set_group_color(i,MSX_COLOR_WHITE);
            }
            for(i=29;i<=31;++i)
            {
                set_group_color(i,MSX_COLOR_CYAN);
            }
        #else
            set_group_color(0,MSX_COLOR_LIGHT_RED);
            set_group_color(1,MSX_COLOR_WHITE);
            set_group_color(2,MSX_COLOR_LIGHT_RED);
            set_group_color(3,MSX_COLOR_LIGHT_YELLOW);
            set_group_color(4,MSX_COLOR_DARK_YELLOW);
            set_group_color(5,MSX_COLOR_MEDIUM_GREEN);
            set_group_color(6,MSX_COLOR_CYAN);
            set_group_color(7,MSX_COLOR_CYAN);
            for(i=8;i<=11;++i)
            {
                set_group_color(i,MSX_COLOR_WHITE);
            }
            for(i=21;i<=31;++i)
            {
                set_group_color(i,MSX_COLOR_LIGHT_RED);
            }
        #endif
        

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
void _XL_INIT_GRAPHICS(void)
{
    
    #if defined(VDP_WAIT_V_SYNC)
        #include <interrupt.h>
        add_raster_int(tick_count_isr);
    #endif

	set_mode(TEXT_MODE_32x40);
    
	set_color(15, 1, 1);
    
	set_udg_colors();
    
	SET_UDG_IMAGES();
    
    _setScreenColors();
}

