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

extern unsigned short level;
extern unsigned int loop;

extern unsigned int invincibleSlowDown;
extern unsigned short invincibleXCountDown;
extern unsigned short invincibleYCountDown ;
extern unsigned int invincibleLoopTrigger;
extern unsigned short invincibleGhostCountTrigger;

int computeInvincibleCountDown(void)
{
	return 90 - level*2;
}


int computeInvincibleSlowDown(void)
{
	if(loop<1000)
	{
		return 25500 - (level/4) * 1000 - loop*4;
	}
	else if(loop<1200)
	{
		return 7000;
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
	if(level==FINAL_LEVEL)
		return 150;
	else if (level==FINAL_LEVEL - 1)
		return 180;
	else if (level==FINAL_LEVEL - 2)
		return 200;
	else
		return 760 - level*15;
}

void computeInvincibleGhostParameters(void)
{
	invincibleSlowDown = computeInvincibleSlowDown();
	invincibleXCountDown = computeInvincibleCountDown();
	invincibleYCountDown = computeInvincibleCountDown();
	invincibleGhostCountTrigger = computeInvincibleGhostCountTrigger();
	invincibleLoopTrigger = computeInvincibleLoopTrigger();	
}
