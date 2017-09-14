/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/

#include "character.h"
#include "settings.h"
#include "display_macros.h"
#include "invincible_enemy.h"
#include "input_macros.h"

extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown;
extern unsigned char playerDirection;
extern unsigned char playerFire;
extern Character player;
extern Character missile;
extern unsigned char guns;

#if defined(FULL_GAME)
	extern unsigned char player_invincibility;
	extern unsigned char oddBlink;	
#endif

#define _DO_MOVE_UP \
	DELETE_PLAYER(player._x,player._y,player._imagePtr); \
	--player._y; \
	invincibleYCountDown = INVINCIBLE_COUNT_DOWN; \
	playerDirection = UP; \
	SHOW_UP();
	
#define _DO_MOVE_DOWN \
	DELETE_PLAYER(player._x,player._y,player._imagePtr); \
	++player._y; \
	invincibleYCountDown = INVINCIBLE_COUNT_DOWN; \
	playerDirection = DOWN; \
	SHOW_DOWN(); 
	
#define _DO_MOVE_LEFT \
	DELETE_PLAYER(player._x,player._y,player._imagePtr); \
	--player._x; \
	invincibleXCountDown = INVINCIBLE_COUNT_DOWN; \
	playerDirection = LEFT; \
	SHOW_LEFT();
 
#define _DO_MOVE_RIGHT \
	DELETE_PLAYER(player._x,player._y,player._imagePtr); \
	++player._x; \
	invincibleXCountDown = INVINCIBLE_COUNT_DOWN; \
	playerDirection = RIGHT; \
	SHOW_RIGHT();

#define _DRAW_PLAYER \
	if(player_invincibility) \
	{ \
		DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr); \
	} \
	else \
	{ \
		DRAW_PLAYER(player._x, player._y, player._imagePtr); \
	}
	
#if defined(WAIT_FOR_KEY)
	#if defined(__SPECTRUM__)
		#include <input.h>

		#if defined(SPECTRUM_NATIVE_DIRECTIVES)
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
	#else // C16 or CBM610 or (Neither Commodore nor Atari/AtariXL nor Spectrum)
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
	void WAIT_PRESS(void)
	{
		unsigned char kbInput;
		while(joy_read(JOY_1))
		{
			JOY_UP(kbInput);
		}
		while(!(joy_read(JOY_1)))
		{
		}
	}	
#endif


#if defined(JOYSTICK_CONTROL)
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
		else if(JOY_BTN_1(joyInput) && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		#if defined(FULL_GAME)
			_DRAW_PLAYER	
		#else
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
		#endif
	}	
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
		else if(kbInput==_FIRE && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		#if defined(FULL_GAME)
			_DRAW_PLAYER
		#else
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
		#endif
	}
#endif

#if defined(KEYBOARD_CONTROL)
	#if defined(__SPECTRUM__)
		#if defined(SPECTRUM_NATIVE_DIRECTIVES)	
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_Inkey());}
		#else	
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_inkey());}		
		#endif
	#elif defined(__MSX__)
		#include<msx/gfx.h>
		void MOVE_PLAYER(void) {if(!get_trigger(0)) {movePlayerByKeyboard(get_stick(0));} else movePlayerByKeyboard(9);}	
	#elif defined(__ZX80__) 
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getch());} // TODO: this makes the game turned-based		
	#elif defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__CBM610__)
		void MOVE_PLAYER(void) {if(kbhit()) { movePlayerByKeyboard(cgetc());}}	
	#elif defined(__ATMOS__)
		void MOVE_PLAYER(void) {movePlayerByKeyboard(GET_CHAR());}				
	#else
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getk());}	
	#endif	
#else
	void MOVE_PLAYER(void) { movePlayerByJoystick(joy_read(JOY_1));}
#endif


