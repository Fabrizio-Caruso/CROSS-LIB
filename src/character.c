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
#endif

#include "character.h"
#include "settings.h"

#include "display_macros.h"
#include "sound_macros.h"
#include "text.h"
#include "sleep_macros.h"

extern unsigned int points;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned char ghostCount;
extern unsigned short loop;

extern unsigned char horizontalWallsLength;

extern unsigned char rebirth;

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character player;


#if defined(FULL_GAME)
	extern unsigned char player_invincibility;
	extern unsigned char innerVerticalWallX;
	extern unsigned char innerVerticalWallY;
	extern unsigned char innerVerticalWallLength;
#endif 

void playerDies(void)
{
	EXPLOSION_SOUND();
	die(&player);
	#if !defined(NO_TEXT)
		printDefeatMessage();
	#endif
	sleep(1);	
}

#if defined(FULL_GAME)
	unsigned char playerKilledBy(Character *enemyPtr)
	{
		return !player_invincibility && areCharctersAtSamePosition(enemyPtr,&player);
	}
#endif

void initializeCharacter(Character* characterPtr, unsigned char x, unsigned char y, unsigned char status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
}

void setCharacterPosition(Character* characterPtr, unsigned char x, unsigned char y)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
}

unsigned char isCharacterAtLocation(unsigned char x, unsigned char y, Character * characterPtr)
{
	return(characterPtr->_x==x) && (characterPtr->_y==y);
}


unsigned char wallReached(Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}

void die(Character * playerPtr)
{
	playerPtr->_status = 0;
}


void ghostDies(Character * ghostPtr)
{
	EXPLOSION_SOUND();
	die(ghostPtr);
	displayStats();
	--ghostCount;
	printGhostCountStats();
}

void checkBombsVsGhost(Character * ghostPtr)
{
	
	if(ghostPtr->_status && playerReachedBombs(ghostPtr))
	{
		points+=GHOST_VS_BOMBS_BONUS;	
		#if defined(FULL_GAME)
		if(rebirth)
		{
			ghostPtr->_imagePtr = (Image *)& DEAD_GHOST_IMAGE;	
		}
		else
		{
			ghostPtr->_imagePtr = (Image *)& BOMB_IMAGE;
		}			
		#else
			ghostPtr->_imagePtr = (Image *)& BOMB_IMAGE;
		#endif
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

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
unsigned char safeLocation(unsigned char x, unsigned char y, Character *dangerPtr, unsigned char dangerSize)
{
	return !(sameLocationAsAnyLocation(x,y,ghosts,GHOSTS_NUMBER) || sameLocationAsAnyLocation(x,y,dangerPtr, dangerSize));
}


void relocateCharacter(Character * characterPtr, Character *dangerPtr, unsigned char dangerSize)
{
	unsigned char x; // = 0; 
	unsigned char y; // = 0; 
	unsigned char x_offset; 
	unsigned char y_offset;
	unsigned char safe;
	do
	{
		x_offset = (unsigned char)(rand() % RELOCATE_RANGE);
		y_offset = (unsigned char)(rand() % RELOCATE_RANGE);


		
		
		x = characterPtr->_x - (unsigned char)(RELOCATE_RANGE/2) + x_offset; 
		y = characterPtr->_y - (unsigned char)(RELOCATE_RANGE/2) + y_offset;

		if((x<3) || (x>XSize-3) || (y<=4) || (y>YSize-3) || (x_offset==0) && (y_offset==0) )
			continue; // TODO: Optimize it!

		safe = safeLocation(x,y,dangerPtr, dangerSize);

		// if((x_offset==0) && (y_offset==0))
			// continue		
	} while(!safe);
	// if(1) //characterPtr== &player)
	// {
		// gotoxy(2,3); cprintf("x_offset: %u\n", x_offset);
		// gotoxy(2,6); cprintf("y_offset: %u\n", y_offset);		
		// gotoxy(2,9 );cprintf("x: %u\n", x );
		// gotoxy(2,12);cprintf("y: %u\n", y);
		// sleep(1);
	// }
	characterPtr->_x = x;
	characterPtr->_y = y;
}

#if defined(FULL_GAME)
	unsigned char innerWallReached(Character *characterPtr)
	{
		return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
	}

	unsigned char horizontalWallsReached(Character *characterPtr)
	{
		return (characterPtr->_y==YSize/2) && 
		       ((characterPtr->_x<=horizontalWallsLength) ||
			    (characterPtr->_x>=-1+XSize-horizontalWallsLength));
	}	
	
	unsigned char nearInnerWall(Character *characterPtr)
	{
		return (characterPtr->_x>=innerVerticalWallX-1) && (characterPtr->_x<=innerVerticalWallX+1) &&
			   (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
	}
#endif

