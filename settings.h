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



#define FAST_NONEXACT_COLLISION_DECTION

#define BOMBS_NUMBER 4
#define GHOSTS_NUMBER 8
#define BUBBLES_NUMBER 4

#define LIVES_NUMBER 5
#define GUNS_NUMBER 3

#define BACKGROUND_COLOR COLOR_BLACK
#define BORDER_COLOR COLOR_BLACK

#define TEXT_COLOR COLOR_WHITE

#define GHOST_VS_GHOST_COLLISION_LEVEL 16
#define GHOST_VS_GHOST_COLLISION_START 200

#define EXTRA_LIFE_THROUGH_POINTS 20000UL

#define EXTRA_POINTS_COOL_DOWN 900UL

#define INVINCIBILITY_COOL_DOWN 1100UL

#define INVINCIBILITY_FIRST_LEVEL 5

#define PLAYER_INVINCIBILITY_COOL_DOWN 100

#define EXTRA_LIFE_COOL_DOWN 3200UL

#define EXTRA_LIFE_FIRST_LEVEL 10

#define MIN_INVINCIBLE_GHOST_HITS 5UL
#define INVINCIBLE_GHOST_POINTS 5000UL;

// Points given by extra points bonus: 
// EXTRA_POINTS (X 2 in mission levels) + level X EXTRA_POINTS_LEVEL_INCREASE
#define EXTRA_POINTS 2500UL 
#define EXTRA_POINTS_LEVEL_INCREASE 200UL


// If two or more ghosts bump into eachother for each ghost
#define GHOST_VS_GHOST_BONUS 500UL

// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 250UL

// Points for shooting a ghost
#define GHOST_VS_MISSILE 100UL

// Extra points for the power up
#define POWER_UP_BONUS 200UL

// Extra points for the power up
#define GUN_BONUS 300UL

// Points for each tick
#define LOOP_POINTS 1UL

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 500UL


// MINE DISTRIBUTION
// LEVEL  1 -  5: Four central bombs
// LEVEL  5 - 10: Three central bombs
// LEVEL 10 - 15: Two central bombs
// LEVEL 15 - 20: One central bomb
// LEVEL 21 - 25: Two bombs next to the vertical borders
// LEVEL 26 - 30: Two bombs next to the lower corners
// LEVEL 31 - 36: Two bombs attached to the vertical borders
// LEVEL 36 - 40: Four bombs at the four corners 

// Starting from this level 4 central bombs
#define INITIAL_LEVEL 1

#define THREE_BOMB_START_LEVEL 6

// Starting from this level only two central bombs
#define TWO_BOMB_START_LEVEL 11

#define ONE_BOMB_START_LEVEL 16

// Starting from this level only 2 bombs close to the vertical borders
#define FIRST_HARD_LEVEL 21

// Starting from this level 2 bombs close to the botton corners
#define FIRST_VERY_HARD_LEVEL 26

// Starting from this level only 2 bombs on the vertical borders
#define FIRST_INSANE_LEVEL 31

// Ultimate level (four bombs at the corners)
#define FIRST_ULTIMATE_LEVEL 36

// Final level 
#define FINAL_LEVEL 40


#define FIRST_BUBBLES_LEVEL 11 // 13 because of check on levels after missile level

#define MAX_GHOST_COUNT_FOR_BUBBLES 3
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
