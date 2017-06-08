
/*
** Portable barely playable chasing game
**
** Fabrizio Caruso (fabrizio_caruso@hotmail.com)
**
*/
 
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include <unistd.h>

#include <time.h>
#include <joystick.h>

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

#include "display_macros.h"

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

unsigned char XSize;
unsigned char YSize;

unsigned int loop;

int main(void)
{
	#ifndef __PLUS4__
	char kbInput;
	#endif // __PLUS4__
	
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
		
		CLEAR_SCREEN();
					
		printStartMessage();
		while(!kbhit() 				
		#ifndef __PLUS4__	
		&& !joy_read(JOY_1)
		#endif //__PLUS4__
		)
		{}
		CLEAR_SCREEN();
				
		deleteCenteredMessage();
		
		lives = 3;
		do // Level (Re-)Start
		{ 
			loop = 0;
			ghostLevel = 1u;
			ghostSmartness = computeGhostSmartness();
			
			computePowerUp(&ghostLevelDecrease, &powerUpInitialCoolDown);
			
			gunInitialCoolDown = computeGunInitialCoolDown();
			invincibleXCountDown = computeInvincibleCountDown();
			invincibleYCountDown = computeInvincibleCountDown();
			invincibleSlowDown = computeInvincibleSlowDown();
			invincibleGhostCountTrigger = computeInvincibleGhostCountTrigger();
			invincibleLoopTrigger = computeInvincibleLoopTrigger();
			ghostCount = 8;
			guns = 0;
			gun._status = 0;
			gunCoolDown = gunInitialCoolDown;
			
			/* Clear the screen, put cursor in upper left corner */
			CLEAR_SCREEN();

			printLevel();
			sleep(2);
			
			/* Wait for the user to press a key */
			printPressKeyToStart();
			while(!kbhit() 				
			#ifndef __PLUS4__	
			&& !joy_read(JOY_1)
			#endif //__PLUS4__
			)
			{}
			deleteCenteredMessage();
			
			/* Draw a border around the screen */
			drawBorders();
			
			// Initialize characters
			innerVerticalWallLength = drawInnerVerticalWallForLevel();
			fillLevelWithCharacters(
								 &player, &powerUp, 
								 &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								 &ghost_5, &ghost_6, &ghost_7, &ghost_8, 
								 &bomb_1, &bomb_2, &bomb_3, &bomb_4, 
								 &invincibleGhost, &missile, &gun);	
			ghostCount = 8;
			
			displayStatsTitles();
			
			while(player._alive && ghostCount>0) // while alive && there are still ghosts
			{
				ghostSlowDown = computeGhostSlowDown();
				invincibleSlowDown = computeInvincibleSlowDown();
				drawInnerVerticalWall();
				
				++loop;

#ifndef __PLUS4__
				if(kbhit())
				{		
					kbInput = cgetc();
					movePlayer(&player, kbInput);
				}
#endif // __PLUS4__
				//else
				{
					joyInput = joy_read (JOY_1);
					
					movePlayerByJoystick(&player, joyInput);	
				}
				if(playerFire && missile._status==0 && guns>0)
				{
					--guns;
					missileDirection = playerDirection;
					missile._status = setMissileInitialPosition(&missile, &player, missileDirection);
					missile._alive = missile._status;
					playerFire = 0;
					displayCharacter(&missile);					
					checkMissileVsGhosts(&missile, 
						&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						&ghost_5, &ghost_6, &ghost_7, &ghost_8);
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
					moveMissile(&missile, missileDirection);
					// TODO: Inefficient
					checkMissileVsGhosts(&missile, 
						&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
						&ghost_5, &ghost_6, &ghost_7, &ghost_8);
					
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
							ghostSmartness, ghostSlowDown);
					
				
				if(playerReached(&ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								 &ghost_5, &ghost_6, &ghost_7, &ghost_8, &player) ||
				   playerReachedBombs(&bomb_1, &bomb_2, &bomb_3, &bomb_4, &player))
				{
					die(&player);
					defeat();
					sleep(1);
				}
				
				if(innerWallReached(&player))
				{
					die(&player);
					defeat();
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
						SET_TEXT_COLOR(GUN_COLOR);
						displayCharacter(&gun);
						SET_TEXT_COLOR(TEXT_COLOR);
					}
				}		
				else if (gunCoolDown == 0)
				{
					
					gun._status = 1;
					do
					{
					relocateCharacter(&gun, &bomb_1, &bomb_2, &bomb_3, &bomb_4, 
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
						decreaseGhostLevel(); 
						points+=POWER_UP_BONUS;
						powerUp._status = 0;	
						powerUpCoolDown = powerUpInitialCoolDown;
					}
					else
					{
						SET_TEXT_COLOR(POWER_UP_COLOR);
						displayCharacter(&powerUp);
						SET_TEXT_COLOR(TEXT_COLOR);
					}		
				}
				else if (powerUpCoolDown == 0)
				{
					powerUp._status = 1;
					do
					{
					relocateCharacter(&powerUp, &bomb_1, &bomb_2, &bomb_3, &bomb_4, 
								   &ghost_1, &ghost_2, &ghost_3, &ghost_4, 
								   &ghost_5, &ghost_6, &ghost_7, &ghost_8);
					} while(innerWallReached(&powerUp));
				}
				else
				{
					--powerUpCoolDown;
				}
					
				if(wallReached(&player))
				{
					die(&player);
					defeat();
					sleep(1);
				}
				
				SET_TEXT_COLOR(COLOR_RED);
				displayCharacter(&bomb_1);
				displayCharacter(&bomb_2);		
				displayCharacter(&bomb_3);
				displayCharacter(&bomb_4);
				SET_TEXT_COLOR(TEXT_COLOR);
				
				if(guns>0 || invincibleGhost._status)
				{
					displayDeadGhosts(&ghost_1, &ghost_2, &ghost_3, &ghost_4, &ghost_5, &ghost_6, &ghost_7, &ghost_8);
				}
				
				displayStatsTitles();
				displayStats();
				
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
					blindChaseCharacterMaxStrategy(&invincibleGhost, &player);
					if(areCharctersAtSamePosition(&invincibleGhost, &player))
					{
						die(&player);
						defeat();
						sleep(1);
					}
				}
				
				if(ghostCount<=0)
				{
					victory();
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

