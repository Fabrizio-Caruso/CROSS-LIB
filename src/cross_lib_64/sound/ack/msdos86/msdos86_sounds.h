
#ifndef _MSDOS86_SOUNDS
#define _MSDOS86_SOUNDS



extern void _speaker_beep(uint16_t freq, uint16_t length);

#define __NOISE 

#if defined(__NOISE)

    #define _speaker_noise(length) \
        do \
        { \
            uint16_t i; \
            \
            for(i=0;i<length;++i) \
            { \
                _speaker_beep((rand()+0x0B00)&0x0FFF,0x007F); \
            } \
        } while(0)

    #define _XL_EXPLOSION_SOUND() _speaker_noise(0x03FF)

    #define _XL_SHOOT_SOUND()     _speaker_noise(0x017F)

#else
    #define _XL_EXPLOSION_SOUND() _speaker_beep(893, 0x3FFF)
    #define _XL_SHOOT_SOUND() _speaker_beep(993, 0x1FFF)
#endif




#define _XL_PING_SOUND()      _speaker_beep(1393, 0x1FFF)

  


#define _XL_TICK_SOUND()      _speaker_beep(1293, 0x1FFF)


#define _XL_TOCK_SOUND()      _speaker_beep(1093, 0x1FFF)


#define _XL_ZAP_SOUND()       _speaker_beep(1193, 0x3FFF)



#endif // _MSDOS86_SOUNDS

