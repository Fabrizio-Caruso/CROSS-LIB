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

	#include "character.h"

	
	#if defined(__CBM610__) || !(defined(__CBM__) || defined(__ATARI__) || defined(__ATARIXL__) || defined(__APPLE2__) || defined(__APPLE2ENH__)) 
	#else
		#include <joystick.h>
	#endif

	#if defined(__SPECTRUM__)
		#include "patch/z88dk_conio_implementation.h"
	#else
		#include <conio.h>
	#endif 
	
	#if defined(__SPECTRUM__) 
		#include <input.h>
		#if !defined(SPECTRUM_NATIVE_DIRECTIVES)
			// #include <arch/zx.h>
			extern void in_wait_key(void) __preserves_regs(b,c,d,e,h,l);
			extern void in_wait_nokey(void) __preserves_regs(b,c,d,e,h,l);
		#else
			extern void in_WaitForKey(void);
			extern void in_WaitForNoKey(void);
		#endif
	#endif
	
	#if defined(__ATMOS__)
		#include "atmos/atmos_input.h"
	#elif defined(__ATARI__) || defined(__ATARIXL__)
		#include "atari/atari_input.h"
	// #elif defined(__SPECTRUM__)
		// #include <input.h>
		// #if defined(SPECTRUM_NATIVE_DIRECTIVES)
			// #include <spectrum.h>
			// #define GET_CHAR() {in_Inkey();};
		// #else
			// #define GET_CHAR() {in_inkey();};
		// #endif
	// #elif defined(__CPC__) || defined(__ZX81__) || defined(__MSX__) || defined(__SC3000__) || defined(__M5__) || defined(__VG5K__) || defined(__AQUARIUS__)
		// #define GET_CHAR() (unsigned int) getk();
	// #else
		// #ifndef GET_CHAR()
			// #define GET_CHAR() cgetc();
		// #endif
	#endif


	#if (defined (__CBM__) && !defined(__CBM610__)) || defined(__ATARI__) || defined(__ATARIXL__)
		#define JOY_INSTALL() { joy_install(joy_static_stddrv); };
		#define GET_JOY1() joy_read (JOY_1);
		#define INIT_INPUT() JOY_INSTALL(); 
	#else
		#define INIT_INPUT()
	#endif

	#if defined(__SPECTRUM__)
		#include<input.h>
		#if defined(SPECTRUM_NATIVE_DIRECTIVES)
			extern uint __LIB__ in_Inkey(void);
		#else
			extern int in_inkey(void);		
		#endif
	#endif
				
	// Move player
	#if !defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__)
		void movePlayerByKeyboard(unsigned char kbInput);
		void WAIT_KEY_PRESS(void);
		#define WAIT_PRESS() WAIT_KEY_PRESS();
	#elif defined(__CBM610__)
		void movePlayerByKeyboard(unsigned char kbInput);
		void WAIT_KEY_PRESS(void);
		#define WAIT_PRESS() WAIT_KEY_PRESS();
	#else // All CBM except CBM610 + ATARI + ATARI XL
		void movePlayerByJoystick(unsigned char joyInput);
		#if defined(__C16__)
			void WAIT_KEY_PRESS(void);
			#define WAIT_PRESS() WAIT_KEY_PRESS();		
		#else
			void WAIT_JOY1_PRESS(void);
			#define WAIT_PRESS() WAIT_JOY1_PRESS();
		#endif
	#endif

void MOVE_PLAYER(void);
#endif // _INPUT_MACROS