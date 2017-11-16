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
#include "level.h"
#include "enemy.h"

#if !defined(TINY_GAME)
	#include "missile.h"
	#include "invincible_enemy.h"
#endif
	
// Input input/output driver headers
#include "display_macros.h"
#include "input_macros.h"
#include "sleep_macros.h"

#include "sound_macros.h"

unsigned short ghostSlowDown;

unsigned short ghostLevel;
unsigned int points;
unsigned int highScore;
unsigned char lives;
unsigned char XSize;
unsigned char YSize;
unsigned short loop;
unsigned char level;

#if !defined(TINY_GAME)
unsigned short invincibleSlowDown;
unsigned char invincibleXCountDown;
unsigned char invincibleYCountDown;
unsigned char powerUpCoolDown;	
unsigned char gunCoolDown;
unsigned short ghostLevelDecrease;
unsigned char missileDirection;
#endif

// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. invincibleXCountDown (time needed to activate the invincible ghost)
// 4. invincibleYCountDown
// 5. invincibleSlowDown (how much the invincible ghost is slowed-down)
// 6. invincibleLoopTrigger (how long before the invincible ghost appears)



extern Image PLAYER_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;

#if !defined(TINY_GAME)
unsigned short invincibleLoopTrigger;
extern Image INVINCIBLE_GHOST_IMAGE;
extern Image POWERUP_IMAGE;
extern Image MISSILE_IMAGE;
extern Image GUN_IMAGE;
extern Image DEAD_GHOST_IMAGE;
#endif

#if !defined(TINY_GAME)
extern unsigned char powerUpBlink;
extern unsigned char gunBlink;	
#endif

#if defined(FULL_GAME)
extern unsigned char extraPointsBlink;
extern unsigned char extraLifeBlink;	
extern unsigned char invincibilityBlink;
extern unsigned char playerBlink;	
#endif

#if defined(DEBUG) && defined(FULL_GAME)
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;
	extern Image BUBBLE_IMAGE;
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;	
#endif

Character player; 

#if !defined(TINY_GAME)
Character invincibleGhost;
Character powerUp;
Character gun;
Character missile;
#endif

Character ghosts[GHOSTS_NUMBER];
Character bombs[BOMBS_NUMBER];

#if defined(FULL_GAME)
	unsigned short invincibleGhostCountTrigger = INVINCIBLE_GHOST_TRIGGER;

	unsigned char innerVerticalWallY; 
	unsigned char innerVerticalWallX; 
	unsigned char innerVerticalWallLength;
	
	Character extraPoints;
	Character extraLife;
	Character invincibility;
	
	unsigned short extraPointsCoolDown;
	unsigned short extraLifeCoolDown;
	unsigned short invincibilityCoolDown;

	unsigned short playerInvincibilityCoolDown;	

	Character leftEnemyMissile;
	Character rightEnemyMissile;

	Character bubbles[BUBBLES_NUMBER];

	unsigned char bubbles_x[BUBBLES_NUMBER];
	
	unsigned char player_invincibility = 0;

	unsigned char dead_bubbles = 0;
	
	unsigned char arrowRange;
	
#endif

unsigned char strategyArray[GHOSTS_NUMBER];


unsigned char playerDirection = 0; // 0: right, 1: down, 2: left, 3: up

unsigned int extraLifeThroughPointsCounter = 1;

unsigned char ghostCount = GHOSTS_NUMBER;

#if !defined(TINY_GAME)
unsigned char playerFire = 0;
unsigned char guns = GUNS_NUMBER;


unsigned char invincibleGhostHits = 0;
unsigned char invincibleGhostAlive = 1;
#endif

// TODO: It should not be here
// unsigned char powerUp_blink = 1;
// unsigned char gun_blink = 1;	

