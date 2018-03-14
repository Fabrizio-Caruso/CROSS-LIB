/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 


#include "character.h"
#include "settings.h"
#include "display_macros.h"
#include "invincible_enemy.h"
#include "input_macros.h"


extern Character player;

#if !defined(TINY_GAME)
	extern unsigned char invincibleXCountDown;
	extern unsigned char invincibleYCountDown;
	extern unsigned char playerFire;
	extern Character missile;
	extern unsigned char guns;
	extern unsigned char playerDirection;
#endif

#if defined(FULL_GAME)
	extern unsigned char player_invincibility;
	extern unsigned char playerBlink;	
#endif

#if !defined(TINY_GAME) 
	#define _DO_MOVE_UP \
		deletePlayer(&player); \
		--player._y; \
		invincibleYCountDown = SKULL_COUNT_DOWN; \
		playerDirection = UP; \
		SHOW_UP();
		
	#define _DO_MOVE_DOWN \
		deletePlayer(&player); \
		++player._y; \
		invincibleYCountDown = SKULL_COUNT_DOWN; \
		playerDirection = DOWN; \
		SHOW_DOWN(); 
		
	#define _DO_MOVE_LEFT \
		deletePlayer(&player); \
		--player._x; \
		invincibleXCountDown = SKULL_COUNT_DOWN; \
		playerDirection = LEFT; \
		SHOW_LEFT();
	 
	#define _DO_MOVE_RIGHT \
		deletePlayer(&player); \
		++player._x; \
		invincibleXCountDown = SKULL_COUNT_DOWN; \
		playerDirection = RIGHT; \
		SHOW_RIGHT();
#else
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

#endif
	
#if defined(WAIT_FOR_KEY)
	#if defined(__SPECTRUM__)
		#include <input.h>

		#if defined(CLIB_ANSI)
			void WAIT_PRESS(void) 
			{ 
				in_WaitForKey();
				in_Inkey();
				in_WaitForNoKey();
			}
		#else
			void WAIT_PRESS(void)
			{ 
				in_wait_key();
				in_wait_nokey();
			}
		#endif
	// TO DO: REMOVE THIS, once the bug in kbhit is fixed
	#elif defined(__OSIC1P__) || defined(__NASCOM__) \
		  || defined(__Z1013__) || defined(__SMS__) || defined(__GCC__)
		#include "sleep_macros.h"		
		
		void WAIT_PRESS(void)
		{
			// Workaround for CC65 bug 
			#if !defined(NO_SLEEP)
				sleep(2);
			#endif
		}
	#elif defined(__CMOC__) && !defined(__WINCMOC__)
		// TODO: Implement this
		#include "sleep_macros.h"		
		#include <cmoc.h>
		
		void WAIT_PRESS(void)
		{
			waitkey(0);
		}	
	#elif defined(__ENTERPRISE__)
		void WAIT_PRESS(void)
		{
			getch();
		}
	#else // C16 or CBM610 or (Neither Commodore nor Atari/AtariXL nor Spectrum)
		#if defined(CONIO_LIB)
			#include<conio.h>
		#endif
		void WAIT_PRESS(void)
		{
			while(kbhit())
				cgetc();
			while(!kbhit())
			{ 
			}; 
			cgetc();
		}
	#endif	
#else
	#include<joystick.h>
	#if defined(NO_INPUT)
	//defined(__ATARI_LYNX__) || defined(__SUPERVISION__)
		void WAIT_PRESS(void)
		{
		}
	#else
	void WAIT_PRESS(void)
	{
		while ((joy_read(JOY_1) & JOY_BTN_1_MASK))
		{
		}
		while (! (joy_read(JOY_1) & JOY_BTN_1_MASK))
		{
		}
	}	
	#endif
#endif


#if defined(JOYSTICK_CONTROL)
	#include <joystick.h>
	// #include <supervision.h>
	
	#if defined(NO_INPUT)
		void movePlayerByJoystick(unsigned char joyInput)
		{
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
			else if(JOY_BTN_1(joyInput) && guns>0 && !missile._status)
			{
				playerFire = 1;
			}
			#endif
		}	
	#endif
#else
	void movePlayerByKeyboard(unsigned char kbInput)
	{
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
		else if(kbInput==_FIRE && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		#endif
	}
#endif

#if defined(KEYBOARD_CONTROL)
	#if defined(__SPECTRUM__)
		#if defined(CLIB_ANSI)
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_Inkey());}
		#else	
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_inkey());}		
		#endif
	#elif defined(__MSX__)
		#include<msx/gfx.h>
		void MOVE_PLAYER(void) {if(!get_trigger(0)) {movePlayerByKeyboard(get_stick(0));} else movePlayerByKeyboard(9);}	
	#elif defined(__ZX80__) 
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getch());} // TODO: this makes the game turned-based		
	#elif (defined(__VIC20__) && defined(TINY_GAME))||defined(__SUPERVISION__) || defined(__CREATIVISION__) \
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
			#include "cmoc/cmoc_input.h"
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
	#else
		void MOVE_PLAYER(void) 
		{
			movePlayerByKeyboard(getk());	
		}	
	#endif	
#else
	void MOVE_PLAYER(void) { movePlayerByJoystick(joy_read(JOY_1));}
#endif

