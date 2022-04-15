#ifndef  __TI99_SOUNDS
#define  __TI99_SOUNDS

#include <sound.h>

void _XL_ZAP_SOUND();

void NOISE(uint16_t duration, uint8_t period);

void CLICK(uint16_t duration, uint8_t period);

#define _XL_EXPLOSION_SOUND() NOISE(1000,5)

#define _XL_SHOOT_SOUND() NOISE(400,6)

#define _XL_PING_SOUND() CLICK(550,5)

#define _XL_TICK_SOUND() CLICK(450,4)

#define _XL_TOCK_SOUND() CLICK(350,3)

#endif // __TI99_SOUNDS

