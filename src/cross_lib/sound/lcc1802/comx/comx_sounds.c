#include "comx_sounds.h"
#include "standard_libs.h"

void _ping_sound(uint8_t freq)
{
    uint8_t i;
    uint8_t j;
    
    generatetone(freq,80,7);
    
    for(j=7;j>0;--j)
    {
        for(i=0;i<90;++i){};
        generatetone(freq,80,j);
    }
    
    generatetone(0,0,0);
}


void _explosion_sound(uint8_t range, uint8_t duration)
{
    uint16_t i;
    uint8_t j;
    
    generatenoise(range,7);
    
    for(j=7;j>0;--j)
    {
        for(i=0;i<duration;++i)
        {
        }
        generatenoise(range,j);
    }
    
    generatenoise(0,0);
}


void _XL_ZAP_SOUND(void)
{
    uint8_t i;
    
    for(i=0;i<40;++i)
    {
        generatetone(i/2,i,7);
    }
    generatetone(0,0,0);
}

