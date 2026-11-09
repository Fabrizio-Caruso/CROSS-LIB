/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
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

// #define DEBUG_STRATEGY
// #define DEBUG_ITEMS
// #define DEBUG_END
// #define DEBUG_SECRETS
// #define DEBUG_LEVEL
// #define DEBUG_NO_SKULL
// #define SECRET_LEVEL

// #define DEBUG_EASY_BOSS_LEVEL

#if !defined(_GAME_SETTINGS)
#define _GAME_SETTINGS

#include "cross_lib.h"

#include "speed_game_settings.h"

#if !defined(INITIAL_LEVEL)
    #define INITIAL_LEVEL 1
#endif

// Final level 
#define FINAL_LEVEL 16

#define SKULLS_NUMBER 4

#if !defined(BULLETS_NUMBER)
    #define BULLETS_NUMBER 5
#endif

#define NUMBER_OF_GHOSTS_ON_PART_II 40

#define SECOND_ROUND_GHOST_COUNT_INCREASE 0

#define INITIAL_BULLET_COLOR _XL_WHITE
#define FIRST_THRESHOLD_BULLET_COLOR _XL_GREEN
#define SECOND_THRESHOLD_BULLET_COLOR _XL_RED

//#define FAST_GHOST_COUNT_MAX_THRESHOLD 1

#define COLOR_BULLET_FIRST_THRESHOLD 4
#define COLOR_BULLET_SECOND_THRESHOLD 6

#define FAST_GHOST_GUN_THRESHOLD 9
#define FAST_GHOST_COUNT_MIN_THRESHOLD (FIRST_LEVEL_GHOSTS_NUMBER+4)
// #define FAST_GHOST_LEVEL_THRESHOLD 4

#define MAX_GUNS 99

#define SECRET_GUNS 20

#define BULLET_GUNS 30

#define SKULL_BOMB_DAMAGE 25

#define CONFUSE_DAMAGE 20

#define MAX_TURN_BASES_LOOP 400

#define BOMBS_NUMBER 3


#if !defined(GHOSTS_NUMBER)
    #if defined(WIDE)
        #define GHOSTS_NUMBER 10
    #else
        #define GHOSTS_NUMBER 9
    #endif
#endif

#if !defined(GHOST_DECREASE)
    #define GHOST_DECREASE 1
#endif

// Bonus for completing the game =  number of lives left X LIVES_LEFT_BONUS
#define LIVES_LEFT_BONUS 2000U

#define FAST_GHOST_EXTRA_POINTS 20U

#define BOSS_EXTRA_POINTS 100U

#define BOSS_LEVEL_SKULL_EXTRA_POINTS 50U

#define FIRST_LEVEL_GHOSTS_NUMBER 10

// 65 is the max value that does not exceed 1000 points
// bonus = 20 + 25 * [level] + 15 * [ghosts left]
// level  0: 20 + 25* 0 + 15*65 = 20 +   0 + 975 = 995
// level 15: 20 + 25*15 + 15*40 = 20 + 375 + 600 = 995
// level 16: 20 + 25*16 + 15*10 = 20 + 400 + 150 = 570
#define ZERO_LEVEL_GHOSTS_NUMBER 65

#if !defined(MAX_ROCKETS_NUMBER)
	#if XSize<=22
		#define MAX_ROCKETS_NUMBER 4
	#elif XSisize<=32
		#define MAX_ROCKETS_NUMBER 5
	#else 
		#define MAX_ROCKETS_NUMBER 6
	#endif
#endif

#define MISSILE_DESTROYER_TRIGGER 3
#define BOMB_DESTROYER_TRIGGER 6

#define DESTROYER_COUNT_DOWN 120

#define LIVES_NUMBER 5
#define GUNS_NUMBER 40

#define EXTRA_LIFE_THROUGH_POINTS 5000

#define HORIZONTAL_MISSILE_BONUS 30U

#define VERTICAL_MISSILE_BONUS 20U

#define SKULL_POINTS 50U

#define CALM_DOWN_SECRET_THRESHOLD 3
#define EXTRA_POINTS_SECRET_THRESHOLD 4
#define FREEZE_SECRET_THRESHOLD 5

#define SECRET_LEVEL_THRESHOLD 7

#define FIRE_POWER_ITEM_THRESHOLD 4
#define FIRE_POWER_LEVEL_THRESHOLD 7

// #define FAST_GHOST_FIRE_POWER_THRESHOLD 3

// Points given by extra points bonus: 
// EXTRA_POINTS + level X EXTRA_POINTS_LEVEL_INCREASE
#define EXTRA_POINTS 10U 
#define EXTRA_POINTS_LEVEL_INCREASE 5U
#define SECRET_LEVEL_EXTRA_POINTS 25U

// Points for shooting a ghost
#define GHOST_VS_MISSILE 5U

// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 15U

#define GHOST_VS_WALL_BONUS 30U

#define GHOSTS_VS_SUICIDE_BONUS 10U

#define GHOSTS_LEFT_BONUS 15U

// Extra points for fire charge (more bullets)
#define CALM_DOWN_BONUS 10U

// Extra points for fire charge (more bullets)
#define FIRE_CHARGE_BONUS 20U

// Extra points for the fire power (more powerful bullets)
#define FIRE_POWER_BONUS 30U

#define FREEZE_BONUS 15U

#define BASE_LEVEL_BONUS 20U

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 25U

