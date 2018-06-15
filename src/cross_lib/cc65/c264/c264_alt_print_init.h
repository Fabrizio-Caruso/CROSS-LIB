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


#include <conio.h>
#include <peekpoke.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../sound/sound_macros.h"

#include "../../display/display_macros.h"

#if defined(FULL_GAME)
	#define UDG_BASE_FACTOR 29
#else
	#define UDG_BASE_FACTOR 15
#endif


#define BASE_ADDR ((unsigned short) 0x0C00)
#define COLOR_ADDR ((unsigned short) 0x0800)


// BLUE
#define _PLAYER '*'
#if defined(REDEFINED_CHARS)
	#define _PLAYER_DOWN (0xB2-0x40)
	//0x3B
	#define _PLAYER_UP  (0xB1-0x40)
	//0x3C
	#define _PLAYER_RIGHT (0xAB-0x40)
	//0x3D 
	#define _PLAYER_LEFT  (0xB3-0x40)
	//0x3E
#endif

#define _GUN '!'

// YELLOW
#define _INVINCIBLE_GHOST '+'
#define _VERTICAL_BRICK '|'
#define _HORIZONTAL_BRICK '-'
#define _EXTRA_LIFE ('*'+128)
#define _CONFUSE ('+'+128)
#define _ZOMBIE 'O'
#define _EXTRA_POINTS '$'

// GREEN
#define _POWERUP  'S'

// RED
#define _BOMB '^'
//0x5E


// CYAN
#define _INVINCIBILITY 'V'
#define _MISSILE '.'

// WHITE
#define _GHOST 'o'

#define _DEAD_GHOST '#'

#define _LEFT_ENEMY_MISSILE '>'

#define _RIGHT_ENEMY_MISSILE '<'

#define _BUBBLE ('^'-64)

#define _BROKEN_WALL _BOMB
