#ifndef  __TI99_SOUNDS
#define  __TI99_SOUNDS

#include <sound.h>

void _XL_ZAP_SOUND();

void NOISE(uint16_t duration, uint8_t mode);

void CLICK(uint16_t duration, uint16_t period);

#define _XL_EXPLOSION_SOUND() NOISE(1000,4)

#define _XL_SHOOT_SOUND() NOISE(200,5)

#define _XL_PING_SOUND() CLICK(550,900)

#define _XL_TICK_SOUND() CLICK(450,700)

#define _XL_TOCK_SOUND() CLICK(350,300)

#endif // __TI99_SOUNDS

