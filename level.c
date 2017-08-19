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
#include "sound_macros.h" 
#include "sleep_macros.h"

#include "level.h"

	
extern unsigned char innerVerticalWallX;
extern unsigned char innerVerticalWallY;
extern unsigned char innerVerticalWallLength;
extern unsigned char level;
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
extern Image BUBBLE_IMAGE;

extern Image LEFT_ENEMY_MISSILE_IMAGE;
extern Image RIGHT_ENEMY_MISSILE_IMAGE;

extern Image EXTRA_POINTS_IMAGE;

extern Image EXTRA_LIFE_IMAGE;
extern Image INVINCIBILITY_IMAGE;

extern Character invincibleGhost;
extern Character player; 
extern Character powerUp;
extern Character gun;
extern Character missile;
extern Character leftEnemyMissile;
extern Character rightEnemyMissile;
extern Character extraPoints;
extern Character extraLife;
extern Character invincibility;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character bubbles[BUBBLES_NUMBER];

extern char bubbles_x[BUBBLES_NUMBER];

extern unsigned char powerUp_blink;
extern unsigned char gun_blink;

void drawInnerVerticalWall(void)
{
	DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);
}


void updateInnerWallVerticalData(void)
{		
	switch(level)
	{
		case 1:
			innerVerticalWallLength = 0;
		break;
		case 2:
			innerVerticalWallLength = YSize-12;
		break;
		case 3:
			innerVerticalWallLength = YSize-8;
		break;
		case 4:
			innerVerticalWallLength = YSize-6;
		break;
		case 5:
			innerVerticalWallLength = 0;		
		break;
		case 6:
			innerVerticalWallLength = 0;
		break;
		case 7:
			innerVerticalWallLength = YSize-12;
		break;
		case 8:
			innerVerticalWallLength = YSize-8;
		break;
		case 9:
			innerVerticalWallLength = YSize-6;
		break;
		case 10:
			innerVerticalWallLength = 0;
		break;
		case 11:
			innerVerticalWallLength = 0;
		break;
		case 12:
			innerVerticalWallLength = YSize-12;
		break;
		case 13:
			innerVerticalWallLength = YSize-8;
		break;
		case 14:
			innerVerticalWallLength = YSize-6;
		break;
		case 15:
			innerVerticalWallLength = 0;
		break;
		case 16:
			innerVerticalWallLength = 0;
		break;
		case 17:
			innerVerticalWallLength = YSize-12;
		break;
		case 18:
			innerVerticalWallLength = YSize-8;
		break;
		case 19:
			innerVerticalWallLength = YSize-6;
		break;
		case 20:
			innerVerticalWallLength = 0;
		break;
		case 21:
			innerVerticalWallLength = YSize-14;
		break;
		case 22:
			innerVerticalWallLength = YSize-12;
		break;
		case 23:
			innerVerticalWallLength = YSize-8;
		break;
		case 24:
			innerVerticalWallLength = YSize-6;
		break;
		case 25:
			innerVerticalWallLength = 0;		
		break;
		case 26:
			innerVerticalWallLength = YSize-14;
		break;
		case 27:
			innerVerticalWallLength = YSize-12;
		break;
		case 28:
			innerVerticalWallLength = YSize-8;
		break;
		case 29:
			innerVerticalWallLength = YSize-6;
		break;
		case 30:
			innerVerticalWallLength = YSize-14;
		break;
		case 31:
			innerVerticalWallLength = YSize-14;
		break;
		case 32:
			innerVerticalWallLength = YSize-12;
		break;
		case 33:
			innerVerticalWallLength = YSize-8;
		break;
		case 34:
			innerVerticalWallLength = YSize-6;
		break;
		case 35:
			innerVerticalWallLength = YSize-14;
		break;
		case 36:
			innerVerticalWallLength = YSize-14;
		break;
		case 37:
			innerVerticalWallLength = YSize-12;
		break;
		case 38:
			innerVerticalWallLength = YSize-8;
		break;
		case 39:
			innerVerticalWallLength = YSize-6;
		break;
		case 40:
			innerVerticalWallLength = YSize-12;		
	}
	innerVerticalWallX = XSize / 2;
    innerVerticalWallY = YSize/2-(innerVerticalWallLength/2);
}


