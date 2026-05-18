
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

#include<conio.h>
// #include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "display_macros.h"

// #define _TILE_4 (0x76-0x20-13-7)

#define _TILE_7 33
#define _TILE_5 34
#define _TILE_17 35

#define _TILE_8 44
#define _TILE_9 45
#define _TILE_11 46
#define _TILE_12 47

#define _TILE_0 '\''
#define _TILE_1 (0x70-0x20-12-7)
// #define _TILE_2 (0x74-0x20-13-7)
#define _TILE_3 (0x72-0x20-13-7)
	

#define _TILE_16 ('^'-0x40)
#define _TILE_25 '$'
#define _TILE_14 ('>'-3)
#define _TILE_15 ('<'-2)
#define _TILE_10 (0x73 - 0x20-13-7)

#define _TILE_6 (('^'-0x40)+1)

#define _TILE_13 (0xC3-128-7)

// #define _TILE_18 (0x75-0x20-13-7)

#define _TILE_19 42
#define _TILE_20 43
#define _TILE_21 37
#define _TILE_22 38
#define _TILE_23 40
#define _TILE_24 41

#define _TILE_26 0

#define _TILE_2 ('Z'+1)
#define _TILE_4 ('Z'+2)
#define _TILE_18 ('Z'+3)
