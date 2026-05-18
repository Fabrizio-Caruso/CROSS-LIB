#ifndef _SUPERVISION_SOUNDS
#define _SUPERVISION_SOUNDS

void play_frequency(uint16_t value);

#define _XL_EXPLOSION_SOUND() play_frequency(350)

#define _XL_PING_SOUND() play_frequency(200)

#define _XL_SHOOT_SOUND() play_frequency(600)

#define _XL_TOCK_SOUND() play_frequency(90)

#define _XL_TICK_SOUND() play_frequency(150)

#define _XL_ZAP_SOUND() play_frequency(1200)

#endif // _SUPERVISION_SOUNDS

