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
	#include "settings.h"
	
	#if defined(__ATMOS__)
		#include "atmos/atmos_input.h"
	#elif defined(__WINCMOC__) || defined(__CMOC__)
		#include "wincmoc/wincmoc_input.h"
	#else
	#endif
				
	void WAIT_PRESS(void);
	
	#if defined(KEYBOARD_CONTROL)
		void movePlayerByKeyboard(unsigned char kbInput);
		#if defined(__ATMOS__)
		// Remove keyboard click sound
			#define INIT_INPUT() { POKE(0x26A,PEEK(0x26A) | 8); };
		#elif defined(__MSX__)
			#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))		
			#define INIT_INPUT() { POKE(0xF3DB,0); };	
		#elif defined(__VIC20__)
			#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))		
			#define INIT_INPUT() { POKE(0x028A ,0xFF); };				
		#else
			#define INIT_INPUT()
		#endif
	#else // All CBM except CBM610 + ATARI + ATARI XL + ATARI 5200
		#include <joystick.h>
		void movePlayerByJoystick(unsigned char joyInput);
		#define INIT_INPUT() { joy_install(joy_static_stddrv); };		
	#endif

	void MOVE_PLAYER(void);
#endif // _INPUT_MACROS