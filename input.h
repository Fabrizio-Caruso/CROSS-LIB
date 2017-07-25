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
//#include "input_macros.h"

#if (defined (__CBM__) && !defined(__CBM610__)) || defined(__ATARI__) || defined(__ATARIXL__)
	#define INIT_INPUT() unsigned char kbInput; JOY_INSTALL(); 
#else
	#define INIT_INPUT() unsigned char kbInput;
#endif


// Move player
#if (defined (__CBM__) && !defined(__CBM610__)) || defined(__NES__) || defined(__ATARI__) || defined(__ATARIXL__)  //|| defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__ATMOS__)
	#define MOVE_PLAYER() { kbInput = GET_JOY1(); movePlayerByJoystick(kbInput);}
	void movePlayerByJoystick(unsigned char joyInput);
#elif defined(__SPECTRUM__)
	#include <input.h>
	#include <spectrum.h>
	unsigned int in_inkey(void);
	#define MOVE_PLAYER() { kbInput = in_Inkey(); movePlayerByKeyboard(kbInput);}
	void movePlayerByKeyboard(char kbInput);
#else
	#define MOVE_PLAYER() IF_KEYBOARD_HIT { kbInput = GET_CHAR(); movePlayerByKeyboard(kbInput);}
	void movePlayerByKeyboard(char kbInput);
#endif


#endif // _INPUT
