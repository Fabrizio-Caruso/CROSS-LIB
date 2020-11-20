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

#ifndef _DEFAULT_GRAPHICS_SETTINGS
#define _DEFAULT_GRAPHICS_SETTINGS

#if defined(NO_UDG)
    #include "char_tiles.h"
#endif

#if !defined(_TILE_19)
    #define _TILE_19 'T'
#endif 

#if !defined(_TILE_20)
    #define _TILE_20 'W'
#endif 

#if !defined(_TILE_21)
    #define _TILE_21 'H'
#endif 

#if !defined(_TILE_22)
    #define _TILE_22 'E'
#endif 

#if !defined(_TILE_23)
    #define _TILE_23 'L'
#endif 

#if !defined(_TILE_24)
    #define _TILE_24 'D'
#endif 


#if !defined(_TILE_1)
    #define _TILE_1 'H'
#endif 

#if !defined(_TILE_3)
    #define _TILE_3 'H'
#endif

#if !defined(_TILE_2)
    #define _TILE_2 'H'
#endif

#if !defined(_TILE_0)
        #define _TILE_0 'H'
#endif
// RED

#if !defined(_TILE_5)
    #define _TILE_5 'X'
#endif

// WHITE
#if !defined(_TILE_4)
    #define _TILE_4 'O'
#endif

#if !defined(_TILE_6)
    #define _TILE_6 'Z'
#endif

// BLUE
#if !defined(_TILE_9)
    #define _TILE_9 'G'
#endif

// YELLOW
#if !defined(_TILE_7)
    #define _TILE_7 'A'
#endif

#if !defined(_TILE_7_B)
    #define _TILE_7_B 'A'
#endif

#if !defined(_TILE_4_B)
    #define _TILE_4_B 'O'	
#endif

#if !defined(_TILE_0_B)
    #define _TILE_0_B _TILE_0
#endif

#if !defined(_DOLLAR_TILE)
    #define _DOLLAR_TILE 'M'
#endif

// GREEN
#if !defined(_TILE_8)
    #define _TILE_8  'S'
#endif

#if !defined(_TILE_17)
    #define _TILE_17 'F'
#endif

#if !defined(_TILE_18)
    #define _TILE_18 'H'
#endif

// CYAN
#if !defined(_TILE_10)
    #define _TILE_10 'V'
#endif


#if !defined(_TILE_11)
    #define _TILE_11 'O'
#endif

#if !defined(_TILE_14)
    #define _TILE_14 'D'
#endif

#if !defined(_TILE_15)
    #define _TILE_15 'C'
#endif

#if !defined(_TILE_16)
    #define _TILE_16 'I'

#endif

#if !defined(_TILE_12)
    #define _TILE_12 'I' 
#endif

#if !defined(_TILE_13)
    #define _TILE_13 'I'

#endif

#if !defined(_TILE_5_B)
    #define _TILE_5_B _TILE_5
#endif

#endif // _DEFAULT_GRAPHICS_SETTINGS