#if !defined(TINY_GAME)
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
			missile._x = 0; missile._y = 0;
			++invincibleGhostHits;
			decreaseGhostLevel();
			#if defined(FULL_GAME)
				reducePowerUpsCoolDowns();
			#endif
			
			// TODO: to TEST
			if(invincibleGhostHits>=MIN_INVINCIBLE_GHOST_HITS)
			{
				invincibleGhost._status = 0;
				DELETE_INVINCIBLE_GHOST(invincibleGhost._x,invincibleGhost._y, invincibleGhost._imagePtr);
				invincibleGhost._x=XSize-2; invincibleGhost._y=YSize-2;
				invincibleGhostAlive = 0;
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


void powerUpReached(Character * powerUpPtr)
{
	ZAP_SOUND();
	DELETE_GUN(powerUpPtr->_x,powerUpPtr->_y,powerUpPtr->_imagePtr);
	DRAW_PLAYER(player._x, player._y, player._imagePtr);
	powerUpPtr->_status = 0;
	displayStats();
}

void relocatePowerUp(Character * powerUpPtr)
{
		powerUpPtr->_status = 1;
		
		#if defined(FULL_GAME)
		do
		{
			relocateCharacter(powerUpPtr, bombs,4);
		} while(nearInnerWall(powerUpPtr));		
		#else
			relocateCharacter(powerUpPtr, bombs,4);
		#endif	
}


void handle_gun_item()
{
	// Manage gun 
	if(gun._status==1)
	{
		if(areCharctersAtSamePosition(&player, &gun))
		{
			guns = GUNS_NUMBER;
			printGunsStats();		
			points+=GUN_BONUS;			
			powerUpReached(&gun);	
			gunCoolDown = GUN_INITIAL_COOLDOWN;
		}
		else
		{
			DRAW_GUN(gun._x, gun._y, gun._imagePtr);
		}
	}		
	else if (gunCoolDown == 0)
	{	
		relocatePowerUp(&gun);
		
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
			points+=POWER_UP_BONUS;
			powerUpReached(&powerUp);
			decreaseGhostLevel(); 
			powerUpCoolDown = POWER_UP_INITIAL_COOLDOWN;
		}
		else
		{
			DRAW_POWERUP(powerUp._x,powerUp._y,powerUp._imagePtr);
		}		
	}
	else if (powerUpCoolDown == 0)
	{
		relocatePowerUp(&powerUp);

		DRAW_POWERUP(powerUp._x,powerUp._y,powerUp._imagePtr);
	}
	else
	{
		--powerUpCoolDown;
	}
}
#endif

void playerDies(void)
{
	EXPLOSION_SOUND();
	die(&player);
	printDefeatMessage();
	sleep(1);	
}

#if defined(FULL_GAME)
	unsigned char computeArrowRange(void)
	{
			return level/10;
	}

	void handle_extraPoints_item()
	{
		// Manage gun 
		if(extraPoints._status==1)
		{
			if(areCharctersAtSamePosition(&player, &extraPoints))
			{
				points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
				powerUpReached(&extraPoints);	
				extraPointsCoolDown = EXTRA_POINTS_COOL_DOWN*2; // second time is harder
			}
			else
			{
				DRAW_EXTRA_POINTS(extraPoints._x, extraPoints._y, extraPoints._imagePtr);
			}
		}		
		else if (extraPointsCoolDown == 0)
		{	
			relocatePowerUp(&extraPoints);
			
			DRAW_EXTRA_POINTS(extraPoints._x, extraPoints._y, extraPoints._imagePtr);
		}
		else
		{
			--extraPointsCoolDown;
		}				
	}


	void handle_extraLife_item()
	{
		// Manage gun 
		if(extraLife._status==1)
		{
			if(areCharctersAtSamePosition(&player, &extraLife))
			{
				powerUpReached(&extraLife);
				++lives;
				extraLifeCoolDown = EXTRA_LIFE_COOL_DOWN*2; // second time is harder
				printLivesStats();
			}
			else
			{
				DRAW_EXTRA_LIFE(extraLife._x, extraLife._y, extraLife._imagePtr);
			}
		}		
		else if (extraLifeCoolDown == 0)
		{	
			relocatePowerUp(&extraLife);
		
			DRAW_EXTRA_LIFE(extraLife._x, extraLife._y, extraLife._imagePtr);
		}
		else
		{
			--extraLifeCoolDown;
		}				
	}


	void handle_invincibility_item()
	{
		// Manage gun 
		if(invincibility._status==1)
		{
			if(areCharctersAtSamePosition(&player, &invincibility))
			{
				powerUpReached(&invincibility);
				player_invincibility = 1;
				invincibilityCoolDown = INVINCIBILITY_COOL_DOWN;
				playerInvincibilityCoolDown = PLAYER_INVINCIBILITY_COOL_DOWN;
			}
			else
			{
				DRAW_INVINCIBILITY(invincibility._x, invincibility._y, invincibility._imagePtr);
			}
		}		
		else if (invincibilityCoolDown == 0)
		{	
			relocatePowerUp(&invincibility);
			
			DRAW_INVINCIBILITY(invincibility._x, invincibility._y, invincibility._imagePtr);
		}
		else
		{
			--invincibilityCoolDown;
		}				
	}
