
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

// If two ghosts bump into eachother you get 2500 points for each ghost
#define GHOST_VS_GHOST_BONUS 1500ul

// If a ghost bumps into a bomb
#define GHOST_VS_BOMBS_BONUS 1000ul

// Points for shooting a ghost
#define GHOST_VS_MISSILE 100ul

// Extra points for the power up
#define POWER_UP_BONUS 500ul

// Extra points for the power up
#define GUN_BONUS 750ul

// Points for each tick
#define LOOP_POINTS 1ul

// Points gained at the end of each level (to be multipled by level)
#define LEVEL_BONUS 1000ul


// First levels that change initial bombs distribution
// LEVEL 1 - 4: Four central bombs
// LEVEL 5 - 9: Two central bombs
// LEVEL 10 - 14: Two bombs next to the vertical borders
// LEVEL 15 - 16: Three bombs attached to the borders
// LEVEL 17 - 18: Two bombs attached to the vertical borders
// LEVEL 19 - 20: Four bombs at the corners 

// Starting from this level 4 central bombs
#define INITIAL_LEVEL 1

// Starting from this level only two central bombs
#define TWO_BOMB_START_LEVEL 5

// Starting from this level only 2 bombs close to the vertical borders
#define FIRST_HARD_LEVEL 10

// Starting from this level 2 bombs close to the botton borders
#define FIRST_VERY_HARD_LEVEL 15

// Starting from this level only 2 bombs on the vertical borders
#define FIRST_INSANE_LEVEL 17

// Ultimate level (four bombs at the corners)
#define FIRST_ULTIMATE_LEVEL 19

// Final level 
#define FINAL_LEVEL 20

// First level that change ghost strategy
// LEVEL 1-6: Ghosts chase use by approaching your X and Y coordinates
// LEVEL 7-20: With more than 3 ghosts there will be up to 3 different ghost groups 

// Starting from this level, the ghosts use a smarter "collective" strategy
#define COLLECTIVE_STRATEGY_START_LEVEL 7

// Directions
#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

unsigned int ghostSlowDown;
unsigned int powerUpCoolDown;
unsigned int gunCoolDown;
unsigned int ghostLevel = 1u;
unsigned long points = 0ul;
unsigned int ghostSmartness = 1u; // 9u is max = impossible 

unsigned short playerDirection = 0; // 0: right, 1: down, 2: left, 3: up
unsigned short missileDirection = 0;
unsigned short playerFire = 0;
unsigned short guns = 3;

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

unsigned int invincibleXCountDown = 100;
unsigned int invincibleYCountDown = 100;
unsigned int invincibleSlowDown = 30000;
unsigned int invincibleLoopTrigger = 1000;
unsigned short ghostCount = 8;
unsigned short invincibleGhostCountTrigger = 2;

struct CharacterStruct
{
	// character coordinates
	int _x;
	int _y;
	
	// how to display the character (i.e., which ASCII character to use
	char _ch;
	
	// _status decides whether the character is active
	short _status;
	
	//_alive decides whether it is dead or alive
	short _alive;
	
};

typedef struct CharacterStruct Character;

void setCharacterPosition(Character* characterPtr, int x, int y)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
}

void setCharacterDisplay(Character* characterPtr, char ch)
{
	characterPtr->_ch = ch;
}

void deleteCharacter(Character* characterPtr)
{
	gotoxy(characterPtr->_x,characterPtr->_y);
	cputc(' ');
}

void displayCharacter(Character* characterPtr)
{
	gotoxy(characterPtr->_x,characterPtr->_y);
	cputc(characterPtr->_ch);
}

int isCharacterAtLocation(int x, int y, Character * characterPtr)
{
	return(characterPtr->_x==x) && (characterPtr->_y==y);
}

int areCharctersAtSamePosition(Character* lhs, Character* rhs)
{
	return (lhs->_x==rhs->_x)&&(lhs->_y==rhs->_y);
}

int leftDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_y == bombPtr->_y) && (characterPtr->_x-1 == bombPtr->_x);
}

int rightDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_y == bombPtr->_y) && (characterPtr->_x+1 == bombPtr->_x);	  
}

int upDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_x == bombPtr->_x) && (characterPtr->_y-1 == bombPtr->_y);
}

int downDanger(Character* characterPtr, Character* bombPtr)
{
	return (characterPtr->_x == bombPtr->_x) && (characterPtr->_y+1 == bombPtr->_y);	  
}


int leftBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return leftDanger(characterPtr, bombPtr1) || leftDanger(characterPtr, bombPtr2) || 
	       leftDanger(characterPtr, bombPtr3) || leftDanger(characterPtr, bombPtr4);
}

int rightBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return rightDanger(characterPtr, bombPtr1) || rightDanger(characterPtr, bombPtr2) || 
	       rightDanger(characterPtr, bombPtr3) || rightDanger(characterPtr, bombPtr4);
}

int upBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return upDanger(characterPtr, bombPtr1) || upDanger(characterPtr, bombPtr2) || 
	       upDanger(characterPtr, bombPtr3) || upDanger(characterPtr, bombPtr4);
}

int downBombs(Character* characterPtr, 
              Character* bombPtr1,  Character* bombPtr2, 
              Character* bombPtr3,  Character* bombPtr4)
{
	return downDanger(characterPtr, bombPtr1) || downDanger(characterPtr, bombPtr2) || 
	       downDanger(characterPtr, bombPtr3) || downDanger(characterPtr, bombPtr4);
}


int leftGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return leftDanger(characterPtr, ghostPtr1) || leftDanger(characterPtr, ghostPtr2) || 
	       leftDanger(characterPtr, ghostPtr3) || leftDanger(characterPtr, ghostPtr4) ||
		   leftDanger(characterPtr, ghostPtr5) || leftDanger(characterPtr, ghostPtr6) || 
		   leftDanger(characterPtr, ghostPtr7);
}

int rightGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return rightDanger(characterPtr, ghostPtr1) || rightDanger(characterPtr, ghostPtr2) || 
	       rightDanger(characterPtr, ghostPtr3) || rightDanger(characterPtr, ghostPtr4) ||
		   rightDanger(characterPtr, ghostPtr5) || rightDanger(characterPtr, ghostPtr6) || 
		   rightDanger(characterPtr, ghostPtr7);
}

int upGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return upDanger(characterPtr, ghostPtr1) || upDanger(characterPtr, ghostPtr2) || 
		   upDanger(characterPtr, ghostPtr3) || upDanger(characterPtr, ghostPtr4) || 
	       upDanger(characterPtr, ghostPtr5) || upDanger(characterPtr, ghostPtr6) ||
		   upDanger(characterPtr, ghostPtr7);
}

int downGhosts(Character* characterPtr, 
              Character* ghostPtr1,  Character* ghostPtr2, 
              Character* ghostPtr3,  Character* ghostPtr4,
              Character* ghostPtr5,  Character* ghostPtr6, 
              Character* ghostPtr7)
{
	return downDanger(characterPtr, ghostPtr1) || downDanger(characterPtr, ghostPtr2) || 
	       downDanger(characterPtr, ghostPtr3) || downDanger(characterPtr, ghostPtr4) ||
		   downDanger(characterPtr, ghostPtr5) || downDanger(characterPtr, ghostPtr6) ||
		   downDanger(characterPtr, ghostPtr7);
}



void chaseCharacterXAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7)
{
	if(!rightBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	   !rightGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	   hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(!leftBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	        !leftGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	         hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}
	else if(!downBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!downGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	        hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(!upBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!upGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	        hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	displayCharacter(hunterPtr);
}

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}
	else if(hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	displayCharacter(hunterPtr);
}

void chaseCharacterYAvoidBombStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7)
{
    if(!downBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	   !downGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
		hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(!upBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
	        !upGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
	        hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	else if(!rightBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!rightGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
			hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(!leftBombs(hunterPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4) && 
			!leftGhosts(hunterPtr, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7) &&
			hunterPtr->_x>preyPtr->_x) 
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}

	displayCharacter(hunterPtr);
}

void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
{
    if(hunterPtr->_y<preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_y;
	}
	else if(hunterPtr->_x<preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		deleteCharacter(hunterPtr);
		--hunterPtr->_x;
	}

	displayCharacter(hunterPtr);
}

void chaseCharacterXYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7)
{
	if(rand()%10 > ghostSmartness)
	{
		if(rand()%2) // Select blind chase strategy
		{
			blindChaseCharacterXStrategy(hunterPtr, preyPtr);
		}
		else
		{
			blindChaseCharacterYStrategy(hunterPtr, preyPtr);
		}
	}
	else
	{
		if(rand()%2) // Select chase strategy that avoids collisions
		{
			chaseCharacterXAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
		else
		{
			chaseCharacterYAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
		}
	}
}

void chaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7)
{
	if(rand()%10 > ghostSmartness)
	{
			blindChaseCharacterYStrategy(hunterPtr, preyPtr);
	}
	else
	{
			chaseCharacterYAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
	}
}

void chaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character* ghostPtr1, Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7)
{
	if(rand()%10 > ghostSmartness)
	{
			blindChaseCharacterXStrategy(hunterPtr, preyPtr);
	}
	else
	{
			chaseCharacterXAvoidBombStrategy(hunterPtr, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
			ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);	
	}
}


void blindChaseCharacter(Character* hunterPtr, Character* preyPtr)
{
	if((hunterPtr->_status==1) && (hunterPtr->_alive==1))
	{
		if(rand()>invincibleSlowDown)
		{
			if(rand()%2) // Select chase strategy
			{
				blindChaseCharacterXStrategy(hunterPtr, preyPtr);
			}
			else
			{
				blindChaseCharacterYStrategy(hunterPtr, preyPtr);
			}
		}
	}
}


void chaseCharacterXStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterXStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
			ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
		}
	}
	
	displayCharacter(ghostPtr1);
}					 

void chaseCharacterYStrategyIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterYStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
			ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
		}
	}
	
	displayCharacter(ghostPtr1);
}

