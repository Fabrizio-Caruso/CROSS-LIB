
#ifndef _MSDOS86_SOUNDS
#define _MSDOS86_SOUNDS

extern void _speaker_beep(uint16_t freq);



#define _XL_EXPLOSION_SOUND() _speaker_beep(1100);


#define _XL_PING_SOUND()      _speaker_beep(1393);

  
#define _XL_SHOOT_SOUND()     _speaker_beep(993);


#define _XL_TICK_SOUND()      _speaker_beep(1293);


#define _XL_TOCK_SOUND()      _speaker_beep(1093);


#define _XL_ZAP_SOUND()       _speaker_beep(1193);



#endif // _MSDOS86_SOUNDS

