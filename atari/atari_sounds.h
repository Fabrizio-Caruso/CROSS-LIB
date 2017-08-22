
#ifndef _ATARI_SOUNDS
#define _ATARI_SOUNDS

	#include<atari.h>

	void _ping_sound(unsigned char freq);
	#define PING_SOUND() _ping_sound(0xF0);	
	#define TOCK_SOUND() _ping_sound(0x08);
	#define TICK_SOUND() _ping_sound(0x60);
	
	void _explosion_sound(unsigned char freq);
	#define SHOOT_SOUND() _explosion_sound(0xF0);
	#define EXPLOSION_SOUND() _explosion_sound(0x80);
	
	void ZAP_SOUND(void);	

#endif // _ATARI_SOUNDS