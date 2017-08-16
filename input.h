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

#if (defined (__CBM__) && !defined(__CBM610__)) || defined(__ATARI__) || defined(__ATARIXL__)
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
#if !defined(__CBM__) && !defined(__ATARI__) && !defined(__ATARIXL__) && !defined(__SPECTRUM__) && !defined(__CPC__) && !defined(__VG5k__)
	void movePlayerByKeyboard(unsigned char kbInput);
#else
	void movePlayerByJoystick(unsigned char joyInput);
#endif

void MOVE_PLAYER(void);

// #elif defined(__SPECTRUM__)
	// #include <input.h>
	// #if defined(SPECTRUM_NATIVE_DIRECTIVES)	
		// #include <spectrum.h>
		// void movePlayerByKeyboard(char kbInput);		
		// unsigned int in_Inkey(void);
		// void MOVE_PLAYER() {movePlayerByKeyboard(in_Inkey());}
	// #else
		// #include<input.h>
		// void movePlayerByKeyboard(char kbInput);
		// unsigned int in_inkey(void);		
		// void MOVE_PLAYER() {movePlayerByKeyboard(in_inkey());}		
	// #endif
// #else
	// void movePlayerByKeyboard(char kbInput);
	// void MOVE_PLAYER() {if(kbhit()) { movePlayerByKeyboard(cgetc());}}
// #endif


#endif // _INPUT
