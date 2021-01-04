

#ifndef  __ASCII_SOUNDS
#define  __ASCII_SOUNDS

#if defined(__MO5__) || defined(__TO7__)
    #define BEEP_ASCII() PUTCH(7)
#else
    #define BEEP_ASCII() putchar('\a')
#endif

#define EXPLOSION_SOUND() BEEP_ASCII()

#define ZAP_SOUND() BEEP_ASCII()

#define SHOOT_SOUND() BEEP_ASCII()

#define PING_SOUND() BEEP_ASCII()

#define TICK_SOUND() BEEP_ASCII()

#define TOCK_SOUND() BEEP_ASCII()

#endif // __ASCII_SOUNDS

