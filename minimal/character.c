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
#include "text.h"

#if defined(__ATMOS__)
	#include<peekpoke.h>
#endif

extern unsigned int points;

extern unsigned char XSize;
extern unsigned char YSize;

extern unsigned char ghostCount;
extern unsigned short loop;

extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

void initializeCharacter(Character* characterPtr, unsigned char x, unsigned char y, unsigned char status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
	//characterPtr->_moved = 0;
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

unsigned char areCharctersAtSamePosition(Character* lhs, Character* rhs)
{
	return (lhs->_x==rhs->_x)&&(lhs->_y==rhs->_y);
}


unsigned char wallReached(Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}

void die(Character * playerPtr)
{
	playerPtr->_status = (unsigned char ) 0;
	//playerPtr->_moved = (unsigned char ) 0;
}

unsigned char playerReached(Character* preyPtr)
{
	unsigned char i=0;
	for(;i<GHOSTS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(&ghosts[i],preyPtr))
			return 1;
	}
	return 0;
}

unsigned char playerReachedBombs(Character* preyPtr)
{
	unsigned char i=0;
	for(;i<BOMBS_NUMBER;++i)
	{
		if(areCharctersAtSamePosition(&bombs[i],preyPtr))
			return 1;
	}
	return 0;
}

void ghostDies(Character * ghostPtr)
{
	EXPLOSION_SOUND();
	ghostPtr->_imagePtr = &BOMB_IMAGE;
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

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
unsigned char safeLocation(unsigned char x, unsigned char y, Character *dangerPtr, unsigned char dangerSize)
{
	char i = 0;
	for(i=0;i<dangerSize;++i)
	{
		if(isCharacterAtLocation(x,y,&dangerPtr[i]))
			return 0;
	}
	return 1;
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
		// TODO: This should be separated (at least partially) and moved into display_macros
		x_offset = rand() % 5;
		y_offset = rand() % 5;
		if((x_offset==0) && (y_offset==0))
			continue;
		x = characterPtr->_x -2 + x_offset; 
		y = characterPtr->_y -2 + y_offset;
		
		// TODO: This check should be separated and moved into display_macros
		if((x<2) || (x>XSize-2) || (y<=2) || (y>YSize-2))
			continue;
		
		safe = safeLocation(x,y,dangerPtr, dangerSize);
	} while(!safe);
	characterPtr->_x = x;
	characterPtr->_y = y;
}

// TODO: Implement this
// Check whether an alive ghost meets another dead ghost (transformed into a bomb)
// unsigned char ghostsMeetDead(unsigned char preyIndex)
// {
	// short i;
	
	// for(i=0;i<GHOSTS_NUMBER;++i)
	// {
		// if((i!=preyIndex) && // not itself
		    // !(ghosts[i]._status) && // ghost[i] is dead (transformed into a bomb) 
		    // areCharctersAtSamePosition(&ghosts[i],&ghosts[preyIndex])) 
			// return 1;
	// }
	// return 0;
// }




