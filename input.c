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
#elif !(defined(__CBM__) || defined(__ATARIXL__) || defined(__APPLE2__) || defined(__APPLE2ENH__)) 
#else
	#include <joystick.h>
#endif
#include "display_macros.h"
#include "invincible_enemy.h"

#include "input.h"


//#define _TRAINER 1

extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown;
extern unsigned short playerDirection;
extern unsigned short playerFire;
extern unsigned short level;
extern Character player;
extern Character missile;

extern unsigned short ghostCount;

extern unsigned char player_invincibility;

extern unsigned char guns;

#if defined(__CBM610__) || defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__)
void movePlayerByKeyboard(char kbInput)
{
	if(/*(kbInput=='W') ||*/ (kbInput=='W'))
	{
		DELETE_PLAYER(player._x,player._y,player._imagePtr);
		--player._y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = UP;
		SHOW_UP();
	}
	else if(/*(kbInput=='S') ||*/ (kbInput=='S'))
	{
		DELETE_PLAYER(player._x,player._y,player._imagePtr);
		++player._y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = DOWN;
		SHOW_DOWN();
	}
	else if(/*(kbInput=='A') ||*/ (kbInput=='A'))
	{
		DELETE_PLAYER(player._x,player._y,player._imagePtr);
		--player._x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = LEFT;
		SHOW_LEFT();
	}
	else if(/*(kbInput=='D') ||*/ (kbInput=='D'))
	{
		DELETE_PLAYER(player._x,player._y,player._imagePtr);
		++player._x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = RIGHT;
		SHOW_RIGHT();
	}
	#ifdef _TRAINER
		else if(/*(kbInput=='Z') || */ (kbInput=='Z'))
		{
			ghostCount = 0;
		}
	#endif // TRAINER
	else 	
	if(kbInput==' ' && guns>0 && !missile._status)
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


