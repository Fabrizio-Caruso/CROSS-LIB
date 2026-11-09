

#ifndef  __ASCII_SOUNDS
#define  __ASCII_SOUNDS

#if defined(__MO5__) || defined(__TO7__)
    #define BEEP_ASCII() PUTCH(7)
#else
    #define BEEP_ASCII() putchar('\a')
#endif

#define _XL_EXPLOSION_SOUND() BEEP_ASCII()

#define _XL_ZAP_SOUND() BEEP_ASCII()

#define _XL_SHOOT_SOUND() BEEP_ASCII()

#define _XL_PING_SOUND() BEEP_ASCII()

#define _XL_TICK_SOUND() BEEP_ASCII()

#define _XL_TOCK_SOUND() BEEP_ASCII()

#endif // __ASCII_SOUNDS

