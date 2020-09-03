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

#if !defined(_TILE_1)
    #define _TILE_1 ')'
#endif 

#if !defined(_TILE_3)
    #define _TILE_3 '('
#endif

#if !defined(_TILE_2)
    #define _TILE_2 '\''
#endif

#if !defined(_TILE_0)
    #if !defined(__CIDELSA__)
        #define _TILE_0 '*'
    #else
        #define _TILE_0 'H'
    #endif
#endif
// RED

#if !defined(_TILE_5)
    #define _TILE_5 'X'
#endif

// WHITE
#if !defined(_TILE_4)
    #if defined(__LCC1802__) 
        #define _TILE_4 'O'
    #else
        #define _TILE_4 'o'
    #endif
#endif

#if !defined(_TILE_6)
    #if defined(__ALPHATRO__)
        #define _TILE_6 'm'
    #else
        #define _TILE_6 '#'
    #endif
#endif

// BLUE
#if !defined(_TILE_9)
    #if defined(__ZX80__) || defined(__CIDELSA__)
        #define _TILE_9 'G'
    #else
        #define _TILE_9 '!'
    #endif
#endif

// YELLOW
#if !defined(_TILE_7)
    #if !defined(__CIDELSA__)
        #define _TILE_7 '+'
    #else
        #define _TILE_7 'A'
    #endif
#endif

#if !defined(_TILE_7_B)
    #if !defined(__CIDELSA__)
        #define _TILE_7_B '+'
    #else
        #define _TILE_7_B 'A'
    #endif
#endif

#if !defined(_TILE_4_B)
    #define _TILE_4_B 'O'	
#endif

#if !defined(_TILE_0_B)
    #define _TILE_0_B _TILE_0
#endif

#if !defined(_DOLLAR_TILE)
    #define _DOLLAR_TILE '$'
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
    #define _TILE_11 '.'
#endif

#if !defined(_TILE_14)
    #define _TILE_14 '>'
#endif

#if !defined(_TILE_15)
    #define _TILE_15 '<'
#endif

#if !defined(_TILE_16)
    #if defined(__ZX80__) || defined(__ZX81__)
        #define _TILE_16 'I'
    #else
        #define _TILE_16 ('^')
    #endif
#endif

#if !defined(_TILE_12)
    #if defined(__C128_Z80__) || defined(__ZX80__) || defined(__ZX81__) || defined(__GAL__)  
        #define _TILE_12 'i'
    #elif defined(__ATARI_LYNX__) || defined(__ABC80__) || defined(__OSIC1P__) || defined(__TMC600__)
        #define _TILE_12 'I' 
    #else
        #define _TILE_12 ('|')
    #endif 
#endif

#if !defined(_TILE_13)
    #define _TILE_13 '-'
#endif

#if !defined(_TILE_5_B)
    #if !defined(NO_COLOR)
        #define _TILE_5_B _TILE_5
    #else
        #define _TILE_5_B '8'
    #endif
#endif

#endif // _DEFAULT_GRAPHICS_SETTINGS

