//VERA $1F9C0 - $1F9C3

#include "standard_libs.h"

#ifndef _CX16_SOUNDS
#define _CX16_SOUNDS

void _play(uint16_t freq);

#define _XL_EXPLOSION_SOUND() _play(3500U)

#define _XL_PING_SOUND() _play(2000U)
  
#define _XL_SHOOT_SOUND() _play(6000U)

#define _XL_TICK_SOUND() _play(8000U)

#define _XL_TOCK_SOUND() _play(9000U)

#define _XL_ZAP_SOUND() _play(1200U)

#endif // _CX16_SOUNDS


