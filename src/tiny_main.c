/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS CHASE by Fabrizio Caruso
//
// Fabrizio_Caruso@hotmail.com
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.

// Permission is granted to anyone to use this software for non-commercial applications, 
// subject to the following restrictions:

// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software. If you use this software in
// a product, an acknowledgment in the product documentation would be
// appreciated but is not required.

// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.

// 3. This notice may not be removed or altered from any source distribution.
/* --------------------------------------------------------------------------------------- */ 
 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "settings.h"
#include "character.h"
#include "strategy.h"
#include "text.h"
#include "missile.h"
#include "level.h"
#include "enemy.h"
#include "invincible_enemy.h"
#include "powerups.h"

// Input input/output driver headers
#include "display_macros.h"
#include "input_macros.h"
#include "sleep_macros.h"
#include "sound_macros.h"

void _printScore(char * text, unsigned int score)
{
}

#include "patch/cmoc_conio_patch.h"	
#include "display_macros.c"
#include "input_macros.c"



unsigned short invincibleSlowDown;
unsigned short invincibleXCountDown;
unsigned short invincibleYCountDown;

unsigned short ghostSlowDown;
unsigned short powerUpCoolDown;
	
unsigned short gunCoolDown;

unsigned short ghostLevelDecrease;
unsigned short powerUpInitialCoolDown; 

unsigned short ghostLevel;
unsigned int points;
unsigned int highScore;

unsigned char lives;
unsigned char missileDirection;
unsigned char XSize;
unsigned char YSize;

unsigned short loop;

// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. invincibleXCountDown (time needed to activate the invincible ghost)
// 4. invincibleYCountDown
// 5. invincibleSlowDown (how much the invincible ghost is slowed-down)
// 6. invincibleLoopTrigger (how long before the invincible ghost appears)
unsigned char level;

unsigned short invincibleLoopTrigger;



Character invincibleGhost;
Character player; 

Character powerUp;
Character gun;


Character missile;


Character ghosts[GHOSTS_NUMBER];
Character bombs[BOMBS_NUMBER];

unsigned char strategyArray[GHOSTS_NUMBER];


unsigned char playerDirection = 0; // 0: right, 1: down, 2: left, 3: up

unsigned char playerFire = 0;
unsigned char guns = GUNS_NUMBER;

unsigned int extraLifeThroughPointsCounter = 1;

unsigned char ghostCount = GHOSTS_NUMBER;

unsigned char invincibleGhostHits = 0;

unsigned char invincibleGhostAlive = 1;

void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
{
	if(hunterPtr->_x<preyPtr->_x)
	{
		++hunterPtr->_x;
	}
	else if(hunterPtr->_x>preyPtr->_x)
	{
		--hunterPtr->_x;
	}
	else if(hunterPtr->_y<preyPtr->_y)
	{
		++hunterPtr->_y;
	}
	else
	{
		--hunterPtr->_y;
	}
}


void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
{
    if(hunterPtr->_y<preyPtr->_y)
	{
		++hunterPtr->_y;
	}
	else if(hunterPtr->_y>preyPtr->_y)
	{
		--hunterPtr->_y;
	}
	else if(hunterPtr->_x<preyPtr->_x)
	{
		++hunterPtr->_x;
	}
	else 
	{
		--hunterPtr->_x;
	}
}

// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
void moveTowardCharacter(Character *hunterPtr, unsigned char strategy)
{
	if(rand()%10 > strategy) // Select blind chase strategy
		{ // 0 - 4
			blindChaseCharacterXStrategy(hunterPtr, &player);
		}
		else
		{ // 5 - 9
			blindChaseCharacterYStrategy(hunterPtr, &player);
		}
}


void computeStrategy(void)
{
	unsigned char i;

	for(i=1; i<GHOSTS_NUMBER-1; ++i) // 6,1,1
	{
		strategyArray[i] = 4; // no preference (approximate straight line)
	}	
	#if GHOSTS_NUMBER>=3
		strategyArray[0] = 2;
		strategyArray[1] = 6;
	
	#else
		strategyArray[0] = 2;
		strategyArray[GHOSTS_NUMBER-1] = 6;
	#endif

}

// Ghosts move to new positions if they get their chanche
void chasePlayer(unsigned short slowDown)
{
	unsigned char i;
	
	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		if((ghosts[i]._status) && (rand()>slowDown))
		{
			DELETE_GHOST(ghosts[i]._x,ghosts[i]._y,ghosts[i]._imagePtr);
			moveTowardCharacter(&ghosts[i], strategyArray[i]);
		}
	}
}



unsigned short computeGhostSlowDown(void)
{
	if(ghostLevel<1000)
		return INITIAL_ENEMY_SLOWDOWN-level*200-ghostLevel*16;
	else
		return 1000;
}

