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


#include "patch/cmoc_conio_patch.h"	
#include "display_macros.c"
#include "input_macros.c"
#include "strategy.c"
#include "enemy.c"
#include "text.c"
#include "character.c"
#include "level.c"

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


void playerDies(void)
{
	EXPLOSION_SOUND();
	die(&player);
	printDefeatMessage();
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

			printLevel();
			sleep(1);
	
			
			// Wait for the user to press a key 
			printPressKeyToStart();
			WAIT_PRESS();
 
			deleteCenteredMessage();
			
			// Draw a border around the screen 
			DRAW_BORDERS();
			
			fillLevelWithCharacters(ghostCount);	
			

			displayStatsTitles();
			displayStats();			
			printLevelStats();
			printLivesStats();
			
			printGunsStats();
			printGhostCountStats();
			

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
					printLivesStats();
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
				printVictoryMessage();
				sleep(2);
				CLEAR_SCREEN();			
				if(level<=10)
				{
					points+= LEVEL_BONUS*level;
					printLevelBonus(LEVEL_BONUS*level);
				}
				else
				{				
					points+= LEVEL_BONUS*10;
					printLevelBonus(LEVEL_BONUS*10);
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
		gameCompleted();
		sleep(2);
	}
	// GAME OVER	
	printGameOver();
	sleep(2);
	CLEAR_SCREEN();
	finalScore();
	sleep(2);
	if(points>highScore)
	{
		highScore = points;
	}
	CLEAR_SCREEN();
	} // while(1) -> restart from the beginning

	return EXIT_SUCCESS;
}
