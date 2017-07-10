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

#include "settings.h"
#include "character.h"
#include "strategy.h"
#include "text.h"
#include "missile.h"
#include "input.h"
#include "level.h"
#include "enemy.h"
#include "invincible_enemy.h"
#include "powerups.h"

// Input input/output driver headers
#include "display_macros.h"
#include "input_macros.h"
#include "sleep_macros.h"
#include "sound_macros.h"


// #define DEBUG_ATARI
#define DEBUG_CHARACTERS

unsigned int invincibleSlowDown = 30000;
unsigned short invincibleXCountDown = 100;
unsigned short invincibleYCountDown = 100;

unsigned int ghostSlowDown;
unsigned int powerUpCoolDown;
	
unsigned int gunInitialCoolDown;
unsigned int gunCoolDown;
unsigned int extraPointsCoolDown;

unsigned int ghostLevelDecrease = 100;
unsigned int powerUpInitialCoolDown = 100; 

unsigned int ghostLevel = 1u;
unsigned long points = 0ul;

unsigned short playerDirection = 0; // 0: right, 1: down, 2: left, 3: up
unsigned short missileDirection = 0;
unsigned short playerFire = 0;
unsigned short guns = GUNS_NUMBER;

unsigned short lives;

unsigned short innerVerticalWallY; 
unsigned short innerVerticalWallX; 
unsigned short innerVerticalWallLength;

unsigned char extraLifeThroughPointsCounter = 1;

// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. invincibleXCountDown (time needed to activate the invincible ghost)
// 4. invincibleYCountDown
// 5. invincibleSlowDown (how much the invincible ghost is slowed-down)
// 6. invincibleLoopTrigger (how long before the invincible ghost appears)
unsigned short level = 1;

unsigned int invincibleLoopTrigger = 1000;
unsigned short invincibleGhostCountTrigger = 2;

unsigned short ghostCount = GHOSTS_NUMBER;

unsigned char XSize;
unsigned char YSize;

unsigned int loop;

unsigned char invincibleGhostHits = 0;

unsigned char invincibleGhostAlive = 1;

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
Character extraPoints;
//Character extraLife;

Character missile;
Character leftEnemyMissile;
Character rightEnemyMissile;

Character* ghosts[GHOSTS_NUMBER];
Character* bombs[BOMBS_NUMBER];
Character* bubbles[BUBBLES_NUMBER];


char strategyArray[GHOSTS_NUMBER];
	
void initializeCharacters(void)
{
	unsigned char i;
	for(i=0;i<GHOSTS_NUMBER;++i)
	{	
		ghosts[i] = (Character *) malloc(sizeof(Character));
	}
	for(i=0;i<BOMBS_NUMBER;++i)
	{
		bombs[i] = (Character *) malloc(sizeof(Character));
	}
	
	for(i=0;i<BUBBLES_NUMBER;++i)
	{
		bubbles[i] = (Character *) malloc(sizeof(Character));
	}
}

