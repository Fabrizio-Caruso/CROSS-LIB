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

#if defined(__CMOC__) && !defined(__WINCMOC__)
	#include <cmoc.h>
#else
	#include <stdlib.h>
	#include <string.h>
	#include <stdio.h>
#endif

#if !defined EXIT_SUCCESS
	#define EXIT_SUCCESS 0
#endif

// TODO: REMOVE THIS
//#define DEBUG_ITEMS

#include "settings.h"
#include "character.h"
#include "item.h"
#include "strategy.h"
#include "text.h"
#include "level.h"
#include "ghost.h"
#include "move_player.h"

#include "text_strings.h"

#if !defined(TINY_GAME)
	#include "bullet.h"
	#include "skull.h"
#endif
	
#include "end_screen.h"

#if defined(FULL_GAME)
	#include "horizontal_missile.h"
	#include "rocket.h"
#endif

	

unsigned short ghostSlowDown;

unsigned short ghostLevel;
unsigned short points;
unsigned short highScore;
unsigned char lives;
unsigned short loop;
unsigned char level;


#if !defined(TINY_GAME)
	unsigned short skullSlowDown;
	unsigned char skullXCountDown;
	unsigned char skullYCountDown;
	unsigned char bulletDirection;
#endif

// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. skullXCountDown (time needed to activate the skull ghost)
// 4. skullYCountDown
// 5. skullSlowDown (how much the skull ghost is slowed-down)
// 6. skullLoopTrigger (how long before the skull ghost appears)

#if !defined(TINY_GAME)
	extern Image SKULL_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image BULLET_IMAGE;
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;	
	extern Image DEAD_GHOST_IMAGE;
#endif


#if defined(FULL_GAME) && !defined(NO_BLINKING)
	unsigned char playerBlink;	
#endif

Character player; 

#if !defined(TINY_GAME)
	Character skull;
	Item powerUp;
	Item powerUp2;
	Item gun;
	Item extraPoints;	
	
	Character bullet;
#endif

Character ghosts[GHOSTS_NUMBER];
Character bombs[BOMBS_NUMBER];

#if defined(FULL_GAME)

	unsigned char innerVerticalWallY; 
	unsigned char innerVerticalWallX; 
	unsigned char innerVerticalWallLength;

	Item freeze;
	Item invincibility;
	
	Item extraLife;
	Item super;
	Item confuse;
	Item zombie;
	Item chase;
	 
	unsigned char extraLife_present_on_level;
	unsigned char super_present_on_level;
	unsigned char zombie_present_on_level;
	#define chase_present_on_level_condition skullsKilled
	#define confuse_present_on_level_condition missileBasesDestroyed

	Character leftEnemyMissile;
	Character rightEnemyMissile;

	Character rockets[ROCKETS_NUMBER];
	
	Character chasingBullet;

	Character * chasedEnemyPtr;

	unsigned char rockets_x[ROCKETS_NUMBER];
	
	unsigned char dead_rockets;
	
	unsigned char arrowRange;
	
	unsigned char skullsKilled;
	
	unsigned char missileBasesDestroyed;
	
	unsigned char horizontalWallsLength;
	
	unsigned char invincibilityActive;
	
	unsigned char confuseActive; 
	
	unsigned char zombieActive; 
		
	unsigned short invincibility_count_down;	
	
	unsigned short confuse_count_down;
	
	unsigned short zombie_count_down;
	
#endif

#if !defined(TINY_GAME)
	unsigned char strategyArray[GHOSTS_NUMBER];

	unsigned char playerDirection; // 0: right, 1: down, 2: left, 3: up
	
	unsigned char freezeActive; // freezeActive	

	unsigned short freeze_count_down;
		
	unsigned short extraLifeThroughPointsCounter;	
#endif

unsigned char ghostCount; // = GHOSTS_NUMBER;

#if !defined(TINY_GAME)
	unsigned char playerFire;
	unsigned char guns; // = GUNS_NUMBER;

	unsigned char skullHits;
	unsigned char skullAlive;
#endif


