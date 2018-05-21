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

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <peekpoke.h>
#include <vic20.h>
#include <string.h>

#include "../input_macros.h"

#define BASE_ADDR 0x1000
#define COLOR_ADDR 0x9400
#define DELTA_ADDR (COLOR_ADDR-BASE_ADDR)


#if !defined(REDEFINED_CHARS)
	#define _PLAYER_DOWN '*'
	#define _PLAYER_UP 0x0E
	#define _PLAYER_RIGHT 0x11
	#define _PLAYER_LEFT 0x10
	#define _PLAYER _PLAYER_DOWN

	// RED
	#define _BOMB '^'

	// WHITE
	#define _GHOST 'o'

	#define _DEAD_GHOST '#'

	// BLUE
	#define _GUN '!'

	// YELLOW
	#define _INVINCIBLE_GHOST '+'
	#define _CONFUSE ('+'+128)
	#define _ZOMBIE 'O'	
	#define _VERTICAL_BRICK '|'
	#define _HORIZONTAL_BRICK '-'
	#define _EXTRA_LIFE ('*'+128)
	#define _EXTRA_POINTS '$'

	// GREEN
	#define _POWERUP  'S';

	// CYAN
	#define _INVINCIBILITY 'V'
	#define _MISSILE '.'

	#define _LEFT_ENEMY_MISSILE '>'

	#define _RIGHT_ENEMY_MISSILE '<'

	#define _BUBBLE ('^'-64)
	
	#define _BROKEN_WALL _BOMB
#else
	#define _PLAYER_DOWN 0x00
	#define _PLAYER_UP 0x0E
	#define _PLAYER_RIGHT 0x11
	#define _PLAYER_LEFT 0x10
	#define _PLAYER _PLAYER_DOWN

	// RED
	#define _BOMB 0x1B

	// WHITE
	#define _GHOST 0x1C

	#define _DEAD_GHOST _GHOST


	// BLUE
	#define _GUN 0x7B

	// YELLOW
	#define _INVINCIBLE_GHOST 0x77
	#define _VERTICAL_BRICK 0x26
	#define _HORIZONTAL_BRICK 0x2B
	#define _EXTRA_LIFE _PLAYER
	#define _EXTRA_POINTS '$'

	// GREEN
	#define _POWERUP  0x7A;

	// CYAN
	#define _INVINCIBILITY 0x73
	#define _MISSILE 0x7C

	#define _LEFT_ENEMY_MISSILE '>'

	#define _RIGHT_ENEMY_MISSILE '<'

	#define _BUBBLE '^'
#endif	