void decreaseGhostLevel(void)
{
	if(ghostLevel>ghostLevelDecrease)
		ghostLevel-=ghostLevelDecrease;
	else
		ghostLevel=0;
}

void displayGhosts(void)
{
	unsigned char i;

	for(i=0;i<GHOSTS_NUMBER;++i)
	{
		DRAW_GHOST(ghosts[i]._x, ghosts[i]._y, ghosts[i]._imagePtr);
	}
}


void initializeCharacter(Character* characterPtr, unsigned char x, unsigned char y, unsigned char status, Image * imagePtr)
{
	characterPtr->_x = x;
	characterPtr->_y = y;
	characterPtr->_status = status;
	characterPtr->_imagePtr = imagePtr;
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
}

unsigned char playerReached(Character* preyPtr)
{
	return sameLocationAstAnyLocation(preyPtr->_x, preyPtr->_y, ghosts, GHOSTS_NUMBER);
}

unsigned char playerReachedBombs(Character* preyPtr)
{
	return sameLocationAstAnyLocation(preyPtr->_x, preyPtr->_y, bombs, BOMBS_NUMBER);
}

void ghostDies(Character * ghostPtr)
{
	EXPLOSION_SOUND();
	die(ghostPtr);
	//displayStats();
	--ghostCount;
	//printGhostCountStats();
}

