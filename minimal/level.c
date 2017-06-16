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
 
#include "character.h"
#include "settings.h"

#include <stdlib.h>

#include "display_macros.h"

#include "level.h"

#if defined(__VIC20__)
	#define TWIN_GHOST_SEPARATOR 2
#else
	#define TWIN_GHOST_SEPARATOR 3
#endif
	
extern unsigned short innerVerticalWallX;
extern unsigned short innerVerticalWallY;
extern unsigned short innerVerticalWallLength;
extern unsigned short level;
extern unsigned char XSize;
extern unsigned char YSize;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;

void drawInnerVerticalWall(void)
{
	innerVerticalWallX = XSize / 2;
    innerVerticalWallY = YSize/2-(innerVerticalWallLength/2);
	DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);
}


void updateInnerWallVerticalLength(void)
{		

	innerVerticalWallLength = 8;

}


void fillLevelWithCharacters(
						  Character * playerPtr, Character * powerUpPtr, 
						  Character **ghosts,
						  Character **bombs,
						  Character * invincibleGhostPtr, 
						  Character * missilePtr, Character * gunPtr)
{
	char i;

	
	INIT_IMAGES();
	
		initializeCharacter(ghosts[1],XSize/6+rand()%3-1,YSize/6+rand()%3-2+1,1,&GHOST_IMAGE);
		DRAW_GHOST(ghosts[1]->_x, ghosts[1]->_y, ghosts[1]->_imagePtr);
		
		initializeCharacter(ghosts[2],XSize/6+rand()%3-1,YSize/2+rand()%3-2,1, &GHOST_IMAGE);
		DRAW_GHOST(ghosts[2]->_x, ghosts[2]->_y, ghosts[2]->_imagePtr);
		
		initializeCharacter(ghosts[3],XSize/6+rand()%3-1,YSize-YSize/6+rand()%3-3,1,&GHOST_IMAGE);
		DRAW_GHOST(ghosts[3]->_x, ghosts[3]->_y, ghosts[3]->_imagePtr);

		initializeCharacter(ghosts[4],XSize/2+rand()%3-1,YSize/6+rand()%3-2+1,1,&GHOST_IMAGE);
		DRAW_GHOST(ghosts[4]->_x, ghosts[4]->_y, ghosts[4]->_imagePtr);
	
		initializeCharacter(ghosts[5],XSize/2+rand()%3-1,YSize-YSize/6+rand()%3-3,1,&GHOST_IMAGE);
		DRAW_GHOST(ghosts[5]->_x, ghosts[5]->_y, ghosts[5]->_imagePtr);
	
		initializeCharacter(ghosts[6],XSize-XSize/6+rand()%3-3,YSize/6+rand()%3-2+1,1,&GHOST_IMAGE);
		DRAW_GHOST(ghosts[6]->_x, ghosts[6]->_y, ghosts[6]->_imagePtr);
		
		initializeCharacter(ghosts[0],XSize-XSize/6+rand()%3-3,YSize-YSize/6+rand()%3-3,1,&GHOST_IMAGE);
		DRAW_GHOST(ghosts[0]->_x, ghosts[0]->_y, ghosts[0]->_imagePtr);

	// Player

	initializeCharacter(playerPtr,XSize/2+rand()%4-4,YSize/2+rand()%4-2,1,&PLAYER_IMAGE);

	SET_TEXT_COLOR(PLAYER_COLOR);
	DRAW_PLAYER(playerPtr->_x,playerPtr->_y,playerPtr->_imagePtr);
	SET_TEXT_COLOR(TEXT_COLOR);
	

	initializeCharacter(bombs[0],1+1, YSize/2-3+rand()%7,0,&BOMB_IMAGE);

	initializeCharacter(bombs[1],XSize-2-1, YSize/2-3+rand()%7,0,&BOMB_IMAGE);

			
	for(i=0;i<BOMBS_NUMBER;++i)
	{
		DRAW_BOMB(bombs[i]->_x, bombs[i]->_y, bombs[i]->_imagePtr);
	}

	do
	{
		// Power-ups
		initializeCharacter(powerUpPtr,XSize/2,YSize/2,1,&POWERUP_IMAGE);
		relocateCharacter(powerUpPtr, bombs, ghosts);	
		initializeCharacter(powerUpPtr,powerUpPtr->_x,powerUpPtr->_y,1,&POWERUP_IMAGE);
	} while(nearInnerWall(powerUpPtr));
	DRAW_POWERUP(powerUpPtr->_x, powerUpPtr->_y, powerUpPtr->_imagePtr);
		
    initializeCharacter(missilePtr, 0, 0,0,&MISSILE_IMAGE);
	
	do
	{
		initializeCharacter(gunPtr, XSize/2, YSize/2, 0, &GUN_IMAGE);
		relocateCharacter(gunPtr, bombs, ghosts);
	} while(nearInnerWall(gunPtr));
	

	initializeCharacter(invincibleGhostPtr,XSize-2,YSize-2,0, &INVINCIBLE_GHOST_IMAGE);
}

