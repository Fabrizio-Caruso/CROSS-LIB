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

extern unsigned long points;
extern unsigned short innerVerticalWallX;
extern unsigned short innerVerticalWallY;
extern unsigned short innerVerticalWallLength;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned short ghostCount;

extern Image DEAD_GHOST_IMAGE;

void initializeCharacter(Character* characterPtr, int x, int y, short status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_alive = 1; // TODO: Maybe we should initialize this with a parameter
	//characterPtr->_imagePtr = 0;//imagePtr;
	// TODO Bogus
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
	//DELETE_CHARACTER(playerPtr);
	playerPtr->_status = 0;
	playerPtr->_alive = 0;
}

void ghost_die(Character * ghostPtr)
{
	SET_TEXT_COLOR(COLOR_RED);
	ghostPtr->_imagePtr = &DEAD_GHOST_IMAGE;
	DRAW_GHOST(ghostPtr->_x, ghostPtr->_y, ghostPtr->_imagePtr);
	SET_TEXT_COLOR(TEXT_COLOR);
	ghostPtr->_status = 0;
	ghostPtr->_alive = 0;
}

int playerReached(Character ** ghosts, 
                  Character* preyPtr)
{
	int i=0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(ghosts[i],preyPtr))
			return 1;
	}
	return 0;
}

int playerReachedBombs(Character ** bombs,  
					   Character* preyPtr)
{
	int i=0;
	for(;i<BOMBS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(bombs[i],preyPtr))
			return 1;
	}
	return 0;
}

int charactersMeet(short preyIndex, Character **ghosts)
{
	short i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((i!=preyIndex)&&areCharctersAtSamePosition(ghosts[i],ghosts[preyIndex]))
			return 1;
	}
	return 0;
}


void checkBombsVsGhost(Character ** bombs,
					   Character * ghostPtr)
{
	if(ghostPtr->_alive && playerReachedBombs(bombs, ghostPtr))
	{
		DRAW_GHOST(ghostPtr->_x, ghostPtr->_y, ghostPtr->_imagePtr);
		ghost_die(ghostPtr);
		points+=GHOST_VS_BOMBS_BONUS;
		--ghostCount;
	}
}
						

void checkBombsVsGhosts(Character ** bombs,
						Character ** ghosts)
{
	char i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		checkBombsVsGhost(bombs, ghosts[i]);
	}
}


int safeLocation(int x, int y, 
				Character ** bombs,
				Character ** ghosts)
{
	char i = 0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(isCharacterAtLocation(x,y,ghosts[i]))
			return 0;
	}
	for(i=0;i<BOMBS_NUMBER;++i)
	{
		if(isCharacterAtLocation(x,y,bombs[i]))
			return 0;
	}
	return 1;
}


void relocateCharacter(Character * characterPtr, 
						Character ** bombs,
						Character ** ghosts)
{
	int x; int y; int x_offset; int y_offset;
	int safe = 0;
	while(!safe)
	{
	x_offset = rand() % 7;
	y_offset = rand() % 7;
	if((x_offset==0) && (y_offset==0))
		continue;
	x = characterPtr->_x -3 + x_offset; 
	y = characterPtr->_y -3 + y_offset;
	if(y<=3) // Avoid score line
		continue;
	if((x<2) || (x>XSize-2))
		continue;
	if((y<2) || (y>YSize-2))
		continue;
	
	safe = safeLocation(x,y, bombs, 
						ghosts);
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


void checkGhostsVsGhosts(Character ** ghosts)
{
	char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if(ghosts[i]->_alive && charactersMeet(i, ghosts))
		{
			ghost_die(ghosts[i]);
			points+=GHOST_VS_GHOST_BONUS;
			--ghostCount;
		}
	}
}
