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
#include <stdlib.h>

extern unsigned char level;
extern unsigned short loop;
extern unsigned char ghostCount;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern unsigned char strategyArray[GHOSTS_NUMBER];

#if defined(FULL_GAME)
	extern unsigned char zombieActive;
#endif


unsigned char moveCharacter(register unsigned char *hunterOffsetPtr, register unsigned char *preyOffsetPtr)
{
	if((unsigned char) *((unsigned char *)hunterOffsetPtr) < (unsigned char) *((unsigned char *)preyOffsetPtr))
	{
		++(*((unsigned char *) hunterOffsetPtr));		
	}
	else if((unsigned char) *((unsigned char *) hunterOffsetPtr) > (unsigned char) *((unsigned char *)preyOffsetPtr))
	{
		--(*((unsigned char *) hunterOffsetPtr));		
	}	
	else
	{
		return 0;
	}
	return 1;
}


#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
	void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
	{
		if(moveCharacter((unsigned char *)hunterPtr+X_MOVE, (unsigned char *)preyPtr + X_MOVE))
		{
			return;
		}
		else
		{
			moveCharacter((unsigned char *)hunterPtr+Y_MOVE, (unsigned char *)preyPtr + Y_MOVE);
		}
	}

	void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
	{
		if(moveCharacter((unsigned char *)hunterPtr+Y_MOVE, (unsigned char *)preyPtr + Y_MOVE))
		{
			return;
		}
		else
		{
			moveCharacter((unsigned char *)hunterPtr+X_MOVE, (unsigned char *)preyPtr + X_MOVE);
		}
	}
#endif


// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
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
#elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
	void moveTowardCharacter(Character* preyPtr, Character *hunterPtr)
	{
		unsigned char offset = (unsigned char) rand()&1;
		
		moveCharacter((unsigned char *)hunterPtr+offset, (unsigned char *)preyPtr + offset);
	}	
#else
	void moveTowardCharacter(Character *hunterPtr)
	{
		unsigned char offset = (unsigned char) rand()&1;
		
		moveCharacter((unsigned char *)hunterPtr+offset,(unsigned char *)(&player)+offset);
	}
#endif


// #if !defined(TINY_GAME) 
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
void computeStrategy(void)
{
	unsigned char i;
	unsigned char skew = (level - 1) / 5;
	
	for(i=5; i<GHOSTS_NUMBER; ++i) // 3 (if total=8)
	{
		strategyArray[i] = 5+skew; // 5,6,7,(8 if GHOSTS are 9): prefer Y (60%, 70%, 80%, 90%)
		strategyArray[9-i] = 3-skew; // 4,3,2,(1 if GHOSTS are 9): prefer X (60%, 70%, 80%, 90%)
			
	}
	strategyArray[0] = 4;
	strategyArray[1] = 4;				
}
#endif


#if defined(__NCURSES__) || defined(__GCC_BUFFERED__)
	#define GHOST_RANDOM_CONDITION ((rand()&0x7fff)>slowDown)
#else
	#define GHOST_RANDOM_CONDITION (rand()>slowDown)
#endif

// #if defined(FULL_GAME)
// Ghosts move to new positions if they get their chanche
#if defined(FULL_GAME)
void chaseCharacter(Character *preyPtr, unsigned short slowDown)
#else
void chaseCharacter(unsigned short slowDown)	
#endif
{
	unsigned char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		#if defined(FULL_GAME)
			if((ghosts[i]._status || (zombieActive && loop&1)) && GHOST_RANDOM_CONDITION)
		#else
			if((ghosts[i]._status) && GHOST_RANDOM_CONDITION)	
		#endif
			{
				deleteGhost(&ghosts[i]);
				#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
					moveTowardCharacter(preyPtr, &ghosts[i], strategyArray[i]);	
				#elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
					moveTowardCharacter(preyPtr, &ghosts[i]);	
				#else
					moveTowardCharacter(&ghosts[i]);	
				#endif
			}
	}
}
