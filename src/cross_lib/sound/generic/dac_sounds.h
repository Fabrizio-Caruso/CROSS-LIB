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


#define _XL_EXPLOSION_SOUND() NOISE(60,5)

#define _XL_ZAP_SOUND() CLICK(10,65)

#define _XL_SHOOT_SOUND() NOISE(30,10)

#define _XL_PING_SOUND() CLICK(5,15)

#define _XL_TICK_SOUND() CLICK(6,30)

#define _XL_TOCK_SOUND() CLICK(7,50)

#endif // __DAC_SOUNDS

