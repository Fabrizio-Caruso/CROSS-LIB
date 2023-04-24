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

#include "cross_lib.h"

extern uint8_t level;
#if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
extern uint16_t loop;
#endif
extern uint16_t skullSlowDown;
extern uint8_t skullXCountDown;
extern uint8_t skullYCountDown ;

extern uint16_t ghostLevel;

extern Character skull;
extern Character player;

extern uint8_t skullActive;

extern uint8_t isBossLevel;

#if defined(FULL_GAME)
	extern uint8_t confuseActive;
#endif


#if defined(FULL_GAME)

extern uint16_t levelSlowDown;


uint16_t computeSkullSlowDown(void)
{
	if(loop<MAX_SKULL_LOOP)
	{
		#if defined(_XL_TURN_BASED)
			return levelSlowDown - ghostLevel*8;				
		#else
			return levelSlowDown - ghostLevel*16;		
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

#define SKULL_RAND_CONDITION (_XL_RAND()>skullSlowDown)

void handle_skull(void)
{
	if(!skullActive)
	{		
		#if defined(FULL_GAME)
		if(((!isBossLevel && NON_BOSS_TRIGGER_REACHED) || 
			 (isBossLevel && BOSS_LOOP_TRIGGER_REACHED)))
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
			// _XL_TOCK_SOUND();
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
        #if defined(FULL_GAME)
        else
        {
            displayCharacter(&skull);
        }
        #endif
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

