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


extern unsigned short level;
extern unsigned short ghostCount;
extern unsigned int invincibleSlowDown;

extern Character player; 
extern Character* ghosts[GHOSTS_NUMBER];
extern Character* bombs[BOMBS_NUMBER];

extern char strategyArray[GHOSTS_NUMBER];

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
void moveTowardCharacter(Character *hunterPtr, Character *preyPtr, char strategy)
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
	char i;
	switch(level)
	{
		case 1: case 2: case 3: case 4:
			for(i=0; i<GHOSTS_NUMBER; ++i) // 8,0,0
			{
				strategyArray[i] = 4; // no preference (approximate straight line)
			}
		break;
		case 5: case 6: case 7: case 8: case 9: // 4,2,2
			for(i=0; i<5; ++i) // 4
			{
				strategyArray[i] = 4; // no preference (approximate straight line)
			}
			for(i=5; i<7; ++i) // 2
			{
				strategyArray[i] = 3; // slightly prefer X (60%)
			}
			for(i=7; i<GHOSTS_NUMBER; ++i) // 2 (if total=8)
			{
				strategyArray[i] = 5; // slightly prefer Y (60%)
			}
		break;
		case 10: case 11: case 12: case 13: case 14:// 4,2,2
			for(i=0; i<5; ++i) // 4
			{
				strategyArray[i] = 4; // no preference (approximate straight line)
			}
			for(i=5; i<7; ++i) // 2
			{
				strategyArray[i] = 2; // prefer X (70%)
			}
			for(i=7; i<GHOSTS_NUMBER; ++i) // 2 (if total=8)
			{
				strategyArray[i] = 6; // prefer Y (70%)
			}
		break;
		case 15: case 16: case 17: case 18: case 19: // 4,2,2
			for(i=0; i<5; ++i) // 4
			{
				strategyArray[i] = 4; // no preference (approximate straight line)
			}
			for(i=5; i<7; ++i) // 2
			{
				strategyArray[i] = 1; // strongly prefer X (80%)
			}
			for(i=7; i<GHOSTS_NUMBER; ++i) // 2 (if total=8)
			{
				strategyArray[i] = 7; // strongly prefer Y (80%)
			}
		break;		
		default: // 4,2,2  from level 15 on - Assuming a minimum of 6 ghosts
			for(i=0; i<2; ++i) // 2
			{
				strategyArray[i] = 4; // no preference (approximate straight line)
			}
			for(i=2; i<5; ++i) // 3
			{
				strategyArray[i] = 1; // strongly prefer X (80%)
			}
			for(i=5; i<GHOSTS_NUMBER; ++i) // 3 (if total=8)
			{
				strategyArray[i] = 7; // strongly prefer Y (80%)
			}
		break;
	}
}

// Ghosts move to new positions if they get their chanche
void chasePlayer(int slowDown)
{
	char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((ghosts[i]->_status) && (rand()>slowDown))
		{
			ghosts[i]->_moved = 1;
			DELETE_GHOST(ghosts[i]->_x,ghosts[i]->_y,ghosts[i]->_imagePtr);
			moveTowardCharacter(ghosts[i], &player, strategyArray[i]);
		}
		else
		{
			ghosts[i]->_moved = 0;
		}
	}
}
