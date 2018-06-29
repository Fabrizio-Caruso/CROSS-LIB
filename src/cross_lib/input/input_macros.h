/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 

#ifndef _INPUT_MACROS
#define _INPUT_MACROS
	#include "input_target_settings.h"
		
	#if defined(Z88DK)
		#define TURN_BASED_INPUT() getch()
	#elif defined(__NCURSES__)
		#define TURN_BASED_INPUT() getchar()
	#else
		#define TURN_BASED_INPUT() cgetc()	
	#endif	
	
	#if defined(KEYBOARD_CONTROL)
		unsigned char GET_CHAR(void);
	#endif			
				
	#if !defined(NO_WAIT) || !defined(NO_SLEEP)
		void WAIT_PRESS(void);
	#else // NO_WAIT + NO_SLEEP
		#define WAIT_PRESS()
	#endif
#endif // _INPUT_MACROS

