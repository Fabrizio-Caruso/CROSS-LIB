
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
#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "display_macros.h"

#define _TILE_8 25
#define _TILE_4 13
#define _TILE_5 16
#define _TILE_9 28

#define _TILE_0 1
#define _TILE_1 4
#define _TILE_2 7
#define _TILE_3 10
	

#define _TILE_11 37
#define _TILE_16 94
#define _DOLLAR_TILE 147
#define _TILE_14 61
#define _TILE_15 91
#define _TILE_10 33

#define _TILE_7 22

#define _TILE_6 19
#define _TILE_0_B _TILE_0
#define _TILE_7_B _TILE_7

#define _TILE_18 126
#define _TILE_17 123

#define _TILE_12 40
#define _TILE_13 58

#define _TILE_4_B _TILE_4

#define _TILE_5_B 'X'

#define _TILE_19 129
#define _TILE_20 132

#define _TILE_21 135
#define _TILE_22 138

#define _TILE_23 141
#define _TILE_24 144
