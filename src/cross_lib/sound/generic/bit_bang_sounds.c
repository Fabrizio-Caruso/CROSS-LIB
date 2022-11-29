
#include "cross_lib.h"
#include "bit_bang_sounds.h"

// #if defined(__APPLE2__) || defined(__APPLE2ENH__)
    // #define _click() \
        // __asm__("LDA $C030")
// #else

#if defined(__APPLE2__) || defined(__APPLE2ENH__)
    void _click(void)
    {
        uint8_t k;
        k = PEEK(BIT_BANG_ADDR);
    }
#elif defined(__MC10__)
    void _click(void)
    {
        POKE(0xBFFF,0x80);
        POKE(0xBFFF,0x00);
    }
#endif


void _XL_ZAP_SOUND(void)
{
    uint16_t i;
    uint8_t j;
    
    for(i=1;i<6;++i)
    {
        CLICK(50-i,i);
        for(j=0;j<5;++j){};
    }
}


void CLICK(uint16_t duration, uint8_t period) 
{
    uint16_t i;
    uint8_t j;
    
    for(i=0;i<duration;++i)
    {
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
            _click();
            for(j=0;j<period;++j){};
        }
    }

}

