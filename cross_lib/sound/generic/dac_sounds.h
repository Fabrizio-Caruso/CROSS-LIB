#ifndef  __DAC_SOUNDS
#define  __DAC_SOUNDS

#if defined(__COCO__) || defined(__DRAGON__)
    #define DAC 0xFF20
    #define SHIFT 2
    #define ROM_ADDR 0x8000
	
	#define CYCLES_FACTOR 1
#elif defined(__COCO3__)
    #define DAC 0xFF20
    #define SHIFT 2
    #define ROM_ADDR 0x8000
	
	#define CYCLES_FACTOR 1
#elif defined(__MO5__)
    #define DAC 0xA7CD
    #define SHIFT 0
    #define ROM_ADDR 0xE000
	
	#define CYCLES_FACTOR 1
#elif defined(__TO7__)
    #define DAC 0xE7CD
    #define SHIFT 0
    #define ROM_ADDR 0xF000
	
	#define CYCLES_FACTOR 1
#endif

#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))
#define PEEK(addr)         (*(uint8_t*) (addr))

void _XL_INIT_SOUND(void);

void CLICK(uint8_t duration, uint8_t period);

void NOISE(uint8_t duration, uint16_t period);


#define _XL_EXPLOSION_SOUND() NOISE(18*CYCLES_FACTOR,5)

#define _XL_ZAP_SOUND() CLICK(15*CYCLES_FACTOR,65)

#define _XL_SHOOT_SOUND() NOISE(9*CYCLES_FACTOR,10)

#define _XL_PING_SOUND() CLICK(1*CYCLES_FACTOR,15)

#define _XL_TICK_SOUND() CLICK(1*CYCLES_FACTOR,30)

#define _XL_TOCK_SOUND() CLICK(1*CYCLES_FACTOR,50)

#endif // __DAC_SOUNDS

