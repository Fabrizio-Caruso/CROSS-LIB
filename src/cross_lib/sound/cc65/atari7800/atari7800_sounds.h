
#ifndef _ATARI7800_SOUNDS
#define _ATARI7800_SOUNDS

extern unsigned char sfx_bling[];
extern unsigned char sfx_explosion[];
extern unsigned char sfx_poof1[];
extern unsigned char sfx_poof2[];
extern unsigned char sfx_powerup[];
extern unsigned char sfx_quack[];

extern void __fastcall__ playsfx0(unsigned char *);


#define _XL_EXPLOSION_SOUND() playsfx0(sfx_explosion);


#define _XL_PING_SOUND() playsfx0(sfx_bling);

  
#define _XL_SHOOT_SOUND() playsfx0(sfx_quack);


#define _XL_TICK_SOUND() playsfx0(sfx_poof1);


#define _XL_TOCK_SOUND() playsfx0(sfx_poof1);


#define _XL_ZAP_SOUND() playsfx0(sfx_powerup);



#endif // _ATARI7800_SOUNDS

