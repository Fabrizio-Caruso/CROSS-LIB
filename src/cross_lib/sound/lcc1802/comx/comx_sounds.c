#include "comx_sounds.h"
#include "standard_libs.h"

void _ping_sound(uint8_t freq)
{
    uint8_t i;
    
    generatetone(80,freq,7);
    
    for(i=0;i<199;++i){};
    
    generatetone(0,0,0);
}

void _explosion_sound(uint8_t range)
{
    uint8_t i;
    
    generatenoise(range,7);
    
    for(i=0;i<199;++i){};
    
    generatenoise(0,0);
}

void _XL_ZAP_SOUND(void)
{
    uint16_t i;
    
    for(i=0;i<299;++i)
    {
        generatetone(i/2,i,7);
    }
    generatetone(0,0,0);
}

