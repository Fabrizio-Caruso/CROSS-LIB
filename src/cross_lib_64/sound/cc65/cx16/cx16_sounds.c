
#include "cx16_sounds.h"
#include <cx16.h>

#include "standard_libs.h"

// VERA $1F9C0 - $1F9C3

#define _FREQ_LO_REG  0x1F9C0
#define _FREQ_HI_REG  0x1F9C1
#define _VOLUME_REG   0x1F9C2
#define _WAVE_REG     0x1F9C3

void _play(uint16_t value)
{
    uint16_t i;
    
    vpoke(0xFF, _VOLUME_REG);
    vpoke(value>>8, _FREQ_HI_REG);
    vpoke(((value&0xFF00)>>8), _FREQ_LO_REG);
    vpoke(0b00011111,_WAVE_REG);
    for(i=0;i<5000;++i){};
    // while(1){};
    vpoke(0x00, _VOLUME_REG);
}

