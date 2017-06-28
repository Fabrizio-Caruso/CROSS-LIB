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
 
#include <stdlib.h>



#include "character.h"
#include "settings.h"

#include "display_macros.h"
#include "sound_macros.h"

extern unsigned long points;
extern unsigned short innerVerticalWallX;
extern unsigned short innerVerticalWallY;
extern unsigned short innerVerticalWallLength;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned short ghostCount;
extern unsigned int loop;

extern Image DEAD_GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Character* ghosts[GHOSTS_NUMBER];
extern Character* bombs[BOMBS_NUMBER];

void initializeCharacter(Character* characterPtr, int x, int y, short status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
}

void setCharacterPosition(Character* characterPtr, short x, short y)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
}

int isCharacterAtLocation(short x, short y, Character * characterPtr)
{
	return(characterPtr->_x==x) && (characterPtr->_y==y);
}

int areCharctersAtSamePosition(Character* lhs, Character* rhs)
{
	return (lhs->_x==rhs->_x)&&(lhs->_y==rhs->_y);
}


int wallReached(Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}

void die(Character * playerPtr)
{
	playerPtr->_status = 0;
}

int playerReached(Character* preyPtr)
{
	int i=0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(ghosts[i],preyPtr))
			return 1;
	}
	return 0;
}

int playerReachedBombs(Character* preyPtr)
{
	int i=0;
	for(;i<BOMBS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(bombs[i],preyPtr))
			return 1;
	}
	return 0;
}

int ghostsMeet(unsigned char preyIndex)
{
	short i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((i!=preyIndex) && 
		    ((!(ghosts[i]->_status)) || !(loop%GHOST_VS_GHOST_COLLISION_LEVEL)) && 
		    areCharctersAtSamePosition(ghosts[i],ghosts[preyIndex]))
			return 1;
	}
	return 0;
}


void checkBombsVsGhost(Character * ghostPtr)
{
	if(ghostPtr->_status && playerReachedBombs(ghostPtr))
	{
		EXPLOSION_SOUND();
		ghostPtr->_imagePtr = &DEAD_GHOST_IMAGE;
		DRAW_GHOST(ghostPtr->_x, ghostPtr->_y, ghostPtr->_imagePtr);
		die(ghostPtr);
		points+=GHOST_VS_BOMBS_BONUS;
		--ghostCount;
	}
}
						

void checkBombsVsGhosts(void)
{
	char i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		checkBombsVsGhost(ghosts[i]);
	}
}

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
int safeLocation(unsigned char x, unsigned char y, Character **danger, unsigned char dangerSize)
{
	char i = 0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(isCharacterAtLocation(x,y,ghosts[i]))
			return 0;
	}
	for(i=0;i<dangerSize;++i)
	{
		if(isCharacterAtLocation(x,y,danger[i]))
			return 0;
	}
	return 1;
}


void relocateCharacter(Character * characterPtr, Character **danger, unsigned char dangerSize)
{
	unsigned char x; 
	unsigned char y; 
	int x_offset; 
	int y_offset;
	int safe = 0;
	while(!safe)
	{
		// TODO: This should be separated (at least partially) and moved into display_macros
		x_offset = rand() % 7;
		y_offset = rand() % 7;
		if((x_offset==0) && (y_offset==0))
			continue;
		x = characterPtr->_x -3 + x_offset; 
		y = characterPtr->_y -3 + y_offset;
		
		// TODO: This check should be separated and moved into display_macros
		if(y<=3) // Avoid score line
			continue;
		if((x<2) || (x>XSize-2)) // Avoid vertical outer wall
			continue;
		if((y<2) || (y>YSize-2)) // Avoid horizontal outer wall 
			continue;
		
		safe = safeLocation(x,y,danger, dangerSize);
	}
	characterPtr->_x = x;
	characterPtr->_y = y;
}


short innerWallReached(Character *characterPtr)
{
	return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
}

short nearInnerWall(Character *characterPtr)
{
	return (characterPtr->_x>=innerVerticalWallX-1) && (characterPtr->_x<=innerVerticalWallX+1) &&
		   (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
}


void checkGhostsVsGhosts()
{
	char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if(ghosts[i]->_status && ghostsMeet(i))
		{
			EXPLOSION_SOUND();
			ghosts[i]->_imagePtr = &DEAD_GHOST_IMAGE;
			DRAW_GHOST(ghosts[i]->_x, ghosts[i]->_y, ghosts[i]->_imagePtr);
			die(ghosts[i]);
			points+=GHOST_VS_GHOST_BONUS;
			--ghostCount;
		}
	}
}
