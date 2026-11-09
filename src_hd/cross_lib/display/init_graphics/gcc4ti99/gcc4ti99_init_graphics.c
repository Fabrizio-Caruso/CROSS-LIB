
#include <system.h>
#include <conio.h>
#include <vdp.h>

#include "8x8_chars.h"

#include "udg_map.h"

#include "display_macros.h"
// #include <stdint.h>

#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define set_group_color(group, color) \
        vdpmemset(gColor+group,16*color,1)

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
	set_group_color(0,COLOR_WHITE);
	set_group_color(1,COLOR_WHITE);
	set_group_color(2,COLOR_WHITE);
	set_group_color(3,COLOR_WHITE);

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


	set_group_color(12,COLOR_WHITE);
	set_group_color(13,COLOR_WHITE);
	set_group_color(14,COLOR_WHITE);
	set_group_color(15,COLOR_WHITE);

// 128
	set_group_color(16,COLOR_CYAN);
	set_group_color(17,COLOR_CYAN);
	set_group_color(18,COLOR_CYAN);
	set_group_color(19,COLOR_CYAN);

// 160
	set_group_color(20,COLOR_CYAN);
	set_group_color(21,COLOR_CYAN);
	set_group_color(22,COLOR_CYAN);
	set_group_color(23,COLOR_CYAN);

// 192
	set_group_color(24,COLOR_CYAN);
	set_group_color(25,COLOR_CYAN);
	set_group_color(26,COLOR_CYAN);
	set_group_color(27,COLOR_CYAN);

// 224
	set_group_color(28,COLOR_CYAN);
	set_group_color(29,COLOR_CYAN);
	set_group_color(30,COLOR_CYAN);
	set_group_color(31,COLOR_CYAN);
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
    // for (i = 0; i < _XL_NUMBER_OF_TILES; ++i) 

    {
        for(j=0;j<2;++j)
        {
            redefine(redefine_map[i].ascii+j*128, redefine_map[i].bitmap);
        }
    } 
}
// settings for mode register 0
// #define VDP_MODE0_BITMAP		0x02		// set bitmap mode
// #define VDP_MODE0_EXTVID		0x01		// enable external video (not connected on TI-99/4A)
// #define VDP_MODE0_80COL                 0x04            // enable 9938/F18A 80 column

// settings for mode register 1
// #define VDP_MODE1_16K			0x80		// set 16k mode (4k mode if cleared)
// #define VDP_MODE1_UNBLANK		0x40		// set to enable display, clear to blank it
// #define VDP_MODE1_INT			0x20		// enable VDP interrupts
// #define VDP_MODE1_TEXT			0x10		// set text mode
// #define VDP_MODE1_MULTI			0x08		// set multicolor mode
// #define VDP_MODE1_SPRMODE16x16	0x02		// set 16x16 sprites
// #define VDP_MODE1_SPRMAG		0x01		// set magnified sprites (2x2 pixels) 

// sprite modes for the mode set functions
// #define VDP_SPR_8x8				0x00
// #define	VDP_SPR_8x8MAG			(VDP_MODE1_SPRMAG)
// #define VDP_SPR_16x16			(VDP_MODE1_SPRMODE16x16)
// #define VDP_SPR_16x16MAG		(VDP_MODE1_SPRMODE16x16 | VDP_MODE1_SPRMAG)

void _XL_INIT_GRAPHICS(void)
{
    // uint8_t i;
    
    // set_bitmap(VDP_SPR_16x16);                        // set video mode
    set_graphics(VDP_SPR_16x16);                        // set video mode

    charsetlc();                                        // load character set with lowercase
    vdpmemset(gImage, 32, 768);                         // clear screen
    VDP_SET_REGISTER(VDP_REG_COL, COLOR_BLACK);      // set screen color
    
    set_udg_colors();
    
    SET_UDG_IMAGES();

    _setScreenColors();
        // set_graphics(VDP_MODE0_BITMAP);                        // set video mode

}

