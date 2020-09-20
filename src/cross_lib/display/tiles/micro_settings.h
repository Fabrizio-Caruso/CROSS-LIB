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

#ifndef _MICRO_GRAPHICS_SETTINGS
#define _MICRO_GRAPHICS_SETTINGS

#define PALETTE_SWITCH 128


#define _TILE_0 ('k')

#define _TILE_1 ('l')

#define _TILE_3 ('m')

#define _TILE_2 ('n')



// RED
#define _TILE_5 ('o'+PALETTE_SWITCH)

// WHITE
#define _TILE_4 0
//('p'+PALETTE_SWITCH)

#define _TILE_6 1


// BLUE
#define _TILE_9 4


// YELLOW
#define _TILE_7 2
#define _TILE_7_B 2
#define _TILE_4_B 0

#define _TILE_0_B (_TILE_0+PALETTE_SWITCH)
#define _DOLLAR_TILE '$'

// GREEN
#define _TILE_8 3

#define _TILE_17 ('z'+1)

#define _TILE_18 ('z'+2+PALETTE_SWITCH)

// CYAN
#define _TILE_10 'j'
#define _TILE_11 5

#define _TILE_14 7

#define _TILE_15 8

#define _TILE_16 9

#define _TILE_12 6
 

#define _TILE_13 ('z'+PALETTE_SWITCH)

#define _TILE_5_B ('X'+PALETTE_SWITCH)

#endif // _MICRO_GRAPHICS_SETTINGS

