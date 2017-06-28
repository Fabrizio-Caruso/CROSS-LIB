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

extern Image LEFT_ENEMY_MISSILE_IMAGE;
extern Image RIGHT_ENEMY_MISSILE_IMAGE;

extern Character invincibleGhost;
extern Character player; 
extern Character powerUp;
extern Character gun;
extern Character missile;
extern Character* ghosts[GHOSTS_NUMBER];
extern Character* bombs[BOMBS_NUMBER];

extern Character leftEnemyMissile;
extern Character rightEnemyMissile;



void drawInnerVerticalWall(void)
{
	DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);
}

void updateInnerWallVerticalData(void)
{		
	switch(level)
	{
		case 1: case 5: case 10:
			innerVerticalWallLength = 0;
		break;
		case 2:
			innerVerticalWallLength = 8;
		break;
		case 3:
			innerVerticalWallLength = 12;
		break;
		case 4:
			innerVerticalWallLength = YSize-6;
		break;
		case 6:
			innerVerticalWallLength = 8;
		break;
		case 7:
			innerVerticalWallLength = 12;
		break;
		case 8:
			innerVerticalWallLength = 16;
		break;
		case 9:
			innerVerticalWallLength = YSize-6;
		break;
		case 11:
			innerVerticalWallLength = 8;
		break;
		case 12:
			innerVerticalWallLength = 12;
		break;
		case 13:
			innerVerticalWallLength = YSize-6;
		break;
		case 14:
			innerVerticalWallLength = YSize-4;
		break;
		case 15:
			innerVerticalWallLength = 8;
		break;
		case 16:
			innerVerticalWallLength = 12;
		break;
		case 17:
			innerVerticalWallLength = YSize-6;
		break;
		case 18:
			innerVerticalWallLength = YSize-4;
		break;
		case 19:
			innerVerticalWallLength = 12;
		break;
		case 20:
			innerVerticalWallLength = YSize-4;
		break;
	}
	innerVerticalWallX = XSize / 2;
    innerVerticalWallY = YSize/2-(innerVerticalWallLength/2);
}