void handle_missile()
{
	// Check if player has fired the gun
	if(playerFire && missile._status==0 && guns>0)
	{
		SHOOT_SOUND();
		--guns;
		missileDirection = playerDirection;
		missile._status = setMissileInitialPosition(&missile, &player, missileDirection);
		playerFire = 0;
		DRAW_MISSILE(missile._x,missile._y,missile._imagePtr);					
		checkMissileVsGhosts(&missile);
		if(areCharctersAtSamePosition(&missile, &invincibleGhost))
			{
				die(&missile);
				DELETE_MISSILE(missile._x,missile._y,missile._imagePtr);
				restoreMissile(&missile);
				extraPointsCoolDown/=2;
				DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
			}		
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
			extraPointsCoolDown/=2;
			++invincibleGhostHits;
			
			// TODO: to TEST
			if(invincibleGhostHits>=MIN_INVINCIBLE_GHOST_HITS)
			{
				unsigned char i;
				invincibleGhost._status = 0;
				DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y, invincibleGhost._imagePtr);
				invincibleGhost._x=XSize-2; invincibleGhost._y=YSize-2;
				invincibleGhostAlive = 0;
				for(i=0;i<7;++i)
					EXPLOSION_SOUND();
				points+=INVINCIBLE_GHOST_POINTS;
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
		if(powerUpReached(&player, &gun))
		{
			ZAP_SOUND();
			DELETE_GUN(gun._x,gun._y,gun._imagePtr);
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
			guns = GUNS_NUMBER;
			points+=GUN_BONUS;
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
		do
		{
			relocateCharacter(&gun, bombs,4);
		} while(nearInnerWall(&gun));
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
		if(powerUpReached(&player, &powerUp))
		{
			ZAP_SOUND();
			die(&powerUp);
			DELETE_POWERUP(powerUp._x,powerUp._y,powerUp._imagePtr);
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
			decreaseGhostLevel(); 
			points+=POWER_UP_BONUS;
			//powerUp._status = 0;	
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
		do
		{
			relocateCharacter(&powerUp, bombs,4);
		} while(nearInnerWall(&powerUp));
		DRAW_POWERUP(powerUp._x,powerUp._y,powerUp._imagePtr);
	}
	else
	{
		--powerUpCoolDown;
	}
}


void handle_extraPoints_item()
{
	// Manage gun 
	if(extraPoints._status==1)
	{
		if(powerUpReached(&player, &extraPoints))
		{
			ZAP_SOUND();
			DELETE_EXTRA_POINTS(extraPoints._x,extraPoints._y,extraPoints._imagePtr);
			DRAW_PLAYER(player._x, player._y, player._imagePtr);
			points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
			if(missileLevel(level))
				points+=EXTRA_POINTS;
			extraPoints._status = 0;	
			extraPointsCoolDown = EXTRA_POINTS_COOL_DOWN;
		}
		else
		{
			DRAW_EXTRA_POINTS(extraPoints._x, extraPoints._y, extraPoints._imagePtr);
		}
	}		
	else if (extraPointsCoolDown == 0)
	{	
		extraPoints._status = 1;
		do
		{
			relocateCharacter(&extraPoints, bombs,4);
		} while(nearInnerWall(&extraPoints));
		DRAW_EXTRA_POINTS(extraPoints._x, extraPoints._y, extraPoints._imagePtr);
	}
	else
	{
		--extraPointsCoolDown;
	}				
}


void handle_invincible_ghost(void)
{
	// Manage invincible ghost
	if(!invincibleGhost._status && invincibleGhostAlive &&
	  ( (invincibleXCountDown==0)     || (invincibleYCountDown==0) || 
	    (loop>=invincibleLoopTrigger) || (ghostCount<=invincibleGhostCountTrigger)))
	{
		invincibleGhost._status = 1;
		DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
	}
	else
	{
		--invincibleXCountDown;
		--invincibleYCountDown;
	}
	if(invincibleGhost._status)
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
		DRAW_BROKEN_WALL(player._x,player._y);
		printDefeatMessage();
		sleep(1);
	}
}


void handle_player_vs_inner_wall(void)
{
	// Check collistion player vs inner wall
	if(innerWallReached(&player))
	{
		EXPLOSION_SOUND();
		die(&player);
		//DELETE_PLAYER(player._x,player._y,player._imagePtr);
		DRAW_BROKEN_WALL(player._x,player._y);		
		printDefeatMessage();
		sleep(1);
	}
}

void handle_player_vs_bombs_and_ghosts(void)
{
	// Check collision player vs ghosts and player vs bombs
	if(playerReached(&player) ||
	   playerReachedBombs(&player))
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
	
	initializeCharacters();
	
	while(1)
	{
		char arrowRange = 3;

		INIT_GRAPHICS();
		INIT_IMAGES();
		
		CLEAR_SCREEN();
		
		#ifdef DEBUG_CHARACTERS
			PRINTF(0,0,"ghost %c\n", GHOST_IMAGE._imageData);
			PRINTF(0,1,"invincible %c\n", INVINCIBLE_GHOST_IMAGE._imageData);
			PRINTF(0,2,"bomb %c\n", BOMB_IMAGE._imageData);
			PRINTF(0,3,"player %c\n", PLAYER_IMAGE._imageData);
			PRINTF(0,4,"powerup %c\n", POWERUP_IMAGE._imageData);
			PRINTF(0,5,"gun %c\n", GUN_IMAGE._imageData);
			PRINTF(0,6,"missile %c\n", MISSILE_IMAGE._imageData);
			PRINTF(0,7,"XSize: %d\n", XSize);
			PRINTF(0,8,"YSize: %d\n", YSize);
			
			// gotoxy(0,0); cputs("hello world1");
			// gotoxy(20,0);cputs("hello world2");
			// gotoxy(0,1); cputs("hello world3");
			// gotoxy(20,1);cputs("hello world4");
			// gotoxy(0,2); cputs("hello world5");
			// gotoxy(20,2);cputs("hello world6");
			// gotoxy(0,3); cputs("hello world7");
			// gotoxy(20,3);cputs("hello world8");
		#endif
		WAIT_PRESS()
		
		initialScreen();
		WAIT_PRESS()
		CLEAR_SCREEN();
		deleteCenteredMessage();

		extraLifeThroughPointsCounter = 1;
		points = 0ul;
		level = INITIAL_LEVEL; 	
		lives = LIVES_NUMBER;
		ghostCount = GHOSTS_NUMBER;
		do // Level (Re-)Start
		{ 
			invincibleGhostAlive = 1;
			invincibleGhostHits = 0;
			extraPointsCoolDown = EXTRA_POINTS_COOL_DOWN;
			//unsigned char i;
			computeStrategy();
			loop = 0;
			ghostLevel = 0u;
			//ghostCount = GHOSTS_NUMBER;
			guns = 0;
			gun._status = 0;
			
			computePowerUp(&ghostLevelDecrease, &powerUpInitialCoolDown);
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
			
			// Initialize characters
			updateInnerWallVerticalData();	
			
			fillLevelWithCharacters(ghostCount);	
			
			displayStatsTitles();
			
			rightEnemyMissile._x = XSize-4; rightEnemyMissile._y = 4;
			leftEnemyMissile._x = 4; leftEnemyMissile._y = YSize-4;
			
	
			while(player._status && ghostCount>0) // while alive && there are still ghosts
			{
				++loop;
				
				if(points>extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS)
				{
					++extraLifeThroughPointsCounter;
					PING_SOUND();
					++lives;
				}
				
				ghostSlowDown = computeGhostSlowDown();
				
				drawInnerVerticalWall();

				if(ghostCount<=MAX_GHOST_COUNT_FOR_BUBBLES && level >= FIRST_BUBBLES_LEVEL && 
				   !missileLevel(level) && !missileLevel(level-1) && !missileLevel(level-2))
				{ 
					unsigned char i;

					for(i=0;i<BUBBLES_NUMBER;i++)
					{
						if(bubbles[i]->_status)
						{
							//unsigned int rnd = rand();

							if(areCharctersAtSamePosition(&player,bubbles[i]))
							{
								EXPLOSION_SOUND();
								die(&player);
								printDefeatMessage();
								sleep(1);
							}
							
							if(rand()%2)//if(!(rnd//%5))
							{
								DELETE_MISSILE(bubbles[i]->_x, bubbles[i]->_y, bubbles[i]->_imagePtr);					
								--(bubbles[i]->_y);

								// if(!(rnd%3))
								// {
									// if(!(rnd%2))
									// {
										// --(bubbles[i]->_x);
									// }
									// else
									// {
										// ++(bubbles[i]->_x);
									// }
								// }
							}

							DRAW_MISSILE(bubbles[i]->_x, bubbles[i]->_y, bubbles[i]->_imagePtr);			
							if(bubbles[i]->_y<=1)
							{	
								bubbles[i]->_status = 0;
								DELETE_MISSILE(bubbles[i]->_x, bubbles[i]->_y, bubbles[i]->_imagePtr);					
							}
						}
						else
						{
							bubbles[i]->_x = (i+1)*(XSize/(BUBBLES_NUMBER+1));
							bubbles[i]->_y = YSize-2;
							bubbles[i]->_status = 1;
						}
					}
				}

				
				if(missileLevel(level))
				{
					arrowRange = computeArrowRange();
					DELETE_MISSILE(leftEnemyMissile._x,leftEnemyMissile._y,leftEnemyMissile._imagePtr);
					if(leftEnemyMissile._x==XSize-2)
					{
						leftEnemyMissile._x=2;
						leftEnemyMissile._y = YSize-4;
					}
					else
					{
						++leftEnemyMissile._x;
						if(loop%2 && player._y>=YSize-4-arrowRange && player._x>=leftEnemyMissile._x)
						{
							if(player._y>leftEnemyMissile._y)
							{
								++leftEnemyMissile._y;
							}
							else if(player._y<leftEnemyMissile._y)
							{
								--leftEnemyMissile._y;
							}
						}
					}
					DRAW_MISSILE(leftEnemyMissile._x,leftEnemyMissile._y,leftEnemyMissile._imagePtr);
					if(areCharctersAtSamePosition(&leftEnemyMissile,&player))
					{
						EXPLOSION_SOUND()
						die(&player);
						printDefeatMessage();
						sleep(1);
					}
					
					DELETE_MISSILE(rightEnemyMissile._x,rightEnemyMissile._y,rightEnemyMissile._imagePtr);
					if(rightEnemyMissile._x==2)
					{
						rightEnemyMissile._x=XSize-2;
						rightEnemyMissile._y = 4;
					}
					else
					{
						--rightEnemyMissile._x;
						if(loop%2 && player._y<=4+arrowRange && player._x<= rightEnemyMissile._x)
						{
							if(player._y>rightEnemyMissile._y)
							{
								++rightEnemyMissile._y;
							}
							else if(player._y<rightEnemyMissile._y)
							{
								--rightEnemyMissile._y;
							}
						}
					}
					DRAW_MISSILE(rightEnemyMissile._x,rightEnemyMissile._y,rightEnemyMissile._imagePtr);				
					if(areCharctersAtSamePosition(&rightEnemyMissile,&player))
					{
						EXPLOSION_SOUND()
						die(&player);
						printDefeatMessage();
						sleep(1);
					}
				}
				
				MOVE_PLAYER();

				handle_missile();
			
				// Chase the player
				chasePlayer(ghostSlowDown);
				
				// TODO: this should detect collisions of ghosts that have just moved
				if(missile._status)
				{
					checkMissileVsGhosts(&missile);
				}
				
				handle_player_vs_bombs_and_ghosts();
				
				handle_player_vs_inner_wall();
			
				// Check collisions bombs vs ghosts
				checkBombsVsGhosts();
				
				// Check collisions ghosts vs ghosts
			    checkGhostsVsGhosts();
				
				handle_gun_item();
				
				handle_powerup_item();

				handle_extraPoints_item();
				
				handle_player_vs_outer_wall();
				
				DRAW_BOMBS();
				
				// Display ghosts
				displayGhosts();

				#if defined(__ATARI__) || defined(__ATARIXL__)
					displayStatsTitles();
				#endif
				displayStats();
				
				handle_invincible_ghost();
				
				// Add points to score
				points+=LOOP_POINTS;
				
				// Increase ghost speed
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


				if(missileLevel(level))
				{	
					CLEAR_SCREEN();
					sleep(1);
					PING_SOUND();
					printExtraLife();
					sleep(1);
					++lives;
				}
				++level;
				
				updateInnerWallVerticalData();
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
	CLEAR_SCREEN();
	} // while(1) -> restart from the beginning

	return EXIT_SUCCESS;
}
