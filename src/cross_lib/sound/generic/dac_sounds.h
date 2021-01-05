#ifndef  __DAC_SOUNDS
#define  __DAC_SOUNDS

#if defined(__COCO__) || defined(__DRAGON__)
    #define DAC 0xFF20
    #define SHIFT 2
    #define ROM_ADDR 0x8000
#elif defined(__MO5__)
    #define DAC 0xA7CD
    #define SHIFT 0
    #define ROM_ADDR 0xE000
#elif defined(__TO7__)
    #define DAC 0xE7CD
    #define SHIFT 0
    #define ROM_ADDR 0xF000
#endif

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))


#if defined(__COCO__) || defined(__DRAGON__)
void INIT_SOUND(void)
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
void INIT_SOUND(void)
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
}

#elif defined(__TO7__)
void INIT_SOUND(void)
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
{ \
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
{ \
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

#define EXPLOSION_SOUND() NOISE(60,5)

#define ZAP_SOUND() CLICK(30,100)

#define SHOOT_SOUND() NOISE(30,10)

#define PING_SOUND() CLICK(20,20)

#define TICK_SOUND() CLICK(20,40)

#define TOCK_SOUND() CLICK(20,80)

#endif // __DAC_SOUNDS

