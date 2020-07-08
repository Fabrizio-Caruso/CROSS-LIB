
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

#define _TILE_8 248
#define _TILE_4 (char) 35
#define _TILE_5 251
#define _TILE_9 246

#define _TILE_0 252
#define _TILE_1 255
#define _TILE_2 254
#define _TILE_3 253
	

#define _TILE_11 242
#define _TILE_16 239
#define _TILE_10 '$'
#define _TILE_14 241
#define _TILE_15 240
#define _TILE_20 238

#define _TILE_7 250

#define _TILE_6 247
#define _TILE_19 _TILE_0
#define _TILE_21 _TILE_7

#define _TILE_18 245
#define _TILE_17 243

#define _TILE_12 249
#define _TILE_13 244

#define _TILE_22 _TILE_4

#define _TILE_23 'X'


