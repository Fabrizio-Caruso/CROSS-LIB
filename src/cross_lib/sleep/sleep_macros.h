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
	#define ALT_SLEEP_SCALE 4000UL
	
	#if !defined(NO_SLEEP)
		#  if !defined(__CMOC__)
			#include <time.h>
			#include <unistd.h>
			#include <stdlib.h>		
		#else
			#include <coco.h>
			#include <cmoc.h>
		#endif	
	#endif

	#  if defined(NO_SLEEP)
		#define SLEEP(s)
	#elif defined(MACRO_SLEEP)
		#define SLEEP(s)  do {unsigned short ii; for(ii=0;ii<ALT_SLEEP_SCALE*sec; ++ii){ii=ii;};} while(0)
	#elif defined(ALT_SLEEP)
		void SLEEP(unsigned char s);
	#else
		#define SLEEP(s) sleep(s)
	#endif

#endif // _SLEEP_MACROS

