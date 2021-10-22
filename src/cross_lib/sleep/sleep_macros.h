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
    
    #if defined(_ZX81__)
        #define ALT_SLEEP_SCALE 1000
    #else
        #define ALT_SLEEP_SCALE 4000
    #endif
  
    #if !defined(NO_SLEEP)
        #  if !defined(__CMOC__) && !defined(__LCC1802__) && !defined(__TI99__)
            #include <time.h>
            #include <unistd.h>
            #include <stdlib.h>        
        #elif defined(__CMC__)
            #include <coco.h>
            #include <cmoc.h>
        #endif    
    #endif

    #if defined(NO_SLEEP)
        #define _XL_SLEEP(s)
    #elif defined(MACRO_SLEEP)
        #define _XL_SLEEP(sec)  do {uint16_t ii; for(ii=0;ii<ALT_SLEEP_SCALE*sec; ++ii){ii=ii;};} while(0)
    #elif defined(ALT_SLEEP)
        void _XL_SLEEP(uint8_t s);
    #else
        #define _XL_SLEEP(s) sleep(s)
    #endif


#if !defined(_XL_SLOW_DOWN_FACTOR)
    #if defined(TURN_BASED)
            #define _XL_SLOW_DOWN_FACTOR 0
    #else 
        #  if defined(__NCURSES__)
            #define _XL_SLOW_DOWN_FACTOR 100
        #elif defined(__OSIC1P__)
            #define _XL_SLOW_DOWN_FACTOR 500U
        #elif defined(__OSCA__)
            #define _XL_SLOW_DOWN_FACTOR 8167U		
        #elif defined(__SC3000__)
            #define _XL_SLOW_DOWN_FACTOR 2500
        #elif defined(__NC100__) || defined(__NC200__)
            #define _XL_SLOW_DOWN_FACTOR 1500
        #elif defined(__MTX__)
            #define _XL_SLOW_DOWN_FACTOR 3300U
        #elif defined(__MSX__)
            #define _XL_SLOW_DOWN_FACTOR 2000U		
        #elif defined(__MC1000__)
            #define _XL_SLOW_DOWN_FACTOR 1200			
        #elif defined(__SAM__)
            #define _XL_SLOW_DOWN_FACTOR 500
        #elif defined(__VIC20__) && defined(VIC20_UNEXPANDED)
            #define _XL_SLOW_DOWN_FACTOR 0
        #elif defined(__PC6001__)
            #define _XL_SLOW_DOWN_FACTOR 500	
        #elif defined(__NASCOM__)
            #define _XL_SLOW_DOWN_FACTOR 8000U
        #elif defined(__ABC80__)
            #define _XL_SLOW_DOWN_FACTOR 8000U		
        #elif defined(__VZ__)
            #define _XL_SLOW_DOWN_FACTOR 3000U
        #elif defined(__TI82__) || defined(__TI83__) || defined(__TI85__) || defined(__TI8X__) || defined(__TI86__)
            #define _XL_SLOW_DOWN_FACTOR 6000U
        #elif defined(__C64__)
            #if defined(USE_WAIT_V_SYNC)
                #define _XL_SLOW_DOWN_FACTOR 0
            #else
                #define _XL_SLOW_DOWN_FACTOR 300
            #endif
        #elif (defined(__VIC20__) && !defined(VIC20_UNEXPANDED)) 
            #define _XL_SLOW_DOWN_FACTOR 600
        #elif defined(__NES__) || defined(__MZ__) || defined(__Z9001__) || defined(__WINCMOC__) || defined(__CMOC__) || \
              defined(__CBM610__) || defined(__MSX__) || defined(__LASER500__)
            #define _XL_SLOW_DOWN_FACTOR 800
        #elif defined(__APPLE2__) || defined(__APPLE2ENH__) || (defined(__C16__) || defined(__PLUS4__))
            #define _XL_SLOW_DOWN_FACTOR 250
        #elif defined(__ATMOS__)
            #define _XL_SLOW_DOWN_FACTOR 50		
        #elif defined(__VG5K__) && !defined(ALT_PRINT)
            #define _XL_SLOW_DOWN_FACTOR 2000U
        #elif defined(__VG5K__) && defined(ALT_PRINT)
            #define _XL_SLOW_DOWN_FACTOR 5000U		
        #elif defined(__GAMATE__) || defined(__X1__) || defined(__AQUARIUS__) && defined(ALT_PRINT)
            #define _XL_SLOW_DOWN_FACTOR 1500
        #elif defined(__CREATIVISION__) || defined(__ATARI5200__) || (defined(__ATARI__) && defined(ATARI_MODE1))  || \
              defined(__ACE__) || defined(__BEE__) || defined(__AQUARIUS__) && !defined(ALT_PRINT) || defined(__PET__)
            #define _XL_SLOW_DOWN_FACTOR 1000
        #elif defined(__PCE__)
            #define _XL_SLOW_DOWN_FACTOR 5500U	
        #elif defined(__SPECTRUM__) && !defined(__TS2068__)
            #define _XL_SLOW_DOWN_FACTOR 1400
        #elif defined(__TS2068__)
            #define _XL_SLOW_DOWN_FACTOR 3000
        #elif defined(__CPC__) && defined(CPCRSLIB)
            #define _XL_SLOW_DOWN_FACTOR 500	
        #elif defined(__SVI__)
            #define _XL_SLOW_DOWN_FACTOR 1200
        #elif defined(__ATARI_LYNX__)
            #define _XL_SLOW_DOWN_FACTOR 6000U
        #elif (defined(__TRS80__) || defined(__EG2K__)) &&  !defined(Z88DK_SPRITES)
            #define _XL_SLOW_DOWN_FACTOR 100	
        #elif defined(__EINSTEIN__)
            #define _XL_SLOW_DOWN_FACTOR 5000	
        #elif defined(__PX4__)
            #define _XL_SLOW_DOWN_FACTOR 2500
        #elif defined(__PX8__)
            #define _XL_SLOW_DOWN_FACTOR 30000U
        #elif defined(__M5__)
            #define _XL_SLOW_DOWN_FACTOR 900		
        #elif defined(__SUPERVISION__)
            #define _XL_SLOW_DOWN_FACTOR 2000		
        #endif
    #endif
#endif

#if !defined(_XL_SLOW_DOWN_FACTOR)
    #define _XL_SLOW_DOWN_FACTOR 0
#endif

#if !defined(NO_SLOW_DOWN_MACRO)
    #if defined(__NCURSES__)
        #include <stdint.h>
        #include <time.h>
        #include <unistd.h>
        #define _XL_SLOW_DOWN(t) \
            usleep((t)*800)
    #elif defined(_XL_SLOW_DOWN_FACTOR) && _XL_SLOW_DOWN_FACTOR>0
        #define _XL_SLOW_DOWN(t) \
        do { \
            uint16_t __i; \
            \
            for(__i=0;__i<(t);++__i) \
            { \
            } \
        } while(0)
    #else
       #define _XL_SLOW_DOWN(t)
    #endif
#else
    void _XL_SLOW_DOWN(uint16_t t);
#endif

#endif // _SLEEP_MACROS

