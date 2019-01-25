
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

#include "../display/display_macros.h"

#define _POWERUP 248
#define _GHOST (char) 35
#define _BOMB 251
#define _GUN 246

#define _PLAYER_DOWN 252
#define _PLAYER_UP 255
#define _PLAYER_RIGHT 254
#define _PLAYER_LEFT 253
	

#define _BULLET 242
#define _ROCKET 239
#define _EXTRA_POINTS '$'
#define _LEFT_HORIZONTAL_MISSILE 241
#define _RIGHT_HORIZONTAL_MISSILE 240
#define _INVINCIBILITY 238

#define _SKULL 250

#define _DEAD_GHOST 247
#define _EXTRA_LIFE _PLAYER_DOWN
#define _CONFUSE _SKULL

#define _SUPER 245
#define _FREEZE 243

#define _SPACE ' '
#define _VERTICAL_BRICK 249
#define _HORIZONTAL_BRICK 244
#define _BROKEN_BRICK _BOMB


