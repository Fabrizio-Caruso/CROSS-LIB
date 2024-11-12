#include "bbc.h"

#include "bbc.h"


extern bbc_sound ping_snd;
extern bbc_sound tick_snd;
extern bbc_sound tock_snd;
extern bbc_sound explosion_snd;
extern bbc_sound shoot_snd;



#define _XL_EXPLOSION_SOUND() OSSOUND(&explosion_snd)

#define _XL_PING_SOUND() OSSOUND(&ping_snd)

#define _XL_SHOOT_SOUND() OSSOUND(&shoot_snd)

#define _XL_TICK_SOUND() OSSOUND(&tick_snd)

#define _XL_TOCK_SOUND() OSSOUND(&tock_snd)

#define _XL_ZAP_SOUND() OSSOUND(&ping_snd)
