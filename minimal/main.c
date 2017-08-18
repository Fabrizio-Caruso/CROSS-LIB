/******* *********************************************************************/
/*                                                                           */
/*                                		                             */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* (C) 2017      Fabrizio Caruso                                             */
/*                					                     */
/*              				                             */
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
 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__SPECTRUM__)
	#include <input.h>
#endif 

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


#define POKE(addr,val)     (*(unsigned char*) (addr) = (val))
#define POKEW(addr,val)    (*(unsigned*) (addr) = (val))
#define PEEK(addr)         (*(unsigned char*) (addr))
#define PEEKW(addr) (*(unsigned*) (addr))
//#define DEBUG_CHARACTERS



unsigned short invincibleSlowDown;
unsigned short invincibleXCountDown;
unsigned short invincibleYCountDown;

unsigned short ghostSlowDown;
unsigned short powerUpCoolDown;
	
unsigned short gunInitialCoolDown;
unsigned short gunCoolDown;

unsigned short ghostLevelDecrease;
unsigned short powerUpInitialCoolDown; 

unsigned short ghostLevel;
unsigned long points;
unsigned long highScore;

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
//unsigned short invincibleGhostCountTrigger;

extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;

extern Image BUBBLE_IMAGE;

Character invincibleGhost;
Character player; 

Character powerUp;
Character gun;


Character missile;
Character leftEnemyMissile;
Character rightEnemyMissile;


Character ghosts[GHOSTS_NUMBER];
Character bombs[BOMBS_NUMBER];

unsigned char strategyArray[GHOSTS_NUMBER];


unsigned char playerDirection = 0; // 0: right, 1: down, 2: left, 3: up

unsigned char playerFire = 0;
unsigned char guns = GUNS_NUMBER;

unsigned long extraLifeThroughPointsCounter = 1;

unsigned char ghostCount = GHOSTS_NUMBER;

unsigned char invincibleGhostHits = 0;

unsigned char invincibleGhostAlive = 1;


void handle_missile()
{
	// Check if player has fired the gun
	if(playerFire && missile._status==0 && guns>0)
	{
		SHOOT_SOUND();
		--guns;
		printGunsStats();
		missileDirection = playerDirection;
		missile._status = setMissileInitialPosition(&missile, &player, missileDirection);
		playerFire = 0;
		DRAW_MISSILE(missile._x,missile._y,missile._imagePtr);					
		checkMissileVsGhosts(&missile);
		// if(areCharctersAtSamePosition(&missile, &invincibleGhost))
			// {
				// die(&missile);
				// DELETE_MISSILE(missile._x,missile._y,missile._imagePtr);
				// restoreMissile(&missile);
				// ++invincibleGhostHits;
				// decreaseGhostLevel();
				// DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
			// }		
	}
	
	// Move missile if fired
	if(missile._status==1)
	{
		moveMissile(&missile, missileDirection);
		// TODO: Inefficient
		checkMissileVsGhosts(&missile);
		
		if(areCharctersAtSamePosition(&missile, &invincibleGhost))
		{
			PING_SOUND();
			die(&missile);
			DELETE_MISSILE(missile._x,missile._y,missile._imagePtr);
			restoreMissile(&missile);			
			++invincibleGhostHits;
			decreaseGhostLevel();
			
			// TODO: to TEST
			if(invincibleGhostHits>=MIN_INVINCIBLE_GHOST_HITS)
			{
				unsigned char i;
				invincibleGhost._status = 0;
				DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y, invincibleGhost._imagePtr);
				invincibleGhost._x=XSize-2; invincibleGhost._y=YSize-2;
				invincibleGhostAlive = 0;
				for(i=0;i<4;++i)
					EXPLOSION_SOUND();
				points+=INVINCIBLE_GHOST_POINTS;
				displayStats();
			}
			else
			{
				DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
			}
		}
	}
}


void handle_gun_item()
{
	// Manage gun 
	if(gun._status==1)
	{
		if(areCharctersAtSamePosition(&player, &gun))
		{
			ZAP_SOUND();
			DELETE_GUN(gun._x,gun._y,gun._imagePtr);
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
			guns = GUNS_NUMBER;
			printGunsStats();
			points+=GUN_BONUS;
			displayStats();
			gun._status = 0;	
			gunCoolDown = gunInitialCoolDown;
		}
		else
		{
			DRAW_GUN(gun._x, gun._y, gun._imagePtr);
		}
	}		
	else if (gunCoolDown == 0)
	{	
		gun._status = 1;

			relocateCharacter(&gun, bombs,4);

		DRAW_GUN(gun._x, gun._y, gun._imagePtr);
	}
	else
	{
		--gunCoolDown;
	}				
}


