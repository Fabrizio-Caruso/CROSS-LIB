#include <peekpoke.h>
#include "display_macros.h"

#include <stdint.h>

#include <nes.h>

#define _RED 6
#define _BROWN 24
#define _WHITE 32
#define _ORANGE 23
#define _GREEN_BROWN 25
#define _CYAN 33
#define _DARK_YELLOW 40
#define _LIGHT_YELLOW 56


void set_color(uint8_t palette_group, uint8_t color_index, uint8_t color)
{
    PPU.vram.address = 0x3f;
    PPU.vram.address = palette_group * 4 + color_index;   
    PPU.vram.data = color;   
}

#define CTRL 0x2000
#define MASK 0x2001
#define SCROLL 0x2005


void All_Off(void) {
	PPU.control = 0;
	PPU.mask = 0;
}
	
void All_On(void) {
	PPU.control = 0x80;
	PPU.mask = 0x1e;
}

void Reset_Scroll(void)
{
	PPU.vram.address = 0;
	PPU.vram.address = 0;
	
	PPU.scroll = 0;
	PPU.scroll = 0;
}


//
#if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
    #define NES_BACKGROUND_COLOR COLOR_WHITE
#else
    #define NES_BACKGROUND_COLOR COLOR_BLACK
#endif


void _XL_INIT_GRAPHICS(void)
{
	All_Off();

	set_color(0,2,_DARK_YELLOW);
	set_color(0,1,_RED);
	set_color(0,3,_WHITE);

	set_color(1,2,_DARK_YELLOW);
	set_color(1,1,_RED);
	set_color(1,3,_WHITE);

	set_color(3,2,_DARK_YELLOW);
	set_color(3,1,_RED);
	set_color(3,3,_WHITE);

	// set_color(1,2,_DARK_YELLOW);
	// set_color(1,1,_GREEN_BROWN);
	// set_color(1,3,_RED);


	// set_color(3,2,_DARK_YELLOW);
	// set_color(3,1,_BROWN);
	// set_color(3,3,_CYAN);
	
	{
		uint8_t j;
	
		for(j=0;j<4;++j)
		{	
			PPU.vram.address = 0x23;
			PPU.vram.address = (0xC0+j);				
			PPU.vram.data = 0xFF;//0b00001111;	
		}
		
		for(j=4;j<8;++j)
		{	
			PPU.vram.address = 0x23;
			PPU.vram.address = (0xC0+j);				
			PPU.vram.data = 0x55;//0b00001111;	
		}
					
		PPU.vram.address = 0x23;
		PPU.vram.address = 0xC3;
		PPU.vram.data = 0xF0;			
		
		PPU.vram.address = 0x23;
		PPU.vram.address = 0xC4;
		PPU.vram.data = 0x00;
				
	}	
	
	Reset_Scroll();
	All_On();

    bgcolor(NES_BACKGROUND_COLOR);
}

