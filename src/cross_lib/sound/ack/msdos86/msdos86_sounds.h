
#ifndef _MSDOS86_SOUNDS
#define _MSDOS86_SOUNDS



extern void _speaker_beep(uint16_t freq, uint16_t length);


#if defined(__NOISE)

    #define _speaker_noise(length) \
        do \
        { \
            uint16_t i; \
            \
            for(i=0;i<length;++i) \
            { \
                _speaker_beep(*(unsigned char *)(0XF0000+i),0x00FF); \
            } \
        } while(0)

    #define _XL_EXPLOSION_SOUND() _speaker_noise(0x01FF)

    #define _XL_SHOOT_SOUND()     _speaker_noise(0x007F)

#else
    #define _XL_EXPLOSION_SOUND() _speaker_beep(893, 0x3FFF)
    #define _XL_SHOOT_SOUND() _speaker_beep(993, 0x1FFF)
#endif




#define _XL_PING_SOUND()      _speaker_beep(1393, 0x1FFF)

  


#define _XL_TICK_SOUND()      _speaker_beep(1293, 0x1FFF)


#define _XL_TOCK_SOUND()      _speaker_beep(1093, 0x1FFF)


#define _XL_ZAP_SOUND()       _speaker_beep(1193, 0x3FFF)



#endif // _MSDOS86_SOUNDS

