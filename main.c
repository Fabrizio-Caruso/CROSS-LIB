
/*
** Portable barely playable chasing game
**
** Fabrizio Caruso (fabrizio_caruso@hotmail.com)
**
*/
 
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include <conio.h>

#include <unistd.h>

#include <time.h>
#include <joystick.h>

#include "settings.h"
#include "character.h"
#include "strategy.h"
#include "display.h"
#include "missile.h"

unsigned int invincibleSlowDown = 30000;

unsigned int invincibleXCountDown = 100;
unsigned int invincibleYCountDown = 100;

unsigned int ghostSlowDown;
unsigned int powerUpCoolDown;
unsigned int gunCoolDown;
unsigned int ghostLevel = 1u;
unsigned long points = 0ul;
unsigned char ghostSmartness = 1u; // 9u is max = impossible 

unsigned short playerDirection = 0; // 0: right, 1: down, 2: left, 3: up
unsigned short missileDirection = 0;
unsigned short playerFire = 0;
unsigned short guns = 3;

unsigned short lives;

unsigned short innerVerticalWallY; 
unsigned short innerVerticalWallX; 
unsigned short innerVerticalWallLength;


// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. ghostSmartness (how smart ghosts are in avoiding their death)
// 4. invincibleXCountDown (time needed to activate the invincible ghost)
// 5. invincibleYCountDown
// 6. invincibleSlowDown (how much the invincible ghost is slowed-down)
// 7. invincibleLoopTrigger (how long before the invincible ghost appears)
unsigned short level = 1;

unsigned int invincibleLoopTrigger = 1000;
unsigned short ghostCount = 8;
unsigned short invincibleGhostCountTrigger = 2;


int playerReached(Character * hunterPtr1, Character * hunterPtr2, Character * hunterPtr3, Character * hunterPtr4, 
				  Character * hunterPtr5, Character * hunterPtr6, Character * hunterPtr7, Character * hunterPtr8, 
                  Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr1,preyPtr) || areCharctersAtSamePosition(hunterPtr2,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr3,preyPtr) || areCharctersAtSamePosition(hunterPtr4,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr5,preyPtr) || areCharctersAtSamePosition(hunterPtr6,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr7,preyPtr) || areCharctersAtSamePosition(hunterPtr8,preyPtr));
}

int playerReachedBombs(Character * bombPtr1, Character * bombPtr2, Character * bombPtr3, Character * bombPtr4,  
					   Character* ghostPtr)
{
	return(areCharctersAtSamePosition(bombPtr1,ghostPtr) || areCharctersAtSamePosition(bombPtr2,ghostPtr) ||
		   areCharctersAtSamePosition(bombPtr3,ghostPtr) || areCharctersAtSamePosition(bombPtr4,ghostPtr));
}

int charactersMeet(Character * hunterPtr1, Character * hunterPtr2, Character * hunterPtr3, 
				   Character * hunterPtr4, Character * hunterPtr5, Character * hunterPtr6,
				   Character * hunterPtr7,
				   Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr1,preyPtr) || areCharctersAtSamePosition(hunterPtr2,preyPtr) ||
	       areCharctersAtSamePosition(hunterPtr3,preyPtr) || areCharctersAtSamePosition(hunterPtr4,preyPtr) ||
	       areCharctersAtSamePosition(hunterPtr5,preyPtr) || areCharctersAtSamePosition(hunterPtr6,preyPtr) ||
		   areCharctersAtSamePosition(hunterPtr7,preyPtr));
}

int powerUpReached(Character * hunterPtr, Character* preyPtr)
{
	return(areCharctersAtSamePosition(hunterPtr, preyPtr));
}

int computeInvincibleCountDown()
{
	return 125 - level*5;
}

int computeInvincibleSlowDown(int loop)
{
	if(loop<1000)
	{
		return 32000 - (level/2+1) * 1000 - loop/2;
	}
	else if(loop<11000)
	{
		return 32000 - level * 1000 - loop;
	}
	return 0; // You must die!
}


