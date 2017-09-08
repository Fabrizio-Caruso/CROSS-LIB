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

#if defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__CBM610__) 
#elif defined(__SPECTRUM__) || defined(__ZX81__) || defined(__CPC__) || defined(__MSX__) || defined(__VG5K__) || defined(__SC3000__) || defined(__AQUARIUS__)
#else
	#include <joystick.h>
#endif

#if defined(__SPECTRUM__)
	#include <input.h>
	#if defined(SPECTRUM_NATIVE_DIRECTIVES)
		#include <spectrum.h>
	#endif
#endif

#include "display_macros.h"
#include "invincible_enemy.h"
#include "input_macros.h"


extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown;
extern unsigned char playerDirection;
extern unsigned char playerFire;
extern Character player;
extern Character missile;

extern unsigned char ghostCount;

extern unsigned char guns;

#if defined(FULL_GAME)
	extern unsigned char player_invincibility;
	extern unsigned char player_blink;	
#endif

#if defined(__CBM610__) || defined(__C16__) || (!defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__))
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
		#include <conio.h>
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


#if (defined(__CBM__) && !defined(__CBM610__)) || defined(__ATARI__) || defined(__ATARIXL__)
	void movePlayerByJoystick(unsigned char joyInput)
	{
		if(JOY_UP(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._y;
			invincibleYCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = UP;
			SHOW_UP();
		}
		else if(JOY_DOWN(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._y;
			invincibleYCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = DOWN;
			SHOW_DOWN();
		}
		else if(JOY_LEFT(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._x;
			invincibleXCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = LEFT;
			SHOW_LEFT();
		}
		else if(JOY_RIGHT(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._x;
			invincibleXCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = RIGHT;
			SHOW_RIGHT();
		}
		else if(JOY_BTN_1(joyInput) && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		#if defined(FULL_GAME)
			if(player_invincibility)
			{
				DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr);
			}
			else
			{
				DRAW_PLAYER(player._x, player._y, player._imagePtr);
			}		
		#else
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
		#endif
	}	
#else
	#if defined(__VG5K__) || defined(__AQUARIUS__) || defined(__CBM610__) || defined(__CPC__)
		#define _MOVE_UP 'i'
		#define _MOVE_DOWN 'k'
		#define _MOVE_LEFT 'j'
		#define _MOVE_RIGHT 'l'
		#define _FIRE ' '
	#elif defined(__ATMOS__) || defined(__APPLE2__) || defined(__APPLE2ENH__)
		#define _MOVE_UP 'I'
		#define _MOVE_DOWN 'K'
		#define _MOVE_LEFT 'J'
		#define _MOVE_RIGHT 'L'
		#define _FIRE ' '
	#elif defined(__MSX__)
		#define _MOVE_UP 1
		#define _MOVE_DOWN 5
		#define _MOVE_LEFT 7
		#define _MOVE_RIGHT 3
		#define _FIRE 9
	#else
	#endif
	void movePlayerByKeyboard(unsigned char kbInput)
	{
		if(kbInput==_MOVE_UP)
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._y;
			invincibleYCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = UP;
			SHOW_UP();
		}
		else if(kbInput==_MOVE_DOWN)
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._y;
			invincibleYCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = DOWN;
			SHOW_DOWN();
		}
		else if(kbInput==_MOVE_LEFT)
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._x;
			invincibleXCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = LEFT;
			SHOW_LEFT();
		}
		else if(kbInput==_MOVE_RIGHT)
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._x;
			invincibleXCountDown = INVINCIBLE_COUNT_DOWN;
			playerDirection = RIGHT;
			SHOW_RIGHT();
		}
		else if(kbInput==_FIRE && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		#if defined(FULL_GAME)
			if(player_invincibility)
			{
				DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr);
			}
			else
			{
				DRAW_PLAYER(player._x, player._y, player._imagePtr);
			}		
		#else
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
		#endif
	}
#endif

#if !defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__)
	#if defined(__SPECTRUM__)
		#if defined(SPECTRUM_NATIVE_DIRECTIVES)	
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_Inkey());}
		#else	
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_inkey());}		
		#endif
	#elif defined(__MSX__)
		#include<msx/gfx.h>
		void MOVE_PLAYER(void) {if(!get_trigger(0)) {movePlayerByKeyboard(get_stick(0));} else movePlayerByKeyboard(9);}
	#elif defined(__CPC__) || defined(__SC3000__)
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getk());}		
	#elif defined(__VG5K__) 
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getk());}	
	#elif defined(__AQUARIUS__) 
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getch());} // TODO: this is wrong (turn-based)
	#elif defined(__ZX81__) 
		void MOVE_PLAYER(void) {movePlayerByKeyboard(getk());}		
	#elif defined(__APPLE2__) || defined(__APPLE2ENH__)
		void MOVE_PLAYER(void) {if(kbhit()) { movePlayerByKeyboard(cgetc());}}	
	#else
		// ATARI and ORIC
		void MOVE_PLAYER(void) {movePlayerByKeyboard(GET_CHAR());}
	#endif
#elif defined(__CBM610__)
		void MOVE_PLAYER(void) {if(kbhit()) { movePlayerByKeyboard(cgetc());}}	
#else
	void MOVE_PLAYER(void) { movePlayerByJoystick(joy_read(JOY_1));}
#endif


