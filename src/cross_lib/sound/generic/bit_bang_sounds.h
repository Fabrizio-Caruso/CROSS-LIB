#ifndef  __BIT_BANG_SOUNDS
#define  __BIT_BANG_SOUNDS

#if defined(__APPLE2__) || defined(__APPLE2ENH__)
    #define BIT_BANG_ADDR 0xC30
    #define ROM_ADDR 0xD000
#endif

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))

void _XL_INIT_SOUND(void);

void CLICK(uint8_t duration, uint8_t period);

void NOISE(uint8_t duration, uint16_t period);


#define _XL_EXPLOSION_SOUND() NOISE(30,5)

#define _XL_ZAP_SOUND() CLICK(15,100)

#define _XL_SHOOT_SOUND() NOISE(15,10)

#define _XL_PING_SOUND() CLICK(10,20)

#define _XL_TICK_SOUND() CLICK(10,40)

#define _XL_TOCK_SOUND() CLICK(10,80)

#endif // __BIT_BANG_SOUNDS