void movePlayer(Character *playerPtr, char kbInput)
{
	if((kbInput=='W') || (kbInput=='w'))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = UP;
	}
	else if((kbInput=='S') || (kbInput=='s'))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = DOWN;
	}
	else if((kbInput=='A') || (kbInput=='a'))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = LEFT;
	}
	else if((kbInput=='D') || (kbInput=='d'))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = RIGHT;
	}
	else if(kbInput==' ')
	{
		playerFire = 1;
	}
	/*
	else if((kbInput=='L') || (kbInput=='l'))
	{
		ghostCount = 0;
		playerPtr->_ch = 'Z';
		displayCharacter(playerPtr);
		sleep(1);
	}
	*/
	displayCharacter(playerPtr);
}


void movePlayerByJoystick(Character *playerPtr, unsigned char joyInput)
{
	if(JOY_BTN_UP(joyInput))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = UP;
	}
	else if(JOY_BTN_DOWN(joyInput))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_y;
		invincibleYCountDown = computeInvincibleCountDown();
		playerDirection = DOWN;
	}
	else if(JOY_BTN_LEFT(joyInput))
	{
		deleteCharacter(playerPtr);
		--playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = LEFT;
	}
	else if(JOY_BTN_RIGHT(joyInput))
	{
		deleteCharacter(playerPtr);
		++playerPtr->_x;
		invincibleXCountDown = computeInvincibleCountDown();
		playerDirection = RIGHT;
	}
	else if(JOY_BTN_FIRE(joyInput))
	{
		playerFire = 1;
	}
	displayCharacter(playerPtr);
}



void checkBombsVsGhost(Character * bombPtr1, Character * bombPtr2, 
					   Character * bombPtr3, Character * bombPtr4,
					   Character * ghostPtr, unsigned short * ghostCountPtr)
{
	if(ghostPtr->_alive && playerReachedBombs(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr))
	{
		gotoxy(ghostPtr->_x,ghostPtr->_y);
		cputc('X');
		ghostPtr->_alive = 0;
		ghostPtr->_status = 0;
		ghostPtr->_ch = 'X';
		points+=GHOST_VS_BOMBS_BONUS;
		--(*ghostCountPtr);
	}
}
						

void checkBombsVsGhosts(Character * bombPtr1, Character * bombPtr2, 
						Character * bombPtr3, Character * bombPtr4,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8,
						unsigned short *ghostCountPtr)
{
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr1, ghostCountPtr);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr2, ghostCountPtr);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr3, ghostCountPtr);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr4, ghostCountPtr);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr5, ghostCountPtr);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr6, ghostCountPtr);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr7, ghostCountPtr);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr8, ghostCountPtr);
}


int safeLocation(int x, int y, 
				Character * bombPtr1, Character * bombPtr2, 
				Character * bombPtr3, Character * bombPtr4,
				Character * ghostPtr1, Character * ghostPtr2, 
				Character * ghostPtr3, Character * ghostPtr4,
				Character * ghostPtr5, Character * ghostPtr6, 
				Character * ghostPtr7, Character * ghostPtr8)
{
	return !isCharacterAtLocation(x,y,bombPtr1) && !isCharacterAtLocation(x,y,bombPtr2) &&
	!isCharacterAtLocation(x,y,bombPtr3) && !isCharacterAtLocation(x,y,bombPtr4) &&
	!isCharacterAtLocation(x,y,ghostPtr1) && !isCharacterAtLocation(x,y,ghostPtr2) &&
	!isCharacterAtLocation(x,y,ghostPtr3) && !isCharacterAtLocation(x,y,ghostPtr4) &&
	!isCharacterAtLocation(x,y,ghostPtr5) && !isCharacterAtLocation(x,y,ghostPtr6) &&
	!isCharacterAtLocation(x,y,ghostPtr7) && !isCharacterAtLocation(x,y,ghostPtr8);
}


