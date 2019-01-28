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
#ifndef _CMOC_GRAPHICS_SETTINGS
#define _CMOC_GRAPHICS_SETTINGS

#include "cross_lib.h"


#define NOT_INVERTED 64
#define INVERTED -64
	
// BLUE
#define _PLAYER ('*'+NOT_INVERTED)
// #define _PLAYER_DOWN 0
// #define _PLAYER_UP 1
// #define _PLAYER_RIGHT 2
// #define _PLAYER_LEFT 3


#define _GUN '!'

// YELLOW
#define _SKULL ('+'+NOT_INVERTED)

#define _EXTRA_LIFE '*'
#define _EXTRA_POINTS '$'

// GREEN
#define _POWERUP  ('S'+INVERTED)
#define _FREEZE ('F'+INVERTED)
#define _SUPER ('H'+INVERTED)

// RED
#define _BOMB ('X'+INVERTED)
#define _DEAD_GHOST '#'

#define _INVINCIBILITY 'I'
#define _BULLET ('.'+NOT_INVERTED)

#define _GHOST 'O'

#define _LEFT_HORIZONTAL_MISSILE ('>'+NOT_INVERTED)
#define _RIGHT_HORIZONTAL_MISSILE ('<'+NOT_INVERTED)
#define _ROCKET ('^'+NOT_INVERTED)

#define _SUPER ('H'+INVERTED)
#define _CONFUSE ('C'+INVERTED)
#define _ZOMBIE ('Z'+INVERTED)	

#define _SPACE (' '+NOT_INVERTED)
#define _VERTICAL_BRICK ('#'+NOT_INVERTED)
#define _HORIZONTAL_BRICK ('-'+NOT_INVERTED)
#define _BROKEN_BRICK _BOMB


#endif // _CMOC_GRAPHICS_SETTINGS

