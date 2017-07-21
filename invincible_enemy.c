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
 
#include "settings.h"
#include "invincible_enemy.h"
#include "level.h"

extern unsigned short level;
extern unsigned int loop;

extern unsigned int invincibleSlowDown;
extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown ;
extern unsigned int invincibleLoopTrigger;
extern unsigned short invincibleGhostCountTrigger;

extern unsigned int ghostLevel;

int computeInvincibleCountDown(void)
{
	return 90 - level*2;
}


int computeInvincibleSlowDown(void)
{
	if(loop<1500)
	{
		return 29000 - (level/4) * 1000 - loop*2 - ghostLevel*16;
	}
	else if(loop<2000)
	{
		if(!bossLevel())
		{
			return 26000 - (level/4) * 1000;
		}
		else
		{
			return 26000 - (level/4) * 1000 - ghostLevel*16;
		}
	}
	return 0; // You must die!
}

int computeInvincibleGhostCountTrigger(void)
{
	if(level<=12)
		return level/4 + 1;
	else
		return 4;
}

int computeInvincibleLoopTrigger(void)
{
	if(bossLevel())
		return 150 - (level/4)*10;
	else
		return 700 - level*15;
}

void computeInvincibleGhostParameters(void)
{
	invincibleSlowDown = computeInvincibleSlowDown();
	invincibleXCountDown = computeInvincibleCountDown();
	invincibleYCountDown = computeInvincibleCountDown();
	invincibleGhostCountTrigger = computeInvincibleGhostCountTrigger();
	invincibleLoopTrigger = computeInvincibleLoopTrigger();	
}
