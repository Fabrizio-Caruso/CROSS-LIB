
#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned int*) (addr) = (val))

#define SCREEN_BASE 0xA0000000

#include <stdlib.h>

extern void init_vga(void);

void _XL_INIT_GRAPHICS(void)
{
    uint16_t i;
    
    init_vga();

    for(i=0;i<1000;++i)
    {
        POKEW(SCREEN_BASE+i,0xFFFF);  
    }
    
    while(1){};
}