void chaseCharacterIf(Character* ghostPtr1, Character* preyPtr, 
                    Character* bombPtr1, Character* bombPtr2,
					Character* bombPtr3, Character* bombPtr4,
					Character *ghostPtr2, Character* ghostPtr3, Character *ghostPtr4,
					Character* ghostPtr5, Character *ghostPtr6, Character* ghostPtr7, Character* ghostPtr8)
{
	// TODO: to fix
	
	if((ghostPtr1->_status==1) && (ghostPtr1->_alive==1))
	{
		if(rand()>ghostSlowDown)
		{
			chaseCharacterXYStrategy(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4, 
			ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
		}
	}
	
	displayCharacter(ghostPtr1);
}	

void chasePlayer(Character * ghostPtr1, Character * ghostPtr2, 
                 Character * ghostPtr3, Character * ghostPtr4,
				 Character * ghostPtr5, Character * ghostPtr6, 
                 Character * ghostPtr7, Character * ghostPtr8, 
                 Character* preyPtr, 
                 Character* bombPtr1, Character* bombPtr2,
				 Character* bombPtr3, Character* bombPtr4
				 )
{
	// 1 - 3*: approximate straight line
	// 4* - 5: get closer in two groups (one approximating x and the other y)
	// 6* - 8: get closer in three groups (as 4* - 5 and one group as 1-3*)
	if((level>=COLLECTIVE_STRATEGY_START_LEVEL) && 
	((ghostCount>=5) || 
	  ((ghostCount==4) && !(ghostPtr1->_alive && ghostPtr3->_alive && ghostPtr5->_alive && ghostPtr7->_alive))))
	{
		if(ghostCount>=6)
		{
		chaseCharacterIf(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
	
		chaseCharacterIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
		ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
		}
		else
		{
	chaseCharacterXStrategyIf(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
	
	chaseCharacterYStrategyIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
		}
	chaseCharacterXStrategyIf(ghostPtr3, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
	
	chaseCharacterYStrategyIf(ghostPtr4, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
    
	chaseCharacterXStrategyIf(ghostPtr5, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr6, ghostPtr7, ghostPtr8);
	
	chaseCharacterYStrategyIf(ghostPtr6, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr7, ghostPtr8);
	
	chaseCharacterXStrategyIf(ghostPtr7, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr8);
	
	chaseCharacterYStrategyIf(ghostPtr8, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
	}
	else // TODO: BOGUS
	{
	chaseCharacterIf(ghostPtr1, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
	
	chaseCharacterIf(ghostPtr2, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
	
	chaseCharacterIf(ghostPtr3, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
	
	chaseCharacterIf(ghostPtr4, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8);
    
	chaseCharacterIf(ghostPtr5, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr6, ghostPtr7, ghostPtr8);
	
	chaseCharacterIf(ghostPtr6, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr7, ghostPtr8);
	
	chaseCharacterIf(ghostPtr7, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr8);
	
	chaseCharacterIf(ghostPtr8, preyPtr, bombPtr1, bombPtr2, bombPtr3, bombPtr4,
	ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7);
	}
}

void displayScore(unsigned long points)
{
	// Draw score 
	gotoxy(2,2);
	cputs("SCORE: ");
	
	gotoxy(9,2);
	cputs("       ");
	gotoxy(9,2);
	cprintf("%lu",points);
}

void displayGhostLevel()
{
	// Draw score 
	gotoxy(2,1);
	cputs("SPEED: ");
	
	gotoxy(9,1);
	cputs("       ");
	gotoxy(9,1);
	cprintf("%u",ghostLevel);
}

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
	else if((kbInput=='L') || (kbInput=='l'))
	{
		ghostCount = 0;
		playerPtr->_ch = 'Z'; // TODO: BOGUS
	}
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


void initializeCharacter(Character* characterPtr, int x, int y, char ch, short status)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_ch = ch;
	characterPtr->_status = status;
	characterPtr->_alive = 1; // TODO: Maybe we should initialize this with a parameter
}

void drawBorders(int XSize, int YSize)
{
	/* Clear the screen, put cursor in upper left corner */
    clrscr ();
	
	/* Top line */
    cputc (CH_ULCORNER);
    chline (XSize - 2);
    cputc (CH_URCORNER);

    /* Vertical line, left side */
    cvlinexy (0, 1, YSize - 2);

    /* Bottom line */
    cputc (CH_LLCORNER);
    chline (XSize - 2);
    cputc (CH_LRCORNER);

    /* Vertical line, right side */
    cvlinexy (XSize - 1, 1, YSize - 2);
}

void setScreenColors()
{
    (void) textcolor (COLOR_WHITE);
    (void) bordercolor (COLOR_BLACK);
    (void) bgcolor (COLOR_BLACK);
}


void printCenteredMessage(int XSize, int YSize, char *Text)
{
	gotoxy ((XSize - strlen (Text)) / 2, YSize / 2);
    cprintf ("%s", Text);
}

void printLevel(int XSize, int YSize)
{
	char levelString[22];

	sprintf(levelString, "LEVEL %d", level);

	printCenteredMessage(XSize, YSize, levelString);
}

void printLevelBonus(int XSize, int YSize)
{
	char levelString[22];

	sprintf(levelString, "LEVEL BONUS = %d", level * 1000);

	printCenteredMessage(XSize, YSize, levelString);
	sleep(1);
}

void printPressKeyToStart(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "PRESS ANY KEY TO START");
}

void deleteCenteredMessage(int XSize, int YSize)
{
	gotoxy ((XSize - 22) / 2, YSize / 2);
    cputs( "                      ");
}

/*
void toggleHunters(Character * hunterPtr1, Character * hunterPtr2, 
                   Character * hunterPtr3, Character * hunterPtr4, 
				   Character * hunterPtr5, Character * hunterPtr6, 
                   Character * hunterPtr7, Character * hunterPtr8, 
				   int loop)
{
	if(loop<=10)
		hunterPtr1->_status = 1;
	else if(loop<=30-level)
		hunterPtr2->_status = 1;
	else if(loop<=40-level)
		hunterPtr3->_status = 1;
	else if(loop<=50-level)
		hunterPtr4->_status = 1;
	else if(loop<=60-level)
		hunterPtr5->_status = 1;
	else if(loop<=70-level)
		hunterPtr6->_status = 1;
	else if(loop<=80-level)
		hunterPtr7->_status = 1;
	else if(loop<=90-level)
		hunterPtr8->_status = 1;
}
*/

void checkBombsVsGhost(Character * bombPtr1, Character * bombPtr2, 
					   Character * bombPtr3, Character * bombPtr4,
					   Character * ghostPtr)
{
	if(ghostPtr->_alive && playerReachedBombs(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr))
	{
		gotoxy(ghostPtr->_x,ghostPtr->_y);
		cputc('X');
		ghostPtr->_alive = 0;
		ghostPtr->_status = 0;
		ghostPtr->_ch = 'X';
		points+=GHOST_VS_BOMBS_BONUS;
		--ghostCount;
	}
}
						

void checkBombsVsGhosts(Character * bombPtr1, Character * bombPtr2, 
						Character * bombPtr3, Character * bombPtr4,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8)
{
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr1);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr2);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr3);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr4);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr5);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr6);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr7);
	checkBombsVsGhost(bombPtr1, bombPtr2, bombPtr3, bombPtr4, ghostPtr8);
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
	x_offset = rand() % 6;
	y_offset = rand() % 6;
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

void drawInnerVerticalWall(int XSize, int YSize)
{
	cvlinexy (XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);
}

/*
void removeInnerVerticalWall(int XSize, int YSize)
{
	innerVerticalWallLength = 0;

	innerVerticalWallX = 0; // TODO: unnecessary?
	innerVerticalWallY = 0;

}
*/

short innerWallReached(Character *characterPtr)
{
	return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
}


void createInnerVerticalWall(int XSize, int YSize, short length)
{
	innerVerticalWallLength = length;
	innerVerticalWallX = XSize / 2;
	innerVerticalWallY = YSize/2-(innerVerticalWallLength/2);
	drawInnerVerticalWall(XSize, YSize);
}

void createInnerVerticalWallIf(XSize,YSize)
{
		
	switch(level)
	{
		case 1:
			createInnerVerticalWall(XSize,YSize, 0);
		break;
		case 2:
			createInnerVerticalWall(XSize,YSize, 8);
		break;
		case 3:
			createInnerVerticalWall(XSize,YSize, 10);
		break;
		case 4:
			createInnerVerticalWall(XSize,YSize, 12);
		break;
		case 5:
			createInnerVerticalWall(XSize,YSize, 0);
		break;
		case 6:
			createInnerVerticalWall(XSize,YSize, 8);
		break;
		case 7:
			createInnerVerticalWall(XSize,YSize, 10);
		break;
		case 8:
			createInnerVerticalWall(XSize,YSize, 12);
		break;
		case 9:
			createInnerVerticalWall(XSize,YSize, 14);
		break;
		case 10:
			createInnerVerticalWall(XSize,YSize, 0);
		break;
		case 11:
			createInnerVerticalWall(XSize,YSize, 8);
		break;
		case 12:
			createInnerVerticalWall(XSize,YSize, 10);
		break;
		case 13:
			createInnerVerticalWall(XSize,YSize, 12);
		break;
		case 14:
			createInnerVerticalWall(XSize,YSize, 14);
		break;
		case 15:
			createInnerVerticalWall(XSize,YSize, 0);
		break;
		case 16:
			createInnerVerticalWall(XSize,YSize, 8);
		break;
		case 17:
			createInnerVerticalWall(XSize,YSize, 0);
		break;
		case 18:
			createInnerVerticalWall(XSize,YSize, 10);
		break;
		case 19:
			createInnerVerticalWall(XSize,YSize, 8);
		break;
		case 20:
			createInnerVerticalWall(XSize,YSize, 14);
		break;
	}
}


void initializeCharacters(int XSize, int YSize,
						  Character * playerPtr, Character * powerUpPtr, 
						  Character * ghostPtr1, Character * ghostPtr2,
						  Character * ghostPtr3, Character * ghostPtr4,
						  Character * ghostPtr5, Character * ghostPtr6,
						  Character * ghostPtr7, Character * ghostPtr8,
						  Character * bombPtr1, Character * bombPtr2,
						  Character * bombPtr3, Character * bombPtr4,
						  Character * invincibleGhostPtr, 
						  Character * missilePtr, Character * gunPtr
						  )
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
	} while(innerWallReached(bombPtr1) || innerWallReached(bombPtr2) || innerWallReached(bombPtr3) || innerWallReached(bombPtr4));
	
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

