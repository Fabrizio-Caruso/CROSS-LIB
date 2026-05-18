
#include "cross_lib.h"
#include "ti99_sounds.h"

#include <sound.h>


// period must be in the range of 1 (highest pitch) to 1023 (lowest pitch)
void CLICK(uint16_t duration, uint16_t period)
{
    volatile uint16_t i;
   
    SOUND=TONE1_VOL;
    SOUND=TONE1_FREQ|(period&0xf);
    SOUND=(period>>4)&0xff;
   
    for(i=0;i<duration;++i){};
   
    SOUND=TONE1_VOL|0x0F;
}


// For NOISE_MODE, you OR in the type of noise you want, from 0-7
void NOISE(uint16_t duration, uint8_t mode) 
{
    volatile uint16_t i;
    volatile uint8_t j;
    
    SOUND=NOISE_VOL&0xF0;
    SOUND=NOISE_MODE|mode;
    
    for(i=0;i<duration;++i){};
    
    for(i=0;i<16;++i)
    {
        SOUND=NOISE_VOL|i;
        for(j=0;j<40;++j){};
    }
}


void _XL_ZAP_SOUND(void)
{
    uint16_t i;
    
    for(i=1;i<29;++i)
    {
        CLICK(100-(i<<1),400+(i<<4));
    }
    SOUND=TONE1_VOL|0x0F;
}




