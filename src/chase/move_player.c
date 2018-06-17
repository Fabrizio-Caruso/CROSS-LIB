
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
	#include <joystick.h>
	// #include <supervision.h>
	
	#if defined(NO_INPUT)
		void movePlayerByJoystick(unsigned char joyInput)
		{
			sleep(1);
		}		
	#else
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
	#if defined(NO_INPUT)
		void movePlayerByKeyboard(unsigned char joyInput)
		{
			sleep(1);
		}	
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
#endif

	
#if defined(KEYBOARD_CONTROL)
	#if defined(NO_INPUT)
		void MOVE_PLAYER(void) {}
	#elif defined(TURN_BASED)
		void MOVE_PLAYER(void) {movePlayerByKeyboard(TURN_BASED_INPUT);} 
	#elif defined(__NCURSES__)
		#define INPUT_LOOPS 40
		void MOVE_PLAYER(void) 
		{ 
			unsigned long delay = 0;
			char _ch;
			char ch;

			while(delay<INPUT_LOOPS)
			{	
				_ch = getch();
				if(_ch!=ERR)
				{
					ch = _ch;
				}
				++delay;
			}
			movePlayerByKeyboard(ch); 
		}  
	#elif defined(__SPECTRUM__)
		#if defined(CLIB_ANSI)
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_Inkey());}
		#else	
			#include <input.h>
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_inkey());}		
		#endif
	#elif defined(__MSX__)
		#include<msx/gfx.h>
		void MOVE_PLAYER(void) {if(!get_trigger(0)) {movePlayerByKeyboard(get_stick(0));} else movePlayerByKeyboard(9);}	
	#elif defined(__ZX80__) 
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getch());} // TODO: this makes the game turned-based		
	#elif (defined(__VIC20__) && defined(FORCE_KEYBOARD))||defined(__SUPERVISION__) || defined(__CREATIVISION__) \
		  || defined(__OSIC1P__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__CBM610__) \
		  || defined(__C16__)
		void MOVE_PLAYER(void) 
		{
			if(kbhit()) 
			{ 
				movePlayerByKeyboard(cgetc());
			}
			
		}	
	#elif defined(__ATMOS__)
		void MOVE_PLAYER(void) 
		{
			movePlayerByKeyboard(GET_CHAR());
		}		
	#elif defined(__WINCMOC__) && defined(__CMOC__)
		#if defined(ASM_KEY_DETECT)
			// #include <basic.h>
			#include "wincmoc/wincmoc_input.h"			
			void MOVE_PLAYER(void) 
				{
					char ch = (char) GET_CHAR(); 
					if(ch!='')
					{
						movePlayerByKeyboard(ch); 
					}
					else
					{
						if(kbhit() && cgetc()==' ')
						{
							movePlayerByKeyboard(' ');
						}
					}
				}
		#else
			void MOVE_PLAYER(void) 
			{
				if(kbhit()) 
				{ 
					movePlayerByKeyboard((char) cgetc());
				}			
			}				
		#endif
	#elif !defined(__WINCMOC__) && defined(__CMOC__)
		#if defined(ASM_KEY_DETECT)
			// #include <basic.h>
			#include "../cross_lib/cmoc/cmoc_input.h"
			
			#include <coco.h>
			void MOVE_PLAYER(void) 
				{
					char ch = (char) GET_CHAR(); 
					if(ch!='')
					{
						movePlayerByKeyboard(ch); 
					}
				}
		#else
			void MOVE_PLAYER(void) 
			{
				if(kbhit()) 
				{ 
					movePlayerByKeyboard((char) cgetc());
				}			
			}				
		#endif	
	#elif defined(__SMS__)
		// TODO: To implement
		void MOVE_PLAYER(void) 
		{
			movePlayerByKeyboard(' ');	
		}	
	#elif defined(__TRS80__) || defined(__EG2K__)
		void MOVE_PLAYER(void) 
		{
			// if(kbhit())
			// {
				movePlayerByKeyboard(GET_CHAR());
			// }
		}			
	#elif defined(__C128_Z80__)
		void MOVE_PLAYER(void) 
		{
			movePlayerByKeyboard(getk());
		}		
	#else
		void MOVE_PLAYER(void) 
		{
			movePlayerByKeyboard(getk());
		}	
	#endif	
#else
	#if defined(__ATARI__) || defined(__ATARIXL__)
	#include <peekpoke.h>
	void MOVE_PLAYER(void) { movePlayerByJoystick(joy_read(JOY_1));	POKE(77,0);}
	#else
	void MOVE_PLAYER(void) { movePlayerByJoystick(joy_read(JOY_1));}
	#endif
#endif


