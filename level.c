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
	unsigned short innerVerticalLevel;
	switch(level)
	{
		case 1: case 5: case 10: case 15:
			innerVerticalLevel = 0;
		break;
		case 2:
			innerVerticalLevel = 8;
		break;
		case 3:
			innerVerticalLevel = 12;
		break;
		case 4:
			innerVerticalLevel = YSize-4;
		break;
		case 6:
			innerVerticalLevel = 8;
		break;
		case 7:
			innerVerticalLevel = 12;
		break;
		case 8:
			innerVerticalLevel = 16;
		break;
		case 9:
			innerVerticalLevel = YSize-4;
		break;
		case 11:
			innerVerticalLevel = 8;
		break;
		case 12:
			innerVerticalLevel = 12;
		break;
		case 13:
			innerVerticalLevel =16;
		break;
		case 14:
			innerVerticalLevel = YSize-4;
		break;
		case 16:
			innerVerticalLevel = 8;
		break;
		case 17:
			innerVerticalLevel = 12;
		break;
		case 18:
			innerVerticalLevel = YSize-4;
		break;
		case 19:
			innerVerticalLevel = 12;
		break;
		case 20:
			innerVerticalLevel = YSize-4;
		break;
	}
	drawInnerVerticalWall();
	return innerVerticalLevel;
}


void fillLevelWithCharacters(
						  Character * playerPtr, Character * powerUpPtr, 
						  Character * ghostPtr1, Character * ghostPtr2,
						  Character * ghostPtr3, Character * ghostPtr4,
						  Character * ghostPtr5, Character * ghostPtr6,
						  Character * ghostPtr7, Character * ghostPtr8,
						  Character * bombPtr1, Character * bombPtr2,
						  Character * bombPtr3, Character * bombPtr4,
						  Character * invincibleGhostPtr, 
						  Character * missilePtr, Character * gunPtr)
{
	short corner = rand()%4;
	short chirality = rand()%2;
	int b1x, b2x, b3x, b4x;
	int b1y, b2y, b3y, b4y;

	// Ghosts
	initializeCharacter(ghostPtr2,XSize/6+rand()%4-2,YSize/6+rand()%4-2+1,'O',1);
	displayCharacter(ghostPtr2);

	initializeCharacter(ghostPtr3,XSize/6+rand()%4-2,YSize/2+rand()%4-2,'O',1);
	displayCharacter(ghostPtr3);
	
	initializeCharacter(ghostPtr4,XSize/6+rand()%4-2,YSize-YSize/6+rand()%4-2,'O',1);
	displayCharacter(ghostPtr4);
	
	initializeCharacter(ghostPtr5,XSize/2+rand()%4-2,YSize/6+rand()%4-2+1,'O',1);
	displayCharacter(ghostPtr5);

	initializeCharacter(ghostPtr6,XSize/2+rand()%4-2,YSize-YSize/6+rand()%4-2,'O',1);
	displayCharacter(ghostPtr6);

	initializeCharacter(ghostPtr7,XSize-XSize/6+rand()%4-2,YSize/6+rand()%4-2+1,'O',1);
	displayCharacter(ghostPtr7);
	
	initializeCharacter(ghostPtr8,XSize-XSize/6+rand()%4-2,YSize/2+rand()%4-2,'O',1);
	displayCharacter(ghostPtr8);
	
	initializeCharacter(ghostPtr1,XSize-XSize/6+rand()%4-2,YSize-YSize/6+rand()%4-2,'O',1);
	displayCharacter(ghostPtr1);

	
	// Player
	do
	{
		initializeCharacter(playerPtr,XSize/2+rand()%4-2,YSize/2+rand()%4-2,'*',1);
	} while(innerWallReached(playerPtr));
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
			initializeCharacter(bombPtr1,b1x, b1y,'X',0);

			initializeCharacter(bombPtr2,b2x, b2y,'X',0);

			initializeCharacter(bombPtr3,b3x, b3y,'X',0);

			initializeCharacter(bombPtr4,b4x, b4y,'X',0);
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
			initializeCharacter(bombPtr1,b1x, b1y,'X',0);
			relocateCharacter(bombPtr1, playerPtr, ghostPtr1, ghostPtr1, ghostPtr1, 
							   ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
							   ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);		
			
			initializeCharacter(bombPtr3,b3x, b3y,'X',0);
			relocateCharacter(bombPtr3, playerPtr, bombPtr1, ghostPtr1, ghostPtr1, 
							ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
							ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);	

			if(level>=TWO_BOMB_START_LEVEL) // only use bomb1 and bomb3 previously relocated
			{
				initializeCharacter(bombPtr2, bombPtr1->_x, bombPtr1->_y, 'X', 0);
				initializeCharacter(bombPtr4, bombPtr3->_x, bombPtr3->_y, 'X', 0);
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
				initializeCharacter(bombPtr2,b2x, b2y,'X',0);
				relocateCharacter(bombPtr2, playerPtr, bombPtr1, bombPtr3, ghostPtr1, 
								   ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
								   ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);		
				
				initializeCharacter(bombPtr4,b4x, b4y,'X',0);
				relocateCharacter(bombPtr4, playerPtr, bombPtr1, bombPtr2, bombPtr3, 
				ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
				ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);	
			}
		}
	} while(nearInnerWall(bombPtr1) || nearInnerWall(bombPtr2) || nearInnerWall(bombPtr3) || nearInnerWall(bombPtr4));
	
	displayCharacter(bombPtr1);
	displayCharacter(bombPtr2);
	displayCharacter(bombPtr3);
	displayCharacter(bombPtr4);
				
	do
	{
		// Power-ups
		initializeCharacter(powerUpPtr,XSize/2,YSize/2,'P',1);
		relocateCharacter(powerUpPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
							   ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
							   ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);	
		initializeCharacter(powerUpPtr,powerUpPtr->_x,powerUpPtr->_y,'P',1);
	} while(innerWallReached(powerUpPtr));
	displayCharacter(powerUpPtr);
		
    initializeCharacter(missilePtr, 0, 0, '.',0);
	
	initializeCharacter(gunPtr, XSize/2, YSize/2, '!', 0);
	relocateCharacter(gunPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
						  ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
						   ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
	
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

