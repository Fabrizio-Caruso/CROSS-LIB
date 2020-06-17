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

#define _PLAYER_DOWN (0x2A-_Z88DK_SPRITE_OFFSET)
#define _PLAYER_UP (0x3C-_Z88DK_SPRITE_OFFSET)
#define _PLAYER_RIGHT (0x3D-_Z88DK_SPRITE_OFFSET) 
#define _PLAYER_LEFT (0x3E - _Z88DK_SPRITE_OFFSET)

#define _PLAYER _PLAYER_DOWN


#define _GUN (0x3F-_Z88DK_SPRITE_OFFSET)

// YELLOW
#define _SKULL (0x27-_Z88DK_SPRITE_OFFSET)
#define _VERTICAL_BRICK (0x26-_Z88DK_SPRITE_OFFSET)
#define _HORIZONTAL_BRICK (0x2B-_Z88DK_SPRITE_OFFSET)
#define _EXTRA_LIFE _PLAYER_DOWN
#define _EXTRA_POINTS (0x24-_Z88DK_SPRITE_OFFSET)

// GREEN
#define _POWERUP  (0x2C-_Z88DK_SPRITE_OFFSET)

#define _FREEZE (0x25-_Z88DK_SPRITE_OFFSET)

#define _SUPER (0x3B-_Z88DK_SPRITE_OFFSET)

// RED
#define _BOMB (0x29-_Z88DK_SPRITE_OFFSET)

// CYAN
#define _INVINCIBILITY (0x21-_Z88DK_SPRITE_OFFSET)
#define _BULLET (0x23-_Z88DK_SPRITE_OFFSET)

#define _CONFUSE _SKULL

#define _BROKEN_BRICK _BOMB

// WHITE
#define _GHOST (0x28-_Z88DK_SPRITE_OFFSET)

#if defined(__MO5__)
	#define _DEAD_GHOST (0x3A-_Z88DK_SPRITE_OFFSET)
#else
	#define _DEAD_GHOST (0x5B-_Z88DK_SPRITE_OFFSET)
#endif

#define _LEFT_HORIZONTAL_MISSILE (0x22-_Z88DK_SPRITE_OFFSET)

#define _RIGHT_HORIZONTAL_MISSILE (0x2E - _Z88DK_SPRITE_OFFSET )

#define _ROCKET (0x2F-_Z88DK_SPRITE_OFFSET)

#endif // _UDG_SETTINGS


