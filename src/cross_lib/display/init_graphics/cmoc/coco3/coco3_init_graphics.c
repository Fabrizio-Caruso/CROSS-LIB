
#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))
#define PEEKW(addr)        (*(unsigned*) (addr))

#include "display_macros.h"


void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;

    // Set 256x192x16 mode
    POKE(0xFF99,0x1A);

    // Set graphics ON
    POKE(0xFF98,0x80);

    // Set actual screen ram at $600000
    POKE(0xFF9D,0xC0);
    POKE(0xFF9E,0x00);

    // Set palette
    
    for(i=0;i<16;++i)
    {
        POKE(0xFFB0+i,i);
    }

    // Set MMU windows at $8000
    POKE(0xFFA4,0x30);
    POKE(0xFFA5,0x31);
    POKE(0xFFA6,0x32);
    
    // TODO: Remove this
    

    for(i=0;i<2000;++i)
    {
        POKE(0x8000+i,i&0xFF);
    }

    // while(1){}
    // setScreenColors();
    
    while(1){};
}

