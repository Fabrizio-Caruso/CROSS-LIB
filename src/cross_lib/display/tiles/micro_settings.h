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


#define _TILE_0 0

#define _TILE_1 27

#define _TILE_3 29

#define _TILE_2 28



// RED
#define _TILE_5 (31+PALETTE_SWITCH)

// WHITE
#define _TILE_4 (30+PALETTE_SWITCH)
//('p'+PALETTE_SWITCH)

#define _TILE_6 (33+PALETTE_SWITCH)


// BLUE
#define _TILE_9 46


// YELLOW
#define _TILE_7 (34+PALETTE_SWITCH)
#define _TILE_7_B 34
#define _TILE_4_B 30

#define _TILE_0_B (_TILE_0+PALETTE_SWITCH)
#define _DOLLAR_TILE ('$'+PALETTE_SWITCH)

// GREEN
#define _TILE_8 35

#define _TILE_17 44

#define _TILE_18 (45+PALETTE_SWITCH)

// CYAN
#define _TILE_10 37
#define _TILE_11 (38+PALETTE_SWITCH)

#define _TILE_14 (41+PALETTE_SWITCH)

#define _TILE_15 (42+PALETTE_SWITCH)

#define _TILE_16 (43+PALETTE_SWITCH)

#define _TILE_12 (39+PALETTE_SWITCH)
 

#define _TILE_13 (40+PALETTE_SWITCH)

#define _TILE_5_B 31

#define _TILE_19 (47)
#define _TILE_20 (58)

#define _TILE_21 (59)

#define _TILE_22 (60)

#define _TILE_23 (61)
#define _TILE_24 (62)

#endif // _MICRO_GRAPHICS_SETTINGS

