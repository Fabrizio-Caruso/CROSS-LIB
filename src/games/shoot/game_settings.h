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

#if !defined(_GAME_SETTINGS)
#define _GAME_SETTINGS

#include "../cross_lib/cross_lib.h"

#include "speed_game_settings.h"

#if !defined(INITIAL_LEVEL)
    #define INITIAL_LEVEL 1
#endif

// Final level 
#define FINAL_LEVEL 16

#define SKULLS_NUMBER 4

#define BULLETS_NUMBER 6

#define MAX_GUNS 99

#define SECRET_GUNS 20

#define BULLET_GUNS 30

#define SKULL_BOMB_THRESHOLD 7
#define SKULL_BOMB_EXTRA_DAMAGE_THRESHOLD 64
#define SKULL_BOMB_NORMAL_DAMAGE 30

#define MAX_TURN_BASES_LOOP 400

#if defined(FORCE_BOMBS_NUMBER)
    #define BOMBS_NUMBER FORCE_BOMBS_NUMBER
#else
    #define BOMBS_NUMBER 3
#endif

#if !defined(GHOSTS_NUMBER)
    #if defined(WIDE) || defined(FORCE_NARROW)
        #define GHOSTS_NUMBER 10
    #else
        #define GHOSTS_NUMBER 8
    #endif
#endif

#define ROCKETS_NUMBER 4

#define MISSILE_DESTROYER_TRIGGER 3
#define BOMB_DESTROYER_TRIGGER 6

#define DESTROYER_COUNT_DOWN 220

#define LIVES_NUMBER 5
#define GUNS_NUMBER 40

#define BACKGROUND_COLOR COLOR_BLACK

#define BORDER_COLOR COLOR_BLACK

#define EXTRA_LIFE_THROUGH_POINTS 5000

#define HORIZONTAL_MISSILE_BONUS 30UL

#define VERTICAL_MISSILE_BONUS 20UL

#define SKULL_POINTS 50UL;

#define CALM_DOWN_SECRET_THRESHOLD 2
#define EXTRA_POINTS_SECRET_THRESHOLD 3
#define FREEZE_SECRET_THRESHOLD 4

#define FIRE_POWER_ITEM_THRESHOLD 4
#define FIRE_POWER_LEVEL_THRESHOLD 7

// Points given by extra points bonus: 
// EXTRA_POINTS + level X EXTRA_POINTS_LEVEL_INCREASE
#define EXTRA_POINTS 10UL 
#define EXTRA_POINTS_LEVEL_INCREASE 5UL

// Points for shooting a ghost
#define GHOST_VS_MISSILE 5UL

// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 15UL

#define GHOST_VS_WALL_BONUS 30UL

#define GHOSTS_VS_SUICIDE_BONUS 10UL

// Extra points for fire charge (more bullets)
#define CALM_DOWN_BONUS 10UL

// Extra points for fire charge (more bullets)
#define FIRE_CHARGE_BONUS 20UL

// Extra points for the fire power (more powerful bullets)
#define FIRE_POWER_BONUS 30UL

#define FREEZE_BONUS 15UL

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 25U

#if defined(WIDE) || defined(FORCE_NARROW)
    #define MIN_SKULL_HITS (60)
    #define BOSS_HITS 254
    #define GHOST_LIFE (30)
#else
    #define MIN_SKULL_HITS (40)
    #define BOSS_HITS 160
    #define GHOST_LIFE (20)
#endif    

#define BOSS_INDEX 2


#define GHOST_SLOW_DOWN_THRESHOLD 5

#if defined(WIDE) || defined(FORCE_NARROW)
    #define BOSS_LEVEL_SKULL_INCREASE 6
    #define BOSS_LEVEL_GHOSTS_NUMBER 8
#else
    #define BOSS_LEVEL_SKULL_INCREASE 4
    #define BOSS_LEVEL_GHOSTS_NUMBER 5
#endif


#if !defined(INITIAL_GHOST_SLOWDOWN)
    #if defined(TURN_BASED)
        #define INITIAL_GHOST_SLOWDOWN 16000
    #elif defined(__PC6001__) || defined(__GAMATE__) || defined(__VZ__) || defined(__CBM610__) || defined(__VIC20__) || defined(__ATARI5200__) || ((defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1)) || defined(__APPLE2__) || defined(__APPLE2ENH__)
        #define INITIAL_GHOST_SLOWDOWN 32000
    #elif defined(__VG5K__) || defined(__MSX__) || defined(__WINCMOC__) || defined(__CMOC__)
        #define INITIAL_GHOST_SLOWDOWN 30000
    #else
        #define INITIAL_GHOST_SLOWDOWN 31000
    #endif
#endif


#define FIRST_HORIZONTAL_WALLS_LEVEL 6

#define FIRST_MOVING_BOMBS_LEVEL 11

#if defined(WIDE)
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

#if defined(WIDE)
    #define HORIZONTAL_MISSILE_OFFSET 3
#else
    #define HORIZONTAL_MISSILE_OFFSET 2
#endif

// -----------------------------------------------------------------------------------
#if GHOSTS_NUMBER>=8
    #define SKULL_TRIGGER 1
#else
    #define SKULL_TRIGGER 1
#endif 

