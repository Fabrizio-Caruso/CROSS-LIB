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
#elif defined(__SPECTRUM__) || defined(__CPC__) || defined(__MSX__) || defined(__VG5K__)
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

extern unsigned char player_invincibility;

extern unsigned char guns;

#if !defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__) && !defined(__SPECTRUM__) && !defined(__CPC__) && !defined(__VG5k__)
	void movePlayerByKeyboard(unsigned char kbInput)
	{
		if(kbInput=='W')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._y;
			invincibleYCountDown = computeInvincibleCountDown();
			playerDirection = UP;
			SHOW_UP();
		}
		else if(kbInput=='S')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._y;
			invincibleYCountDown = computeInvincibleCountDown();
			playerDirection = DOWN;
			SHOW_DOWN();
		}
		else if(kbInput=='A')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._x;
			invincibleXCountDown = computeInvincibleCountDown();
			playerDirection = LEFT;
			SHOW_LEFT();
		}
		else if(kbInput=='D')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._x;
			invincibleXCountDown = computeInvincibleCountDown();
			playerDirection = RIGHT;
			SHOW_RIGHT();
		}
		else if(kbInput==' ' && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		if(player_invincibility)
		{
			DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr);
		}
		else
		{
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
		}
	}
#elif defined(__CBM610__) || defined (__SPECTRUM__) || defined(__CPC__) || defined(__VG5K__)
	void movePlayerByKeyboard(unsigned char kbInput)
	{
		if(kbInput=='w')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._y;
			invincibleYCountDown = computeInvincibleCountDown();
			playerDirection = UP;
			SHOW_UP();
		}
		else if(kbInput=='s')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._y;
			invincibleYCountDown = computeInvincibleCountDown();
			playerDirection = DOWN;
			SHOW_DOWN();
		}
		else if(kbInput=='a')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._x;
			invincibleXCountDown = computeInvincibleCountDown();
			playerDirection = LEFT;
			SHOW_LEFT();
		}
		else if(kbInput=='d')
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._x;
			invincibleXCountDown = computeInvincibleCountDown();
			playerDirection = RIGHT;
			SHOW_RIGHT();
		}
		else if(kbInput==' ' && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		if(player_invincibility)
		{
			DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr);
		}
		else
		{
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
		}
	}
#else
	void movePlayerByJoystick(unsigned char joyInput)
	{
		if(JOY_BTN_UP(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._y;
			invincibleYCountDown = computeInvincibleCountDown();
			playerDirection = UP;
			SHOW_UP();
		}
		else if(JOY_BTN_DOWN(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._y;
			invincibleYCountDown = computeInvincibleCountDown();
			playerDirection = DOWN;
			SHOW_DOWN();
		}
		else if(JOY_BTN_LEFT(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			--player._x;
			invincibleXCountDown = computeInvincibleCountDown();
			playerDirection = LEFT;
			SHOW_LEFT();
		}
		else if(JOY_BTN_RIGHT(joyInput))
		{
			DELETE_PLAYER(player._x,player._y,player._imagePtr);
			++player._x;
			invincibleXCountDown = computeInvincibleCountDown();
			playerDirection = RIGHT;
			SHOW_RIGHT();
		}
		else if(JOY_BTN_FIRE(joyInput) && guns>0 && !missile._status)
		{
			playerFire = 1;
		}
		if(player_invincibility)
		{
			DRAW_BLINKING_PLAYER(player._x, player._y, player._imagePtr);
		}
		else
		{
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
		}
	}	
#endif

#if !defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__) && !defined(__CPC__) && !defined(__VG5k__)
	#if defined(__SPECTRUM__)
		#if defined(SPECTRUM_NATIVE_DIRECTIVES)	
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_Inkey());}
		#else	
			void MOVE_PLAYER(void) {movePlayerByKeyboard(in_inkey());}		
		#endif
	#else
		void MOVE_PLAYER(void) {if(kbhit()) { movePlayerByKeyboard(cgetc());}}
	#endif
#elif defined(__CBM610__)
	void MOVE_PLAYER(void) {if(kbhit()) { movePlayerByKeyboard(cgetc());}}
#else
	void MOVE_PLAYER(void) { movePlayerByJoystick(joy_read(JOY_1));}
#endif


