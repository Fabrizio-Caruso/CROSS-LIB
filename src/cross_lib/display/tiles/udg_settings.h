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

#ifndef _UDG_SETTINGS
#define _UDG_SETTINGS

#define _TILE_0  (0x2A - _Z88DK_SPRITE_OFFSET)
#define _TILE_1  (0x3C - _Z88DK_SPRITE_OFFSET)
#define _TILE_2  (0x3D - _Z88DK_SPRITE_OFFSET) 
#define _TILE_3  (0x3E - _Z88DK_SPRITE_OFFSET)


#define _TILE_9  (0x3F-_Z88DK_SPRITE_OFFSET)

// YELLOW
#define _TILE_7  (0x27 - _Z88DK_SPRITE_OFFSET)
#define _TILE_12 (0x26 -_Z88DK_SPRITE_OFFSET)
#define _TILE_13 (0x2B -_Z88DK_SPRITE_OFFSET)
#define _TILE_0_B _TILE_0
#define _DOLLAR_TILE (0x24 - _Z88DK_SPRITE_OFFSET)

// GREEN
#define _TILE_8  (0x2C - _Z88DK_SPRITE_OFFSET)

#define _TILE_17 (0x25 - _Z88DK_SPRITE_OFFSET)

#define _TILE_18 (0x3B - _Z88DK_SPRITE_OFFSET)

// RED
#define _TILE_5  (0x29 - _Z88DK_SPRITE_OFFSET)

// CYAN
#define _TILE_10 (0x21 - _Z88DK_SPRITE_OFFSET)
#define _TILE_11 (0x23 - _Z88DK_SPRITE_OFFSET)

#define _TILE_7_B _TILE_7

#if !defined(NO_COLOR)
    #define _TILE_5_B _TILE_5
#else
    #define _TILE_5_B 'X'
#endif

// WHITE
#define _TILE_4  (0x28 - _Z88DK_SPRITE_OFFSET)

#if defined(__MO5__)
	#define _TILE_6 (0x3A - _Z88DK_SPRITE_OFFSET)
#else
	#define _TILE_6 (0x5B - _Z88DK_SPRITE_OFFSET)
#endif

#define _TILE_14 (0x22 - _Z88DK_SPRITE_OFFSET)

#define _TILE_15 (0x2E - _Z88DK_SPRITE_OFFSET )

#define _TILE_16 (0x2F - _Z88DK_SPRITE_OFFSET)

#define _TILE_4_B _TILE_4


#endif // _UDG_SETTINGS


