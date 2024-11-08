#include "bbc.h"

#include "bbc.h"

// typedef struct {
  // int channel;
  // int amplitude;
  // int pitch;
  // int duration;
// } bbc_sound;

extern bbc_sound ping_snd;
extern bbc_sound tick_snd;
extern bbc_sound tock_snd;
// extern bbc_sound snd;
// extern bbc_sound snd;



#define _XL_EXPLOSION_SOUND() OSSOUND(&tock_snd)

#define _XL_PING_SOUND() OSSOUND(&ping_snd)

#define _XL_SHOOT_SOUND() OSSOUND(&tock_snd)

#define _XL_TICK_SOUND() OSSOUND(&tick_snd)

#define _XL_TOCK_SOUND() OSSOUND(&tock_snd)

#define _XL_ZAP_SOUND() OSSOUND(&ping_snd)