#endif


#if !defined(TINY_GAME)
void handle_invincible_ghost(void)
{
	if(!invincibleGhost._status)
	{
		// Manage invincible ghost
		
		#if defined(FULL_GAME)
		if((!bossLevel() && invincibleGhostAlive &&
							((invincibleXCountDown==0)|| (invincibleYCountDown==0) || (loop>=invincibleLoopTrigger) || (ghostCount<=invincibleGhostCountTrigger))) || 
		   (bossLevel() && loop>=invincibleLoopTrigger))
		#elif defined(TINY_GAME)
		if(loop>=invincibleLoopTrigger) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER))
		#else
		if(invincibleGhostAlive &&
							((invincibleXCountDown==0)     || (invincibleYCountDown==0) || 
							 (loop>=invincibleLoopTrigger) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER)))
		#endif
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
			moveTowardCharacter(&invincibleGhost, 4);
		}
		DRAW_INVINCIBLE_GHOST(invincibleGhost._x, invincibleGhost._y, invincibleGhost._imagePtr);
		#if defined(FULL_GAME)
		if(!player_invincibility && areCharctersAtSamePosition(&invincibleGhost, &player))
		#else
		if(areCharctersAtSamePosition(&invincibleGhost, &player))
		#endif
		{
			playerDies();
		}
	}
}
#endif

#if !defined(NO_INITIAL_SCREEN)			
void initialScreen(void)
{
	// Set Screen Colors
	setScreenColors();			
	CLEAR_SCREEN();					
	printStartMessage();
	
	#if defined(FULL_GAME)
		WAIT_PRESS();
		CLEAR_SCREEN();
		printHints();	
	#endif
}
#endif

#if defined(DEBUG)
void DEBUG_PRINT()
{
	{
		// unsigned ch;
		
		unsigned char i;
		
		CLEAR_SCREEN();			
		// for(i=0;i<8;++i)
		// {
			// ch = cgetc();
			// printf("%d\n", ch);
		// }
		
		for(i=32;i<255;++i)
		{
			printf(" %d: %c", i, i);
		}
	}
		
	// PRINTF(1,0,"ghost %c\n", GHOST_IMAGE._imageData);
	// PRINTF(1,1,"invincible %c\n", INVINCIBLE_GHOST_IMAGE._imageData);
	// PRINTF(1,2,"bomb %c\n", BOMB_IMAGE._imageData);
	// PRINTF(1,3,"player %c\n", PLAYER_IMAGE._imageData);
	// #if defined(REDEFINED_CHARS)
		// PRINTF(1,4,"player %c\n", PLAYER_UP._imageData);
		// PRINTF(1,5,"player %c\n", PLAYER_DOWN._imageData);
		// PRINTF(1,6,"player %c\n", PLAYER_LEFT._imageData);
		// PRINTF(1,7,"player %c\n", PLAYER_RIGHT._imageData);					
	// #endif
	// #if defined(FULL_GAME)
		// PRINTF(1,8,"extra life %c\n", EXTRA_LIFE_IMAGE._imageData); 
		// PRINTF(1,9,"invincibility %c\n", INVINCIBILITY_IMAGE._imageData); 
		// PRINTF(1,10,"extra points %c\n", EXTRA_POINTS_IMAGE._imageData);
		// PRINTF(1,11,"bubble %c\n", BUBBLE_IMAGE._imageData);		
		// PRINTF(1,12,"left missile %c\n", LEFT_ENEMY_MISSILE_IMAGE._imageData);
		// PRINTF(1,13,"right missile %c\n", RIGHT_ENEMY_MISSILE_IMAGE._imageData);					
	// #endif
	// PRINTF(1,15,"powerup %c\n", POWERUP_IMAGE._imageData);
	// PRINTF(1,16,"gun %c\n", GUN_IMAGE._imageData); 
	// PRINTF(1,17,"missile %c\n", MISSILE_IMAGE._imageData); 
	// PRINTF(1,18,"dead ghost %c\n", DEAD_GHOST_IMAGE._imageData); 
	
	// #if defined(__MSX__)
		// msx_vpoke(6144+32,EXTRA_LIFE_IMAGE._imageData);			
		// msx_vpoke(6145+32,INVINCIBILITY_IMAGE._imageData);
		// msx_vpoke(6146+32,GHOST_IMAGE._imageData);
		// msx_vpoke(6147+32, MISSILE_IMAGE._imageData);
		// msx_vpoke(6148+32, DEAD_GHOST_IMAGE._imageData);
	// #endif
	
	// WAIT_PRESS();
	// CLEAR_SCREEN();	
}
#endif