#if defined(DEBUG_EASY_BOSS_LEVEL)
    #define MIN_SKULL_HITS 10
    #define BOSS_HITS 30
    #define GHOST_LIFE 5
    #define FAST_GHOST_LIFE ((GHOST_LIFE)/4)

#else
    #if defined(WIDE)
        #define MIN_SKULL_HITS 60
        #define BOSS_HITS 254
        #define GHOST_LIFE 30
        #define FAST_GHOST_LIFE ((GHOST_LIFE)/2)
    #else
        #define MIN_SKULL_HITS 40
        #define BOSS_HITS 170
        #define GHOST_LIFE 20
        #define FAST_GHOST_LIFE ((GHOST_LIFE)/4)
    #endif    
#endif

#define BOSS_INDEX 2


#define GHOST_SLOW_DOWN_THRESHOLD 5

#if defined(WIDE) 
    #define BOSS_LEVEL_SKULL_INCREASE 5
    #define BOSS_LEVEL_GHOSTS_NUMBER GHOSTS_NUMBER
#else
    #define BOSS_LEVEL_SKULL_INCREASE 4
    #define BOSS_LEVEL_GHOSTS_NUMBER (GHOSTS_NUMBER-3)
#endif


#if !defined(INITIAL_GHOST_SLOWDOWN)
    #if defined(WIDE)
        #define INITIAL_GHOST_SLOWDOWN 27000
    #else
        #define INITIAL_GHOST_SLOWDOWN 28000
    #endif
#endif

#if defined(WIDE)
    #define FAST_GHOST_SLOWDOWN 15000
#else
    #define FAST_GHOST_SLOWDOWN 19000
#endif

#define FIRST_HORIZONTAL_WALLS_LEVEL 6

#define FIRST_MOVING_BOMBS_LEVEL 11

#if XSize>=32
    #define RELOCATE_RANGE 7
    #define HORIZONTAL_WALLS_INITIAL_LENGTH 2
    #define HORIZONTAL_WALLS_INCREASE_LOOP 200
#else
    #define RELOCATE_RANGE 5
    #define HORIZONTAL_WALLS_INITIAL_LENGTH 1
    #define HORIZONTAL_WALLS_INCREASE_LOOP 300
#endif

#define FIRST_ROCKETS_LEVEL 7 // 8 because of check on levels after missile level

// Maximum number of ghost that co-exist with rockets (in non-boss levels)
#define MAX_GHOST_COUNT_FOR_ROCKETS 4

#if XSize>=32
    #define HORIZONTAL_MISSILE_OFFSET 3
#else
    #define HORIZONTAL_MISSILE_OFFSET 2
#endif

// -----------------------------------------------------------------------------------

#if GHOSTS_NUMBER>=8
    #define SKULL_TRIGGER 2
#else
    #define SKULL_TRIGGER 2
#endif 

    
#define FIRE_CHARGE_COOL_DOWN 110

#define BOMB_CHARGE_COOL_DOWN 400

#define CALM_DOWN_COOL_DOWN 240


#define FREEZE_COOL_DOWN 320

#define FIRE_POWER_COOL_DOWN 190

#define SUPER_COOL_DOWN 160

#define CONFUSE_COOL_DOWN 800
#define SECOND_CONFUSE_COOL_DOWN 20000

#define SUICIDE_COOL_DOWN 120
#define SECOND_SUICIDE_COOL_DOWN 800

#define CALM_DOWN_EFFECT_FACTOR 50

#define EXTRA_POINTS_COOL_DOWN 2000
#define SECOND_EXTRA_POINTS_COOL_DOWN 20000

#define INVINCIBILITY_COOL_DOWN 3300
#define SECRET_LEVEL_INVINCIBILITY_COOL_DOWN 100

#define EXTRA_LIFE_COOL_DOWN 1400

#define FREEZE_COUNT_DOWN 28

#define INITIAL_FROZEN_COUNT_DOWN FROZEN_COUNT_DOWN

#define CONFUSE_COUNT_DOWN 100

#define INVINCIBILITY_COUNT_DOWN 88

#define INITIAL_INVINCIBILITY_COUNT_DOWN (INVINCIBILITY_COUNT_DOWN/2)

#define SUICIDE_COUNT_DOWN 200

#define DESTROYER_COOL_DOWN 32000

#define SUICIDE_BONUS 5

#define SKULL_LOOP_TRIGGER 40

#define SKULL_COUNT_DOWN 130

#if defined(WIDE)
    #define SKULL_MIN_SLOWDOWN 100
    #define GHOST_MIN_SLOWDOWN 500
#else
    #define SKULL_MIN_SLOWDOWN 8000
    #define GHOST_MIN_SLOWDOWN 6000
#endif


#if defined(_XL_TURN_BASED)
    #define MAX_GHOST_LEVEL_SCALE 3
    #define MAX_SKULL_LOOP_SCALE 3
#else
    #define MAX_GHOST_LEVEL_SCALE 1
    #define MAX_SKULL_LOOP_SCALE 1
#endif

#define MAX_GHOST_LEVEL (1400/MAX_GHOST_LEVEL_SCALE)

#define MAX_SKULL_LOOP (2000/MAX_SKULL_LOOP_SCALE)

#define TURN_BASED_MAX_LOOP MAX_SKULL_LOOP

#define GHOST_LEVEL_DECREASE (MAX_GHOST_LEVEL/560)

#define MISSILES_FOR_SUICIDE 5

#define SKULLS_FOR_SUPER 2


#endif // _GAME_SETTINGS
