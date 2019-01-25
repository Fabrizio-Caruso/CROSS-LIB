

#ifndef _MOVE_PLAYER
#define _MOVE_PLAYER
		
	#if defined(KEYBOARD_CONTROL)
		void movePlayerByKeyboard(unsigned char kbInput);
	#else // All CBM except CBM610 + ATARI + ATARI XL + ATARI 5200
		void movePlayerByJoystick(unsigned char joyInput);
	#endif

	void MOVE_PLAYER(void);
	
#endif // _MOVE_PLAYER