#if !defined(TINY_GAME)
	void resetItems()
	{
		gun._coolDown = GUN_COOL_DOWN;
		powerUp._coolDown = POWER_UP_COOL_DOWN;
		powerUp2._coolDown = POWER_UP2_COOL_DOWN;
		extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;		
		
		#if defined(FULL_GAME)
			freeze._coolDown = FREEZE_COOL_DOWN;				
			invincibility._coolDown = INVINCIBILITY_COOL_DOWN;

			chase._coolDown = CHASE_COOL_DOWN;
			
			super._coolDown = SUPER_COOL_DOWN;
			extraLife._coolDown = EXTRA_LIFE_COOL_DOWN;
			
			confuse._coolDown = CONFUSE_COOL_DOWN;
			zombie._coolDown = ZOMBIE_COOL_DOWN;				
		#endif
	}

	// Constructor for all items
	void constructItems()
	{
		powerUp._effect = &powerUpEffect;
		powerUp2._effect = &powerUp2Effect;
		gun._effect = &gunEffect;
		extraPoints._effect = &extraPointsEffect;
		#if defined(FULL_GAME)
			freeze._effect = &freezeEffect;
			extraLife._effect = &extraLifeEffect;
			invincibility._effect = &invincibilityEffect;
			super._effect = &superEffect;
			confuse._effect = &confuseEffect;
			zombie._effect = &zombieEffect;
			chase._effect = &chaseEffect;
		#endif	
	}	

#endif

#if !defined(NO_INITIAL_SCREEN)			
void initialScreen(void)
{	
	CLEAR_SCREEN();					
	printStartMessage();
	WAIT_PRESS();
	
	#if defined(FULL_GAME) && !defined(NO_HINTS)
		CLEAR_SCREEN();
		printHints();	
	#endif
}
#endif


#if defined(FULL_GAME)
	
	void handle_special_triggers(void)
	{
		// confuse_present_on_level_condition is defined as missileBasesDestroyed
		zombie_present_on_level = missileBasesDestroyed>=2;
		super_present_on_level = skullsKilled>=2;
		// chase_present_on_level_condition is defined as skullsKilled;
		extraLife_present_on_level = super_present_on_level && confuse_present_on_level_condition;
	}
	
#endif

#if defined(SLOW_DOWN)
	#  if defined(__NCURSES__)
		void _slow_down(void)
		{
			usleep(GAME_SLOW_DOWN*1024);
		}
	#else
	void _slow_down(void)
	{
		short i;
		for(i=0;i<GAME_SLOW_DOWN;++i)
		{	
		}
	}
	#endif
#endif