int victoryCondition()
{
	return ghostCount==0;
}

void printGameOver(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "G A M E   O V E R");
}

void printVictoryMessage(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U   W O N ! !");
}

void printDefeatMessage(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U   L O S T !");
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

int wallReached(int XSize, int YSize, Character *characterPtr)
{
	return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
		   (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}

void die(Character * playerPtr)
{
	gotoxy(playerPtr->_x,playerPtr->_y);
	cputc('X');
	playerPtr->_status = 0;
	playerPtr->_alive = 0;
	playerPtr->_ch = 'X';
}

void defeat(int XSize, int YSize)
{
	printDefeatMessage(XSize, YSize);
	sleep(1);
}

void win(Character * playerPtr)
{
	gotoxy(playerPtr->_x,playerPtr->_y);
	cputc('!');
}

void victory(int XSize, int YSize)
{
	printVictoryMessage(XSize, YSize);
	sleep(1);
}

void decreaseGhostLevel(int level)
{
	if(ghostLevel>level)
		ghostLevel-=level;
	else
		ghostLevel=0;
}

void checkGhostsVsGhosts(Character *ghostPtr1, Character *ghostPtr2, Character *ghostPtr3, Character *ghostPtr4,
						 Character *ghostPtr5, Character *ghostPtr6, Character *ghostPtr7, Character *ghostPtr8)
{
	if(ghostPtr8->_alive && charactersMeet(ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8))
	{
		die(ghostPtr8);
		points+=GHOST_VS_GHOST_BONUS;
	    --ghostCount;
	}
	if(ghostPtr1->_alive && charactersMeet(ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1))
	{
		die(ghostPtr1);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr2->_alive && charactersMeet(ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2))
	{
		die(ghostPtr2);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr3->_alive && charactersMeet(ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3))
	{
		die(ghostPtr3);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr4->_alive && charactersMeet(ghostPtr5, ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4))
	{
		die(ghostPtr4);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr5->_alive && charactersMeet(ghostPtr6, ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5))
	{
		die(ghostPtr5);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr6->_alive && charactersMeet(ghostPtr7, ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6))
	{
		die(ghostPtr6);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
	}
	if(ghostPtr7->_alive && charactersMeet(ghostPtr8, ghostPtr1, ghostPtr2, ghostPtr3, ghostPtr4, ghostPtr5, ghostPtr6, ghostPtr7))
	{
		die(ghostPtr7);
		points+=GHOST_VS_GHOST_BONUS;
		--ghostCount;
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

void gameCompleted(int XSize, int YSize)
{
	printCenteredMessage(XSize, YSize, "Y O U  M A D E  I T !"); 
	sleep(2);
	printCenteredMessage(XSize, YSize, "    T H E   E N D    "); 
	sleep(2);
}

void finalScore(int XSize, int YSize)
{
	char scoreString[22];
	clrscr();
	sprintf(scoreString, "SCORE:  %lu", points);
	printCenteredMessage(XSize, YSize, scoreString);
	sleep(3);
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

void checkMissileVsGhost(Character * missilePtr,
					   Character * ghostPtr)
{
	if(ghostPtr->_alive && 
	areCharctersAtSamePosition(missilePtr, ghostPtr))
	{
		gotoxy(ghostPtr->_x,ghostPtr->_y);
		die(ghostPtr); 
		points+=GHOST_VS_MISSILE;
		--ghostCount;
	}
}
						

void checkMissileVsGhosts(Character * missilePtr,
						Character * ghostPtr1, Character * ghostPtr2, 
						Character * ghostPtr3, Character * ghostPtr4,
						Character * ghostPtr5, Character * ghostPtr6, 
						Character * ghostPtr7, Character * ghostPtr8)
{
	checkMissileVsGhost(missilePtr, ghostPtr1);
	checkMissileVsGhost(missilePtr, ghostPtr2);
	checkMissileVsGhost(missilePtr, ghostPtr3);
	checkMissileVsGhost(missilePtr, ghostPtr4);
	checkMissileVsGhost(missilePtr, ghostPtr5);
	checkMissileVsGhost(missilePtr, ghostPtr6);
	checkMissileVsGhost(missilePtr, ghostPtr7);
	checkMissileVsGhost(missilePtr, ghostPtr8);
}
	
int setMissileInitialPosition(int XSize, int YSize, Character *missilePtr, Character *playerPtr)
{
	int newX = playerPtr->_x; 
	int newY = playerPtr->_y;
	switch(missileDirection)
		{
			case RIGHT:
				++newX;
			break;
			case DOWN:
				++newY;
			break;
			case UP:
				--newY;
			break;
			case LEFT:
				--newX;
			break;
		}
	 
	missilePtr->_x = newX;
	missilePtr->_y = newY;	
	if(wallReached(XSize,YSize, missilePtr))
	{
		die(missilePtr);
		missilePtr->_ch = '.';
		return 0;
	}
	return 1;
}
	
void moveMissile(int XSize, int YSize, Character * missilePtr)
{
	int newX = missilePtr->_x; 
	int newY = missilePtr->_y;
	switch(missileDirection)
	{
		case RIGHT:
			++newX;
		break;
		case DOWN:
			++newY;
		break;
		case UP:
			--newY;
		break;
		case LEFT:
			--newX;
		break;
	}
	deleteCharacter(missilePtr);
	missilePtr->_x = newX;
	missilePtr->_y = newY;
	if(wallReached(XSize,YSize, missilePtr))
	{
		die(missilePtr);
		missilePtr->_ch = '.';
	}
	else
	{
		displayCharacter(missilePtr);
	}
}

void printStartMessage(int XSize, int YSize)
{
	gotoxy ((XSize - 22) / 2, YSize / 2 - 9);
	cprintf ("%s", "A S C I I   C H A S E");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 7);
	cprintf ("%s", "by Fabrizio Caruso");
	
	gotoxy ((XSize - 9) / 2, YSize / 2 - 4);
	cprintf ("%s", "GAME PLAY");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 3);
	cprintf ("%s", "You * are chased by O");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 2);
	cprintf ("%s", "Force O into X");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 - 1);	
	cprintf ("%s", "Take P to slow O down");
	
	
	gotoxy ((XSize - 22) / 2, YSize / 2);
	cprintf ("%s", "Catch ! for bullets!");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 +1);
	cprintf ("%s", "Flee from +!");
	
	
	gotoxy ((XSize - 4) / 2, YSize / 2 + 3);
	cprintf ("%s", "KEYS");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 + 4);
	cprintf ("%s", "Move with W A S D");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 + 5);
	cprintf("%s",  "and shoot with <SPACE>");
	
	gotoxy ((XSize - 22) / 2, YSize / 2 + 8);
	cprintf("%s",  "PRESS ANY KEY TO START");
	cgetc();
}

