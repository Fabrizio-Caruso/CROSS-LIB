#include "comx_sounds.h"
#include "standard_libs.h"

void _ping_sound(uint8_t freq)
{
    #if !defined(__CIDELSA__)
    uint8_t i;
    
    generatetone(80,freq,7);
    
    for(i=0;i<199;++i){};
    
    generatetone(0,0,0);
    #endif
}

void _explosion_sound(uint8_t range)
{
    #if !defined(__CIDELSA__)

    uint8_t i;
    
    generatenoise(range,7);
    
    for(i=0;i<199;++i){};
    
    generatenoise(0,0);
    #endif
}

void ZAP_SOUND(void)
{
    #if !defined(__CIDELSA__)
    uint16_t i;
    
    for(i=0;i<299;++i)
    {
        generatetone(i/2,i,7);
    }
    generatetone(0,0,0);
    #endif
}

