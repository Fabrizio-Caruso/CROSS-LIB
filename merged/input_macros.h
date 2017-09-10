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

#ifndef _INPUT_MACROS
#define _INPUT_MACROS

	#if defined(__SPECTRUM__)
		#include "patch/z88dk_conio_implementation.h"
	#else
		#include <conio.h>
	#endif 
	
	#if defined(__ATMOS__)
		#include "atmos/atmos_input.h"
	#endif
				
	void WAIT_PRESS(void);
	
	#if defined(__CBM610__) || (!defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__) && !defined(__ATARI5200__))
		void movePlayerByKeyboard(unsigned char kbInput);
		#define INIT_INPUT()
	#else // All CBM except CBM610 + ATARI + ATARI XL
		#include <joystick.h>
		void movePlayerByJoystick(unsigned char joyInput);
		#define INIT_INPUT() { joy_install(joy_static_stddrv); };		
	#endif

	void MOVE_PLAYER(void);
#endif // _INPUT_MACROS