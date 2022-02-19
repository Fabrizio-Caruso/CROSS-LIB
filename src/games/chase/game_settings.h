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

#if !defined(_GAME_SETTINGS)
#define _GAME_SETTINGS

// #define DEBUG_END
// #define DEBUG_ITEMS
// #define DEBUG_ITEMS_IN_GAME

#include "cross_lib.h"

#include "speed_game_settings.h"

#if !defined(FORCE_LEVEL)
	#define INITIAL_LEVEL 1
#else
	#define INITIAL_LEVEL FORCE_LEVEL
#endif

// Final level 
#define FINAL_LEVEL 20

#define MAX_TURN_BASES_LOOP 400

#if defined(FORCE_BOMBS_NUMBER)
	#define BOMBS_NUMBER FORCE_BOMBS_NUMBER
#else
	#define BOMBS_NUMBER 4
#endif
	
#if defined(FORCE_GHOSTS_NUMBER)
	#define GHOSTS_NUMBER FORCE_GHOSTS_NUMBER
#elif defined(TURN_BASED)
	#if !defined(TINY_GAME)
		#define GHOSTS_NUMBER 9
	#else
		#define GHOSTS_NUMBER 8
	#endif
#else	
	#if XSize>=32 && YSize>=20
		#define GHOSTS_NUMBER 9
	#elif XSize>=16 && YSize>=16
		#define GHOSTS_NUMBER 8
	#else
		#define GHOSTS_NUMBER 6
	#endif
#endif

#if !defined(ROCKETS_NUMBER)
    #define ROCKETS_NUMBER 4
#endif

#define LIVES_NUMBER 5
#define GUNS_NUMBER 3

#define EXTRA_LIFE_THROUGH_POINTS 2000

#define HORIZONTAL_MISSILE_BONUS 50

#define VERTICAL_MISSILE_BONUS 20

#define SKULL_POINTS 200;

// Points given by extra points bonus: 
// EXTRA_POINTS + level X EXTRA_POINTS_LEVEL_INCREASE
#define EXTRA_POINTS 100
#define EXTRA_POINTS_LEVEL_INCREASE 5

// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 25

// Points for shooting a ghost
#define GHOST_VS_MISSILE 10

// Extra points for the power up
#define POWER_UP_BONUS 20

// Extra points for the power up
#define GUN_BONUS 30

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 100

#if defined(WIDE) || defined(FORCE_NARROW)
	#define NON_BOSS_SKULL_HITS 4
#else
	#define NON_BOSS_SKULL_HITS 3
#endif	

#define BOSS_SKULL_HITS (NON_BOSS_SKULL_HITS + 4)



#if defined(INITIAL_SLOWDOWN)
	#define INITIAL_GHOST_SLOWDOWN INITIAL_SLOWDOWN
#elif defined(TURN_BASED)
	#define INITIAL_GHOST_SLOWDOWN 16000
#else
	#define INITIAL_GHOST_SLOWDOWN 28000
#endif


#define FIRST_HORIZONTAL_WALLS_LEVEL 6

#define FIRST_MOVING_BOMBS_LEVEL 11

#if defined(WIDE)
	#define RELOCATE_RANGE 7
	#define HORIZONTAL_WALLS_INITIAL_LENGTH 4
	#define HORIZONTAL_WALLS_INCREASE_LOOP 200
#else
	#define RELOCATE_RANGE 5
	#define HORIZONTAL_WALLS_INITIAL_LENGTH 2
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
	#define SKULL_TRIGGER 4
#else
	#define SKULL_TRIGGER 3
#endif 

#if !defined(TURN_BASED_EFFECT_SCALE)
    #define TURN_BASED_EFFECT_SCALE 5
#endif

#if !defined(TURN_BASED_WAIT_SCALE)
    #define TURN_BASED_WAIT_SCALE 4
#endif 
#define ACTION_EFFECT_SCALE 1
#define ACTION_WAIT_SCALE 1

#if defined(TURN_BASED)
	#define EFFECT_SCALE TURN_BASED_EFFECT_SCALE
	#define WAIT_SCALE TURN_BASED_WAIT_SCALE
#else
	#define EFFECT_SCALE ACTION_EFFECT_SCALE
	#define WAIT_SCALE ACTION_WAIT_SCALE
#endif
	
#define POWER_UP_COOL_DOWN (150/WAIT_SCALE)

#define POWER_UP2_COOL_DOWN (400/WAIT_SCALE)

#define FREEZE_COOL_DOWN (700/WAIT_SCALE)

#define GUN_COOL_DOWN (200/WAIT_SCALE)	

