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
#include "character.h"
#include "strategy.h"


extern unsigned char level;
extern unsigned char ghostCount;
extern unsigned short invincibleSlowDown;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern unsigned char strategyArray[GHOSTS_NUMBER];

// TODO: Design issue: we delete the invincible enemy
// This should be made generic even though it works
void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(hunterPtr->_x<preyPtr->_x)
	{
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		--hunterPtr->_x;
	}
	else if(hunterPtr->_y<preyPtr->_y)
	{
		++hunterPtr->_y;
	}
	else
	{
		--hunterPtr->_y;
	}
}


void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
{
    if(hunterPtr->_y<preyPtr->_y)
	{
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		--hunterPtr->_y;
	}
	else if(hunterPtr->_x<preyPtr->_x)
	{
		++hunterPtr->_x;
	}
	else 
	{
		--hunterPtr->_x;
	}
}

// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
void moveTowardCharacter(Character *hunterPtr, Character *preyPtr, unsigned char strategy)
{
	if(rand()%10 > strategy) // Select blind chase strategy
		{ // 0 - 4
			blindChaseCharacterXStrategy(hunterPtr, preyPtr);
		}
		else
		{ // 5 - 9
			blindChaseCharacterYStrategy(hunterPtr, preyPtr);
		}
}


void computeStrategy(void)
{
	unsigned char i;	
	for(i=1; i<GHOSTS_NUMBER-1; ++i) // 8,0,0
	{
		strategyArray[i] = 4; // no preference (approximate straight line)
	}	
	strategyArray[0] = 2;
	strategyArray[GHOSTS_NUMBER-1] = 6;
}

// Ghosts move to new positions if they get their chanche
void chasePlayer(unsigned short slowDown)
{
	unsigned char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((ghosts[i]._status) && (rand()>slowDown))
		{
			DELETE_GHOST(ghosts[i]._x,ghosts[i]._y,ghosts[i]._imagePtr);
			moveTowardCharacter(&ghosts[i], &player, strategyArray[i]);
		}
	}
}
