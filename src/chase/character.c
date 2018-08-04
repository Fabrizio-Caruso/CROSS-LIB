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

#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
#endif

#include "character.h"
#include "settings.h"

#include "../cross_lib/cross_lib.h"

#include "game_text.h"

extern unsigned short points;

extern unsigned char ghostCount;
extern unsigned short loop;
extern unsigned char level;

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
	extern unsigned char invincibilityActive;
	extern unsigned char innerVerticalWallX;
	extern unsigned char innerVerticalWallY;
	extern unsigned char innerVerticalWallLength;
	
	extern unsigned char horizontalWallsLength;
	extern unsigned char zombieActive;	
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
	unsigned char playerKilledBy(Character *enemyPtr)
	{
		return !invincibilityActive && areCharctersAtSamePosition(enemyPtr,&player);
	}
#endif

void initializeCharacter(register Character* characterPtr, unsigned char x, unsigned char y, unsigned char status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
	
	#if defined(DEBUG)
		displayCharacter(characterPtr);
	#endif
}


unsigned char isCharacterAtLocation(unsigned char x, unsigned char y, Character * characterPtr)
{
	return(characterPtr->_x==x) && (characterPtr->_y==y);
}


unsigned char wallReached(register Character *characterPtr)
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
				unsigned char i;

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
	
	if(ghostPtr->_status && playerReachedBombs(ghostPtr))
	{		
		points+=GHOST_VS_BOMBS_BONUS;
		
		#if !defined(TINY_GAME)
			ghostPtr->_x=1+GHOSTS_NUMBER-ghostCount;
		#else
			ghostPtr->_x=1;
		#endif
		ghostPtr->_y=1;
		
		
		ghostDies(ghostPtr);
	}
	
}
						

void checkBombsVsGhosts(void)
{
	unsigned char i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	  {
		 checkBombsVsGhost(&ghosts[i]);
	  }
}


unsigned char sameLocationAsAnyLocation(unsigned char x, unsigned char y, Character *characterList, unsigned char length)
{
	unsigned char i;

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
unsigned char safeLocation(unsigned char x, unsigned char y)
{
	return !(sameLocationAsAnyLocation(x,y,ghosts,GHOSTS_NUMBER) 
	      || sameLocationAsAnyLocation(x,y,bombs, BOMBS_NUMBER)
		  || (x<SAFETY) || (x>XSize-SAFETY) || (y<=SAFETY) || (y>YSize-SAFETY));
}


void relocateCharacter(register Character * characterPtr)
{
	unsigned char x; 
	unsigned char y;  
	
	do
	{
		x = characterPtr->_x - (unsigned char)(RELOCATE_RANGE/2) + (unsigned char)(rand() % RELOCATE_RANGE);
		y = characterPtr->_y - (unsigned char)(RELOCATE_RANGE/2) + (unsigned char)(rand() % RELOCATE_RANGE);

	} while(!safeLocation(x,y));

	characterPtr->_x = x;
	characterPtr->_y = y;
}

#if defined(FULL_GAME)
	unsigned char innerWallReached(register Character *characterPtr)
	{
		return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
	}

	unsigned char horizontalWallsReached(register Character *characterPtr)
	{
		return (characterPtr->_y==YSize/2) && 
		       ((characterPtr->_x<=horizontalWallsLength) ||
			    (characterPtr->_x>=-1+XSize-horizontalWallsLength));
	}	
	
	unsigned char nearInnerWall(register Character *characterPtr)
	{
		return (characterPtr->_x>=innerVerticalWallX-1) && (characterPtr->_x<=innerVerticalWallX+1) &&
			   (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
	}
#endif

