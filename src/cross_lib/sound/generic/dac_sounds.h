#ifndef  __DAC_SOUNDS
#define  __DAC_SOUNDS

#if defined(__COCO__) || defined(__DRAGON__)
    #define DAC 0xFF20
    #define SHIFT 2
#elif defined(__MO5__) || defined(__TO7__)
    #define DAC 0xE7CD
    #define SHIFT 0
#endif

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

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
        LDA   <$C0          ; PIA systeme
        ANDA  #$FB          ; clear mute bit
        STA   <$C0          ; modification PIA
        LDA   <$CF          ; lecture registre de ctrl B
        ANDA  #$FB          ; raz bit 2
        STA   <$CF          ; selection DDRB
        LDB   #$3F          ; set bits 0-5
        STB   <$A7          ; bits CNA en sortie
        ORA   #$04          ; set b2
        STA   <$CF          ; selection PB
    }
}

#elif defined(__TO7__)
void INIT_SOUND(void)
    asm 
    { 
        LDA   <$C0          ; PIA systeme 
        ANDA  #$FB          ; clear mute bit
        STA   <$C0          ; modification PIA 
        LDA   <$CF          ; lecture registre de ctrl B 
        ANDA  #$FB          ; raz bit 2 
        STA   <$CF          ; selection DDRB 
        LDB   #$3F          ; set bits 0-5 
        STB   <$E7          ; bits CNA en sortie 
        ORA   #$04          ; set b2 
        STA   <$CF          ; selection PB         
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
        POKE(DAC,i&31); \
    }
    POKE(DAC,0);
}

#define EXPLOSION_SOUND() NOISE(80,1600)

#define ZAP_SOUND() CLICK(30,100)

#define SHOOT_SOUND() NOISE(10,800)

#define PING_SOUND() CLICK(20,20)

#define TICK_SOUND() CLICK(20,40)

#define TOCK_SOUND() CLICK(20,80)

#endif // __DAC_SOUNDS

