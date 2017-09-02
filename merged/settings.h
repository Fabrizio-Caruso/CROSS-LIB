/*****************************************************************************/
/*                                                                           */
/*                                		                                     */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                  		     */
/*                					                                         */
/*              				                                             */
/* EMail:        Fabrizio_Caruso@hotmail.com                                 */
/*                                                                           */
/*                                                                           */
/* This software is provided 'as-is', without any expressed or implied       */
/* warranty.  In no event will the authors be held liable for any damages    */
/* arising from the use of this software.                                    */
/*                                                                           */
/* Permission is granted to anyone to use this software for any purpose,     */
/* including commercial applications, and to alter it and redistribute it    */
/* freely, subject to the following restrictions:                            */
/*                                                                           */
/* 1. The origin of this software must not be misrepresented; you must not   */
/*    claim that you wrote the original software. If you use this software   */
/*    in a product, an acknowledgment in the product documentation would be  */
/*    appreciated but is not required.                                       */
/* 2. Altered source versions must be plainly marked as such, and must not   */
/*    be misrepresented as being the original software.                      */
/* 3. This notice may not be removed or altered from any source              */
/*    distribution.                                                          */
/*                                                                           */
/*****************************************************************************/

#if !defined(_SETTINGS)
#define _SETTINGS

#include <stdlib.h>

#define C64_HARDWARE_SPRITES 0

#if defined(__VIC20__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 300
#elif ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) 
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 200
#elif defined(__APPLE2__) || defined(__APPLE2ENH__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 200
#elif defined(__CBM610__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 700
#elif defined(__VG5K__)
	#define SLOW_DOWN
	#define GAME_SLOW_DOWN 500	
#else
	#define GAME_SLOW_DOWN 0
#endif

//#define FAST_NONEXACT_COLLISION_DECTION

#define BOMBS_NUMBER 4

// Possible current values are 6,7,8
#if defined(__CPC__) 
	#define GHOSTS_NUMBER 6
#elif defined(__SPECTRUM__) 
	#define GHOSTS_NUMBER 7
#elif defined(__VIC20__)
	#define GHOSTS_NUMBER 8
#elif defined(__VG5K__)
	#define GHOSTS_NUMBER 8
#else
	#define GHOSTS_NUMBER 8
#endif
	
#if defined(__SPECTRUM__) || defined(__VIC20__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) || defined(__APPLE2__) || defined(__APPLE2ENH__)
	#define BUBBLES_NUMBER 2
#else
	#define BUBBLES_NUMBER 4
#endif

#define LIVES_NUMBER 5
#define GUNS_NUMBER 3

#define BACKGROUND_COLOR COLOR_BLACK
#define BORDER_COLOR COLOR_BLACK

#define TEXT_COLOR COLOR_WHITE

#define GHOST_VS_GHOST_COLLISION_LEVEL 16
#define GHOST_VS_GHOST_COLLISION_START 200

#define EXTRA_LIFE_THROUGH_POINTS 20000UL

#define EXTRA_POINTS_COOL_DOWN 900UL

#define INVINCIBILITY_COOL_DOWN 2000UL

#define INVINCIBILITY_FIRST_LEVEL 5

#define PLAYER_INVINCIBILITY_COOL_DOWN 150

#define EXTRA_LIFE_COOL_DOWN 4000UL

#define EXTRA_LIFE_FIRST_LEVEL 10

#if defined(DEBUG_CRASH)
	#define MIN_INVINCIBLE_GHOST_HITS 1
#else
	#define MIN_INVINCIBLE_GHOST_HITS 5
#endif
	
#define HORIZONTAL_MISSILE_BONUS 100UL

#define VERTICAL_MISSILE_BONUS 50UL

#define INVINCIBLE_GHOST_POINTS 500UL;

// Points given by extra points bonus: 
// EXTRA_POINTS (X 2 in boss levels) + level X EXTRA_POINTS_LEVEL_INCREASE
#define EXTRA_POINTS 250UL 
#define EXTRA_POINTS_LEVEL_INCREASE 2UL


// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 25UL

// Points for shooting a ghost
#define GHOST_VS_MISSILE 10UL

// Extra points for the power up
#define POWER_UP_BONUS 20UL

// Extra points for the power up
#define GUN_BONUS 30UL

// Points for each tick
#define LOOP_POINTS 1UL

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 50UL

#if defined(__CBM610__) || defined(__VIC20__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) || defined(__APPLE2__) || defined(__APPLE2ENH__)
	#define INITIAL_ENEMY_SLOWDOWN 31000
	#define INITIAL_SKULL_SLOWDOWN 32000	
#elif defined(__VG5K__)
	#define INITIAL_ENEMY_SLOWDOWN 29000
	#define INITIAL_SKULL_SLOWDOWN 30000	
#else
	#define INITIAL_ENEMY_SLOWDOWN 28000
	#define INITIAL_SKULL_SLOWDOWN 29000
#endif


#define INITIAL_LEVEL 1

// Final level 
#define FINAL_LEVEL 40

#define FIRST_BUBBLES_LEVEL 7 // 8 because of check on levels after missile level

// Maximum number of ghost that co-exist with rockets (in non-boss levels)
#define MAX_GHOST_COUNT_FOR_BUBBLES 4

// Directions
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

// -----------------------------------------------------------------------------------
#define INVINCIBLE_GHOST_TRIGGER 3

#define GUN_INITIAL_COOLDOWN (180 + level * 2)

#define INVINCIBLE_COUNT_DOWN 80

#define INVINCIBLE_LOOP_TRIGGER (700 - level*15)
#endif // _SETTINGS
