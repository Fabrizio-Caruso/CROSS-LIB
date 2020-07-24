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




#define _TILE_1 ')'
#define _TILE_3 '('
#define _TILE_2 '\''

#if !defined(__CIDELSA__)
    #define _TILE_0 '*'
#else
    #define _TILE_0 'H'

#endif
// RED
#define _TILE_5 'X'

// WHITE
#if defined(__LCC1802__) 
    #define _TILE_4 'O'
#else
    #define _TILE_4 'o'
#endif

#if defined(__ALPHATRO__)
	#define _TILE_6 'm'
#else
	#define _TILE_6 '#'
#endif

// BLUE
#if defined(__ZX80__) || defined(__CIDELSA__)
	#define _TILE_9 'G'
#else
	#define _TILE_9 '!'
#endif

// YELLOW
#if !defined(__CIDELSA__)
    #define _TILE_7 '+'
    #define _TILE_21 '+'
#else
    #define _TILE_7 'A'
    #define _TILE_21 'A'
#endif

#define _TILE_22 'O'	

#define _TILE_19 _TILE_0
#define _DOLLAR_TILE '$'

// GREEN
#define _TILE_8  'S'

#define _TILE_17 'F'

#define _TILE_18 'H'

// CYAN
#define _TILE_10 'V'
#define _TILE_11 '.'

#define _TILE_14 '>'

#define _TILE_15 '<'

#if defined(__ZX80__) || defined(__ZX81__)
	#define _TILE_16 'I'
#else
	#define _TILE_16 ('^')
#endif

#if defined(__C128_Z80__) || defined(__ZX80__) || defined(__ZX81__) || defined(__GAL__)  
	#define _TILE_12 'i'
#elif defined(__ATARI_LYNX__) || defined(__ABC80__) || defined(__OSIC1P__) || defined(__TMC600__)
	#define _TILE_12 'I' 
#else
	#define _TILE_12 ('|')
#endif 

#define _TILE_13 '-'

#if !defined(NO_COLOR)
    #define _TILE_23 _TILE_5
#else
    #define _TILE_23 '8'
#endif

#endif // _DEFAULT_GRAPHICS_SETTINGS

