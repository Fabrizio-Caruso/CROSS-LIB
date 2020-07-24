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

#ifndef _PETSCII_MEMORY_MAPPED_SETTINGS
#define _PETSCII_MEMORY_MAPPED_SETTINGS

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <peekpoke.h>
#include <string.h>

#define _TILE_0 (0xB2-0x40)
//0x3B
#define _TILE_1  (0xB1-0x40)
//0x3C
#define _TILE_2 (0xAB-0x40)
//0x3D 
#define _TILE_3  (0xB3-0x40)
//0x3E

#define _TILE_5 '^'

#define _TILE_4 'o'

#define _TILE_6 '#'

#define _TILE_9 '!'

#define _TILE_7 '+'
#define _TILE_21 ('+'+128)
#define _TILE_22 'O'	

#define _TILE_19 ('*'+128)
#define _DOLLAR_TILE '$'

#define _TILE_8  'S'
#define _TILE_17 'F'
#define _TILE_18 'H'

#define _TILE_10 'V'
#define _TILE_11 '.'

#define _TILE_14 '>'

#define _TILE_15 '<'

#define _TILE_16 ('^'-64)

#define _TILE_12 ('|'-128)
#define _TILE_13 '-'

#if !defined(NO_COLOR)
    #define _TILE_23 _TILE_5	
#else
    #define _TILE_23 '8'
#endif


#endif // _PETSCII_MEMORY_MAPPED_SETTINGS