int main (void)
{
    unsigned char XSize, YSize;
	
	char kbInput;
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
		
	int loop, victoryFlag, ghostLevelDecrease, powerUpInitialCoolDown, gunInitialCoolDown;
	unsigned char Err = joy_load_driver (joy_stddrv);
			
	joy_install (joy_static_stddrv);	
	
	/* Ask for the screen size */
	screensize (&XSize, &YSize);	
	
	while(1)
	{
		victoryFlag = 0;
		loop = 0;	
		points = 0ul;
		level = INITIAL_LEVEL; 

		// Set Screen Colors
		setScreenColors();			
		
		clrscr ();
					
		printStartMessage(XSize,YSize);
		
		clrscr ();
				
		deleteCenteredMessage(XSize, YSize);
			
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

			printLevel(XSize, YSize);
			sleep(2);
			
			/* Wait for the user to press a key */
			printPressKeyToStart(XSize, YSize);
			cgetc();
			deleteCenteredMessage(XSize, YSize);
			
			/* Draw a border around the screen */
			drawBorders(XSize, YSize);
			
			// Initialize characters
			createInnerVerticalWallIf(XSize,YSize);
			initializeCharacters(XSize, YSize,
								 &player, &powerUp, 
								 &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								 &ghost_5, &ghost_6, &ghost_7, &ghost_8, 
								 &bomb_1, &bomb_2, &bomb_3, &bomb_4, 
								 &invincibleGhost, &missile, &gun);	
			victoryFlag = 0;
			while(player._alive && !victoryFlag)
			{
				ghostSlowDown = computeGhostSlowDown();
				invincibleSlowDown = computeInvincibleSlowDown(loop);
				drawInnerVerticalWall(XSize,YSize);
				
				++loop;


				if(kbhit())
				{		
					kbInput = cgetc();
					movePlayer(&player, kbInput);
				}
				

				{
					joyInput = joy_read (JOY_1);
					
					movePlayerByJoystick(&player, joyInput);	
				}
				if(playerFire && missile._status==0 && guns>0)
				{
					--guns;
					missileDirection = playerDirection;
					missile._status = setMissileInitialPosition(XSize, YSize, &missile, &player);
					missile._alive = missile._status;
					playerFire = 0;
					displayCharacter(&missile);					
					checkMissileVsGhosts(&missile, 
					&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
					&ghost_5, &ghost_6, &ghost_7, &ghost_8);
				}
				if(missile._status==1 && missile._alive==1)
				{

					moveMissile(XSize, YSize, &missile);
					// TODO: Inefficient
					checkMissileVsGhosts(&missile, 
					&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
					&ghost_5, &ghost_6, &ghost_7, &ghost_8);
				}
			
				
				chasePlayer(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
							&ghost_5, &ghost_6, &ghost_7, &ghost_8, &player, 
							&bomb_1, &bomb_2, &bomb_3, &bomb_4);
				
				if(missile._status==1 && missile._alive==1)
				{
				// TODO: Inefficient
				checkMissileVsGhosts(&missile, 
				&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
				&ghost_5, &ghost_6, &ghost_7, &ghost_8);
				}				
				
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
								   &ghost_5, &ghost_6, &ghost_7, &ghost_8);
				
				checkGhostsVsGhosts(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
									&ghost_5, &ghost_6, &ghost_7, &ghost_8);

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
				
				displayScore(points);
				displayGhostLevel();
				
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
					blindChaseCharacter(&invincibleGhost, &player);
					if(areCharctersAtSamePosition(&invincibleGhost, &player))
					{
						die(&player);
						defeat(XSize, YSize);
						sleep(1);
					}
				}
				
				
				if(victoryFlag=victoryCondition())
				{
					win(&player);
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
				printLevelBonus(XSize,YSize);
								
				sleep(1);
				
				points+= LEVEL_BONUS*level;
				++level;

			}
		} while (victoryFlag && (level<FINAL_LEVEL+1)); // middle while (one match) 
			
	if(level==FINAL_LEVEL+1)
	{
		gameCompleted(XSize, YSize);
		sleep(1);
	}
	finalScore(XSize, YSize);
	// GAME OVER	
	printGameOver(XSize, YSize);
	sleep(1);
	clrscr ();
	}

	return EXIT_SUCCESS;
}