void handle_powerup_item()
{
	// Manage powerUp
	if(powerUp._status == 1)
	{	
		if(areCharctersAtSamePosition(&player, &powerUp))
		{
			ZAP_SOUND();
			die(&powerUp);
			DELETE_POWERUP(powerUp._x,powerUp._y,powerUp._imagePtr);
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
			decreaseGhostLevel(); 
			points+=POWER_UP_BONUS;
			displayStats();
			powerUpCoolDown = powerUpInitialCoolDown;
		}
		else
		{
			DRAW_POWERUP(powerUp._x,powerUp._y,powerUp._imagePtr);
		}		
	}
	else if (powerUpCoolDown == 0)
	{
		powerUp._status = 1;

			relocateCharacter(&powerUp, bombs,4);

		DRAW_POWERUP(powerUp._x,powerUp._y,powerUp._imagePtr);
	}
	else
	{
		--powerUpCoolDown;
	}
}


void handle_invincible_ghost(void)
{
	if(!invincibleGhost._status)
	{
		// Manage invincible ghost
		if(invincibleGhostAlive &&
							((invincibleXCountDown==0)     || (invincibleYCountDown==0) || 
							 (loop>=invincibleLoopTrigger) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER)))
		{
			invincibleGhost._status = 1;
			DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
		}
		else
		{
			--invincibleXCountDown;
			--invincibleYCountDown;
		}
	}
	else
	{ 	
		invincibleSlowDown = computeInvincibleSlowDown();

		if(rand()>invincibleSlowDown)
		{
			TOCK_SOUND();
			DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y,invincibleGhost.imagePtr);
			moveTowardCharacter(&invincibleGhost, &player, 4);
		}
		DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
		if(areCharctersAtSamePosition(&invincibleGhost, &player))
		{
			EXPLOSION_SOUND();
			die(&player);
			printDefeatMessage();
			sleep(1);
		}
	}
}


void handle_player_vs_outer_wall(void)
{
	// Check collision player vs outer wall
	if(wallReached(&player))
	{
		EXPLOSION_SOUND();
		die(&player);
		//DELETE_PLAYER(player._x,player._y,player._imagePtr);
		// DRAW_BROKEN_WALL(player._x,player._y);
		printDefeatMessage();
		sleep(1);
	}
}

void handle_player_vs_bombs_and_ghosts(void)
{
	// Check collision player vs ghosts and player vs bombs
	if((playerReached(&player) || playerReachedBombs(&player)))
	{
		EXPLOSION_SOUND();
		die(&player);
		//DELETE_PLAYER(player._x,player._y,player._imagePtr);
		printDefeatMessage();
		sleep(1);
	}	
}
			
void initialScreen(void)
{
	// Set Screen Colors
	setScreenColors();			
	CLEAR_SCREEN();					
	printStartMessage();

}
		
int main(void)
{		
	INIT_INPUT();
	
	// Ask for the screen size 
	GET_SCREEN_SIZE(&XSize, &YSize);
	
	highScore = 0;
	
	INIT_GRAPHICS();
	while(1)
	{

		INIT_IMAGES();

		initialScreen();
		WAIT_PRESS()
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
			
			
			gunCoolDown = computeGunInitialCoolDown();
			
			computeInvincibleGhostParameters();

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
			drawBorders();

			
			fillLevelWithCharacters(ghostCount);	
			

			displayStatsTitles();
			displayStats();			
			printLevelStats();
			printLivesStats();
			//
			printGunsStats();
			printGhostCountStats();
			

			while(player._status && (ghostCount>0) ) // while alive && there are still ghosts
			{
				#if defined(SLOW_DOWN)
					unsigned char i;
					for(i=0;i<GAME_SLOW_DOWN;++i)
					{
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

				
				handle_missile();
			

				chasePlayer(ghostSlowDown);
				
				// This detects collisions of ghosts that have just moved
				if(missile._status)
				{
					checkMissileVsGhosts(&missile);
				}
				
				handle_player_vs_bombs_and_ghosts();

				// Check collisions bombs vs ghosts
				checkBombsVsGhosts();
				
				handle_gun_item();
				
				handle_powerup_item();

				handle_player_vs_outer_wall();
				
				DRAW_BOMBS();
				
				// Display ghosts
				displayGhosts();
				
				#if (defined(__ATARI__) || defined(__ATARIXL__)) && defined(ATARI_MODE1) 
					displayStatsTitles();
					printLevelStats();
					printLivesStats();
					printGunsStats();
					printGhostCountStats();
				#endif

				
				handle_invincible_ghost();

				++ghostLevel;
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

			if(player._status) // if level finished
			{
				sleep(1);
				printVictoryMessage();
				sleep(1);
				if(level<=10)
				{
					CLEAR_SCREEN();
					points+= LEVEL_BONUS*level;
					printLevelBonus(LEVEL_BONUS*level);
				}
				else
				{
					CLEAR_SCREEN();				
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
		sleep(1);
	}
	// GAME OVER	
	printGameOver();
	sleep(1);
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
