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

#ifndef _SETTINGS
#define _SETTINGS

#include <stdlib.h>

#define C64_HARDWARE_SPRITES 0
#define C64_REDEFINED_CHARACTERS 0

#define EXTRA_LIFE_THROUGH_POINTS 20000ul

#define FAST_NONEXACT_COLLISION_DECTION

#define BOMBS_NUMBER 4
#define GHOSTS_NUMBER 8
#define LIVES_NUMBER 5
#define GUNS_NUMBER 3

#define BACKGROUND_COLOR COLOR_WHITE
#define BORDER_COLOR COLOR_WHITE

#define TEXT_COLOR COLOR_BLACK


#define PLAYER_COLOR COLOR_BLACK
#define INVINCIBLE_ENEMY COLOR_RED

#define POWER_UP_COLOR COLOR_BLACK
#define GUN_COLOR COLOR_BLACK

#define GHOST_VS_GHOST_COLLISION_LEVEL 16
#define GHOST_VS_GHOST_COLLISION_START 80

// If two or more ghosts bump into eachother for each ghost
#define GHOST_VS_GHOST_BONUS 500ul

// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 250ul

// Points for shooting a ghost
#define GHOST_VS_MISSILE 100ul

// Extra points for the power up
#define POWER_UP_BONUS 200ul

// Extra points for the power up
#define GUN_BONUS 300ul

// Points for each tick
#define LOOP_POINTS 1ul

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 1000ul


// MINE DISTRIBUTION
// LEVEL 1 - 4: Four central bombs
// LEVEL 5 - 9: Two central bombs
// LEVEL 10 - 14: Two bombs next to the vertical borders
// LEVEL 15 - 16: Three bombs attached to the borders
// LEVEL 17 - 18: Two bombs attached to the vertical borders
// LEVEL 19 - 20: Four bombs at the corners 

// Starting from this level 4 central bombs
#define INITIAL_LEVEL 1

// Starting from this level only two central bombs
#define TWO_BOMB_START_LEVEL 5

// Starting from this level only 2 bombs close to the vertical borders
#define FIRST_HARD_LEVEL 10

// Starting from this level 2 bombs close to the botton borders
#define FIRST_VERY_HARD_LEVEL 15

// Starting from this level only 2 bombs on the vertical borders
#define FIRST_INSANE_LEVEL 17

// Ultimate level (four bombs at the corners)
#define FIRST_ULTIMATE_LEVEL 19

// Final level 
#define FINAL_LEVEL 20

// GHOST STRATEGIES
// LEVEL 1: Ghosts chase use by approaching randomly either X or Y coordinates (in sort of a direct line)
// LEVEL 2-7: Most ghosts behave as in 1-3 but 2 may embush the player (starting from at 8 to at least 3 in game)
// LEVEL 8-9: With more than 3 ghosts there will be up to 3 different ghost groups (x-embush, y-embush, direct line) 
// LEVEL 10-20: Ghosts will try to avoid to be aligned with the player 

// Starting from this level, the ghosts use a smarter "collective" strategy
// #define EASY_COLLECTIVE_STRATEGY_START_LEVEL 2
// #define HARD_COLLECTIVE_STRATEGY_START_LEVEL 8
// #define MAX_STRATEGY_START_LEVEL 10

// Starting from this level, the invincible ghost will try to be aligned with the player to defend other ghosts
// #define NASTY_INVINCIBLE_START_LEVEL 5

// Directions
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3


#endif // _SETTINGS
