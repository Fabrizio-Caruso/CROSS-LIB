
#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned int*) (addr) = (val))

#define SCREEN_BASE 0xA0000000UL
#define TEXT_BASE   0xB8000000UL

#include <stdlib.h>

extern void init_vga(void);



void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    
    init_vga();

    video_mode();
    
    cls();

    // for(i=0;i<2000;++i)
    // {
        // POKE(TEXT_BASE+i,i);
        // POKEW(TEXT_BASE+i,i);  
    // }
    
    // write_tile(0,0,10);
    
    while(1){};
}
