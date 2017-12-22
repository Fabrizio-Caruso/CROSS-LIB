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

#if defined(__VIC20__) && defined(REDEFINED_CHARS) && !defined(FULL_GAME) && !defined(TINY_GAME)
	#pragma code-name(push, "CODE2")
#endif


#include "settings.h"
#include "character.h"
#include "strategy.h"


#define X_MOVE 0
#define Y_MOVE 1

extern unsigned char level;
extern unsigned char ghostCount;
extern unsigned short invincibleSlowDown;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern unsigned char strategyArray[GHOSTS_NUMBER];

unsigned char move(Character* hunterPtr, Character* preyPtr, unsigned char offset)
{
	if((unsigned char) *((unsigned char *)hunterPtr+offset) < (unsigned char) *((unsigned char *)preyPtr+offset))
	{
		// ++((unsigned char) *((unsigned char *)hunterPtr+offset));
		++(*((unsigned char *) hunterPtr+offset));		
	}
	else if((unsigned char) *((unsigned char *) hunterPtr+offset) > (unsigned char) *((unsigned char *)preyPtr+offset))
	{
		//--((unsigned char) *((unsigned char *) hunterPtr+offset));
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
#if !defined(TINY_GAME)
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
		switch(level)
		{
			case 1: case 2: case 3: case 4: case 5:
				for(i=0; i<4; ++i) // 4
				{
					strategyArray[i] = 4; // no preference (approximate straight line)
				}
				for(i=4; i<6; ++i) // 2
				{
					strategyArray[i] = 3; // slightly prefer X (60%)
				}
				for(i=6; i<GHOSTS_NUMBER; ++i) // 2 (if total=8)
				{
					strategyArray[i] = 5; // slightly prefer Y (60%)
				}
			break;
			case 6: case 7: case 8: case 9: case 10: // 4,2,2
				for(i=0; i<4; ++i) // 4
				{
					strategyArray[i] = 4; // no preference (approximate straight line)
				}
				for(i=4; i<6; ++i) // 2
				{
					strategyArray[i] = 2; // prefer X (70%)
				}
				for(i=6; i<GHOSTS_NUMBER; ++i) // 2 (if total=8)
				{
					strategyArray[i] = 6; // prefer Y (70%)
				}
			break;
			case 11: case 12: case 13: case 14: case 15: // 4,2,2		
				for(i=0; i<4; ++i) // 4
				{
					strategyArray[i] = 4; // no preference (approximate straight line)
				}
				for(i=4; i<6; ++i) // 2
				{
					strategyArray[i] = 1; // strongly prefer X (80%)
				}
				for(i=6; i<GHOSTS_NUMBER; ++i) // 2 (if total=8)
				{
					strategyArray[i] = 7; // strongly prefer Y (80%)
				}
			break;		
			default: // 2, 3, 3 from level 21 on
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
	#else
		#if !defined(TINY_GAME)
			for(i=1; i<GHOSTS_NUMBER-1; ++i) // 6,1,1
			{
				strategyArray[i] = 4; // no preference (approximate straight line)
			}	
			#if GHOSTS_NUMBER>=3
				strategyArray[0] = 2;
				strategyArray[1] = 6;
			
			#else
				strategyArray[0] = 2;
				strategyArray[GHOSTS_NUMBER-1] = 6;
			#endif
		#else
			for(i=0; i<GHOSTS_NUMBER; ++i) 
			{
				strategyArray[i] = 4; // no preference (approximate straight line)
			}				
		#endif
	#endif
}
#endif

// Ghosts move to new positions if they get their chanche
void chasePlayer(unsigned short slowDown)
{
	unsigned char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((ghosts[i]._status) && (rand()>slowDown))
		{
			DELETE_GHOST(ghosts[i]._x,ghosts[i]._y,ghosts[i]._imagePtr);
			#if defined(TINY_GAME)
				moveTowardCharacter(&ghosts[i]);
			#else
				moveTowardCharacter(&ghosts[i], strategyArray[i]);
			#endif
		}
	}
}


#if defined(__VIC20__) && defined(REDEFINED_CHARS) && !defined(FULL_GAME) && !defined(TINY_GAME)
	#pragma code-name(pop)
#endif

