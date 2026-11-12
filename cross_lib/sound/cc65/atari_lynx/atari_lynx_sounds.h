
#ifndef _ATARI_LYNX_SOUNDS
#define _ATARI_LYNX_SOUNDS

void _play_frequency(uint16_t freq);

#define _XL_EXPLOSION_SOUND() _play_frequency(128+40)

#define _XL_PING_SOUND() _play_frequency(60)
  
#define _XL_SHOOT_SOUND() _play_frequency(128+20)

#define _XL_TICK_SOUND() _play_frequency(45)

#define _XL_TOCK_SOUND() _play_frequency(25)

#define _XL_ZAP_SOUND() _play_frequency(50)


#endif // _ATARI_LYNX_SOUNDS

