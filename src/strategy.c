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
#include "character.h"
#include "strategy.h"

extern unsigned char level;
extern unsigned char ghostCount;
extern unsigned short invincibleSlowDown;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern unsigned char strategyArray[GHOSTS_NUMBER];

#if defined(FULL_GAME)
extern unsigned char zombieActive;
#endif

unsigned char move(Character* hunterPtr, Character* preyPtr, unsigned char offset)
{
	if((unsigned char) *((unsigned char *)hunterPtr+offset) < (unsigned char) *((unsigned char *)preyPtr+offset))
	{
		++(*((unsigned char *) hunterPtr+offset));		
	}
	else if((unsigned char) *((unsigned char *) hunterPtr+offset) > (unsigned char) *((unsigned char *)preyPtr+offset))
	{
		--(*((unsigned char *) hunterPtr+offset));		
	}	
	else
	{
		return 0;
	}
	return 1;
}


// TODO: Design issue: we delete the invincible enemy
// This should be made generic even though it works
void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(move(hunterPtr, preyPtr,X_MOVE))
	{
		return;
	}
	else
	{
		move(hunterPtr, preyPtr,Y_MOVE);
	}
}

void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(move(hunterPtr, preyPtr,Y_MOVE))
	{
		return;
	}
	else
	{
		move(hunterPtr, preyPtr,X_MOVE);
	}
}


// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
#if defined(FULL_GAME)
	void moveTowardCharacter(Character* preyPtr, Character *hunterPtr, unsigned char strategy)
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
#elif !defined(TINY_GAME)
	void moveTowardCharacter(Character *hunterPtr, unsigned char strategy)
	{
		if(rand()%10 > strategy) // Select blind chase strategy
			{ // 0 - 4
				blindChaseCharacterXStrategy(hunterPtr, &player);	
			}
			else
			{ // 5 - 9
				blindChaseCharacterYStrategy(hunterPtr, &player);
			}
	}
#else
	void moveTowardCharacter(Character *hunterPtr)
	{
		if(rand()%10 > 4) // Select blind chase strategy
			{ // 0 - 4
				blindChaseCharacterXStrategy(hunterPtr, &player);
			}
			else
			{ // 5 - 9
				blindChaseCharacterYStrategy(hunterPtr, &player);
			}
	}	
#endif

#if !defined(TINY_GAME)
void computeStrategy(void)
{
	unsigned char i;

	#if defined(FULL_GAME)
		unsigned char skew = ((level - 1) / 5) % 4;
		for(i=5; i<GHOSTS_NUMBER; ++i) // 3 (if total=8)
		{
			strategyArray[i] = 5+skew; // 6,7,8,(9 if GHOSTS are 9) (prefer Y (60%, 70%, 80%, 90)
			strategyArray[9-i] = 3-skew; // 4,3,2,(1 if GHOSTS are 9) prefer X (60%, 70%, 80%, 90%)
				
		}
		strategyArray[0] = 4;
		strategyArray[1] = 4;				
	#else
		for(i=1; i<GHOSTS_NUMBER; ++i) // 6,1,1
		{
			strategyArray[i] = i; // no preference (approximate straight line)
		}	
		strategyArray[0] = 4;		
	#endif
}
#endif


#if defined(FULL_GAME)
// Ghosts move to new positions if they get their chanche
void chaseCharacter(Character *preyPtr, unsigned short slowDown)
{
	unsigned char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((ghosts[i]._status || zombieActive) && (rand()>slowDown))
		{
			deleteGhost(&ghosts[i]);
			moveTowardCharacter(preyPtr, &ghosts[i], strategyArray[i]);			
		}
	}
}
#elif !defined(TINY_GAME)
void chaseCharacter(unsigned short slowDown)
{
	unsigned char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((ghosts[i]._status) && (rand()>slowDown))
		{
			deleteGhost(&ghosts[i]);
			moveTowardCharacter(&ghosts[i], strategyArray[i]);			
		}
	}
}
#else
void chaseCharacter(unsigned short slowDown)
{
	unsigned char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((ghosts[i]._status) && (rand()>slowDown))
		{
			deleteGhost(&ghosts[i]);
			moveTowardCharacter(&ghosts[i]);			
		}
	}
}	
#endif

