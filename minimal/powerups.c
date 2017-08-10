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
 
 
#include "character.h"
#include "powerups.h"


extern unsigned char level;

extern unsigned short gunInitialCoolDown;

extern unsigned short extraPointsCoolDown;
extern unsigned short invincibilityCoolDown;
extern unsigned short extraLifeCoolDown;

unsigned short powerUpReached(Character * hunterPtr, Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr, preyPtr));
}


void computePowerUp(unsigned short *coolDownDecreasePtr, unsigned short *powerUpInitialCoolDownPtr)
{
	*coolDownDecreasePtr = 140-(level*2);
	*powerUpInitialCoolDownPtr = 200+(level*2);
}

unsigned short computeGunInitialCoolDown(void)
{
	return 180 + level * 2;
}

void reducePowerUpsCoolDowns(void)
{
		extraPointsCoolDown/=2;
		invincibilityCoolDown/=2;
		extraLifeCoolDown/=2;	
}	
