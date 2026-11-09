
#include "cross_lib.h"
#include "dac_sounds.h"

#if defined(__COCO__) || defined(__DRAGON__)
void _XL_INIT_SOUND(void)
{
    asm
    {
        LDA $FF01
        ANDA #$F7
        STA $FF01
        LDA $FF03
        ANDA #$F7
        STA $FF03
        LDA $FF23
        ORA #8
        STA $FF23
    }
}
#elif defined(__COCO3__)
void _XL_INIT_SOUND(void)
{
    asm
    {
        LDA $FF01
        ANDA #$F7
        STA $FF01
        LDA $FF03
        ANDA #$F7
        STA $FF03
        LDA $FF23
        ORA #8
        STA $FF23
    }
}
#elif defined(__MO5__)
void _XL_INIT_SOUND(void)
{
    asm 
    {
        orcc #$50
        lda $a7c0
        anda #$fb
        sta $a7c0
        lda $a7cf
        anda #$fb
        sta $a7cf
        lda #$3f
        sta $a7cd
        lda $a7cf
        ora #$04
        sta $a7cf
        andcc #$af
    }
    // TODO: Remove this HACK to reset border color by fixing the Assembly code above
    // #if defined(__BACKGROUND_COLOR) && __BACKGROUND_COLOR==1
        // PUTCH(0x1B);PUTCH(103);
    // #else
        // PUTCH(0x1B);PUTCH(96);
    // #endif
}

#elif defined(__TO7__)
void _XL_INIT_SOUND(void)
{
    asm 
    {
        orcc #$50
        lda $e7c0
        anda #$fb
        sta $e7c0
        lda $e7cf
        anda #$fb
        sta $e7cf
        lda #$3f
        sta $e7cd
        lda $e7cf
        ora #$04
        sta $e7cf
        andcc #$af
    }
}
#endif



void CLICK(uint8_t duration, uint8_t period) 
{
    uint8_t i;
    uint16_t j;
    
    for(i=0;i<duration;++i)
    {
        POKE(DAC,63<<SHIFT);
        for(j=0;j<period;++j){};
        POKE(DAC,0); \
        for(j=0;j<period;++j){};
    }
}

void NOISE(uint8_t duration, uint16_t period) 
{
    uint8_t i;
    uint16_t j;
    
    for(i=0;i<duration;++i)
    {
        POKE(DAC,(63)<<SHIFT);
        for(j=0;j<period;++j){};
        POKE(DAC,PEEK(ROM_ADDR+i)); \
        for(j=0;j<period;++j){};
    }
    POKE(DAC,0);
}

