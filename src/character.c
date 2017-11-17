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
 
#include <stdlib.h>



#include "character.h"
#include "settings.h"

#include "display_macros.h"
#include "sound_macros.h"
#include "text.h"

extern unsigned int points;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned char ghostCount;
extern unsigned short loop;

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

#if defined(FULL_GAME)
	extern unsigned char innerVerticalWallX;
	extern unsigned char innerVerticalWallY;
	extern unsigned char innerVerticalWallLength;
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
	playerPtr->_status = (unsigned char ) 0;
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
		ghostPtr->_imagePtr = (Image *)& BOMB_IMAGE;
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
	unsigned char i = 0;
	for(;i<length;++i)
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
	unsigned char safe = 0;
	do
	{
		x_offset = rand() % 5;
		y_offset = rand() % 5;

		if((x_offset==0) && (y_offset==0))
			continue;
		#if defined(WIDE)
			x = characterPtr->_x -2 + x_offset; 
			y = characterPtr->_y -2 + y_offset;
		#else
			x = characterPtr->_x -1 + x_offset; 
			y = characterPtr->_y -1 + y_offset;
		#endif	
		
		// TODO: This check should be separated and moved into display_macros
		if((x<2) || (x>XSize-2) || (y<=2) || (y>YSize-2))
			continue;
		
		safe = safeLocation(x,y,dangerPtr, dangerSize);
	} while(!safe);
	characterPtr->_x = x;
	characterPtr->_y = y;
}

#if defined(FULL_GAME)
	unsigned char innerWallReached(Character *characterPtr)
	{
		return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
	}

	unsigned char nearInnerWall(Character *characterPtr)
	{
		return (characterPtr->_x>=innerVerticalWallX-1) && (characterPtr->_x<=innerVerticalWallX+1) &&
			   (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
	}
#endif


