#ifndef  _SOUNDS_MACROS
#define _SOUNDS_MACROS
	#if defined(__ATMOS__)
		#include "atmos/atmos_sounds.h"
	#else
		#define EXPLOSION_SOUND() {};
		#define PING_SOUND() {};
		#define SHOOT_SOUND() {};
		#define TICK_SOUND() {};
		#define TOCK_SOUND() {};
		#define ZAP_SOUND() {};
	#endif
#endif _SOUNDS_MACROS