
#include "cross_lib.h"
#include "bit_bang_sounds.h"


void CLICK(uint8_t duration, uint8_t period) 
{
    uint8_t i;
    uint16_t j;
    
    for(i=0;i<duration;++i)
    {
        POKE(BIT_BANG_ADDR,0);
        for(j=0;j<period;++j){};
    }
}

void NOISE(uint8_t duration, uint16_t period) 
{
    uint8_t i;
    uint16_t j;
    
    for(i=0;i<duration;++i)
    {

        if(PEEK(ROM_ADDR+i)&1)
        {
            POKE(BIT_BANG_ADDR,0);
        }

        for(j=0;j<period;++j){};
    }

}