#define TURN_BASED_EFFECT_SCALE 5
#define TURN_BASED_WAIT_SCALE 4
#define ACTION_EFFECT_SCALE 1
#define ACTION_WAIT_SCALE 1

#if defined(TURN_BASED)
    #define EFFECT_SCALE TURN_BASED_EFFECT_SCALE
    #define WAIT_SCALE TURN_BASED_WAIT_SCALE
#else
    #define EFFECT_SCALE ACTION_EFFECT_SCALE
    #define WAIT_SCALE ACTION_WAIT_SCALE
#endif
    
#define FIRE_CHARGE_COOL_DOWN (110/WAIT_SCALE)

#define BOMB_CHARGE_COOL_DOWN (400/WAIT_SCALE)

#define CALM_DOWN_COOL_DOWN (240/WAIT_SCALE)


#define FREEZE_COOL_DOWN (320/WAIT_SCALE)

#define FIRE_POWER_COOL_DOWN (190/WAIT_SCALE)    

#define SUPER_COOL_DOWN (150/WAIT_SCALE)

#define CONFUSE_COOL_DOWN (800/WAIT_SCALE)
#define SECOND_CONFUSE_COOL_DOWN 20000

#define SUICIDE_COOL_DOWN (120/WAIT_SCALE)
#define SECOND_SUICIDE_COOL_DOWN 800

#define CALM_DOWN_EFFECT_FACTOR 30

#define EXTRA_POINTS_COOL_DOWN (2000/WAIT_SCALE)
#define SECOND_EXTRA_POINTS_COOL_DOWN 20000

#define INVINCIBILITY_COOL_DOWN (3300/WAIT_SCALE)

#define EXTRA_LIFE_COOL_DOWN (1400/WAIT_SCALE)

#define FREEZE_COUNT_DOWN (32/EFFECT_SCALE)

#define INITIAL_FROZEN_COUNT_DOWN FROZEN_COUNT_DOWN

#define CONFUSE_COUNT_DOWN (70/EFFECT_SCALE)

#define INVINCIBILITY_COUNT_DOWN (100/EFFECT_SCALE)

#define INITIAL_INVINCIBILITY_COUNT_DOWN (INVINCIBILITY_COUNT_DOWN/2)

#define SUICIDE_COUNT_DOWN (200/EFFECT_SCALE)

#define DESTROYER_COOL_DOWN 32000

#define SUICIDE_BONUS 5

#define TURN_BASED_SKULL_LOOP_TRIGGER_SCALE 4
#define ACTION_SKULL_LOOP_TRIGGER_SCALE 1

#if defined(TURN_BASED)
    #define SKULL_LOOP_TRIGGER_SCALE TURN_BASED_SKULL_LOOP_TRIGGER_SCALE
#else
    #define SKULL_LOOP_TRIGGER_SCALE ACTION_SKULL_LOOP_TRIGGER_SCALE
#endif

#define SKULL_LOOP_TRIGGER ((100-(level<<2))/SKULL_LOOP_TRIGGER_SCALE)

#define SKULL_COUNT_DOWN 100

#define TURN_BASED_SKULL_MIN_SLOWDOWN_SCALE 4
#define TURN_BASED_GHOST_MIN_SLOWDOWN_SCALE 3
#define ACTION_SKULL_MIN_SLOWDOWN_SCALE 1
#define ACTION_GHOST_MIN_SLOWDOWN_SCALE 1

#if defined(TURN_BASED)
    #define SKULL_MIN_SLOWDOWN_SCALE TURN_BASED_SKULL_MIN_SLOWDOWN_SCALE
    #define GHOST_MIN_SLOWDOWN_SCALE TURN_BASED_GHOST_MIN_SLOWDOWN_SCALE
#else
    #define SKULL_MIN_SLOWDOWN_SCALE ACTION_SKULL_MIN_SLOWDOWN_SCALE
    #define GHOST_MIN_SLOWDOWN_SCALE ACTION_GHOST_MIN_SLOWDOWN_SCALE    
#endif

#if defined(WIDE)
    #define SKULL_MIN_SLOWDOWN (5000/SKULL_MIN_SLOWDOWN_SCALE)
    #define GHOST_MIN_SLOWDOWN (3000/GHOST_MIN_SLOWDOWN_SCALE)    
#else
    #define SKULL_MIN_SLOWDOWN (15000/SKULL_MIN_SLOWDOWN_SCALE)
    #define GHOST_MIN_SLOWDOWN (130000/GHOST_MIN_SLOWDOWN_SCALE)    
#endif


#if defined(TURN_BASED)
    #define MAX_GHOST_LEVEL_SCALE 3
    #define MAX_SKULL_LOOP_SCALE 3
#else
    #define MAX_GHOST_LEVEL_SCALE 1
    #define MAX_SKULL_LOOP_SCALE 1
#endif

#define MAX_GHOST_LEVEL (1400/MAX_GHOST_LEVEL_SCALE)

#define MAX_SKULL_LOOP (2200/MAX_SKULL_LOOP_SCALE)

#define TURN_BASED_MAX_LOOP MAX_SKULL_LOOP

#define GHOST_LEVEL_DECREASE (MAX_GHOST_LEVEL/100)

#define MISSILES_FOR_SUICIDE 5

#define SKULLS_FOR_SUPER 2


#endif // _GAME_SETTINGS
