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
#ifndef _CMOC_GRAPHICS_SETTINGS
#define _CMOC_GRAPHICS_SETTINGS

#include "cross_lib.h"

#if defined(NO_UDG)
    #include "char_tiles.h"
#endif



#if !defined(_TILE_19)
    #if !defined(__CIDELSA__)
        #define _TILE_19 '*'
    #else
        #define _TILE_19 'H'
    #endif
#endif 

#if !defined(_TILE_20)
    #if !defined(__CIDELSA__)
        #define _TILE_20 '*'
    #else
        #define _TILE_20 'H'
    #endif
#endif 

#if !defined(_TILE_21)
    #if !defined(__CIDELSA__)
        #define _TILE_21 '*'
    #else
        #define _TILE_21 'H'
    #endif
#endif 

#if !defined(_TILE_22)
    #if !defined(__CIDELSA__)
        #define _TILE_22 '*'
    #else
        #define _TILE_22 'H'
    #endif
#endif 

#if !defined(_TILE_23)
    #if !defined(__CIDELSA__)
        #define _TILE_23 '*'
    #else
        #define _TILE_23 'H'
    #endif
#endif 

#if !defined(_TILE_24)
    #if !defined(__CIDELSA__)
        #define _TILE_24 '*'
    #else
        #define _TILE_24 'H'
    #endif
#endif 

// BLUE
#if !defined(_TILE_0)
#define _TILE_0 ('*'+NOT_INVERTED)
#endif

#if !defined(_TILE_1)
#define _TILE_1 ('*'+NOT_INVERTED)
#endif

#if !defined(_TILE_2)
#define _TILE_2 ('*'+NOT_INVERTED)
#endif

#if !defined(_TILE_3)
#define _TILE_3 ('*'+NOT_INVERTED)
#endif

#if !defined(_TILE_9)
    #define _TILE_9 '!'
#endif 

#if !defined(_TILE_7)
    #define _TILE_7 ('+'+NOT_INVERTED)
#endif

#if !defined(TILE_0_B)
    #define _TILE_0_B '*'
#endif 

#define _DOLLAR_TILE '$'

#if !defined(_TILE_8)
    #define _TILE_8  ('S'+INVERTED)
#endif

#if !defined(_TILE_17)
    #define _TILE_17 ('F'+INVERTED)
#endif 

#if !defined(_TILE_18)
    #define _TILE_18 ('H'+INVERTED)
#endif

#if !defined(_TILE_5)
    #define _TILE_5 ('X'+INVERTED)
#endif 

#if !defined(_TILE_6)
    #define _TILE_6 '#'
#endif

#if !defined(_TILE_10)
    #define _TILE_10 'I'
#endif

#if !defined(_TILE_11)
    #define _TILE_11 ('.'+NOT_INVERTED)
#endif

#if !defined(_TILE_4)
    #define _TILE_4 'O'
#endif

#if !defined(_TILE_14)
    #define _TILE_14 ('>'+NOT_INVERTED)
#endif

#if !defined(_TILE_15)
    #define _TILE_15 ('<'+NOT_INVERTED)
#endif

#if !defined(_TILE_16)
    #define _TILE_16 ('^'+NOT_INVERTED)
#endif

#if !defined(_TILE_18)
    #define _TILE_18 ('H'+INVERTED)
#endif 

#if !defined(_TILE_7_B)
    #define _TILE_7_B ('C'+INVERTED)
#endif 

#if !defined(_TILE_4_B)
    #define _TILE_4_B ('Z'+INVERTED)	
#endif

#if !defined(_TILE_12)
    #define _TILE_12 ('#'+NOT_INVERTED)
#endif

#if !defined(_TILE_13)
    #define _TILE_13 ('-'+NOT_INVERTED)
#endif

#if !defined(_TILE_5_B)
    #define _TILE_5_B 'X'
#endif


#endif // _CMOC_GRAPHICS_SETTINGS

