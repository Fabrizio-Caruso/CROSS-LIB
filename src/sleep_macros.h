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

#ifndef _SLEEP_MACROS
#define _SLEEP_MACROS

	#if defined(NO_SLEEP)
		#define sleep(sec)
	#elif defined(__APPLE2__) || defined(__APPLE2ENH__) || defined(__CREATIVISION__) || defined(__SUPERVISION__) || defined(__OSIC1P__)
		void sleep(unsigned int sec);
	// #elif  defined(__MC1000__) || defined(__OSIC1P__) || defined(__GAL__) || defined(__Z9001__) || defined(__PC6001__) || defined(__P2000__) ||defined(__NASCOM__) || defined(__BEE__) || defined(__TI8X__) || defined(__TI82__) || defined(__TI83__) || defined(__TI85__) || defined(__TI86__) || defined(__TI86S__) || defined(__MZ__) || defined(__SVI__) ||defined(__TRS80__) || defined(__M5__) || defined(__MTX__) || defined(__VZ__) || defined(__SC3000__) || defined(__MSX__) || defined(__CPC__) || defined(__VG5K__) || defined(__AQUARIUS__)
		// void sleep(unsigned int sec);
	// TODO: This is a temporary fix
	#elif defined(Z88DK)	
		void sleep(unsigned int sec);
	#else	
		#include <time.h>
		#include <unistd.h>
	#endif	
#endif // _SLEEP_MACROS

