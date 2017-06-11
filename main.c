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
 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



#include "settings.h"
#include "character.h"
#include "strategy.h"
#include "display.h"
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

// #if defined(__APPLE2__)
	// void sleep(unsigned int sec) { /* BOGUS */ sec = 1; } 
// #else
	// #include <time.h>
	// #include <unistd.h>
// #endif

unsigned int invincibleSlowDown = 30000;

unsigned short invincibleXCountDown = 100;
unsigned short invincibleYCountDown = 100;

unsigned int ghostSlowDown;
unsigned int powerUpCoolDown;
	
unsigned int gunInitialCoolDown;
unsigned int gunCoolDown;

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
unsigned short ghostCount = GHOSTS_NUMBER;
unsigned short invincibleGhostCountTrigger = 2;

unsigned char XSize;
unsigned char YSize;

unsigned int loop;



extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image DEAD_GHOST_IMAGE;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;


int main(void)
{	
	Character* ghosts[GHOSTS_NUMBER];
	Character invincibleGhost;
	Character* bombs[BOMBS_NUMBER];
	Character player; 
	Character powerUp;
	Character gun;
	Character missile;
	
	char i;
	
	
//	PLAYER_IMAGE._imageData = '*';
// PLAYER_IMAGE._color = COLOR_BLUE;

// GHOST_IMAGE._imageData = 'O';
// GHOST_IMAGE._color = COLOR_BLACK;

// DEAD_GHOST_IMAGE._imageData = '#';
// DEAD_GHOST_IMAGE._color = COLOR_RED;

// INVINCIBLE_GHOST_IMAGE._imageData = '+';
// INVINCIBLE_GHOST_IMAGE._color = COLOR_BLUE;

// BOMB_IMAGE._imageData = 'O';
// BOMB_IMAGE._color = COLOR_BLACK;

// POWERUP_IMAGE._imageData = '*';
// POWERUP_IMAGE._color = COLOR_BLUE;

// MISSILE_IMAGE._imageData = 'O';
// MISSILE_IMAGE._color = COLOR_BLACK;

// GUN_IMAGE._imageData = '*';
// GUN_IMAGE._color = COLOR_BLUE;

	
	
	#if defined (__PLUS4__) || defined(__C64__) || defined(__VIC20__) || defined(__NES__)
	// Do nothing
	#else 
		unsigned char kbInput;
	#endif
	
	#if defined (__ATMOS__) || defined(__NES__) 
	// Do nothing
	#else 
		unsigned char joyInput;
		JOY_INSTALL()
	#endif

	/* Ask for the screen size */
	GET_SCREEN_SIZE(&XSize, &YSize);

	for(i=0;i<GHOSTS_NUMBER;++i)
	{	
		ghosts[i] = (Character *) malloc(sizeof(Character));
		ghosts[i]->_imagePtr = (Image *) malloc(sizeof(Image));
	}
	for(i=0;i<BOMBS_NUMBER;++i)
	{
		bombs[i] = (Character *) malloc(sizeof(Character));
		bombs[i]->_imagePtr = (Image *) malloc(sizeof(Image));
	}
	player._imagePtr = (Image *) malloc(sizeof(Image));
	invincibleGhost._imagePtr  = (Image *) malloc(sizeof(Image));
	powerUp._imagePtr = (Image *) malloc(sizeof(Image));
	gun._imagePtr  = (Image *) malloc(sizeof(Image));

	
	while(1)
	{
		ghostCount = GHOSTS_NUMBER;
		loop = 0;	
		points = 0ul;
		level = INITIAL_LEVEL; 

		// Set Screen Colors
		setScreenColors();			
		
		CLEAR_SCREEN();
					
		printStartMessage();
		
		#if defined (__ATMOS__) || defined(__NES__) 
			// Do nothing
		#else 
			WAIT_JOY_PRESS();
		#endif
	
		CLEAR_SCREEN();
				
		deleteCenteredMessage();
		
		lives = LIVES_NUMBER;
		do // Level (Re-)Start
		{ 
			loop = 0;
			ghostLevel = 1u;
			
			computePowerUp(&ghostLevelDecrease, &powerUpInitialCoolDown);
			
			gunInitialCoolDown = computeGunInitialCoolDown();
			invincibleXCountDown = computeInvincibleCountDown();
			invincibleYCountDown = computeInvincibleCountDown();
			invincibleSlowDown = computeInvincibleSlowDown();
			invincibleGhostCountTrigger = computeInvincibleGhostCountTrigger();
			invincibleLoopTrigger = computeInvincibleLoopTrigger();
			ghostCount = GHOSTS_NUMBER;
			guns = 0;
			gun._status = 0;
			gunCoolDown = gunInitialCoolDown;
			
			/* Clear the screen, put cursor in upper left corner */
			CLEAR_SCREEN();

			printLevel();
			sleep(2);
			
			
			
			/* Wait for the user to press a key */
			printPressKeyToStart();
			
			#if defined (__ATMOS__) || defined(__NES__) 
				// Do nothing
			#else 
				WAIT_JOY_PRESS();
			#endif


			deleteCenteredMessage();
			
			/* Draw a border around the screen */
			drawBorders();
			
			// Initialize characters
			innerVerticalWallLength = drawInnerVerticalWallForLevel();
			
			fillLevelWithCharacters(
								 &player, &powerUp, 
								 ghosts, 
								 bombs, 
								 &invincibleGhost, &missile, &gun);	
			ghostCount = GHOSTS_NUMBER;
			
			displayStatsTitles();
			
			while(player._alive && ghostCount>0) // while alive && there are still ghosts
			{
				ghostSlowDown = computeGhostSlowDown();
				invincibleSlowDown = computeInvincibleSlowDown();
				drawInnerVerticalWall();
				
				++loop;

				#if defined (__PLUS4__) || defined(__C64__) || defined(__VIC20__) || defined(__NES__)
					// Do nothing
				#else 
				IF_KEYBOARD_HIT
					{				
						kbInput = GET_CHAR();
						movePlayerByKeyboard(&player, kbInput);
					}
				#endif // defined (__PLUS4__) || defined(__C64__) || defined(__VIC20__)
				#if defined(__ATMOS__) || defined(__NES__)
					// Do nothing here
				#else		
					{
						joyInput = GET_JOY1();			
						movePlayerByJoystick(&player, joyInput);	
					}
				#endif				
				if(playerFire && missile._status==0 && guns>0)
				{
					--guns;
					missileDirection = playerDirection;
					missile._status = setMissileInitialPosition(&missile, &player, missileDirection);
					missile._alive = missile._status;
					playerFire = 0;
					//displayCharacter(&missile);
					DRAW(&missile);					
					checkMissileVsGhosts(&missile, ghosts);
					if(areCharctersAtSamePosition(&missile, &invincibleGhost))
						{
							//missile._status = 0; missile._alive = 0;
							die(&missile);
							restoreMissile(&missile);
							//displayCharacter(&invincibleGhost);
							DRAW(&invincibleGhost);
						}		
				}
				if(missile._status==1 && missile._alive==1)
				{
					moveMissile(&missile, missileDirection);
					// TODO: Inefficient
					checkMissileVsGhosts(&missile, ghosts);
					
					if(areCharctersAtSamePosition(&missile, &invincibleGhost))
					{
						die(&missile);
						restoreMissile(&missile);
						//displayCharacter(&invincibleGhost);
						DRAW(&invincibleGhost);
					}
				}
			
				chasePlayer(ghosts, &player, ghostSlowDown);
				
				if(playerReached(ghosts, &player) ||
				   playerReachedBombs(bombs, &player))
				{
					die(&player);
					printDefeatMessage();
					sleep(1);
				}
				
				if(innerWallReached(&player))
				{
					die(&player);
					printDefeatMessage();
					sleep(1);
				}
			
				checkBombsVsGhosts(bombs, 
								   ghosts);
				
				checkGhostsVsGhosts(ghosts);

				if(gun._status==1)
				{
					if(powerUpReached(&player, &gun))
					{
						guns = GUNS_NUMBER;
						points+=GUN_BONUS;
						gun._status = 0;	
						gunCoolDown = gunInitialCoolDown;
					}
					else
					{
						//displayCharacter(&gun);
						DRAW(&gun);
					}
				}		
				else if (gunCoolDown == 0)
				{
					
					gun._status = 1;
					do
					{
					relocateCharacter(&gun, bombs, ghosts);
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
						decreaseGhostLevel(); 
						points+=POWER_UP_BONUS;
						powerUp._status = 0;	
						powerUpCoolDown = powerUpInitialCoolDown;
					}
					else
					{
						//displayCharacter(&powerUp);
						DRAW(&powerUp);
					}		
				}
				else if (powerUpCoolDown == 0)
				{
					powerUp._status = 1;
					do
					{
					relocateCharacter(&powerUp, bombs, ghosts);
					} while(innerWallReached(&powerUp));
				}
				else
				{
					--powerUpCoolDown;
				}
					
				if(wallReached(&player))
				{
					die(&player);
					printDefeatMessage();
					sleep(1);
				}
				
				SET_TEXT_COLOR(COLOR_RED);
				for(i=0;i<BOMBS_NUMBER;++i)
				{
					//displayCharacter(bombs[i]);
					DRAW(bombs[i]);
				}
				SET_TEXT_COLOR(TEXT_COLOR);
				
				displayGhosts(ghosts);

				displayStatsTitles();
				displayStats();
				
				if(!invincibleGhost._status && 
				  ((invincibleXCountDown==0)||(invincibleYCountDown==0)) || 
				   (loop>=invincibleLoopTrigger) || (ghostCount<=invincibleGhostCountTrigger))
				{
					invincibleGhost._status = 1;
					//displayCharacter(&invincibleGhost);
					DRAW(&invincibleGhost);
				}
				else
				{
					--invincibleXCountDown;
					--invincibleYCountDown;
				}
				if(invincibleGhost._status)
				{ 
					chaseCharacter(&invincibleGhost, &player, invincibleSlowDown);
					if(areCharctersAtSamePosition(&invincibleGhost, &player))
					{
						die(&player);
						printDefeatMessage();
						sleep(1);
					}
				}
				
				if(ghostCount<=0)
				{
					printVictoryMessage();
					sleep(1);
				}
				
				// Add points to score
				points+=LOOP_POINTS;
				
				// Increase ghost speed
				++ghostLevel;
			}; // end inner while [while (player._alive && !victoryFlag)]
			if(player._alive)
			{
				CLEAR_SCREEN();
				printLevelBonus();
								
				sleep(1);
				
				points+= LEVEL_BONUS*level;
				++level;
				innerVerticalWallLength = drawInnerVerticalWallForLevel();
			}
			else
			{
				--lives;
				if(lives>0)
				{
					player._alive = 1;
				}
			}
		} while (player._alive || ((ghostCount<=0) && (level<(FINAL_LEVEL+1)))); // while game not ended (some lives and levels left)
			
	if(level==FINAL_LEVEL+1)
	{
		gameCompleted();
		sleep(1);
	}
	finalScore();
	// GAME OVER	
	printGameOver();
	sleep(1);
	CLEAR_SCREEN();
	}

	return EXIT_SUCCESS;
}