void fillLevelWithCharacters(void)
{
	short corner = rand()%4;
	short chirality = rand()%2;
	int b1x, b2x, b3x, b4x;
	int b1y, b2y, b3y, b4y;
	Character *dummyBombs[BOMBS_NUMBER];
	char i;
	int x1,x2,x3,x4,x5,x6,x0;
	int y1,y2,y3,y4,y5,y6,y0;
	#if GHOSTS_NUMBER == 8
	{
		int x7; int y7;
		x7 = XSize-XSize/6+rand()%3-3; 
		y7 = YSize/2+rand()%3-2;
		initializeCharacter(ghosts[7],x7,y7,1,&GHOST_IMAGE);
		DRAW_GHOST(ghosts[7]->_x, ghosts[7]->_y, ghosts[7]->_imagePtr);
	}
	#endif
	
	INIT_IMAGES();
	

	// 7 Ghosts case
	x1 = XSize/6+rand()%3-1;       y1 = YSize/6+rand()%3-2+1;
	x2 = XSize/6+rand()%3-1;       y2 = YSize/2+rand()%3-2;
	x3 = XSize/6+rand()%3-1;       y3 = YSize-YSize/6+rand()%3-3;
	x4 = XSize/2+rand()%3-1;       y4 = YSize/6+rand()%3-2+1;
	x5 = XSize/2+rand()%3-1;       y5 = YSize-YSize/6+rand()%3-3;
	x6 = XSize-XSize/6+rand()%3-3; y6 = YSize/6+rand()%3-2+1;
	x0 = XSize-XSize/6+rand()%3-3; y0 = YSize-YSize/6+rand()%3-3;

	
	initializeCharacter(ghosts[1],x1,y1,1,&GHOST_IMAGE);
	DRAW_GHOST(ghosts[1]->_x, ghosts[1]->_y, ghosts[1]->_imagePtr);
	
	initializeCharacter(ghosts[2],x2,y2,1, &GHOST_IMAGE);
	DRAW_GHOST(ghosts[2]->_x, ghosts[2]->_y, ghosts[2]->_imagePtr);
	
	initializeCharacter(ghosts[3],x3,y3,1,&GHOST_IMAGE);
	DRAW_GHOST(ghosts[3]->_x, ghosts[3]->_y, ghosts[3]->_imagePtr);

	initializeCharacter(ghosts[4],x4,y4,1,&GHOST_IMAGE);
	DRAW_GHOST(ghosts[4]->_x, ghosts[4]->_y, ghosts[4]->_imagePtr);

	initializeCharacter(ghosts[5],x5,y5,1,&GHOST_IMAGE);
	DRAW_GHOST(ghosts[5]->_x, ghosts[5]->_y, ghosts[5]->_imagePtr);

	initializeCharacter(ghosts[6],x6,y6,1,&GHOST_IMAGE);
	DRAW_GHOST(ghosts[6]->_x, ghosts[6]->_y, ghosts[6]->_imagePtr);
	
	initializeCharacter(ghosts[0],x0,y0,1,&GHOST_IMAGE);
	DRAW_GHOST(ghosts[0]->_x, ghosts[0]->_y, ghosts[0]->_imagePtr);
	


	// Player
	do
	{
		initializeCharacter(&player,XSize/2+rand()%4-2,YSize/2+rand()%4-2,1,&PLAYER_IMAGE);
	} while(nearInnerWall(&player));
	SET_TEXT_COLOR(PLAYER_COLOR);
	DRAW_PLAYER(player._x,player._y,player._imagePtr);
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
			
			// TODO: Remove hard-code size of bomb list
			dummyBombs[0] = ghosts[0];
			dummyBombs[1] = ghosts[0];
			dummyBombs[2] = ghosts[0];
			dummyBombs[3] = &player;
			for(i=0;i<BOMBS_NUMBER;++i)
			{	
				relocateCharacter(bombs[0], dummyBombs,4);		
			}
			
			initializeCharacter(bombs[2],b3x, b3y,0, &BOMB_IMAGE);
			
			// Keep below comments
			//dummyBombs[0] = ghosts[0];
			//dummyBombs[1] = ghosts[0];
			dummyBombs[2] = bombs[0];
			//dummyBombs[3] = &player;

			relocateCharacter(bombs[2], dummyBombs,4);		
								

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
				dummyBombs[3] = &player;

				relocateCharacter(bombs[1], dummyBombs,4);		

				
				initializeCharacter(bombs[3],b4x, b4y,0,&BOMB_IMAGE);
				
				// Keep below comments
				dummyBombs[0] = bombs[1];
				//dummyBombs[1] = bombs[2];
				//dummyBombs[2] = bombs[0];
				//dummyBombs[3] = &player;
				for(i=0;i<BOMBS_NUMBER;++i)
				{
					relocateCharacter(bombs[1], dummyBombs,4);
				}				
			}
		}
	} while(nearInnerWall(bombs[0]) || nearInnerWall(bombs[1]) || nearInnerWall(bombs[2]) || nearInnerWall(bombs[3]));
	
	for(i=0;i<BOMBS_NUMBER;++i)
	{
		DRAW_BOMB(bombs[i]->_x, bombs[i]->_y, bombs[i]->_imagePtr);
	}

	do
	{
		// Power-ups
		initializeCharacter(&powerUp,XSize/2,YSize/2,1,&POWERUP_IMAGE);
		relocateCharacter(&powerUp, bombs,4);	
		initializeCharacter(&powerUp,powerUp._x,powerUp._y,1,&POWERUP_IMAGE);
	} while(nearInnerWall(&powerUp));
	DRAW_POWERUP(powerUp._x, powerUp._y, powerUp._imagePtr);
		
    initializeCharacter(&missile, 0, 0,0,&MISSILE_IMAGE);
	
	do
	{
		initializeCharacter(&gun, XSize/2, YSize/2, 0, &GUN_IMAGE);
		relocateCharacter(&gun, bombs,4);
	} while(nearInnerWall(&gun));
	
	switch(corner)
	{
		case 0:
			invincibleGhost._x = 2;
			invincibleGhost._y = 2;
		break;
		case 1:
			invincibleGhost._x = 2;
			invincibleGhost._y = YSize-2;
		break;
		case 2:
			invincibleGhost._x = XSize-2;
			invincibleGhost._y = 1;
		break;
		case 3:
			invincibleGhost._x = XSize-2;
			invincibleGhost._y = YSize-2;
		break;
	}
	initializeCharacter(&invincibleGhost,invincibleGhost._x,invincibleGhost._y, 0,&INVINCIBLE_GHOST_IMAGE);

	initializeCharacter(&leftEnemyMissile,                2,YSize-2,            1,&LEFT_ENEMY_MISSILE_IMAGE);
	
	initializeCharacter(&rightEnemyMissile,         XSize-2,YSize-2,            1,&RIGHT_ENEMY_MISSILE_IMAGE);
}

