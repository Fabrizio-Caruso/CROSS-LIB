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
 
#ifndef _INPUT
#define _INPUT

#include "character.h"


#if defined (__PLUS4__) || defined(__C16__) || defined(__C64__) || defined(__VIC20__) || defined(__NES__)
// Do nothing
#else 
	#define INIT_INPUT() unsigned char kbInput;
#endif

#if defined (__ATMOS__) || defined(__NES__) 
// Do nothing
#else 
	#define INIT_INPUT() unsigned char joyInput; JOY_INSTALL();
#endif

// Move player
#if defined (__PLUS4__) || defined(__C16__) || defined(__C64__) || defined(__VIC20__) || defined(__NES__)
	// No keyboard support yet for PLUS4, C16, C64, VIC20
	// Do nothing
#else 
	#define MOVE_PLAYER() IF_KEYBOARD_HIT { kbInput = GET_CHAR(); movePlayerByKeyboard(&player, kbInput);}
#endif // defined (__PLUS4__) || defined(__C64__) || defined(__VIC20__)
#if defined(__ATMOS__) || defined(__NES__)
	// No joystick support yet for ATMOS and NES (NES has NO implemented INPUT!)
	// Do nothing here
#else
	#define MOVE_PLAYER() { joyInput = GET_JOY1(); movePlayerByJoystick(&player, joyInput);}
#endif

void movePlayerByKeyboard(Character *playerPtr, char kbInput);

void movePlayerByJoystick(Character *playerPtr, unsigned char joyInput);

#endif // _INPUT