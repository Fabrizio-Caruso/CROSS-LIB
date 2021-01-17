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

void _XL_INIT_SOUND(void);

void CLICK(uint8_t duration, uint8_t period);

void NOISE(uint8_t duration, uint16_t period);


#define EXPLOSION_SOUND() NOISE(60,5)

#define ZAP_SOUND() CLICK(30,100)

#define SHOOT_SOUND() NOISE(30,10)

#define PING_SOUND() CLICK(20,20)

#define TICK_SOUND() CLICK(20,40)

#define TOCK_SOUND() CLICK(20,80)

#endif // __DAC_SOUNDS

