/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
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

#include "../../cross_lib/cross_lib.h"

#include "display_macros.h"

// #if defined(__VZ__) || defined(__WINCMOC__) || defined(__CMOC__) || defined(__OSIC1P__)
	// #define VERTICAL_BRICK '#'
// #else
	// #define VERTICAL_BRICK '|'
// #endif

#if !defined(NO_SET_SCREEN_COLORS)
	void setScreenColors(void)
	{
		SET_TEXT_COLOR(TEXT_COLOR);
		SET_BORDER_COLOR(BORDER_COLOR);
		SET_BACKGROUND_COLOR(BACKGROUND_COLOR);
	}
#endif

#if !defined(REDEFINED_CHARS)
	Image PLAYER_IMAGE;
#else
	Image PLAYER_DOWN;
	Image PLAYER_UP;
	Image PLAYER_RIGHT;
	Image PLAYER_LEFT;	
#endif

#if defined(NO_MESSAGE)	
	extern short highScore;
#endif

Image GHOST_IMAGE;
Image BOMB_IMAGE;

#if !defined(DNO_DEAD_GHOSTS)
	Image DEAD_GHOST_IMAGE;
#endif

#if !defined(TINY_GAME)
	Image POWERUP_IMAGE;
	Image BULLET_IMAGE;
	Image GUN_IMAGE;	
	Image SKULL_IMAGE;		
	Image EXTRA_POINTS_IMAGE;	
#endif

#if defined(FULL_GAME)
	Image LEFT_HORIZONTAL_MISSILE_IMAGE;
	Image RIGHT_HORIZONTAL_MISSILE_IMAGE;

	Image ROCKET_IMAGE;

	Image FREEZE_IMAGE;	
	
	Image EXTRA_LIFE_IMAGE;
	Image INVINCIBILITY_IMAGE;	
	
	Image CHASE_IMAGE;
	
	Image SUPER_IMAGE;
	Image CONFUSE_IMAGE;
	Image ZOMBIE_IMAGE;
	
	Image BROKEN_BRICK_IMAGE;
	
	Image HORIZONTAL_BRICK_IMAGE;
	
	Image VERTICAL_BRICK_IMAGE;
#endif

/*
#if defined(__VG5K__)
	//
#elif defined(__C64__) && defined(REDEFINED_CHARS)
	// 
#elif defined(__ATMOS__) && defined(REDEFINED_CHARS)
	// 
#elif (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1) && defined(REDEFINED_CHARS)
	// 
#elif (defined(__C16__)   || defined(__PLUS4__))   && defined(REDEFINED_CHARS)
	// 
#elif defined(__C16__) && defined(ALT_PRINT)
	// 
#elif defined(__SPECTRUM__)
	//	
#elif defined(__MSX__) && defined(REDEFINED_CHARS) && defined(MSX_VPOKE)
	// 
#elif defined(__SVI__) && defined(MSX_MODE0)
	//
#elif defined(__CPC__)
	//
#elif defined(__AQUARIUS__) && defined(EXT_GRAPHICS)
	//	
#elif defined(__NES__) && defined(NES_COLOR)
	//	
#elif defined(__ZX80__) || defined(__ZX81__) || defined(__ACE__) || defined(__LAMBDA__)
	//
#elif defined(__ATARI_LYNX__)
	//
#elif defined(__GAMATE__)
	//	
#elif defined(__VIC20__) && defined(REDEFINED_CHARS)
	//
#elif defined(__VIC20__) && defined(VIC20_UNEXPANDED)
	//
#elif defined(__VIC20__) && defined(ALT_PRINT)
	//	
#elif (defined(__WINCMOC__) && defined(__CMOC__)) && defined(REDEFINED_CHARS)
	//
#elif defined(__CMOC__) && !defined(__WINCMOC__)
	//
#elif defined(__ENTERPRISE__)
	//
#elif defined(__SUPERVISION__)
	//
#elif defined(__EG2K__) || defined(__TRS80__)
	//
#elif defined(Z88DK_SPRITES)
	//

#endif
*/


