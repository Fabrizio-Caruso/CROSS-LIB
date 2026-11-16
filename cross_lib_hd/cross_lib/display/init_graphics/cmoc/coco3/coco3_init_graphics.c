
#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))

#include "display_macros.h"

#include "coco.h"

// #include "color_definitions.h"
	
#define _GIME_BLACK 0b000000
#define _GIME_WHITE 0b111111

#define _GIME_RED 0b100100
#define _GIME_GREEN 0b010010
#define _GIME_CYAN 0b011011

#define _GIME_YELLOW 0b110110

#define _GIME_PURPLE 0b101101

#define _GIME_BLUE 0b001001


const unsigned char _color_encoding[] = 
	{
		_GIME_BLACK,_GIME_WHITE,_GIME_RED,_GIME_GREEN,_GIME_CYAN,_GIME_YELLOW,_GIME_PURPLE,_GIME_BLUE,
		_GIME_BLACK,_GIME_WHITE,_GIME_RED,_GIME_GREEN,_GIME_CYAN,_GIME_YELLOW,_GIME_PURPLE,_GIME_BLUE,
	};


void _XL_INIT_GRAPHICS(void)
{
    // uint16_t i;
	// uint16_t j;
	uint8_t color;

	disableInterrupts();

	// High speed (2 Mhz CPU clock)
	POKE(65497,0);

    // Enable MMU
    // POKE(0xFF90,0x40);
    POKE(0xFF90,0x44);

    // Set graphics ON
    POKE(0xFF98,0x80);

    // Set 256x192x16 mode
    POKE(0xFF99,0x1A);

    // Set actual screen ram at $600000
    POKE(0xFF9D,0xC0);
    POKE(0xFF9E,0x00);

    // Set palette   
    for(color=0;color<16;++color)
    {
        POKE(0xFFB0+color,_color_encoding[color]);
    }
    // POKE(0xFFB,0x3F);
    
    // Set MMU windows at $8000
    POKE(0xFFA4,0x30);
    POKE(0xFFA5,0x31);
    POKE(0xFFA6,0x32);

	// color = 0;

    // for(i=0;i<24000/32;++i)
    // {
		
        // POKE(0x8000+i,255);

		// for(j=0;j<700;++j)
		// {
		// }
		// if((i&31)==31)
		// {
			// _XL_WAIT_FOR_INPUT();
			// ++color;
			// color&=7;
		// }
    // }

    // while(1){};
}