#define SUPER_COOL_DOWN (150/WAIT_SCALE)

#define CONFUSE_COOL_DOWN (250/WAIT_SCALE)
#define SECOND_CONFUSE_COOL_DOWN 20000

#define ZOMBIE_COOL_DOWN (600/WAIT_SCALE)
#define SECOND_ZOMBIE_COOL_DOWN 20000

#define CHASE_COOL_DOWN (400/WAIT_SCALE)

#define EXTRA_POINTS_COOL_DOWN (2000/WAIT_SCALE)
#define SECOND_EXTRA_POINTS_COOL_DOWN 20000

#define INVINCIBILITY_COOL_DOWN (3300/WAIT_SCALE)

#define EXTRA_LIFE_COOL_DOWN (1300/WAIT_SCALE)

#define FROZEN_COUNT_DOWN (22/EFFECT_SCALE)

#define INITIAL_FROZEN_COUNT_DOWN FROZEN_COUNT_DOWN

#define CONFUSE_COUNT_DOWN (240/EFFECT_SCALE)

#define INVINCIBILITY_COUNT_DOWN (120/EFFECT_SCALE)

#define INITIAL_INVINCIBILITY_COUNT_DOWN (INVINCIBILITY_COUNT_DOWN/2)

#define ZOMBIE_COUNT_DOWN (255/EFFECT_SCALE)

#define ZOMBIE_BONUS 5

#define TURN_BASED_SKULL_LOOP_TRIGGER_SCALE 4
#define ACTION_SKULL_LOOP_TRIGGER_SCALE 1

#if defined(TURN_BASED)
	#define SKULL_LOOP_TRIGGER_SCALE TURN_BASED_SKULL_LOOP_TRIGGER_SCALE
#else
	#define SKULL_LOOP_TRIGGER_SCALE ACTION_SKULL_LOOP_TRIGGER_SCALE
#endif

#define SKULL_LOOP_TRIGGER ((280-(level<<2))/SKULL_LOOP_TRIGGER_SCALE)

#define SKULL_COUNT_DOWN 50

#if !defined(TURN_BASED_SKULL_MIN_SLOWDOWN_SCALE)
    #define TURN_BASED_SKULL_MIN_SLOWDOWN_SCALE 4
#endif

#if !defined(TURN_BASED_GHOST_MIN_SLOWDOWN_SCALE)
    #define TURN_BASED_GHOST_MIN_SLOWDOWN_SCALE 3
#endif

#define ACTION_SKULL_MIN_SLOWDOWN_SCALE 1
#define ACTION_GHOST_MIN_SLOWDOWN_SCALE 1

#if defined(TURN_BASED)
	#define SKULL_MIN_SLOWDOWN_SCALE TURN_BASED_SKULL_MIN_SLOWDOWN_SCALE
	#define GHOST_MIN_SLOWDOWN_SCALE TURN_BASED_GHOST_MIN_SLOWDOWN_SCALE
#else
	#define SKULL_MIN_SLOWDOWN_SCALE ACTION_SKULL_MIN_SLOWDOWN_SCALE
	#define GHOST_MIN_SLOWDOWN_SCALE ACTION_GHOST_MIN_SLOWDOWN_SCALE	
#endif

#if !defined(SKULL_MIN_SLOWDOWN)
    #define SKULL_MIN_SLOWDOWN (3200/SKULL_MIN_SLOWDOWN_SCALE)
#endif

#if !defined(GHOST_MIN_SLOWDOWN)
    #define GHOST_MIN_SLOWDOWN (1600/GHOST_MIN_SLOWDOWN_SCALE)	
#endif

#if defined(TURN_BASED)
	#define MAX_GHOST_LEVEL_SCALE 3
	#define MAX_SKULL_LOOP_SCALE 3
	#define MAX_GHOST_LOOP_SCALE 3
#else
	#define MAX_GHOST_LEVEL_SCALE 1
	#define MAX_SKULL_LOOP_SCALE 1
	#define MAX_GHOST_LOOP_SCALE 1
#endif

#define MAX_GHOST_LEVEL (600/MAX_GHOST_LEVEL_SCALE)

#define MAX_SKULL_LOOP (1600/MAX_SKULL_LOOP_SCALE)

#define MAX_GHOST_LOOP (1800/MAX_GHOST_LOOP_SCALE)

#define TURN_BASED_MAX_LOOP MAX_SKULL_LOOP

#define GHOST_LEVEL_DECREASE (MAX_GHOST_LEVEL/20)

#define MISSILES_FOR_ZOMBIE 5

#define SKULLS_FOR_SUPER 2

#endif // _GAME_SETTINGS
