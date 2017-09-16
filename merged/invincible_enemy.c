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
 
#include "settings.h"
#include "invincible_enemy.h"
#include "level.h"

extern unsigned char level;
extern unsigned short loop;

extern unsigned short invincibleSlowDown;
extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown ;
extern unsigned short invincibleLoopTrigger;

extern unsigned short ghostLevel;


unsigned short computeInvincibleSlowDown(void)
{
	if(loop<1500)
	{
		return INITIAL_SKULL_SLOWDOWN - (level/4) * 1000 - ghostLevel*16;
	}
	return 0; // You must die!
}

void computeInvincibleGhostParameters(void)
{
	invincibleSlowDown = computeInvincibleSlowDown();
	invincibleXCountDown = INVINCIBLE_COUNT_DOWN;
	invincibleYCountDown = INVINCIBLE_COUNT_DOWN;
	invincibleLoopTrigger = INVINCIBLE_LOOP_TRIGGER;	
}
