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

#include "sleep_macros.h"

#include "cross_lib.h" 

#include "standard_libs.h"

#if !defined(_XL_NO_SLEEP)

	#if defined(__OSIC1P__) || defined(__ZX81__)
		#define CYCLES 500
	#elif defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__VIC20__)
        #define CYCLES 2000
    #elif defined(__COCO__) || defined(__DRAGON__)
        #define CYCLES 3500
    #elif defined(__SUPERVISION__)
        #define CYCLES 6000
    #elif defined(__TI99__)
        #define CYCLES 17000
    #else
		#define CYCLES 1000
	#endif

    #if defined(ALT_SLEEP)
	void _XL_SLEEP(uint8_t sec)
	{
        #if defined(__TI99__)
        volatile uint16_t ii;
        #else
		uint16_t ii;
        #endif
        
        // Flush the video buffer to make sure that the latest character is displayed
        #if defined(__COMX__) || defined(__PECOM__) || defined(__TMC600__) || defined(__MICRO__) || defined(__CIDELSA__)
            VIDFLUSH();
        #endif
		for(ii=0;ii<sec*CYCLES;++ii){}; 
	}
    #endif
#endif

#if !defined(_XL_NO_SLEEP)
	#if defined(__NCURSES__) 
		#include <stdint.h>
		#include <time.h>
		#include <unistd.h>
		void _XL_SLOW_DOWN(uint16_t t)
		{
			usleep((t)*800);
		}
		
	#elif defined(__TI99__)
		void _XL_SLOW_DOWN(uint16_t t)
		{
			volatile uint16_t i;

			for(i=0;i<(t);++i)
			{
			}
		}
	#elif defined(__CMOC__) // TODO: There should be a less hackish way
		void _XL_SLOW_DOWN(uint16_t t)
		{
			uint16_t i;
			uint16_t foo;

			for(i=0;i<(t);++i)
			{
				foo=42+i;
			}
		}
	#else
		void _XL_SLOW_DOWN(uint16_t t)
		{
			uint16_t i;

			for(i=0;i<(t);++i)
			{
			}       
		}
	#endif
#endif
