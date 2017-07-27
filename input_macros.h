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


	#if !(defined(__CBM__) || defined(__ATARI__) || defined(__ATARIXL__) || defined(__APPLE2__) || defined(__APPLE2ENH__)) 
	#else
		#include <joystick.h>
	#endif

	#if defined(__SPECTRUM__)
		#include "patch/z88dk_conio_implementation.h"
	#else
		#include <conio.h>
	#endif 
	
	#if defined(__SPECTRUM__) 
		#if !defined(SPECTRUM_NATIVE_DIRECTIVES)
			#include <arch/zx.h>
			void in_wait_key(void);
			void in_wait_nokey(void); 
		#else
			void in_WaitForKey(void);
			void in_WaitForNoKey(void);
		#endif
	#endif
	
	#if !defined(__SPECTRUM__)
		#define IF_KEYBOARD_HIT if(kbhit()) 
	#endif

	#if defined(__ATMOS__)
		#include "atmos/atmos_input.h"
	// #elif defined(__APPLE2__) || (__APPLE2ENH__)
		// #include "apple2/apple2_input.h"
	#elif defined(__ATARI__) || defined(__ATARIXL__)
		#include "atari/atari_input.h"
	#elif defined(__SPECTRUM__)
		#include <input.h>
		#if defined(SPECTRUM_NATIVE_DIRECTIVES)
			#include <spectrum.h>
			#define GET_CHAR() {in_Inkey();};
			//{getk();};
		#else
			#define GET_CHAR() {in_inkey();};
		#endif
	#elif defined(__CPC__) || defined(__MSX__) || defined(__SC3000__) || defined(__M5__)
		#define GET_CHAR() (unsigned int) getk();
	#else
		#ifndef GET_CHAR()
			#define GET_CHAR() cgetc();
		#endif
	#endif

	#define WAIT_JOY1_PRESS() \
	{ \
		while(joy_read(JOY_1)) \
		{ \
			JOY_BTN_UP(kbInput); \
		} \
		while(!(joy_read(JOY_1))) \
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

	#if defined(__SPECTRUM__)
		#include <input.h>

		#if defined(SPECTRUM_NATIVE_DIRECTIVES)
			#define WAIT_KEY_PRESS() \
			{ \
				in_WaitForKey(); \
				in_Inkey(); \
				in_WaitForNoKey(); \
			}
		#else
			#define WAIT_KEY_PRESS() \
			{ \
				in_wait_key(); \
				in_wait_nokey(); \
			};
		#endif
	
	#else
		#define WAIT_KEY_PRESS() \
		{ \
			while(kbhit()) \
				cgetc(); \
			while(!kbhit()) \
			{ \
			}; \
			cgetc(); \
		};
	#endif
	
	#if (!defined(__CBM__) || defined(__PET__) || defined(__CBM610__) || defined(__C16__) || defined(__PLUS4__)) && !defined(__ATARI__) && !defined(__ATARIXL__)
		#define WAIT_PRESS() WAIT_KEY_PRESS();
	#else
		#define WAIT_PRESS() WAIT_JOY1_PRESS();
	#endif

	#if (!defined(__CBM__) || defined(__CBM610__)) && !defined(__ATARI__) && !defined(__ATARIXL__)
	#else
		#define JOY_INSTALL() { joy_install(joy_static_stddrv); };

		#define GET_JOY1() joy_read (JOY_1);

		#define GET_JOY2() joy_read (JOY_2);
	#endif
#endif // _INPUT_MACROS