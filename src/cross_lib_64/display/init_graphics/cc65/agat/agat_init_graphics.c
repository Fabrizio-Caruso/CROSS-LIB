// STA $C133 ; map 3-rd physical memory page to 3-rd CPU memory segment
// LDA #$12  ; load red:green to A
// STA $6000 ; store to 3-rd memory segment
// LDA #$34  ; load yellow:blue to A
// STA $6001 ; store to 3-rd memory segment
// STA $C731 ; switch display controller to 3-rd memory page in 128x128 mode.

#include<peekpoke.h>
#include "standard_libs.h"

void _XL_INIT_GRAPHICS(void)
{
    // uint16_t i;
    // uint16_t j;
    
    POKE(0xC133,0);
    POKE(0xC731,0);

    // for(i=0;i<8192;i+=2)
    // {
        // POKE(0x6000+i,0x12);
        // POKE(0x6001+i,0x34);
        // for(j=0;j<300;++j)
        // {
        // };
    // }
    
    
    // while(1){};
}


