#ifndef  __BIT_BANG_SOUNDS
#define  __BIT_BANG_SOUNDS

#if defined(__APPLE2__) || defined(__APPLE2ENH__)
    #define BIT_BANG_ADDR 0xC030
    #define ROM_ADDR 0xD000
#endif

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))

//void _XL_INIT_SOUND(void);

void CLICK(uint16_t duration, uint8_t period);

void NOISE(uint16_t duration, uint8_t period);

void _XL_ZAP_SOUND(void);

#define _XL_EXPLOSION_SOUND() NOISE(75,9)

// #define _XL_ZAP_SOUND() CLICK(150,2)

#define _XL_SHOOT_SOUND() NOISE(100,5)

#define _XL_PING_SOUND() CLICK(55,5)

#define _XL_TICK_SOUND() CLICK(45,4)

#define _XL_TOCK_SOUND() CLICK(35,3)

#endif // __BIT_BANG_SOUNDS

