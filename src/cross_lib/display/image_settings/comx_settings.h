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

#ifndef _COMX_GRAPHICS_SETTINGS
#define _COMX_GRAPHICS_SETTINGS

#define PALETTE_SWITCH 128


#if defined(ANIMATE_PLAYER)
    #define _PLAYER_DOWN ('k')
    #define _PLAYER_UP ('l')
    #define _PLAYER_LEFT ('m')
    #define _PLAYER_RIGHT ('n')
#else
    #define _PLAYER ('k')
#endif


// RED
#define _BOMB ('o'+PALETTE_SWITCH)

// WHITE
#define _GHOST ('p'+PALETTE_SWITCH)

#define _DEAD_GHOST ('u'+PALETTE_SWITCH)


// BLUE
#define _GUN ('s'+PALETTE_SWITCH)


// YELLOW
#define _SKULL ('q'+PALETTE_SWITCH)
#define _CONFUSE _SKULL
#define _ZOMBIE 'O'	

#define _EXTRA_LIFE ('*')
#define _EXTRA_POINTS '$'

// GREEN
#define _POWERUP  'r'

#define _FREEZE 'F'

#define _SUPER 'H'

// CYAN
#define _INVINCIBILITY 'j'
#define _BULLET ('t'+PALETTE_SWITCH)

#define _LEFT_HORIZONTAL_MISSILE '>'

#define _RIGHT_HORIZONTAL_MISSILE '<'


#define _ROCKET ('^')


#if defined(__C128_Z80__) || defined(__ZX80__) || defined(__ZX81__) || defined(__GAL__)
	#define _VERTICAL_BRICK 'i'
#elif defined(__ATARI_LYNX__) || defined(__ABC80__) || defined(__OSIC1P__) || defined(__COMX__)
	#define _VERTICAL_BRICK 'I' 
#else
	#define _VERTICAL_BRICK ('|')
#endif 

#define _HORIZONTAL_BRICK '-'

#define _BROKEN_BRICK _BOMB	

#define _SPACE ' '

#endif // _COMX_GRAPHICS_SETTINGS

