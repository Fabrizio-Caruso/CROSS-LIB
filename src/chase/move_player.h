

#ifndef _MOVE_PLAYER
#define _MOVE_PLAYER
		
	#if defined(KEYBOARD_CONTROL)
		void movePlayerByKeyboard(unsigned char kbInput);
		#if defined(__ATMOS__)
			#include <peekpoke.h>
		// Remove keyboard click sound
			#define INIT_INPUT() { POKE(0x26A,PEEK(0x26A) | 8); };
		#elif defined(__MSX__)
			#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))		
			#define INIT_INPUT() { POKE(0xF3DB,0); };
		#elif defined(__VIC20__)
			#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))		
			#define INIT_INPUT() { POKE(0x028A ,0xFF); };				
		#else
			#define INIT_INPUT()
		#endif
	#else // All CBM except CBM610 + ATARI + ATARI XL + ATARI 5200
		#include <joystick.h>
		#if defined(__SUPERVISION__)
			#include <supervision.h>
		#endif
		void movePlayerByJoystick(unsigned char joyInput);
		#define INIT_INPUT() { joy_install(joy_static_stddrv); };		
	#endif

	void MOVE_PLAYER(void);
	
#endif // _MOVE_PLAYER