unsigned char rocketLevel(void)
{
	return level >= FIRST_BUBBLES_LEVEL && ((level%5)==2 || (level%5)==3);
}

unsigned char missileLevel(void)
{
	return level%5==4;
}	

unsigned char bossLevel(void)
{
	return level%5==0;
}

void initializeAwayFromWall(Character * characterPtr, unsigned char x, unsigned char y, unsigned char status, Image *imagePtr)
{
	do{
		initializeCharacter(characterPtr, x, y, status, imagePtr);
		relocateCharacter(characterPtr,bombs,4);
	} while(nearInnerWall(characterPtr));
}

	void fillLevelWithCharacters(unsigned char nGhosts)
	{
		unsigned short corner = rand()%4;
		unsigned short chirality = rand()%2;
		unsigned char b1x, b2x, b3x, b4x;
		unsigned char b1y, b2y, b3y, b4y;
		Character dummyBombs[BOMBS_NUMBER];
		unsigned char i;

		// TODO Replace with something else
		#if defined(__ATMOS__)
			INIT_GRAPHICS();
		#endif

		if(rocketLevel() || bossLevel())
		{
			for(i=0;i<BUBBLES_NUMBER;i++)
			{
				bubbles_x[i] = (unsigned char) (i+1)*(XSize/(BUBBLES_NUMBER+1));
				initializeCharacter(&bubbles[i],bubbles_x[i],YSize-1,1,&BUBBLE_IMAGE);
				DRAW_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);
			}
		}

		if(bossLevel())
		{
			nGhosts = level/8;
		}

		initializeCharacter(&ghosts[0],XSize-XSize/6+rand()%3-3,YSize-YSize/6+rand()%3-3,1,&GHOST_IMAGE);
		//DRAW_GHOST(ghosts[0]._x, ghosts[0]._y, ghosts[0]._imagePtr);
		
		if(nGhosts>1)
		{
			initializeCharacter(&ghosts[1],XSize/6+rand()%3-1,YSize/6+rand()%3-2+1,1,&GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[1],7,1,0,&DEAD_GHOST_IMAGE);
		}
		
		if(nGhosts>2)
		{
			initializeCharacter(&ghosts[2],XSize/6+rand()%3-1,YSize/2+rand()%3-2,1, &GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[2],6,1,0, &DEAD_GHOST_IMAGE);
		}
		
		if(nGhosts>3)
		{
			initializeCharacter(&ghosts[3],XSize/6+rand()%3-1,YSize-YSize/6+rand()%3-3,1,&GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[3],5,1,0,&DEAD_GHOST_IMAGE);
		}
		
		if(nGhosts>4)
		{
			initializeCharacter(&ghosts[4],XSize/2+rand()%3-1,YSize/6+rand()%3-2+1,1,&GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[4],4,1,0,&DEAD_GHOST_IMAGE);	
		}
		
		if(nGhosts>5)
		{
			initializeCharacter(&ghosts[5],XSize/2+rand()%3-1,YSize-YSize/6+rand()%3-3,1,&GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[5],3,1,0,&DEAD_GHOST_IMAGE);	
		}
		
		#if GHOSTS_NUMBER>=7
		if(nGhosts>6)
		{
			initializeCharacter(&ghosts[6],XSize-XSize/6+rand()%3-3,YSize/6+rand()%3-2+1,1,&GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[6],2,1,0,&DEAD_GHOST_IMAGE);	
		}
		#endif
		
		#if GHOSTS_NUMBER==8
		if(nGhosts>7)
		{
			initializeCharacter(&ghosts[7],XSize-XSize/6+rand()%3-3,YSize/2+rand()%3-2,1,&GHOST_IMAGE);
		}
		else
		{
			initializeCharacter(&ghosts[7],1,1,0,&DEAD_GHOST_IMAGE);	
		}
		#endif
		
		for(i=0;i<GHOSTS_NUMBER;++i)
		{
			DRAW_GHOST(ghosts[i]._x, ghosts[i]._y, ghosts[i]._imagePtr);
		}
		
		// Player
		do
		{
			initializeCharacter(&player,XSize/2+rand()%4-2,YSize/2+rand()%4-2,1,&PLAYER_IMAGE);
		} while(nearInnerWall(&player));
		DRAW_PLAYER(player._x,player._y,player._imagePtr);


		do
		{
			// Bombs
			if(level<FIRST_VERY_HARD_LEVEL) // HARD but NOT VERY HARD -> 2 bombs close to vertical borders
			{	
				b2x = 1+1;
				b2y = YSize/2-2+rand()%5;
				
				b3x = XSize-2-1;
				b3y = YSize/2-2+rand()%5;
				
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
				b2y = YSize/2-2+rand()%5;
				
				b3x = XSize-2;
				b3y = YSize/2-2+rand()%5;
				
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
				initializeCharacter(&bombs[0],b1x, b1y,0,&BOMB_IMAGE);

				initializeCharacter(&bombs[1],b2x, b2y,0,&BOMB_IMAGE);

				initializeCharacter(&bombs[2],b3x, b3y,0,&BOMB_IMAGE);

				initializeCharacter(&bombs[3],b4x, b4y,0,&BOMB_IMAGE);
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
				initializeCharacter(&bombs[0],b1x, b1y,0,&BOMB_IMAGE);
				
				// TODO: Remove hard-code size of bomb list
				dummyBombs[0]._x = ghosts[0]._x;				
				dummyBombs[0]._y = ghosts[0]._y;
				dummyBombs[1]._x = ghosts[0]._x;
				dummyBombs[1]._y = ghosts[0]._y;
				dummyBombs[2]._x = ghosts[0]._x;
				dummyBombs[2]._y = ghosts[0]._y;
				dummyBombs[3]._x = player._x;
				dummyBombs[3]._y = player._y;
				for(i=0;i<BOMBS_NUMBER;++i)
				{	
					relocateCharacter(&bombs[0], dummyBombs,4);		
				}
				

				initializeCharacter(&bombs[2],b3x, b3y,0, &BOMB_IMAGE);
				
				// Keep below comments
				//dummyBombs[0] = ghosts[0];
				//dummyBombs[1] = ghosts[0];
				dummyBombs[2]._x = bombs[0]._x;
				dummyBombs[2]._y = bombs[0]._y;
				//dummyBombs[3] = &player;

				relocateCharacter(&bombs[2], dummyBombs,4);		
			
				if(level>=TWO_BOMB_START_LEVEL && level<ONE_BOMB_START_LEVEL) // only use bomb1 and bomb3 previously relocated
				{
					initializeCharacter(&bombs[1], bombs[0]._x, bombs[0]._y, 0,&BOMB_IMAGE);
					initializeCharacter(&bombs[3], bombs[2]._x, bombs[2]._y, 0,&BOMB_IMAGE);
				}
				else if (level>=ONE_BOMB_START_LEVEL) // only use bomb1 and bomb3 previously relocated
				{
					initializeCharacter(&bombs[2], bombs[0]._x, bombs[0]._y, 0,&BOMB_IMAGE);
					initializeCharacter(&bombs[1], bombs[0]._x, bombs[0]._y, 0,&BOMB_IMAGE);
					initializeCharacter(&bombs[3], bombs[0]._x, bombs[0]._y, 0,&BOMB_IMAGE);
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
					initializeCharacter(&bombs[1],b2x, b2y, 0, &BOMB_IMAGE);
					
					dummyBombs[0]._x = ghosts[0]._y;
					dummyBombs[1]._x = bombs[2]._y;
					dummyBombs[2]._x = bombs[0]._y;
					dummyBombs[3]._x = player._y;
					dummyBombs[0]._y = ghosts[0]._y;
					dummyBombs[1]._y = bombs[2]._y;
					dummyBombs[2]._y = bombs[0]._y;
					dummyBombs[3]._y = player._y;
					relocateCharacter(&bombs[1], dummyBombs,4);		

					if(level<THREE_BOMB_START_LEVEL)
					{
						initializeCharacter(&bombs[3],b4x, b4y,0,&BOMB_IMAGE);
						
						// Keep below comments
						dummyBombs[0]._x = bombs[1]._x;
						dummyBombs[0]._y = bombs[1]._y;						
						//dummyBombs[1] = bombs[2];
						//dummyBombs[2] = bombs[0];
						//dummyBombs[3] = &player;
						for(i=0;i<BOMBS_NUMBER;++i)
						{
							relocateCharacter(&bombs[1], dummyBombs,4);
						}		
					}
					else
					{
						//initializeCharacter(bombs[3],b2x, b2y,0,&BOMB_IMAGE);
						bombs[3]._x = bombs[1]._x;
						bombs[3]._y = bombs[1]._y;
						bombs[3]._imagePtr = &BOMB_IMAGE;
					}
				}
			}
		} while(nearInnerWall(&bombs[0]) || nearInnerWall(&bombs[1]) || nearInnerWall(&bombs[2]) || nearInnerWall(&bombs[3]));
		
		for(i=0;i<BOMBS_NUMBER;++i)
		{
			DRAW_BOMB(bombs[i]._x, bombs[i]._y, bombs[i]._imagePtr);
		}


		initializeAwayFromWall(&powerUp,XSize/2,YSize/2,1,&POWERUP_IMAGE);
		DRAW_POWERUP(powerUp._x, powerUp._y, powerUp._imagePtr);
			
		initializeCharacter(&missile, 0, 0,0,&MISSILE_IMAGE);
		
		if(bossLevel())
		{
			initializeAwayFromWall(&gun,XSize/2, YSize/2, 1, &GUN_IMAGE);
			DRAW_GUN(gun._x, gun._y, gun._imagePtr);
		}
		else
		{
			initializeAwayFromWall(&gun,XSize/2, YSize/2, 0, &GUN_IMAGE);
		}

		initializeAwayFromWall(&extraPoints, XSize/2, YSize/2, 0, &EXTRA_POINTS_IMAGE);
		
		initializeAwayFromWall(&extraLife, XSize/2, YSize/2, 0, &EXTRA_LIFE_IMAGE);

		initializeAwayFromWall(&invincibility, XSize/2, YSize/2, 0, &INVINCIBILITY_IMAGE);
		
		switch(corner)
		{
			case 0:
				invincibleGhost._x = (unsigned char) 2;
				invincibleGhost._y = (unsigned char) 2;
			break;
			case 1:
				invincibleGhost._x = (unsigned char) 2;
				invincibleGhost._y = (unsigned char) YSize-2;
			break;
			case 2:
				invincibleGhost._x = (unsigned char) XSize-2;
				invincibleGhost._y = (unsigned char) 1;
			break;
			default:
				invincibleGhost._x = (unsigned char) XSize-2;
				invincibleGhost._y = (unsigned char) YSize-2;
			break;
		}
		initializeCharacter(&invincibleGhost,invincibleGhost._x,invincibleGhost._y, 0, &INVINCIBLE_GHOST_IMAGE);
		if(missileLevel() || bossLevel())
		{	
			initializeCharacter(&rightEnemyMissile,         XSize-1,      4,            1,&RIGHT_ENEMY_MISSILE_IMAGE);
			initializeCharacter(&leftEnemyMissile,                0,YSize-4,            1,&LEFT_ENEMY_MISSILE_IMAGE);
			DRAW_MISSILE(leftEnemyMissile._x,leftEnemyMissile._y,leftEnemyMissile._imagePtr);		
			DRAW_MISSILE(rightEnemyMissile._x,rightEnemyMissile._y,rightEnemyMissile._imagePtr);			
		}
	}




unsigned char computeArrowRange(void)
{
		return level/10;
}