void relocateCharacter(int XSize, int YSize, Character * characterPtr, 
						Character * bombPtr1, Character * bombPtr2, 
						Character * bombPtr3, Character * bombPtr4,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8)
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
	
	safe = safeLocation(x,y, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
						ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
						ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
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
	return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
}



unsigned short drawInnerVerticalWallForLevel(int XSize,int YSize, unsigned short level)
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
	drawInnerVerticalWall(XSize,YSize, innerVerticalLevel, &innerVerticalWallX, &innerVerticalWallY);
	return innerVerticalLevel;
}


void fillLevelWithCharacters(int XSize, int YSize,
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
	displayCharacter(playerPtr);

	
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
			relocateCharacter(XSize, YSize, bombPtr1, playerPtr, ghostPtr1, ghostPtr1, ghostPtr1, 
							   ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
							   ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);		
			
			initializeCharacter(bombPtr3,b3x, b3y,'X',0);
			relocateCharacter(XSize, YSize, bombPtr3, playerPtr, bombPtr1, ghostPtr1, ghostPtr1, 
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
				relocateCharacter(XSize, YSize, bombPtr2, playerPtr, bombPtr1, bombPtr3, ghostPtr1, 
								   ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
								   ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);		
				
				initializeCharacter(bombPtr4,b4x, b4y,'X',0);
				relocateCharacter(XSize, YSize, bombPtr4, playerPtr, bombPtr1, bombPtr2, bombPtr3, 
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
		relocateCharacter(XSize, YSize, powerUpPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
							   ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, 
							   ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);	
		initializeCharacter(powerUpPtr,powerUpPtr->_x,powerUpPtr->_y,'P',1);
	} while(innerWallReached(powerUpPtr));
	displayCharacter(powerUpPtr);
		
    initializeCharacter(missilePtr, 0, 0, '.',0);
	
	initializeCharacter(gunPtr, XSize/2, YSize/2, '!', 0);
	relocateCharacter(XSize, YSize, gunPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
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



unsigned int computeGhostSlowDown()
{
   if(ghostLevel<10)
   {
	   return 32000-level*200;
   }
   else if(ghostLevel<500)
   {
	   return 32000-ghostLevel*10-level*200;
   }
   else if(ghostLevel<900)
   {
	   return 32000-ghostLevel*15-level*200;
   }
   else if(ghostLevel<1000)
   {
	   return 32000-ghostLevel*20-level*200;
   }
   else
	   return 0; // You must die!
}


void decreaseGhostLevel(unsigned short level)
{
	if(ghostLevel>level)
		ghostLevel-=level;
	else
		ghostLevel=0;
}

void checkGhostsVsGhosts(Character *ghostPtr1, Character *ghostPtr2, Character *ghostPtr3, Character *ghostPtr4,
						 Character *ghostPtr5, Character *ghostPtr6, Character *ghostPtr7, Character *ghostPtr8,
						 unsigned short *ghostCountPtr)
{
	if(ghostPtr8->_alive && charactersMeet(ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8))
	{
		die(ghostPtr8);
		points+=GHOST_VS_GHOST_BONUS;
	    --(*ghostCountPtr);
	}
	if(ghostPtr1->_alive && charactersMeet(ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1))
	{
		die(ghostPtr1);
		points+=GHOST_VS_GHOST_BONUS;
		--(*ghostCountPtr);
	}
	if(ghostPtr2->_alive && charactersMeet(ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2))
	{
		die(ghostPtr2);
		points+=GHOST_VS_GHOST_BONUS;
		--(*ghostCountPtr);
	}
	if(ghostPtr3->_alive && charactersMeet(ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3))
	{
		die(ghostPtr3);
		points+=GHOST_VS_GHOST_BONUS;
		--(*ghostCountPtr);
	}
	if(ghostPtr4->_alive && charactersMeet(ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4))
	{
		die(ghostPtr4);
		points+=GHOST_VS_GHOST_BONUS;
		--(*ghostCountPtr);
	}
	if(ghostPtr5->_alive && charactersMeet(ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5))
	{
		die(ghostPtr5);
		points+=GHOST_VS_GHOST_BONUS;
		--(*ghostCountPtr);
	}
	if(ghostPtr6->_alive && charactersMeet(ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6))
	{
		die(ghostPtr6);
		points+=GHOST_VS_GHOST_BONUS;
		--(*ghostCountPtr);
	}
	if(ghostPtr7->_alive && charactersMeet(ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7))
	{
		die(ghostPtr7);
		points+=GHOST_VS_GHOST_BONUS;
		--(*ghostCountPtr);
	}
}

int computeGhostSmartness()
{
	if(level<=4)
	{
		return level+3;
	}
	else
	{
		return 8;
	}
}

void computePowerUp(int *coolDownDecreasePtr, int *powerUpInitialCoolDownPtr)
{
	*coolDownDecreasePtr = 200-(level/2-1)*10;
	*powerUpInitialCoolDownPtr = 200+(level/2-1)*10;
}

int computeGunInitialCoolDown()
{
	if(level<=5)
		return 600;
	else
		return 600 + level * 10;
}

int computeInvincibleGhostCountTrigger()
{
	if(level<=7)
		return level/2 + 1;
	else
		return 5;
}

int computeInvincibleLoopTrigger()
{
	if(level==FINAL_LEVEL)
		return 50;
	else if (level==FINAL_LEVEL - 1)
		return 250;
	else if (level==FINAL_LEVEL - 2)
		return 500;
	else
		return 1000 - level*20;
}



int main(void)
{
    unsigned char XSize, YSize; // TODO: Make it global
	#ifdef _KEYBOARD
	char kbInput;
	#endif // _KEYBOARD
	
	unsigned char joyInput;
	
	Character ghost_1; 	
	Character ghost_2; 
    Character ghost_3; 	
	Character ghost_4; 
	Character ghost_5; 	
	Character ghost_6; 
    Character ghost_7; 	
	Character ghost_8; 
	Character invincibleGhost;
	
	Character bomb_1;
	Character bomb_2;
	Character bomb_3;
	Character bomb_4;
	
	Character player; 
	
	Character powerUp;
	Character gun;
	
	Character missile;
		
	int loop, ghostLevelDecrease, powerUpInitialCoolDown, gunInitialCoolDown;
	unsigned char Err = joy_load_driver (joy_stddrv);
			
	joy_install (joy_static_stddrv);	
	
	/* Ask for the screen size */
	screensize (&XSize, &YSize);	
	
	while(1)
	{
		ghostCount = 8;
		loop = 0;	
		points = 0ul;
		level = INITIAL_LEVEL; 

		// Set Screen Colors
		setScreenColors();			
		
		clrscr ();
					
		printStartMessage(XSize,YSize);
		while(!kbhit() && !joy_read(JOY_1))
		{}
		clrscr ();
				
		deleteCenteredMessage(XSize, YSize);
		
		lives = 3;
		do // Level Start
		{
			loop = 0;
			ghostLevel = 1u;
			ghostSmartness = computeGhostSmartness();
			computePowerUp(&ghostLevelDecrease, &powerUpInitialCoolDown);
			gunInitialCoolDown = computeGunInitialCoolDown();
			invincibleXCountDown = computeInvincibleCountDown();
			invincibleYCountDown = computeInvincibleCountDown();
			invincibleSlowDown = computeInvincibleSlowDown(loop);
			invincibleGhostCountTrigger = computeInvincibleGhostCountTrigger();
			invincibleLoopTrigger = computeInvincibleLoopTrigger();
			ghostCount = 8;
			guns = 0;
			gun._status = 0;
			gunCoolDown = gunInitialCoolDown;
			
			/* Clear the screen, put cursor in upper left corner */
			clrscr ();

			printLevel(XSize, YSize, level);
			sleep(2);
			
			/* Wait for the user to press a key */
			printPressKeyToStart(XSize, YSize);
			while(!kbhit() && !joy_read(JOY_1))
			{}
			deleteCenteredMessage(XSize, YSize);
			
			/* Draw a border around the screen */
			drawBorders(XSize, YSize);
			
			// Initialize characters
			innerVerticalWallLength = drawInnerVerticalWallForLevel(XSize,YSize,level);
			fillLevelWithCharacters(XSize, YSize,
								 &player, &powerUp, 
								 &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								 &ghost_5, &ghost_6, &ghost_7, &ghost_8, 
								 &bomb_1, &bomb_2, &bomb_3, &bomb_4, 
								 &invincibleGhost, &missile, &gun);	
			ghostCount = 8;
			
			displayStatsTitles();
			
			while(player._alive && ghostCount>0)
			{
				ghostSlowDown = computeGhostSlowDown();
				invincibleSlowDown = computeInvincibleSlowDown(loop);
				drawInnerVerticalWall(XSize,YSize, innerVerticalWallLength, 
				&innerVerticalWallX, &innerVerticalWallY);
				
				++loop;

#ifdef _KEYBOARD
				if(kbhit())
				{		
					kbInput = cgetc();
					movePlayer(&player, kbInput);
				}
#endif // _KEYBOARD
				//else
				{
					joyInput = joy_read (JOY_1);
					
					movePlayerByJoystick(&player, joyInput);	
				}
				if(playerFire && missile._status==0 && guns>0)
				{
					--guns;
					missileDirection = playerDirection;
					missile._status = setMissileInitialPosition(XSize, YSize, &missile, &player, missileDirection);
					missile._alive = missile._status;
					playerFire = 0;
					displayCharacter(&missile);					
					checkMissileVsGhosts(&missile, 
						&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						&ghost_5, &ghost_6, &ghost_7, &ghost_8, &points, &ghostCount);
					if(areCharctersAtSamePosition(&missile, &invincibleGhost))
						{
							//missile._status = 0; missile._alive = 0;
							die(&missile);
							restoreMissile(&missile);
							displayCharacter(&invincibleGhost);
						}		
				}
				if(missile._status==1 && missile._alive==1)
				{
					moveMissile(XSize, YSize, &missile, missileDirection);
					// TODO: Inefficient
					checkMissileVsGhosts(&missile, 
						&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						&ghost_5, &ghost_6, &ghost_7, &ghost_8, &points, &ghostCount);
					
					if(areCharctersAtSamePosition(&missile, &invincibleGhost))
					{
						die(&missile);
						restoreMissile(&missile);
						displayCharacter(&invincibleGhost);
					}
				}
			
				
				chasePlayer(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
							&ghost_5, &ghost_6, &ghost_7, &ghost_8, &player, 
							&bomb_1, &bomb_2, &bomb_3, &bomb_4, 
							ghostSmartness, ghostSlowDown, ghostCount, level);
					
				
				if(playerReached(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								 &ghost_5, &ghost_6, &ghost_7, &ghost_8, &player) ||
				   playerReachedBombs(&bomb_1, &bomb_2, &bomb_3, &bomb_4, &player))
				{
					die(&player);
					defeat(XSize, YSize);
					sleep(1);
				}
				
				if(innerWallReached(&player))
				{
					die(&player);
					defeat(XSize, YSize);
					sleep(1);
				}
			
				checkBombsVsGhosts(&bomb_1, &bomb_2, &bomb_3, &bomb_4, 
								   &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								   &ghost_5, &ghost_6, &ghost_7, &ghost_8, &ghostCount);
				
				checkGhostsVsGhosts(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
									&ghost_5, &ghost_6, &ghost_7, &ghost_8, &ghostCount);

				if(gun._status==1)
				{
					if(powerUpReached(&player, &gun))
					{
						guns = 3;
						points+=GUN_BONUS;
						gun._status = 0;	
						gunCoolDown = gunInitialCoolDown;
					}
					else
					{
						displayCharacter(&gun);
					}
				}		
				else if (gunCoolDown == 0)
				{
					
					gun._status = 1;
					do
					{
					relocateCharacter(XSize, YSize, &gun, &bomb_1, &bomb_2, &bomb_3, &bomb_4, 
								   &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								   &ghost_5, &ghost_6, &ghost_7, &ghost_8);
					} while(innerWallReached(&gun));
				}
				else
				{
					--gunCoolDown;
				}				
									
				if(powerUp._status == 1)
				{
					if(powerUpReached(&player, &powerUp))
					{
						decreaseGhostLevel(ghostLevelDecrease);	
						points+=POWER_UP_BONUS;
						powerUp._status = 0;	
						powerUpCoolDown = powerUpInitialCoolDown;
					}
					else
					{
						displayCharacter(&powerUp);
					}		
				}
				else if (powerUpCoolDown == 0)
				{
					powerUp._status = 1;
					do
					{
					relocateCharacter(XSize, YSize, &powerUp, &bomb_1, &bomb_2, &bomb_3, &bomb_4, 
								   &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								   &ghost_5, &ghost_6, &ghost_7, &ghost_8);
					} while(innerWallReached(&powerUp));
				}
				else
				{
					--powerUpCoolDown;
				}
					
				if(wallReached(XSize, YSize, &player))
				{
					die(&player);
					defeat(XSize, YSize);
					sleep(1);
				}
				
				displayCharacter(&bomb_1);
				displayCharacter(&bomb_2);		
				displayCharacter(&bomb_3);
				displayCharacter(&bomb_4);
				
				displayStatsTitles();
				displayStats(level, lives, guns, points, ghostCount, ghostLevel);
				
				if(!invincibleGhost._status && 
				  ((invincibleXCountDown==0)||(invincibleYCountDown==0)) || 
				   (loop>=invincibleLoopTrigger) || (ghostCount<=invincibleGhostCountTrigger))
				{
					invincibleGhost._status = 1;
					displayCharacter(&invincibleGhost);
				}
				else
				{
					--invincibleXCountDown;
					--invincibleYCountDown;
				}
				if(invincibleGhost._status)
				{
					blindChaseCharacterMaxStrategy(&invincibleGhost, &player, 
												   invincibleSlowDown, level);
					if(areCharctersAtSamePosition(&invincibleGhost, &player))
					{
						die(&player);
						defeat(XSize, YSize);
						sleep(1);
					}
				}
				
				if(ghostCount<=0)
				{
					victory(XSize, YSize);
					sleep(1);
				}
				
				// Add points to score
				points+=LOOP_POINTS;
				
				// Increase ghost speed
				++ghostLevel;
			}; // end inner while [while (player._alive && !victoryFlag)]
			
			if(player._alive)
			{
				clrscr();
				printLevelBonus(XSize,YSize, level);
								
				sleep(1);
				
				points+= LEVEL_BONUS*level;
				++level;

			}
			else
			{
				--lives;
				if(lives>0)
				{
					player._alive = 1;
				}
			}
		} while (player._alive || ((ghostCount<=0) && (level<(FINAL_LEVEL+1)))); // middle while (one match) 
			
	if(level==FINAL_LEVEL+1)
	{
		gameCompleted(XSize, YSize);
		sleep(1);
	}
	finalScore(XSize, YSize, points);
	// GAME OVER	
	printGameOver(XSize, YSize);
	sleep(1);
	clrscr ();
	}

	return EXIT_SUCCESS;
}

