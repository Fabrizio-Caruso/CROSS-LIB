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
// #define DEBUG_ITEMS
// #define DEBUG_END

#include "settings.h"

#include "character.h"
#include "item.h"
#include "strategy.h"
#include "game_text.h"
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

#include "variables.h"


// Level
// The level affects:
// 1. powerUpCoolDown (how long before a new powerUp is spawned)
// 2. ghostSlowDown (how much the power up slows the enemies down)
// 3. skullXCountDown (time needed to activate the skull ghost)
// 4. skullYCountDown
// 5. skullSlowDown (how much the skull ghost is slowed-down)
// 6. skullLoopTrigger (how long before the skull ghost appears)



#if !defined(TINY_GAME)
	void resetItems(void)
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
	void constructItems(void)
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
		zombie_present_on_level = missileBasesDestroyed>=MISSILES_FOR_ZOMBIE;
		super_present_on_level = skullsKilled>=SKULLS_FOR_SUPER;
		// chase_present_on_level_condition is defined as skullsKilled;
		extraLife_present_on_level = super_present_on_level && zombie_present_on_level;
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
	INIT_GRAPHICS();

	INIT_INPUT();

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
				missileBasesDestroyed = 99;
				skullsKilled = 99;
			#else
				missileBasesDestroyed = 0;
				skullsKilled = 0;			
			#endif			
		#endif
		
		
		do // Level (Re-)Start
		{ 	
			#if !defined(TINY_GAME) || defined(TURN_BASED)
			loop = 0;
			#endif
			
			#if !defined(TINY_GAME)
			ghostLevel = 0;
			#endif
		
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
				
				#if defined(FULL_GAME)
					skullHits = (!bossLevel())<<2;
				#else
					skullHits = 0;
				#endif
				guns = 0;
				
				resetItems();
				
				#if defined(INITIAL_GHOST_FREEZE)
					freezeActive = 1;
					freeze_count_down = INITIAL_FROZEN_COUNT_DOWN;
				#endif
				
				computeSkullParameters();				
			#endif

			#if defined(TINY_GAME)
				ghostSlowDown = INITIAL_GHOST_SLOWDOWN-(unsigned short)level*256;
			#else
				ghostSlowDown = computeGhostSlowDown();
			#endif
			
			CLEAR_SCREEN();
			#if !defined(LESS_TEXT)
				// Clear the screen, put cursor in upper left corner

				printLevel();
				SLEEP(1);
				CLEAR_SCREEN();
			#endif
				
			
			#if defined(FULL_GAME)
						
				arrowRange = computeArrowRange();
			
				if(bossLevel())
				{
					printKillTheSkull();
					SLEEP(2);
					ghostCount = 1+(level>>2);
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
			
			#if !defined(TINY_GAME)
				constructItems();	
				
				displayStatsTitles();
			#endif
			
			displayStats();		
			#if !defined(NO_STATS)	
				printLevelStats();
				printLivesStats();
			#endif
			
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
				
				#if defined(DEBUG_END)
					gameCompleted();
				#endif
					
				#if !defined(TURN_BASED)
					MOVE_PLAYER();				
					_DRAW_PLAYER();	
				#endif
				
				#if defined(SLOW_DOWN)
					_slow_down();
				#endif
						
				#if defined(FULL_GAME)
					handle_rockets();
					handle_horizontal_missiles();
				#endif
				
				#if !defined(TINY_GAME) || defined(TURN_BASED)
				++loop;
				#endif
				
				#if !defined(TINY_GAME)
				if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
				{
					++extraLifeThroughPointsCounter;
					PING_SOUND();
					++lives;
					printLivesStats();
				}
				#endif
				
				#if defined(TINY_GAME)
					if(ghostSlowDown) 
					{
						--ghostSlowDown;
					}
				#else
					ghostSlowDown = computeGhostSlowDown();
				#endif
			
				#if !defined(TINY_GAME)
					handle_bullet();
				#endif
				
				#if !defined(TINY_GAME)						

					if(!freezeActive)
					{
						#if defined(FULL_GAME)
							if(confuseActive && skullAlive && skull._status)
							{
								chasedByGhosts=&skull;
							}
							else
							{
								chasedByGhosts=&player;
							}
							chaseCharacter(chasedByGhosts, ghostSlowDown);


							#if BOMBS_NUMBER==4
								if((level==15 || level==20) && ghostCount<=2)
								{
									deleteCharacter(&bombs[loop&3]);
									#if defined(SIMPLE_STRATEGY)
										moveTowardCharacter(chasedByGhosts, &bombs[loop&3]);
									#else
										moveTowardCharacter(chasedByGhosts, &bombs[loop&3], (unsigned char) (1+(unsigned char)(loop&3))<<1);
									#endif
								}
							#endif							
							
						#else
							chaseCharacter(ghostSlowDown);
						#endif
						++ghostLevel;			
						
					}
				
					handle_skull();

					// This detects collisions of ghosts that have just moved
					if(bullet._status)
					{
						checkBullet(&bullet);
					}
				#else
					#if !defined(NO_CHASE)
						chaseCharacter(ghostSlowDown);
						#if !defined(TINY_GAME)
							++ghostLevel;
						#endif
					#endif
				#endif
				
				// Check collisions bombs vs ghosts
				for(ind=0;ind<GHOSTS_NUMBER;++ind)
					{
						checkBombsVsGhost(&ghosts[ind]);
					}
				
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
					   (!invincibilityActive && (playerReachedGhosts() || characterReachedBombs(&player) || innerWallReached(&player) || (horizontalWallsLevel() && horizontalWallsReached())))
					  )
				#else
					if(wallReached(&player) || playerReachedGhosts() || characterReachedBombs(&player))
				#endif
					{
						playerDies();
					}

				
				#if defined(FULL_GAME)
					SKIP_DRAW
					{
						displayBombs();	
					}
					SKIP_MORE_DRAW
					{						
						DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);			
				
						if(horizontalWallsLevel())
						{				
							horizontalWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + (level>>4) + (unsigned char) (loop/HORIZONTAL_WALLS_INCREASE_LOOP);		
							DRAW_HORIZONTAL_WALLS(horizontalWallsLength);	
						}						
					}
										
				#else
					SKIP_MORE_DRAW
					{
						displayBombs();
					}
				#endif
				
				// Display ghosts
				SKIP_DRAW
				{
					for(ind=0;ind<GHOSTS_NUMBER;++ind)
					{
						displayGhost(&ghosts[ind]);
					}
				}
					
				#if defined(TURN_BASED) 
					if((loop<TURN_BASED_MAX_LOOP) || loop&1)
					{
						MOVE_PLAYER();
					}					
					_DRAW_PLAYER();	
				#endif	
				REFRESH();
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

			if(player._status) // if level finished
			{
				#if defined(BETWEEN_LEVEL)
					chasedEnemyPtr = &player;
					SHOW_DOWN();
				#endif
				
				#if !defined(TINY_GAME)
					#if !defined(LESS_TEXT)
						SLEEP(1);
						printVictoryMessage();
						SLEEP(2);

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
					#endif
					SLEEP(1);					
					CLEAR_SCREEN();						
				#else
					// points += LEVEL_BONUS * 4;
				#endif			

				ghostCount = GHOSTS_NUMBER;
				
				#if defined(FULL_GAME)			
					if(bossLevel())
					{	
						PING_SOUND();
						#if !defined(LESS_TEXT)
							printExtraLife();
							SLEEP(2);
						#endif
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
				#if defined(END_SCREEN) || defined(DANCE)
					for(bulletDirection=0;bulletDirection<80;++bulletDirection)
					{
						for(ind=0;ind<GHOSTS_NUMBER;++ind)
						{
							if(ghosts[ind]._status)
								dance(&ghosts[ind]);
						}
					}
				#endif
				
				#if defined(BETWEEN_LEVEL)
					chasedEnemyPtr = &skull;	
				#endif
				
				#if !defined(TINY_GAME)
					CLEAR_SCREEN();
				#endif
				if(--lives>0)
				{
					player._status = 1;
				}
			}
			#if defined(BETWEEN_LEVEL)
				spiral(chasedEnemyPtr, 2*MIN_SIZE-18);
				SLEEP(1);
			#endif				
		} while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game 
			
		if(level==FINAL_LEVEL+1) // if completed game
		{
			gameCompleted();
			#if !defined(NO_SLEEP)
				SLEEP(1);
			#else
				WAIT_PRESS();
			#endif
		}

		// GAME OVER	
		CLEAR_SCREEN();
		printGameOver();
		
		#if !defined(NO_SLEEP)
			SLEEP(1);
		#else
			WAIT_PRESS();
		#endif
		
		#if !defined(TINY_GAME) && !defined(LESS_TEXT)
			CLEAR_SCREEN();
			finalScore();
		
			#if !defined(NO_SLEEP)
				SLEEP(1);
			#else
				WAIT_PRESS();
			#endif
		
		#endif
		if(points>highScore)
		{
			highScore = points;
		}
		
	} // while(1) -> restart from the beginning

	return EXIT_SUCCESS;
}
