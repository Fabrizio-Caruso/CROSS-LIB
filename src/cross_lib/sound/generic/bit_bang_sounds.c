
#include "cross_lib.h"
#include "bit_bang_sounds.h"

// #if defined(__APPLE2__) || defined(__APPLE2ENH__)
    // #define _click() \
        // __asm__("LDA $C030")
// #else
#define _click() \
do \
{ \
   uint8_t k; \
   \
   k = PEEK(BIT_BANG_ADDR); \
} while(0)
// #endif


void CLICK(uint16_t duration, uint8_t period) 
{
    uint16_t i;
    uint8_t j;
    
    for(i=0;i<duration;++i)
    {
        // k = PEEK(BIT_BANG_ADDR);
        // __asm__("LDA $C030");
        // (void) PEEK(BIT_BANG_ADDR);
        _click();
        for(j=0;j<period;++j){};
    }
}


void NOISE(uint16_t duration, uint8_t period) 
{
    uint16_t i;
    uint8_t j;
    
    for(i=0;i<duration;++i)
    {

        if(rand()&1)
        {
            // k = PEEK(BIT_BANG_ADDR);
            // __asm__("LDA $C030");
            // (void) PEEK(BIT_BANG_ADDR);
            _click();
            for(j=0;j<period;++j){};
        }
    }

}