int main(void)
{		
	INIT_INPUT();

	INIT_GRAPHICS();

	// Ask for the screen size 
	// GET_SCREEN_SIZE(&XSize, &YSize);
	
	highScore = 0;
	
	while(1)
	{

		INIT_IMAGES();
		
		#if !defined(NO_SET_SCREEN_COLORS)
			// Set Screen Colors
			setScreenColors();				
		#endif
		
		#if !defined(NO_INITIAL_SCREEN)
			initialScreen();
			
			#if !defined(TINY_GAME)
				WAIT_PRESS();
				CLEAR_SCREEN();	
			#endif
		#else
			CLEAR_SCREEN();	
			
			printPressKeyToStart();				
		#endif

		#if !defined(LESS_TEXT) || defined(ALT_HIGHSCORE)
			highScoreScreen();
			WAIT_PRESS();	
		#endif
		// #if ((defined(TINY_GAME) || defined(FULL_GAME)) && !defined(LESS_TEXT)) || defined(ALT_HISCORE)
			// highScoreScreen();
			// WAIT_PRESS();
		// #endif
		#if !defined(TINY_GAME)

			CLEAR_SCREEN();
		#endif
		
		#if !defined(TINY_GAME)
			extraLifeThroughPointsCounter = 1;
		#endif
		points = 0;
		level = INITIAL_LEVEL; 	
		lives = LIVES_NUMBER;
		ghostCount = GHOSTS_NUMBER;
		#if defined(FULL_GAME)
			#if defined(DEBUG_ITEMS)
				missileBasesDestroyed = 2;
				skullsKilled = 2;
			#else
				missileBasesDestroyed = 0;
				skullsKilled = 0;			
			#endif			
		#endif
		
		
		do // Level (Re-)Start
		{ 	
			loop = 0;
			ghostLevel = 0;
		
			#if defined(FULL_GAME)
			
				dead_rockets = 0;
				
				invincibilityActive = 1;				
				invincibility_count_down = INITIAL_INVINCIBILITY_COUNT_DOWN;
				
				#if !defined(INITIAL_GHOST_FREEZE)
					freezeActive = 0;
				#endif
				confuseActive = 0;
				zombieActive = 0; 
				
				handle_special_triggers();
				
				#if !defined(SIMPLE_STRATEGY)
					computeStrategy();			
				#endif
								
			#endif			
			
			#if !defined(TINY_GAME)
				skullAlive = 1;
				skullHits = 0;						
				guns = 0;
				
				resetItems();
				
				#if defined(INITIAL_GHOST_FREEZE)
					freezeActive = 1;
					freeze_count_down = INITIAL_FROZEN_COUNT_DOWN;
				#endif
				
				computeSkullParameters();				
			#endif

			ghostSlowDown = computeGhostSlowDown();
			
			CLEAR_SCREEN();
			#if !defined(LESS_TEXT)
				// Clear the screen, put cursor in upper left corner

				printLevel();
				sleep(1);
				CLEAR_SCREEN();
			#endif
				
			
			#if defined(FULL_GAME)
						
				arrowRange = computeArrowRange();
			
				if(bossLevel())
				{
					printKillTheSkull();
					sleep(2);
				}
				CLEAR_SCREEN();
				
				updateInnerWallVerticalData();	
			#endif
			
			printPressKeyToStart();
			WAIT_PRESS();
			CLEAR_SCREEN();
			
			
			#if !defined(TINY_GAME) && !defined(NO_BORDERS)
				DRAW_BORDERS();
			#endif
			
			fillLevelWithCharacters(ghostCount);			
			
			// #if defined(TINY_GAME)
				// displayBombs();
			// #endif	
			
			#if !defined(TINY_GAME)
				constructItems();	
				
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
			while(player._status && ((ghostCount>0 && !bossLevel()) || (skullAlive && bossLevel()))) // while alive && there are still ghosts
			#else
			while(player._status && (ghostCount>0) )
			#endif
			{
				#if !defined(TURN_BASED)
					MOVE_PLAYER();				
					_DRAW_PLAYER();	
				#endif
				
				#if defined(SLOW_DOWN)
					_slow_down();
				#endif
						
				#if defined(FULL_GAME)
					handle_rockets();
					handle_enemy_missiles();
				#endif
				
				++loop;
				
				#if !defined(TINY_GAME)
				if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
				{
					++extraLifeThroughPointsCounter;
					PING_SOUND();
					++lives;
					printLivesStats();
				}
				#endif
				
				ghostSlowDown = computeGhostSlowDown();

				#if !defined(TINY_GAME)
					handle_bullet();
				#endif
				
				#if !defined(TINY_GAME)						

					if(!freezeActive)
					{
						#if defined(FULL_GAME)
							if(confuseActive && skullAlive && skull._status)
							{
								chaseCharacter(&skull, ghostSlowDown);
							}
							else
							{
								chaseCharacter(&player, ghostSlowDown);
							}
						#else
							chaseCharacter(ghostSlowDown);
						#endif
						++ghostLevel;				
					}
				
					handle_skull_ghost();

					// This detects collisions of ghosts that have just moved
					if(bullet._status)
					{
						checkBullet(&bullet);
					}
				#else
					#if !defined(NO_CHASE)
					chaseCharacter(ghostSlowDown);
					++ghostLevel;
					#endif
				#endif
				
				// Check collisions bombs vs ghosts
				checkBombsVsGhosts();
				
				#if !defined(TINY_GAME)
					handle_extraPoints_item();
					handle_gun_item();
					handle_powerup_item();
					handle_powerup2_item();	
					handle_freeze_count_down();					
				#endif
				
				#if defined(FULL_GAME)
					handle_freeze_item();	
					handle_invincibility_item();
					handle_invincibility_count_down();					

					if(chase_present_on_level_condition)
					{
						handle_chase_item();
						handle_chasing_bullet();						
						if(super_present_on_level)
						{
							handle_super_item();
							if(extraLife_present_on_level)
							{
								handle_extraLife_item();
							}
						}
					}
										
					if(confuse_present_on_level_condition)
					{
						handle_confuse_item();
						handle_confuse_count_down();
						if(zombie_present_on_level)
						{
							handle_zombie_item();
							handle_zombie_count_down();	
							if(zombieActive && !(loop&15))
							{
								points+=ZOMBIE_BONUS;
								displayStats();
								reducePowerUpsCoolDowns();
							}
						}
					}
					
				#endif
				
				#if defined(FULL_GAME)
					if(wallReached(&player) || 
					   (!invincibilityActive && (playerReached(&player) || playerReachedBombs(&player) || innerWallReached(&player) || (horizontalWallsLevel() && horizontalWallsReached(&player))))
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
						SET_TEXT_COLOR(WALL_COLOR);
						DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);			
						displayBombs();	
				
						if(horizontalWallsLevel())
						{				
							horizontalWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + (level>>4) + (unsigned char) (loop/HORIZONTAL_WALLS_INCREASE_LOOP);		
							DRAW_HORIZONTAL_WALLS(horizontalWallsLength);	
						}						
					}
										
				#else
					// #if !defined(TINY_GAME)
					SKIP_MORE_DRAW
						displayBombs();
					// #endif
				#endif
				
				// Display ghosts
				SKIP_DRAW
					displayGhosts();
					
				#if defined(TURN_BASED) 
					if((loop<TURN_BASED_MAX_LOOP) || loop&1)
					{
						MOVE_PLAYER();
					}					
					_DRAW_PLAYER();	
				#endif				
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

			if(player._status) // if level finished
			{
				#if defined(BETWEEN_LEVEL)
					chasedEnemyPtr = &player;
					SHOW_DOWN();
				#endif
				
				#if !defined(TINY_GAME)
					#if !defined(LESS_TEXT)
						sleep(1);
						printVictoryMessage();
						sleep(2);

						CLEAR_SCREEN();
					#endif

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
				#else
					points += LEVEL_BONUS * 4;
				#endif			

				ghostCount = GHOSTS_NUMBER;
				
				#if defined(FULL_GAME)			
					if(bossLevel())
					{	
						CLEAR_SCREEN();
						sleep(1);
						PING_SOUND();
						#if !defined(LESS_TEXT)
							printExtraLife();
						#endif
						sleep(2);
						++lives;
						skullsKilled = 1;
						missileBasesDestroyed/=2;
					}
					else
					{
						if(!skullAlive)
						{
							++skullsKilled;
						}
						missileBasesDestroyed+=dead_rockets;
					}
				#endif
				++level;

			}
			else // if dead
			{		
				#if defined(BETWEEN_LEVEL)
					for(bulletDirection=0;bulletDirection<80;++bulletDirection)
					{
						unsigned char i;
						for(i=0;i<GHOSTS_NUMBER;++i)
						{
							if(ghosts[i]._status)
								dance(&ghosts[i]);
						}
					}
					chasedEnemyPtr = &skull;					
				#endif
				
				#if !defined(TINY_GAME)
					CLEAR_SCREEN();
				#endif
				--lives;
				if(lives>0)
				{
					player._status = 1;
				}
			}
			#if defined(BETWEEN_LEVEL)
				spiral(chasedEnemyPtr, 2*MIN_SIZE-18);
				sleep(1);
			#endif				
		} while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game 
			
	if(level==FINAL_LEVEL+1) // if completed game
	{
		gameCompleted();
		#if !defined(NO_SLEEP)
			sleep(2);
		#else
			CLEAR_SCREEN();
			WAIT_PRESS();
		#endif
	}

	// GAME OVER	
	printGameOver();

	sleep(2);
	
	#if !defined(TINY_GAME)
		CLEAR_SCREEN();
	
		finalScore();
	#endif
	
	#if defined(NO_SLEEP)
		WAIT_PRESS();
	#endif
	
	sleep(2);
	if(points>highScore)
	{
		highScore = points;
	}
	CLEAR_SCREEN();
	} // while(1) -> restart from the beginning

	return EXIT_SUCCESS;
}
