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

#ifndef _DEFAULT_GRAPHICS_SETTINGS
#define _DEFAULT_GRAPHICS_SETTINGS



#if defined(ANIMATE_PLAYER)
    #define _PLAYER_DOWN '*'
    #define _PLAYER_UP ')'
    #define _PLAYER_LEFT '('
    #define _PLAYER_RIGHT '\''
#else
    #define _PLAYER '*'
#endif


// RED
#define _BOMB 'X'

// WHITE
#define _GHOST 'o'

#if defined(__ALPHATRO__)
	#define _DEAD_GHOST _GHOST
#else
	#define _DEAD_GHOST '#'
#endif

// BLUE
#if defined(__ZX80__)
	#define _GUN 'G'
#else
	#define _GUN '!'
#endif

// YELLOW
#define _SKULL '+'
#define _CONFUSE '+'
#define _ZOMBIE 'O'	

#define _EXTRA_LIFE ('*')
#define _EXTRA_POINTS '$'

// GREEN
#define _POWERUP  'S'

#define _FREEZE 'F'

#define _SUPER 'H'

// CYAN
#define _INVINCIBILITY 'V'
#define _BULLET '.'

#define _LEFT_HORIZONTAL_MISSILE '>'

#define _RIGHT_HORIZONTAL_MISSILE '<'

#if defined(__ZX80__) || defined(__ZX81__)
	#define _ROCKET 'I'
#else
	#define _ROCKET ('^')
#endif

#if defined(__C128_Z80__) || defined(__ZX80__) || defined(__ZX81__) || defined(__GAL__)
	#define _VERTICAL_BRICK 'i'
#elif defined(__ATARI_LYNX__) || defined(__ABC80__)
	#define _VERTICAL_BRICK 'I' 
#else
	#define _VERTICAL_BRICK ('|')
#endif 

#define _HORIZONTAL_BRICK '-'

#define _BROKEN_BRICK _BOMB	

#define _SPACE ' '


#endif // _DEFAULT_GRAPHICS_SETTINGS

