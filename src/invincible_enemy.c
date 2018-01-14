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
#include "invincible_enemy.h"
#include "level.h"
#include "display_macros.h"
#include "sound_macros.h"
#include "strategy.h"

extern unsigned char level;
extern unsigned short loop;

extern unsigned short invincibleSlowDown;
extern unsigned char invincibleXCountDown;
extern unsigned char invincibleYCountDown ;

extern unsigned short ghostLevel;

extern Character invincibleGhost;
extern Character player;

extern unsigned char invincibleGhostAlive;
extern unsigned char invincibleGhostCountTrigger;
extern unsigned short invincibleLoopTrigger;

#if defined(FULL_GAME)
	extern unsigned char confuseActive;
#endif

unsigned short computeInvincibleSlowDown(void)
{
	if(loop<MAX_INVINCIBLE_LOOP)
	{
		return INITIAL_SKULL_SLOWDOWN - level * 250 - ghostLevel*16;		
	}
	return INVINCIBLE_MIN_SLOWDOWN; // You must die!
}

void computeInvincibleGhostParameters(void)
{
	invincibleSlowDown = computeInvincibleSlowDown();
	invincibleXCountDown = SKULL_COUNT_DOWN;
	invincibleYCountDown = SKULL_COUNT_DOWN;
}


void handle_invincible_ghost(void)
{
	if(!invincibleGhost._status)
	{
		// Manage invincible ghost
		
		#if defined(FULL_GAME)
		if((!bossLevel() && invincibleGhostAlive &&
							((invincibleXCountDown==0)|| (invincibleYCountDown==0) || (loop>=INVINCIBLE_LOOP_TRIGGER) || (ghostCount<=invincibleGhostCountTrigger))) || 
		   (bossLevel() && loop>=INVINCIBLE_LOOP_TRIGGER))
		#else
		if(invincibleGhostAlive &&
							((invincibleXCountDown==0)     || (invincibleYCountDown==0) || 
							 (loop>=INVINCIBLE_LOOP_TRIGGER) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER)))
		#endif
		{
			invincibleGhost._status = 1;
			DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
		}
		else
		{
			--invincibleXCountDown;
			--invincibleYCountDown;
		}
	}
	else
	{ 	
		invincibleSlowDown = computeInvincibleSlowDown();

		if(rand()>invincibleSlowDown)
		{
			TOCK_SOUND();
			DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y,invincibleGhost.imagePtr);
			#if defined(FULL_GAME)
				if(!confuseActive || loop&3)
				{
					moveTowardCharacter(&player, &invincibleGhost, 4);
				}
			#else
			moveTowardCharacter(&invincibleGhost, 4);
			#endif
		}
		DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
		#if defined(FULL_GAME)
		if (playerKilledBy(&invincibleGhost))
		#else
		if(areCharctersAtSamePosition(&invincibleGhost, &player))
		#endif
		{
			playerDies();
		}
	}
}


#endif