void checkBombsVsGhost(Character * ghostPtr)
{
	
	if(ghostPtr->_status && playerReachedBombs(ghostPtr))
	{
		points+=GHOST_VS_BOMBS_BONUS;	
		ghostPtr->_imagePtr = &BOMB_IMAGE;
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


unsigned char sameLocationAstAnyLocation(unsigned char x, unsigned char y, Character *characterList, unsigned char length)
{
	unsigned char i = 0;
	for(;i<length;++i)
	{
		if(isCharacterAtLocation(x,y,&characterList[i]))
			return 1;
	}	
	return 0;
}

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
unsigned char safeLocation(unsigned char x, unsigned char y, Character *dangerPtr, unsigned char dangerSize)
{
	return !(sameLocationAstAnyLocation(x,y,ghosts,GHOSTS_NUMBER) || sameLocationAstAnyLocation(x,y,dangerPtr, dangerSize));
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


void fillLevelWithCharacters(unsigned char nGhosts)
{

	unsigned char i;
	unsigned char j;
	unsigned char count = 0;

	
	for(i=0;i<3;++i)
	{
		for(j=0;j<3;++j)
		{
			if(nGhosts>count)
			{
				if(!((i==1) && (j==1)))
				{				
					initializeCharacter(&ghosts[count],(unsigned char) ( XSize/6+j*2*(XSize/6)),(unsigned char) (YSize/6+i*2*(YSize/6)+i),1,&GHOST_IMAGE);
					DRAW_GHOST(ghosts[count]._x, ghosts[count]._y, ghosts[count]._imagePtr);
				}
				else
				{
					initializeCharacter(&ghosts[count],(unsigned char) (XSize-4),(unsigned char) (YSize-4),1,&GHOST_IMAGE);					
				}
			}
			else
			{
				initializeCharacter(&ghosts[count],(unsigned char) (GHOSTS_NUMBER-count),(unsigned char) (1),0,&DEAD_GHOST_IMAGE);
			}
			++count;
		}
	}


	#if BOMBS_NUMBER==4
	{
		count = 0;
		for(i=1;i<=2;++i)
		{
			for(j=1;j<=2;++j)
			{
				initializeCharacter(&bombs[count],(unsigned char) ((XSize/3)*i-1+rand()%3), (unsigned char) ((YSize/3)*j-1+rand()%3),0,&BOMB_IMAGE);
				++count;
			}
		}
	}
	#elif BOMBS_NUMBER==3	
	{
		unsigned char rnd = rand()%4;
		initializeCharacter(&bombs[0],(unsigned char) XSize/3-2+rnd, (unsigned char) (YSize/3)-1+rnd,0,&BOMB_IMAGE);

		initializeCharacter(&bombs[1],(unsigned char) XSize/2-2+rnd, (unsigned char) ((YSize/3)*2)-1+rnd,0,&BOMB_IMAGE);

		initializeCharacter(&bombs[2],(unsigned char) 2*(XSize/3)-2+rnd, (unsigned char) (YSize/3)-1-rnd,0,&BOMB_IMAGE);
	}
	#elif BOMBS_NUMBER==2
		initializeCharacter(&bombs[0],(unsigned char) XSize/2-3+rand()%7, (unsigned char) ((YSize/3))-1+rand()%3,0,&BOMB_IMAGE);

		initializeCharacter(&bombs[1],(unsigned char) XSize/2-3+rand()%7,(unsigned char)  ((YSize/3)*2)-1+rand()%3,0,&BOMB_IMAGE);
	#elif BOMBS_NUMBER==1
		initializeCharacter(&bombs[0],(unsigned char) XSize/2-3+rand()%7,(unsigned char)  ((YSize/2))-1+rand()%3,0,&BOMB_IMAGE);
	#endif



	
	initializeCharacter(&player,(unsigned char) (XSize/2+rand()%4-2),(unsigned char) (YSize/2+rand()%4-2),1,&PLAYER_IMAGE);	

	DRAW_PLAYER(player._x,player._y,player._imagePtr);

	
}

void playerDies(void)
{
	EXPLOSION_SOUND();
	die(&player);
	//printDefeatMessage();
	sleep(1);	
}
		
int main(void)
{		
	INIT_IMAGES();
	
	// Ask for the screen size 
	//GET_SCREEN_SIZE(&XSize, &YSize);
	XSize = 32;
	YSize = 16;
	
	highScore = 0;
	
	while(1)
	{
		//initialScreen();
		WAIT_PRESS();
		CLEAR_SCREEN();

		// TODO:
		highScoreScreen();

		WAIT_PRESS();
		CLEAR_SCREEN();
		
		extraLifeThroughPointsCounter = 1;
		points = 0;
		level = INITIAL_LEVEL; 	
		lives = LIVES_NUMBER;
		ghostCount = GHOSTS_NUMBER;
		do // Level (Re-)Start
		{ 
			invincibleGhostAlive = 1;
			invincibleGhostHits = 0;

			computeStrategy();
			
			loop = 0;
			ghostLevel = 0;
			
			guns = 0;
			gun._status = 0;
			
			// computePowerUp(&ghostLevelDecrease, &powerUpInitialCoolDown);
			ghostLevelDecrease = 140-(level*2);
			powerUpInitialCoolDown = 200+(level*2);
			
			
			gunCoolDown = GUN_INITIAL_COOLDOWN;


			ghostSlowDown = computeGhostSlowDown();
			
			// Clear the screen, put cursor in upper left corner
			CLEAR_SCREEN();

			//printLevel();
			sleep(1);
	
			
			// Wait for the user to press a key 
			//printPressKeyToStart();
			WAIT_PRESS();
 
			//deleteCenteredMessage();
			
			// Draw a border around the screen 
			DRAW_BORDERS();
			
			fillLevelWithCharacters(ghostCount);	
			

			// displayStatsTitles();
			// displayStats();			
			// printLevelStats();
			// printLivesStats();
			
			// printGunsStats();
			// printGhostCountStats();
			

			while(player._status && (ghostCount>0) )
			{
				#if defined(SLOW_DOWN)
				{
					unsigned short i;
					for(i=0;i<GAME_SLOW_DOWN;++i)
					{
					}
				}
				#endif

				++loop;
				
				if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
				{
					++extraLifeThroughPointsCounter;
					PING_SOUND();
					++lives;
					//printLivesStats();
				}
				
				ghostSlowDown = computeGhostSlowDown();

				MOVE_PLAYER();

			
				chasePlayer(ghostSlowDown);
				
				
				// Check collisions bombs vs ghosts
				checkBombsVsGhosts();

				if(wallReached(&player) || playerReached(&player) || playerReachedBombs(&player))
				{
					playerDies();
				}
			
				DRAW_BOMBS();
				
				// Display ghosts

				displayGhosts();

				++ghostLevel;
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

			if(player._status) // if level finished
			{
				sleep(1);
				//printVictoryMessage();
				sleep(2);
				CLEAR_SCREEN();			
				if(level<=10)
				{
					points+= LEVEL_BONUS*level;
					//printLevelBonus(LEVEL_BONUS*level);
				}
				else
				{				
					points+= LEVEL_BONUS*10;
					//printLevelBonus(LEVEL_BONUS*10);
				}
				sleep(1);
				CLEAR_SCREEN();				

				ghostCount = GHOSTS_NUMBER;
				
				++level;

			}
			else // if dead
			{
				CLEAR_SCREEN();
				--lives;
				if(lives>0)
				{
					player._status = 1;
				}
			}
		} while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game 
			
	if(level==FINAL_LEVEL+1) // if completed game
	{
		//gameCompleted();
		sleep(2);
	}
	// GAME OVER	
	//printGameOver();
	sleep(2);
	CLEAR_SCREEN();
	//finalScore();
	sleep(2);
	if(points>highScore)
	{
		highScore = points;
	}
	CLEAR_SCREEN();
	} // while(1) -> restart from the beginning

	return EXIT_SUCCESS;
}
