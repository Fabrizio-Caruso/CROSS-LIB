
#include "settings.h"

#include "move_player.h"

#include "character.h"
#include "skull.h"

#include "definitions.h"


extern Character player;

#if !defined(TINY_GAME)
	extern unsigned char skullXCountDown;
	extern unsigned char skullYCountDown;
	extern unsigned char playerFire;
	extern Character bullet;
	extern unsigned char guns;
	extern unsigned char playerDirection;
#endif

#if defined(FULL_GAME)
	extern unsigned char player_invincibility;
	extern unsigned char playerBlink;	
#endif

#if !defined(TINY_GAME) 
	#if !defined(ALT_MOVE)
		#define _DO_MOVE_UP \
			deletePlayer(&player); \
			--player._y; \
			SHOW_UP(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = UP;
		#define _DO_MOVE_DOWN \
			deletePlayer(&player); \
			++player._y; \
			SHOW_DOWN(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = DOWN;
		#define _DO_MOVE_LEFT \
			deletePlayer(&player); \
			--player._x; \
			SHOW_LEFT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = LEFT;
		#define _DO_MOVE_RIGHT \
			deletePlayer(&player); \
			++player._x; \
			SHOW_RIGHT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = RIGHT;
	#else
		#define _DO_MOVE_UP \
			--player._y; \
			SHOW_UP(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = UP;
		#define _DO_MOVE_DOWN \
			++player._y; \
			SHOW_DOWN(); \
			skullYCountDown = SKULL_COUNT_DOWN; \
			playerDirection = DOWN;	
		#define _DO_MOVE_LEFT \
			--player._x; \
			SHOW_LEFT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = LEFT;
		 
		#define _DO_MOVE_RIGHT \
			++player._x; \
			SHOW_RIGHT(); \
			skullXCountDown = SKULL_COUNT_DOWN; \
			playerDirection = RIGHT;
	#endif
#else
	#if !defined(ALT_MOVE)
		#define _DO_MOVE_UP \
			deletePlayer(&player); \
			--player._y; \
			SHOW_UP();
			
		#define _DO_MOVE_DOWN \
			deletePlayer(&player); \
			++player._y; \
			SHOW_DOWN();
			
		#define _DO_MOVE_LEFT \
			deletePlayer(&player); \
			--player._x; \
			SHOW_LEFT();
			
		#define _DO_MOVE_RIGHT \
			deletePlayer(&player); \
			++player._x; \
			SHOW_RIGHT();
	#else
		#define _DO_MOVE_UP \
			--player._y; \
			SHOW_UP();
			
		#define _DO_MOVE_DOWN \
			++player._y; \
			SHOW_DOWN();
			
		#define _DO_MOVE_LEFT \
			--player._x; \
			SHOW_LEFT();
			
		#define _DO_MOVE_RIGHT \
			++player._x; \
			SHOW_RIGHT();		
	#endif
#endif



#if defined(JOYSTICK_CONTROL)
	#if defined(Z88DK_JOYSTICK)
		#include <games.h>
		
		void movePlayerByJoystick(unsigned char joyInput)
		{
			if(joyInput & MOVE_UP)
			{
				_DO_MOVE_UP
			}
			else if(joyInput & MOVE_DOWN)
			{
				_DO_MOVE_DOWN
			}
			else if(joyInput & MOVE_LEFT)
			{
				_DO_MOVE_LEFT
			}
			else if(joyInput & MOVE_RIGHT)
			{
				_DO_MOVE_RIGHT
			}
			#if !defined(TINY_GAME)
			else if(joyInput & MOVE_FIRE && guns>0 && !bullet._status)
			{
				playerFire = 1;
			}
			#endif
		}	
	#else
		#include <joystick.h>
		
		void movePlayerByJoystick(unsigned char joyInput)
		{
			if(JOY_UP(joyInput))
			{
				_DO_MOVE_UP
			}
			else if(JOY_DOWN(joyInput))
			{
				_DO_MOVE_DOWN
			}
			else if(JOY_LEFT(joyInput))
			{
				_DO_MOVE_LEFT
			}
			else if(JOY_RIGHT(joyInput))
			{
				_DO_MOVE_RIGHT
			}
			#if !defined(TINY_GAME)
			else if(JOY_BTN_1(joyInput) && guns>0 && !bullet._status)
			{
				playerFire = 1;
			}
			#endif
		}	
	#endif
#else
	void movePlayerByKeyboard(unsigned char kbInput)
	{
		#if defined(ALT_MOVE)
			deletePlayer(&player);
		#endif
		if(kbInput==_MOVE_UP)
		{
			_DO_MOVE_UP
		}
		else if(kbInput==_MOVE_DOWN)
		{
			_DO_MOVE_DOWN
		}
		else if(kbInput==_MOVE_LEFT)
		{
			_DO_MOVE_LEFT
		}
		else if(kbInput==_MOVE_RIGHT)
		{
			_DO_MOVE_RIGHT
		}
		#if !defined(TINY_GAME)
		else if(kbInput==_FIRE && guns>0 && !bullet._status)
		{
			playerFire = 1;
		}
		#endif
	}
#endif

	
#if defined(NO_INPUT)
	void MOVE_PLAYER(void) {}
#elif defined(KEYBOARD_CONTROL)
	void MOVE_PLAYER(void) 
	{ 
		#if defined(ALT_MOVE)
			if(kbhit())
			{
		#endif
				movePlayerByKeyboard(GET_CHAR()); 
		#if defined(ALT_MOVE)
			}
		#endif
	}
#else
	#if defined(__ATARI__) || defined(__ATARIXL__)
		#include <peekpoke.h>
		#define INPUT_POST_PROC() POKE(0x4D,0)
	#elif defined(__ATARI5200__)
		#include <peekpoke.h>
		#define INPUT_POST_PROC() POKE(0x04,0)	
	#else
		#define INPUT_POST_PROC()
	#endif
	#if defined(Z88DK_JOYSTICK)
		extern unsigned char stick;
		
		void MOVE_PLAYER(void) { movePlayerByJoystick(joystick(stick));}
	#else
		void MOVE_PLAYER(void) { movePlayerByJoystick(joy_read(JOY_1));}
	#endif
#endif


