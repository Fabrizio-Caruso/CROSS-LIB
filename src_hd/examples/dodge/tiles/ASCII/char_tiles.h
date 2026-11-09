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

#ifndef _CHAR_TILES_H
#define _CHAR_TILES_H

#if defined(__TARGET_SPECIFIC_ASCII) && defined(__AQUARIUS__)
    #include "aquarius/aquarius_char_tiles.h"
#else


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
        #define _TILE_24 'W'
    #else
        #define _TILE_24 'W'
    #endif
#endif 

#if !defined(_TILE_1)
    #if !defined(__CIDELSA__)
        #define _TILE_1 '*'
    #else
        #define _TILE_1 'H'
    #endif
#endif 

#if !defined(_TILE_3)
    #if !defined(__CIDELSA__)
        #define _TILE_3 '*'
    #else
        #define _TILE_3 'H'
    #endif
#endif

#if !defined(_TILE_2)
    #if !defined(__CIDELSA__)
        #define _TILE_2 '*'
    #else
        #define _TILE_2 'H'
    #endif
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
    #if defined(__ZX81__) || defined(__ZX80__)
        #define _TILE_5 'x'
    #else
        #define _TILE_5 'X'
    #endif
#endif

// WHITE
#if !defined(_TILE_4)
    #if defined(__LCC1802__) || defined(__SV8000__) || defined(__CIDELSA__) || defined(__MC10__)
        #define _TILE_4 'O'
    #else
        #define _TILE_4 'o'
    #endif
#endif

#if !defined(_TILE_6)
    #if defined(__ALPHATRO__)
        #define _TILE_6 'm'
    #elif defined(__CIDELSA__)
        #define _TILE_6 'M'
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
    #elif defined(__ZX81__) || defined(__ZX80__)
        #define _TILE_7 'a'
    #else
        #define _TILE_7 'A'
    #endif
#endif


#if !defined(_TILE_25)
    #define _TILE_25 '$'
#endif

// GREEN
#if !defined(_TILE_8)
    #if defined(__ZX81__) || defined(__ZX80__)
        #define _TILE_8 's'
    #else
        #define _TILE_8  'S'
    #endif
#endif

#if !defined(_TILE_17)
    #if defined(__ZX81__) || defined(__ZX80__)
        #define _TILE_17 'f'
    #else
        #define _TILE_17 'F'
    #endif
#endif

#if !defined(_TILE_18)
    #if defined(__ZX81__) || defined(__ZX80__)
        #define _TILE_18 'h'
    #else
        #define _TILE_18 'H'
    #endif
#endif

// CYAN
#if !defined(_TILE_10)
    #if defined(__ZX81__) || defined(__ZX80__)
        #define _TILE_10 'v'
    #else
        #define _TILE_10 'V'
    #endif
#endif

#if !defined(_TILE_11)
    #if defined(__CIDELSA__)
        #define _TILE_11 'O'
    #else
        #define _TILE_11 '.'
    #endif
#endif

#if !defined(_TILE_14)
    #define _TILE_14 '>'
#endif

#if !defined(_TILE_15)
    #define _TILE_15 '<'
#endif

#if !defined(_TILE_16)
    #if defined(__ZX80__) || defined(__ZX81__) 
        #define _TILE_16 'i'
    #elif defined(__CIDELSA__) || defined(__MC10__)
        #define _TILE_16 'I'
    #else
        #define _TILE_16 ('^')
    #endif
#endif

#if !defined(_TILE_12)
    #if defined(__C128_Z80__) || defined(__GAL__)  || defined(__VZ__) || defined(__COCO__) || defined(__DRAGON__) 
        #define _TILE_12 'i'
    #elif defined(__APPLE2__) || defined(__ATARI_LYNX__) || defined(__ABC80__) || defined(__OSIC1P__) || defined(__TMC600__) \
    || defined(__MIKRO80__) || defined(__BIC__) || defined(__SUPER80__) || defined(__CIDELSA__) \
    || defined(__ZX80__) || defined(__ZX81__) || defined(__MC10__)
        #define _TILE_12 'I' 
    #else
        #define _TILE_12 ('|')
    #endif 
#endif

#if !defined(_TILE_13)
    #if defined(__CIDELSA__)
        #define _TILE_13 'I'
    #else
        #define _TILE_13 '-'
    #endif
#endif

#if !defined(_TILE_26)
    #if defined(__C128_Z80__) || defined(__GAL__)  || defined(__VZ__) || defined(__COCO__) || defined(__DRAGON__) 
        #define _TILE_26 't'
    #else
        #define _TILE_26 'T'
    #endif
#endif


#endif

#endif // _CHAR_TILES_H

