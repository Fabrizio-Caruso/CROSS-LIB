
#include "cross_lib.h"
#include "ti99_sounds.h"

#include <sound.h>

void CLICK(uint16_t duration, uint8_t period) 
{
    volatile uint16_t i;
    
    SOUND=TONE1_VOL;
    SOUND=TONE1_FREQ|period;
    
    for(i=0;i<duration;++i){};
    
    SOUND=TONE1_VOL|0x0F;
}


void NOISE(uint16_t duration, uint8_t period) 
{
    volatile uint16_t i;
    volatile uint8_t j;
    
    SOUND=NOISE_VOL&0xF0;
    SOUND=NOISE_MODE|period;
    
    for(i=0;i<duration;++i){};
    
    for(i=0;i<16;++i)
    {
        SOUND=NOISE_VOL|i;
        for(j=0;j<40;++j){};
    }
}


void _XL_ZAP_SOUND(void)
{
    uint8_t i;
    
    for(i=1;i<90;++i)
    {
        CLICK(250-i,i);
    }
}




