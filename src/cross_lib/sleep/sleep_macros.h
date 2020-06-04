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

    #include <standard_libs.h>
    #define ALT_SLEEP_SCALE 4000
    
    #if !defined(NO_SLEEP)
        #  if !defined(__CMOC__) && !defined(__LCC1802__)
            #include <time.h>
            #include <unistd.h>
            #include <stdlib.h>        
        #elif defined(__CMC__)
            #include <coco.h>
            #include <cmoc.h>
        #endif    
    #endif

    #if defined(NO_SLEEP)
        #define SLEEP(s)
    #elif defined(MACRO_SLEEP)
        #define SLEEP(sec)  do {uint16_t ii; for(ii=0;ii<ALT_SLEEP_SCALE*sec; ++ii){ii=ii;};} while(0)
    #elif defined(ALT_SLEEP)
        void SLEEP(uint8_t s);
    #else
        #define SLEEP(s) sleep(s)
    #endif


#if defined(__NCURSES__)
    #define DO_SLOW_DOWN(t) \
        usleep((t)*800);
#else
    #define DO_SLOW_DOWN(t) \
    { \
        uint16_t i; \
        for(i=0;i<(t);++i) \
        { \
        } \
    }
#endif

#if defined(TURN_BASED)
		#define SLOW_DOWN 0
#else	
	#  if defined(__NCURSES__)
		#define SLOW_DOWN 100
    #elif defined(__OSIC1P__)
        #define SLOW_DOWN 500U
	#elif defined(__OSCA__)
		#define SLOW_DOWN 8167U		
	#elif defined(__SC3000__)
		#define SLOW_DOWN 2500
	#elif defined(__NC100__) || defined(__NC200__)
		#define SLOW_DOWN 1500
	#elif defined(__MTX__)
		#define SLOW_DOWN 3300U
	#elif defined(__MSX__)
		#define SLOW_DOWN 2000U		
	#elif defined(__MC1000__)
		#define SLOW_DOWN 1200			
	#elif defined(__VIC20__) && defined(ALT_PRINT)
		#define SLOW_DOWN 500	
	#elif defined(__VIC20__) && !defined(TINY_GAME) 
		#define SLOW_DOWN 250
	#elif defined(__SAM__)
		#define SLOW_DOWN 500
	#elif defined(__VIC20__) && defined(TINY_GAME) && !defined(VIC20_UNEXPANDED)
		#define SLOW_DOWN 400
	#elif defined(__VIC20__) && defined(TINY_GAME) && defined(VIC20_UNEXPANDED)
		#define SLOW_DOWN 0	
	#elif defined(__WINCMOC__) || defined(__CMOC__) 
		#define SLOW_DOWN 600
	#elif defined(__PC6001__)
		#define SLOW_DOWN 500	
	#elif defined(__NASCOM__)
		#define SLOW_DOWN 8000U
	#elif defined(__ABC80__)
		#define SLOW_DOWN 8000U		
	#elif defined(__GAMATE__)
		#define SLOW_DOWN 1200
	#elif defined(__CREATIVISION__) || defined(__ATARI5200__) || (defined(__ATARI__) && defined(ATARI_MODE1)) 
		#define SLOW_DOWN 1000
	#elif defined(__Z9001__)
		#define SLOW_DOWN 800	
	#elif defined(__X1__)
		#define SLOW_DOWN 1500			
	#elif defined(__VZ__)
		#define SLOW_DOWN 3000U
	#elif defined(__TI82__) || defined(__TI83__) || defined(__TI85__) || defined(__TI8X__) || defined(__TI86__)
		#define SLOW_DOWN 6000U		
	#elif defined(__NES__)
		#define SLOW_DOWN 700
	#elif defined(__MZ__)
		#define SLOW_DOWN 500		
	#elif defined(__APPLE2__) || defined(__APPLE2ENH__)
		#define SLOW_DOWN 200
	#elif (defined(__C16__) || defined(__PLUS4__)) && defined(FULL_GAME)
		#define SLOW_DOWN 250
	#elif (defined(__C16__) || defined(__PLUS4__)) && !defined(FULL_GAME)
		#define SLOW_DOWN 250			
	#elif defined(__ATMOS__)
		#define SLOW_DOWN 50		
	#elif defined(__CBM610__)
		#define SLOW_DOWN 700
	#elif defined(__VG5K__) && !defined(ALT_PRINT)
		#define SLOW_DOWN 2000U
	#elif defined(__VG5K__) && defined(ALT_PRINT)
		#define SLOW_DOWN 5000U		
	#elif defined(__AQUARIUS__) && defined(ALT_PRINT)
		#define SLOW_DOWN 1500		
	#elif defined(__AQUARIUS__) && !defined(ALT_PRINT)
		#define SLOW_DOWN 800	
	#elif defined(__ACE__) || defined(__BEE__)
		#define SLOW_DOWN 900	
	#elif defined(__MSX__)
		#define SLOW_DOWN 550		
	#elif defined(__PCE__)
		#define SLOW_DOWN 4200U	
	#elif defined(__SPECTRUM__) && !defined(__TS2068__)
		#define SLOW_DOWN 1400
	#elif defined(__TS2068__)
		#define SLOW_DOWN 3000
	#elif defined(__CPC__) && defined(CPCRSLIB)
		#define SLOW_DOWN 500	
	#elif defined(__SVI__) && defined(MSX_MODE0)
		#define SLOW_DOWN 1200
	#elif defined(__ATARI_LYNX__)
		#define SLOW_DOWN 6000U
	#elif defined(__SVI__) && !defined(MSX_MODE0)
		#define SLOW_DOWN 1200	
	#elif (defined(__TRS80__) || defined(__EG2K__)) &&  !defined(Z88DK_SPRITES)
		#define SLOW_DOWN 100	
	#elif defined(__EINSTEIN__)
		#define SLOW_DOWN 5000	
	#elif defined(__PX4__)
		#define SLOW_DOWN 2500
	#elif defined(__PX8__)
		#define SLOW_DOWN 30000U
	#elif defined(__M5__)
		#define SLOW_DOWN 900		
	#elif defined(__LASER500__)
		#define SLOW_DOWN 500			
	#else
		#define SLOW_DOWN 0
	#endif
#endif

#endif // _SLEEP_MACROS