int main(void)
{		
	INIT_INPUT();
	INIT_GRAPHICS();
	
	// Ask for the screen size 
	GET_SCREEN_SIZE(&XSize, &YSize);
	
	highScore = 0;
	
	while(1)
	{

		INIT_IMAGES();
		
		#if defined(DEBUG)
			DEBUG_PRINT();
		#endif
		
		#if !defined(NO_INITIAL_SCREEN)
		initialScreen();
		WAIT_PRESS();
		#endif
		CLEAR_SCREEN();


		#if !defined(TINY_GAME)
		highScoreScreen();

		WAIT_PRESS();
		CLEAR_SCREEN();
		#endif
		
		extraLifeThroughPointsCounter = 1;
		points = 0;
		level = INITIAL_LEVEL; 	
		lives = LIVES_NUMBER;
		ghostCount = GHOSTS_NUMBER;
		do // Level (Re-)Start
		{ 

			#if defined(FULL_GAME)
				dead_bubbles = 0;
				extraPointsCoolDown = EXTRA_POINTS_COOL_DOWN;
				extraLifeCoolDown = EXTRA_LIFE_COOL_DOWN;
				invincibilityCoolDown = INVINCIBILITY_COOL_DOWN;
				player_invincibility = 0;
			#endif			
						
			
			computeStrategy();
			
			loop = 0;
			ghostLevel = 0;

			#if !defined(TINY_GAME)
			ghostLevelDecrease = 140-(level*2);
			

			invincibleGhostAlive = 1;
			invincibleGhostHits = 0;
									
			guns = 0;
			gun._status = 0;
						
			gunCoolDown = GUN_INITIAL_COOLDOWN;
			
			computeInvincibleGhostParameters();
			#endif

			ghostSlowDown = computeGhostSlowDown();
			
			#if !defined(TINY_GAME)
			// Clear the screen, put cursor in upper left corner
			CLEAR_SCREEN();

			printLevel();
			sleep(1);
			#endif
			
			#if defined(FULL_GAME)
						
				arrowRange = computeArrowRange();
			
				if(bossLevel())
				{
					CLEAR_SCREEN();
					printKillTheSkull();
					sleep(1);
				}
			CLEAR_SCREEN();
			
			updateInnerWallVerticalData();	
			#endif
			
			// Wait for the user to press a key 
			printPressKeyToStart();
			WAIT_PRESS();

			deleteCenteredMessage();

			#if !defined(TINY_GAME)
			DRAW_BORDERS();

			#endif
			
			fillLevelWithCharacters(ghostCount);				

			#if !defined(TINY_GAME)
			displayStatsTitles();
			#endif
			displayStats();			
			printLevelStats();
			printLivesStats();
			//
			#if !defined(TINY_GAME)
			printGunsStats();
			printGhostCountStats();
			#endif

			#if defined(FULL_GAME)
			while(player._status && ((ghostCount>0 && !bossLevel()) || (invincibleGhostAlive && bossLevel()))) // while alive && there are still ghosts
			#else
			while(player._status && (ghostCount>0) )
			#endif
			{
				#if defined(SLOW_DOWN)
				{
					unsigned short i;
					for(i=0;i<GAME_SLOW_DOWN;++i)
					{
					}
				}
				#endif
				
				#if defined(FULL_GAME)
					if(player_invincibility && playerInvincibilityCoolDown<=0)
					{
						player_invincibility = 0;
					}
					else
					{
						--playerInvincibilityCoolDown;
					}
					
					if((ghostCount<=MAX_GHOST_COUNT_FOR_BUBBLES && rocketLevel()) || bossLevel())
					{ 
						unsigned char i;

						for(i=0;i<BUBBLES_NUMBER;++i)
						{
							if(bubbles[i]._status)
							{
								if(!player_invincibility && areCharctersAtSamePosition(&player,&bubbles[i]))
								{
									playerDies();
								}
								
								if(rand()%2)
								{
									DELETE_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);					
									--(bubbles[i]._y);
								}

								DRAW_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);			
								if(bubbles[i]._y<=1)
								{	
									DELETE_MISSILE(bubbles[i]._x, bubbles[i]._y, bubbles[i]._imagePtr);
									//
									bubbles[i]._x = (i+1)*(XSize/(BUBBLES_NUMBER+1));
									bubbles[i]._y = YSize-2;							
								}
							}
						}
					}

					
					if(missileLevel() || bossLevel() || oneMissileLevel())
					{
						if(!oneMissileLevel() && leftEnemyMissile._status)
						{
							DELETE_MISSILE(leftEnemyMissile._x,leftEnemyMissile._y,leftEnemyMissile._imagePtr);
							if(leftEnemyMissile._x==XSize-2)
							{
								leftEnemyMissile._x=0;
								leftEnemyMissile._y = YSize-1-ENEMY_MISSILE_OFFSET;
							}
							else
							{
								ADVANCED_LEFT_MISSILE();
								if(loop%2 && player._y>=YSize-1-ENEMY_MISSILE_OFFSET-arrowRange && player._x>=leftEnemyMissile._x)
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
							if(!player_invincibility && areCharctersAtSamePosition(&leftEnemyMissile,&player))
							{
								playerDies();
							}
						}
						
						if(rightEnemyMissile._status)
						{
							DELETE_MISSILE(rightEnemyMissile._x,rightEnemyMissile._y,rightEnemyMissile._imagePtr);
							if(rightEnemyMissile._x==1)
							{
								rightEnemyMissile._x= XSize-1;
								if(oneMissileLevel())
								{
									rightEnemyMissile._y = YSize/2;					
								}
								else
								{
									rightEnemyMissile._y = ENEMY_MISSILE_OFFSET;
								}
							}
							else
							{
								ADVANCED_RIGHT_MISSILE();
								if((!oneMissileLevel() && loop%2 && player._y<=ENEMY_MISSILE_OFFSET+arrowRange && player._x<= rightEnemyMissile._x) ||
								    (oneMissileLevel() && loop%2 && player._y<=YSize/2+arrowRange && player._y>=YSize/2-arrowRange && player._x<= rightEnemyMissile._x))
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
							if(!player_invincibility && areCharctersAtSamePosition(&rightEnemyMissile,&player))
							{
								playerDies();
							}
						}
					}	
					
					handle_extraPoints_item();
					
					if (level>=EXTRA_LIFE_FIRST_LEVEL && rocketLevel())
					{
						handle_invincibility_item();
						handle_extraLife_item();
					}
					else if(level>=INVINCIBILITY_FIRST_LEVEL)
					{
						handle_invincibility_item();
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
	
				#if !defined(TINY_GAME)
				handle_missile();
				#endif
				
				chasePlayer(ghostSlowDown);
				
				#if !defined(TINY_GAME)
				// This detects collisions of ghosts that have just moved
				if(missile._status)
				{
					checkMissileVsGhosts(&missile);
				}
				#endif
				
				// Check collisions bombs vs ghosts
				checkBombsVsGhosts();
				
				#if !defined(TINY_GAME)
				handle_gun_item();
				
				handle_powerup_item();
				#endif

				#if defined(FULL_GAME)
				if(wallReached(&player) || 
				   (!player_invincibility && (playerReached(&player) || playerReachedBombs(&player) || innerWallReached(&player)))
				  )
				#else
				if(wallReached(&player) || playerReached(&player) || playerReachedBombs(&player))
				#endif
				{
					playerDies();
				}
			
				#if defined(FULL_GAME)
					SKIP_MORE_DRAW
					{
						DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);			
						DRAW_BOMBS();	
					}
				#else
					SKIP_MORE_DRAW
						DRAW_BOMBS();
				#endif
				// Display ghosts
				SKIP_DRAW
					displayGhosts();

				#if !defined(TINY_GAME)
				handle_invincible_ghost();
				#endif
				
				++ghostLevel;
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

			if(player._status) // if level finished
			{
				#if !defined(TINY_GAME)
				sleep(1);
				printVictoryMessage();
				sleep(2);
				#endif
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
				
				#if defined(FULL_GAME)
					if(bossLevel())
					{	
						CLEAR_SCREEN();
						sleep(1);
						PING_SOUND();
						printExtraLife();
						sleep(2);
						++lives;
					}
				#endif
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
	#if !defined(TINY_GAME)
	// GAME OVER	
	printGameOver();
	sleep(2);
	#endif
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
