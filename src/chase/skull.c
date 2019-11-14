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
#if !defined(TINY_GAME)

#include "settings.h"

#include "skull.h"
#include "level.h"
#include "strategy.h"

extern uint8_t level;
extern uint16_t loop;

extern uint16_t skullSlowDown;
extern uint8_t skullXCountDown;
extern uint8_t skullYCountDown ;

extern uint16_t ghostLevel;

extern Character skull;
extern Character player;

extern uint8_t skullActive;

#if defined(FULL_GAME)
	extern uint8_t confuseActive;
#endif

#if defined(FULL_GAME)
uint16_t computeSkullSlowDown(void)
{
	if(loop<MAX_SKULL_LOOP)
	{
		#if defined(TURN_BASED)
			return INITIAL_SKULL_SLOWDOWN - level * 256 - ghostLevel*8;				
		#else
			return INITIAL_SKULL_SLOWDOWN - level * 256 - ghostLevel*16;		
		#endif
	}
	return SKULL_MIN_SLOWDOWN; // You must die!
}
#endif


// Redability macros
#define INACTIVITY_COUNT_DOWN_REACHED ((skullXCountDown==0) || (skullYCountDown==0))
#define GHOST_COUNT_TRIGGER_REACHED (ghostCount<=SKULL_TRIGGER)
#define BOSS_LOOP_TRIGGER_REACHED (loop>=SKULL_LOOP_TRIGGER)

#define NON_BOSS_TRIGGER_REACHED (INACTIVITY_COUNT_DOWN_REACHED || GHOST_COUNT_TRIGGER_REACHED)

#if defined(__NCURSES__)
	#define SKULL_RAND_CONDITION ((rand()&0x7fff)>skullSlowDown)
#else
	#define SKULL_RAND_CONDITION (rand()>skullSlowDown)
#endif

void handle_skull(void)
{
	if(!skullActive)
	{		
		#if defined(FULL_GAME)
		if(((!bossLevel() && NON_BOSS_TRIGGER_REACHED) || 
			 (bossLevel() && BOSS_LOOP_TRIGGER_REACHED)))
		#else
		if(NON_BOSS_TRIGGER_REACHED)
		#endif
		{
			skullActive = 1;
		}
		else
		{
			--skullXCountDown;
			--skullYCountDown;
		}
	}
	else
	{ 	
		#if defined(FULL_GAME)
			skullSlowDown = computeSkullSlowDown();
		#else
			--skullSlowDown;
		#endif

		if(SKULL_RAND_CONDITION)
		{
			TOCK_SOUND();
			deleteSkull(&skull);
			#if defined(FULL_GAME)
				if(!confuseActive || !(loop&3))
				{
					#if !defined(SIMPLE_STRATEGY)
						moveTowardCharacter(&player, &skull, 4);
					#else
						moveTowardCharacter(&player, &skull);						
					#endif
				}
			#else
				moveTowardCharacter(&skull);
			#endif
		}
		displaySkull(&skull);
		#if defined(FULL_GAME)
		if (playerKilledBy(&skull))
		#else
		if(areCharctersAtSamePosition(&skull, &player))
		#endif
		{
			playerDies();
		}
	}
}


#endif

