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

#include "character.h"
#include "settings.h"

#include "cross_lib.h"

#include "game_text.h"

extern uint16_t points;

extern uint8_t ghostCount;
#if !defined(TINY_GAME) || defined(TURN_BASED)
extern uint16_t loop;
#endif
extern uint8_t level;

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character player;


void displayCharacter(register Character * characterPtr)
{
	DRAW_CHARACTER(characterPtr->_x, characterPtr->_y, characterPtr->_imagePtr);
}

void deleteCharacter(Character * characterPtr)
{
	DELETE_CHARACTER(characterPtr->_x, characterPtr->_y);
}

#if defined(FULL_GAME)
	extern uint8_t invincibilityActive;
	extern uint8_t innerVerticalWallY;
	extern uint8_t innerVerticalWallLength;
	
	extern uint8_t horizontalWallsLength;
	extern uint8_t zombieActive;	
#endif 

void playerDies(void)
{
	EXPLOSION_SOUND();
	player._status=0;
	#if !defined(LESS_TEXT)
		printDefeatMessage();
	#endif
	SLEEP(1);	
}

#if defined(FULL_GAME)
	uint8_t playerKilledBy(Character *enemyPtr)
	{
		return !invincibilityActive && areCharctersAtSamePosition(enemyPtr,&player);
	}
#endif

void initializeCharacter(register Character* characterPtr, uint8_t x, uint8_t y, uint8_t status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
}


uint8_t isCharacterAtLocation(uint8_t x, uint8_t y, Character * characterPtr)
{
	return(characterPtr->_y==y) && (characterPtr->_x==x);
}


uint8_t wallReached(register Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}


void ghostDies(Character * ghostPtr)
{
	EXPLOSION_SOUND();
	
	ghostPtr->_status=0;
	displayStats();
	
	#if defined(FULL_GAME)
		if(level>=FIRST_MOVING_BOMBS_LEVEL)
			{
				uint8_t i;

				for(i=0;i<BOMBS_NUMBER;++i)
				{
					deleteCharacter(&bombs[i]);
					if(ghostCount&1)
					{
						--bombs[i]._x;
						--bombs[i]._y;
					}
					else
					{
						++bombs[i]._x;
						++bombs[i]._y;
					}
				}
			}		
	#endif
		
	#if !defined(NO_DEAD_GHOSTS)
		ghostPtr->_imagePtr = (Image *)&DEAD_GHOST_IMAGE;
	#elif !defined(TINY_GAME)
		ghostPtr->_imagePtr = (Image *)&SKULL_IMAGE;			
	#endif
	
	--ghostCount;
	printGhostCountStats();
}

void checkBombsVsGhost(register Character * ghostPtr)
{
	
	if(ghostPtr->_status && characterReachedBombs(ghostPtr))
	{		
		points+=GHOST_VS_BOMBS_BONUS;
		

		#	if defined(TINY_GAME) && defined(MOVE_DEAD_GHOST)
			ghostPtr->_y=1;
		#elif defined(TINY_GAME) && defined(BOMB_DEAD_GHOST)
			ghostPtr->_imagePtr = &BOMB_IMAGE;			
		#elif defined(TINY_GAME) && defined(HIDE_DEAD_GHOST)
			//
		#elif defined(TINY_GAME)
			ghostPtr->_x=ghostCount;
			ghostPtr->_y=1;				
		#else
			ghostPtr->_x=1+GHOSTS_NUMBER-ghostCount;
			ghostPtr->_y=1;	
		#endif
		
		ghostDies(ghostPtr);
	}
	
}
						

uint8_t sameLocationAsAnyLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length)
{
	uint8_t i;

	for(i=0;i<length;++i)
	{
		if(isCharacterAtLocation(x,y,&characterList[i]))
			return 1;
	}	
	return 0;
}


#if YSize<XSize
	#define SAFETY YSize/4
#else
	#define SAFETY XSize/4
#endif

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
uint8_t safeLocation(uint8_t x, uint8_t y)
{
	return !(sameLocationAsAnyLocation(x,y,ghosts,GHOSTS_NUMBER) 
	      || sameLocationAsAnyLocation(x,y,bombs, BOMBS_NUMBER)
		  || (x<SAFETY) || (x>XSize-SAFETY) || (y<=SAFETY) || (y>YSize-SAFETY));
}

#if !defined(TINY_GAME)
void relocateCharacter(register Character * characterPtr)
{
	uint8_t x; 
	uint8_t y;  
	
	do
	{
		x = characterPtr->_x  + (uint8_t)(rand() % RELOCATE_RANGE) - (uint8_t)(RELOCATE_RANGE/2);
		y = characterPtr->_y  + (uint8_t)(rand() % RELOCATE_RANGE) - (uint8_t)(RELOCATE_RANGE/2);

	} while(!safeLocation(x,y));

	characterPtr->_x = x;
	characterPtr->_y = y;
}
#endif

#if defined(FULL_GAME)
	uint8_t innerWallReached(register Character *characterPtr)
	{
		return (characterPtr->_x==XSize/2) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
	}

	uint8_t horizontalWallsReached(void)
	{
		return (player._y==YSize/2) && 
		       ((player._x<=horizontalWallsLength) ||
			    (player._x>=-1+XSize-horizontalWallsLength));
	}	
	
#endif

