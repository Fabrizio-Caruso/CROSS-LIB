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

	#include <conio.h>
	#if !(defined(__CBM__) || defined(__ATARIXL__) || defined(__APPLE2__) || defined(__APPLE2ENH__)) 
	#else
		#include <joystick.h>
	#endif

	#if !(defined(__CBM__) || defined(__ATMOS__) || defined(__ATARIXL__) || defined(__APPLE2__) || defined(__APPLE2ENH__))
		#include "patch/z88dk_conio_patch.h"
	#endif 
	
	#define IF_KEYBOARD_HIT if(kbhit()) 
		
	#if defined(__ATMOS__)
		#include "atmos/atmos_input.h"
	#elif defined(__APPLE2__) || (__APPLE2ENH__)
		#include "apple2/apple2_input.h"
	#elif defined(__ATARI__) || defined(__ATARIXL__)
		#include "atari/atari_input.h"
	#else
		#define GET_CHAR() cgetc();
	#endif

	#define WAIT_JOY1_PRESS() \
	{ \
		while(kbInput = joy_read(JOY_1)) \
		{ \
			JOY_BTN_UP(kbInput); \
		} \
		while(!(kbInput = joy_read(JOY_1))) \
		{ \
		} \
	}	
	

	#define WAIT_JOY2_PRESS() \
	{ \
		while(kbInput = joy_read(JOY_2)) \
		{ \
			JOY_BTN_UP(kbInput); \
		} \
		while(!(kbInput = joy_read(JOY_2))) \
		{ \
		} \
	}	
	
	#define WAIT_KEY_PRESS() \
	{ \
		while(kbhit()) \
			cgetc(); \
		while(!kbhit()) \
		{ \
		}; \
		cgetc(); \
	};

	#if defined (__CBM610__) || defined(__PLUS4__) || defined(__C16__)
		#define WAIT_PRESS() WAIT_KEY_PRESS();
	#elif defined (__CBM__) || defined(__NES__)
		#define WAIT_PRESS() WAIT_JOY1_PRESS();
	#elif defined(__ATARI__) || defined(__ATARIXL__) 
		#define WAIT_PRESS() WAIT_JOY1_PRESS();
	#else
		#define WAIT_PRESS() WAIT_KEY_PRESS();
	#endif

	#if defined (__CBM610__)
	#else
		#define JOY_INSTALL() { joy_install(joy_static_stddrv); };

		#define GET_JOY1() joy_read (JOY_1);

		#define GET_JOY2() joy_read (JOY_2);
	#endif
#endif // _INPUT_MACROS