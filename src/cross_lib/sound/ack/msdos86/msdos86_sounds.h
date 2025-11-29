
#ifndef _MSDOS86_SOUNDS
#define _MSDOS86_SOUNDS

extern void _speaker_beep(uint16_t freq, uint16_t length);



#define _XL_EXPLOSION_SOUND() _speaker_beep(1100, 0x3FFF)


#define _XL_PING_SOUND()      _speaker_beep(1393, 0x1FFF)

  
#define _XL_SHOOT_SOUND()     _speaker_beep( 993, 0x2FFF)


#define _XL_TICK_SOUND()      _speaker_beep(1293, 0x1FFF)


#define _XL_TOCK_SOUND()      _speaker_beep(1093, 0x1FFF)


#define _XL_ZAP_SOUND()       _speaker_beep(1193, 0x3FFF)



#endif // _MSDOS86_SOUNDS

