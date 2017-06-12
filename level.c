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


unsigned short drawInnerVerticalWallForLevel(void)
{		
	unsigned short wallLength;
	switch(level)
	{
		case 1: case 5: case 10: case 15:
			wallLength = 0;
		break;
		case 2:
			wallLength = 8;
		break;
		case 3:
			wallLength = 12;
		break;
		case 4:
			wallLength = YSize-6;
		break;
		case 6:
			wallLength = 8;
		break;
		case 7:
			wallLength = 12;
		break;
		case 8:
			wallLength = 16;
		break;
		case 9:
			wallLength = YSize-6;
		break;
		case 11:
			wallLength = 8;
		break;
		case 12:
			wallLength = 12;
		break;
		case 13:
			wallLength =16;
		break;
		case 14:
			wallLength = YSize-4;
		break;
		case 16:
			wallLength = 8;
		break;
		case 17:
			wallLength = 12;
		break;
		case 18:
			wallLength = YSize-4;
		break;
		case 19:
			wallLength = 12;
		break;
		case 20:
			wallLength = YSize-4;
		break;
	}
	drawInnerVerticalWall();
	return wallLength;
}


void fillLevelWithCharacters(
						  Character * playerPtr, Character * powerUpPtr, 
						  Character **ghosts,
						  Character **bombs,
						  Character * invincibleGhostPtr, 
						  Character * missilePtr, Character * gunPtr)
{
	short corner = rand()%4;
	short chirality = rand()%2;
	int b1x, b2x, b3x, b4x;
	int b1y, b2y, b3y, b4y;
	Character *dummyBombs[BOMBS_NUMBER];
	char i;
	int x1,x2,x3,x4,x5,x6,x7,x0;
	int y1,y2,y3,y4,y5,y6,y7,y0;
	int j;
	
	initImages();
	

	// Ghosts

	
	x1 = XSize/6+rand()%3-1;       y1 = YSize/6+rand()%3-2+1;
	x2 = XSize/6+rand()%3-1;       y2 = YSize/2+rand()%3-2;
	x3 = XSize/6+rand()%3-1;       y3 = YSize-YSize/6+rand()%3-3;
	x4 = XSize/2+rand()%3-1;       y4 = YSize/6+rand()%3-2+1;
	x5 = XSize/2+rand()%3-1;       y5 = YSize-YSize/6+rand()%3-3;
	x6 = XSize-XSize/6+rand()%3-3; y6 = YSize/6+rand()%3-2+1;
	x7 = XSize-XSize/6+rand()%3-3; y7 = YSize/2+rand()%3-2;
	x0 = XSize-XSize/6+rand()%3-3; y0 = YSize-YSize/6+rand()%3-3;

	for (j=0;j<(GHOSTS_NUMBER/8);++j)
	{
		initializeCharacter(ghosts[1+j*8],x1+TWIN_GHOST_SEPARATOR*j,y1+TWIN_GHOST_SEPARATOR*j,1,&GHOST_IMAGE);
		DRAW(ghosts[1+j*8]);
		
		initializeCharacter(ghosts[2+j*8],x2+TWIN_GHOST_SEPARATOR*j,y2+TWIN_GHOST_SEPARATOR*j,1, &GHOST_IMAGE);
		DRAW(ghosts[2+j*8]);
		
		initializeCharacter(ghosts[3+j*8],x3+TWIN_GHOST_SEPARATOR*j,y3+TWIN_GHOST_SEPARATOR*j,1,&GHOST_IMAGE);
		DRAW(ghosts[3+j*8]);

		initializeCharacter(ghosts[4+j*8],x4+TWIN_GHOST_SEPARATOR,y4+TWIN_GHOST_SEPARATOR*j,1,&GHOST_IMAGE);
		DRAW(ghosts[4+j*8]);
	
		initializeCharacter(ghosts[5+j*8],x5+TWIN_GHOST_SEPARATOR,y5+TWIN_GHOST_SEPARATOR*j,1,&GHOST_IMAGE);
		DRAW(ghosts[5+j*8]);
	
		initializeCharacter(ghosts[6+j*8],x6+TWIN_GHOST_SEPARATOR,y6+TWIN_GHOST_SEPARATOR*j,1,&GHOST_IMAGE);
		DRAW(ghosts[6+j*8]);
	
		initializeCharacter(ghosts[7+j*8],x7+TWIN_GHOST_SEPARATOR,y7+TWIN_GHOST_SEPARATOR*j,1,&GHOST_IMAGE);
		DRAW(ghosts[7+j*8]);
		
		initializeCharacter(ghosts[0+j*8],x0+TWIN_GHOST_SEPARATOR,y0 + TWIN_GHOST_SEPARATOR*j,1,&GHOST_IMAGE);
		DRAW(ghosts[0+j*8]);

	}
	// Player
	do
	{
		initializeCharacter(playerPtr,XSize/2+rand()%4-2,YSize/2+rand()%4-2,1,&PLAYER_IMAGE);
	} while(nearInnerWall(playerPtr));
	SET_TEXT_COLOR(PLAYER_COLOR);
	DRAW(playerPtr);
	SET_TEXT_COLOR(TEXT_COLOR);
	
	do
	{
		// Bombs
		if(level<FIRST_VERY_HARD_LEVEL) // HARD but NOT VERY HARD -> 2 bombs close to vertical borders
		{	
			b2x = 1+1;
			b2y = YSize/2-3+rand()%7;
			
			b3x = XSize-2-1;
			b3y = YSize/2-3+rand()%7;
			
			b4x = b3x;
			b4y = b3y;
			
			b1x = b2x;
			b1y = b2y;
		}
		else if (level<FIRST_INSANE_LEVEL) // VERY HARD but NOT INSANE -> 2 bombs close to the botton corners
		{
			b2x = 1+1;
			b2y = YSize-2-1;
			
			
			b4x = XSize-2-1;
			b4y = YSize-2-1;
			
			b1x = b2x;
			b1y = b2y;
			
			b3x = b4x;
			b3y = b4y;
		}
		else if (level<FIRST_ULTIMATE_LEVEL)// INSANE but not FINAL -> 2 bombs placed on the vertical borders
		{
			b2x = 1;
			b2y = YSize/2-3+rand()%7;
			
			b3x = XSize-2;
			b3y = YSize/2-3+rand()%7;
			
			b4x = b3x;
			b4y = b3y;
			
			b1x = b3x;
			b1y = b3y;
		}
		else // ULTIMATE
		{
			b1x = 1;
			b1y = 1;
			
			b2x = 1;
			b2y = YSize-2;
			
			b3x = XSize-2;
			b3y = 1;
			
			b4x = XSize-2;
			b4y = YSize-2;
		}
		
		if(level>=FIRST_HARD_LEVEL)
		{
			initializeCharacter(bombs[0],b1x, b1y,0,&BOMB_IMAGE);

			initializeCharacter(bombs[1],b2x, b2y,0,&BOMB_IMAGE);

			initializeCharacter(bombs[2],b3x, b3y,0,&BOMB_IMAGE);

			initializeCharacter(bombs[3],b4x, b4y,0,&BOMB_IMAGE);
		}
		else if(level<FIRST_HARD_LEVEL)
		{
			if(chirality)
			{
				b1x = XSize/2-5;
				b1y = YSize/2+5;
				
				b3x = XSize/2+5;
				b3y = YSize/2-5;
			}
			else
			{
				b1x = XSize/2-5;
				b1y = YSize/2-5;
				
				b3x = XSize/2+5;
				b3y = YSize/2+5;
			}
			initializeCharacter(bombs[0],b1x, b1y,0,&BOMB_IMAGE);
			
			dummyBombs[0] = ghosts[0];
			dummyBombs[1] = ghosts[0];
			dummyBombs[2] = ghosts[0];
			dummyBombs[3] = playerPtr;
			for(i=0;i<BOMBS_NUMBER;++i)
			{	
				relocateCharacter(bombs[0], dummyBombs, ghosts);		
			}
			
			initializeCharacter(bombs[2],b3x, b3y,0, &BOMB_IMAGE);
			//dummyBombs[0] = ghosts[0];
			//dummyBombs[1] = ghosts[0];
			dummyBombs[2] = bombs[0];
			//dummyBombs[3] = playerPtr;
			for(i=0;i<BOMBS_NUMBER;++i)
			{	
				relocateCharacter(bombs[2], dummyBombs, ghosts);		
			}									

			if(level>=TWO_BOMB_START_LEVEL) // only use bomb1 and bomb3 previously relocated
			{
				initializeCharacter(bombs[1], bombs[0]->_x, bombs[0]->_y, 0,&BOMB_IMAGE);
				initializeCharacter(bombs[3], bombs[2]->_x, bombs[2]->_y, 0,&BOMB_IMAGE);
			}
			else // place bomb2 and bomb4
			{
				if(chirality)
				{
					b2x = XSize/2-5;
					b2y = YSize/2-5;
					
					b4x = XSize/2+5;
					b4y = YSize/2+5;
				}
				else
				{
					b2x = XSize/2-5;
					b2y = YSize/2+5;
					
					b4x = XSize/2+5;
					b4y = YSize/2-5;
				}
				initializeCharacter(bombs[1],b2x, b2y, 0, &BOMB_IMAGE);
				
				dummyBombs[0] = ghosts[0];
				dummyBombs[1] = bombs[2];
				dummyBombs[2] = bombs[0];
				dummyBombs[3] = playerPtr;
				for(i=0;i<BOMBS_NUMBER;++i)
				{	
					relocateCharacter(bombs[1], dummyBombs, ghosts);		
				}
				
				initializeCharacter(bombs[3],b4x, b4y,0,&BOMB_IMAGE);
				
				dummyBombs[0] = bombs[1];
				//dummyBombs[1] = bombs[2];
				//dummyBombs[2] = bombs[0];
				//dummyBombs[3] = playerPtr;
				for(i=0;i<BOMBS_NUMBER;++i)
				{
					relocateCharacter(bombs[1], dummyBombs, ghosts);
				}				
			}
		}
	} while(nearInnerWall(bombs[0]) || nearInnerWall(bombs[1]) || nearInnerWall(bombs[2]) || nearInnerWall(bombs[3]));
	
	for(i=0;i<BOMBS_NUMBER;++i)
	{
		DRAW(bombs[i]);
	}

	do
	{
		// Power-ups
		initializeCharacter(powerUpPtr,XSize/2,YSize/2,1,&POWERUP_IMAGE);
		relocateCharacter(powerUpPtr, bombs, ghosts);	
		initializeCharacter(powerUpPtr,powerUpPtr->_x,powerUpPtr->_y,1,&POWERUP_IMAGE);
	} while(nearInnerWall(powerUpPtr));
	DRAW(powerUpPtr);
		
    initializeCharacter(missilePtr, 0, 0,0,&MISSILE_IMAGE);
	
	do
	{
		initializeCharacter(gunPtr, XSize/2, YSize/2, 0, &GUN_IMAGE);
		relocateCharacter(gunPtr, bombs, ghosts);
	} while(nearInnerWall(gunPtr));
	
	switch(corner)
	{
		case 0:
			invincibleGhostPtr->_x = 2;
			invincibleGhostPtr->_y = 2;
		break;
		case 1:
			invincibleGhostPtr->_x = 2;
			invincibleGhostPtr->_y = YSize-2;
		break;
		case 2:
			invincibleGhostPtr->_x = XSize-2;
			invincibleGhostPtr->_y = 1;
		break;
		case 3:
			invincibleGhostPtr->_x = XSize-2;
			invincibleGhostPtr->_y = YSize-2;
		break;
	}
	initializeCharacter(invincibleGhostPtr,invincibleGhostPtr->_x,invincibleGhostPtr->_y,0, &INVINCIBLE_GHOST_IMAGE);
}

