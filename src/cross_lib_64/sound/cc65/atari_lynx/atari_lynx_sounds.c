#include "cross_lib.h"

#include "atari_lynx_sounds.h"

// #include "cross_lib.h"

extern void chibisound(unsigned char sound);

void _play_frequency(uint16_t freq)
{
    uint16_t i;
    
    chibisound(127);
    
    chibisound(freq);

    for(i=0;i<1000;++i){};
    
    chibisound(0);
}

