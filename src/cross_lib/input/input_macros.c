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

#include "../cross_lib.h"

#include "../sleep/sleep_macros.h"

#include "input_macros.h"

#if defined(NO_WAIT) && !defined(NO_SLEEP)
	void WAIT_PRESS(void)
	{
		sleep(2);
	}
#elif defined(NO_WAIT)
//
#elif defined(WAIT_FOR_KEY)
	#  if defined(__NCURSES__)
		#include <ncurses.h>
		
		void WAIT_PRESS(void)
		{
			#if !defined(TURN_BASED)
				while(getch()==ERR)
				{}
			#else
				getch();
			#endif
		}
	#elif defined(__SPECTRUM__)
		#include <input.h>

		#if defined(CLIB_ANSI)
			void WAIT_PRESS(void) 
			{ 
				in_WaitForKey();
				in_Inkey();
				in_WaitForNoKey();
			}
		#else
			void WAIT_PRESS(void)
			{ 
				in_wait_key();
				in_wait_nokey();
			}
		#endif
	#elif defined(__CMOC__) && !defined(__WINCMOC__)	
		#include <cmoc.h>
		
		void WAIT_PRESS(void)
		{
			waitkey(0);
		}	
	#else 
		#if defined(CONIO_LIB)
			#include<conio.h>
		#endif
		
		void WAIT_PRESS(void)
		{
			while(kbhit())
				cgetc();
			while(!kbhit())
			{ 
			}; 
			cgetc();
		}
	#endif	
#else
	#include<joystick.h>

	void WAIT_PRESS(void)
	{
		while ((joy_read(JOY_1) & JOY_BTN_1_MASK))
		{
		}
		while (! (joy_read(JOY_1) & JOY_BTN_1_MASK))
		{
		}
	}	
#endif

