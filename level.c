#include "character.h"
#include "settings.h"

#include <stdlib.h>

#include "settings.h"
#include "display_macros.h"

#include "level.h"

extern unsigned short innerVerticalWallX;
extern unsigned short innerVerticalWallY;
extern unsigned short innerVerticalWallLength;
extern unsigned short level;
extern unsigned char XSize;
extern unsigned char YSize;

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
	
	// Ghosts
	initializeCharacter(ghosts[1],XSize/6+rand()%4-2,YSize/6+rand()%4-2+1,'O',1);
	displayCharacter(ghosts[1]);

	initializeCharacter(ghosts[2],XSize/6+rand()%4-2,YSize/2+rand()%4-2,'O',1);
	displayCharacter(ghosts[2]);
	
	initializeCharacter(ghosts[3],XSize/6+rand()%4-2,YSize-YSize/6+rand()%4-2,'O',1);
	displayCharacter(ghosts[3]);
	
	initializeCharacter(ghosts[4],XSize/2+rand()%4-2,YSize/6+rand()%4-2+1,'O',1);
	displayCharacter(ghosts[4]);

	initializeCharacter(ghosts[5],XSize/2+rand()%4-2,YSize-YSize/6+rand()%4-2,'O',1);
	displayCharacter(ghosts[5]);

	initializeCharacter(ghosts[6],XSize-XSize/6+rand()%4-2,YSize/6+rand()%4-2+1,'O',1);
	displayCharacter(ghosts[6]);
	
	initializeCharacter(ghosts[7],XSize-XSize/6+rand()%4-2,YSize/2+rand()%4-2,'O',1);
	displayCharacter(ghosts[7]);
	
	initializeCharacter(ghosts[0],XSize-XSize/6+rand()%4-2,YSize-YSize/6+rand()%4-2,'O',1);
	displayCharacter(ghosts[0]);

	
	// Player
	do
	{
		initializeCharacter(playerPtr,XSize/2+rand()%4-2,YSize/2+rand()%4-2,'*',1);
	} while(nearInnerWall(playerPtr));
	SET_TEXT_COLOR(PLAYER_COLOR);
	displayCharacter(playerPtr);
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
			initializeCharacter(bombs[0],b1x, b1y,'X',0);

			initializeCharacter(bombs[1],b2x, b2y,'X',0);

			initializeCharacter(bombs[2],b3x, b3y,'X',0);

			initializeCharacter(bombs[3],b4x, b4y,'X',0);
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
			initializeCharacter(bombs[0],b1x, b1y,'X',0);
			
			dummyBombs[0] = ghosts[0];
			dummyBombs[1] = ghosts[0];
			dummyBombs[2] = ghosts[0];
			dummyBombs[3] = playerPtr;
			for(i=0;i<BOMBS_NUMBER;++i)
			{	
				relocateCharacter(bombs[0], dummyBombs, ghosts);		
			}
			
			initializeCharacter(bombs[2],b3x, b3y,'X',0);
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
				initializeCharacter(bombs[1], bombs[0]->_x, bombs[0]->_y, 'X', 0);
				initializeCharacter(bombs[3], bombs[2]->_x, bombs[2]->_y, 'X', 0);
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
				initializeCharacter(bombs[1],b2x, b2y,'X',0);
				
				dummyBombs[0] = ghosts[0];
				dummyBombs[1] = bombs[2];
				dummyBombs[2] = bombs[0];
				dummyBombs[3] = playerPtr;
				for(i=0;i<BOMBS_NUMBER;++i)
				{	
					relocateCharacter(bombs[1], dummyBombs, ghosts);		
				}
				
				initializeCharacter(bombs[3],b4x, b4y,'X',0);
				
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
		displayCharacter(bombs[i]);
	}

	do
	{
		// Power-ups
		initializeCharacter(powerUpPtr,XSize/2,YSize/2,'P',1);
		relocateCharacter(powerUpPtr, bombs, ghosts);	
		initializeCharacter(powerUpPtr,powerUpPtr->_x,powerUpPtr->_y,'P',1);
	} while(nearInnerWall(powerUpPtr));
	displayCharacter(powerUpPtr);
		
    initializeCharacter(missilePtr, 0, 0, '.',0);
	
	do
	{
		initializeCharacter(gunPtr, XSize/2, YSize/2, '!', 0);
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
	initializeCharacter(invincibleGhostPtr,invincibleGhostPtr->_x,invincibleGhostPtr->_y,'+',0);
}

