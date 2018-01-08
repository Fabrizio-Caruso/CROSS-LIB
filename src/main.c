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
	unsigned char frozen;
	unsigned short frozenCountDown;
	unsigned short invincibleSlowDown;
	unsigned char invincibleXCountDown;
	unsigned char invincibleYCountDown;
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
	// unsigned short invincibleLoopTrigger;
	extern Image INVINCIBLE_GHOST_IMAGE;
	extern Image POWERUP_IMAGE;
	extern Image MISSILE_IMAGE;
	extern Image GUN_IMAGE;
	extern Image EXTRA_POINTS_IMAGE;	
	extern Image DEAD_GHOST_IMAGE;
#endif

#if defined(FULL_GAME)
	extern unsigned char playerBlink;	
#endif

#if defined(DEBUG) && defined(FULL_GAME)
	extern Image EXTRA_LIFE_IMAGE;
	extern Image INVINCIBILITY_IMAGE;
	extern Image BUBBLE_IMAGE;
	extern Image LEFT_ENEMY_MISSILE_IMAGE;
	extern Image RIGHT_ENEMY_MISSILE_IMAGE;	
#endif

Character player; 

#if !defined(TINY_GAME)
	Character invincibleGhost;
	Item powerUp;
	Item powerUp2;
	Item freeze;
	Item gun;
	Item extraPoints;	
	Item super;
	Character missile;
#endif

Character ghosts[GHOSTS_NUMBER];
Character bombs[BOMBS_NUMBER];

#if defined(FULL_GAME)
	unsigned short invincibleGhostCountTrigger = INVINCIBLE_GHOST_TRIGGER;

	unsigned char innerVerticalWallY; 
	unsigned char innerVerticalWallX; 
	unsigned char innerVerticalWallLength;

	Item powerUp3;
	Item extraLife;
	Item invincibility;
	
	unsigned short playerInvincibilityCoolDown;	

	Character leftEnemyMissile;
	Character rightEnemyMissile;

	Character bubbles[BUBBLES_NUMBER];

	unsigned char bubbles_x[BUBBLES_NUMBER];
	
	unsigned char player_invincibility;

	unsigned char dead_bubbles;
	
	unsigned char arrowRange;
	
	unsigned char skullsKilled;
	
	unsigned char horizontalWallsLength;
#endif

#if !defined(TINY_GAME)
	unsigned char strategyArray[GHOSTS_NUMBER];

	unsigned char playerDirection; // 0: right, 1: down, 2: left, 3: up
#endif

unsigned int extraLifeThroughPointsCounter;

unsigned char ghostCount = GHOSTS_NUMBER;

#if !defined(TINY_GAME)
	unsigned char playerFire;
	unsigned char guns = GUNS_NUMBER;

	unsigned char invincibleGhostHits;
	unsigned char invincibleGhostAlive;
#endif

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
			reducePowerUpsCoolDowns();
			
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

void _commonPowerUpEffect(void)
{
	points+=POWER_UP_BONUS;
	decreaseGhostLevel();
	frozen = 1;	
	frozenCountDown += FROZEN_COUNT_DOWN;	
}

void powerUpEffect(void)
{
	_commonPowerUpEffect();
	powerUp._coolDown = POWER_UP_INITIAL_COOLDOWN;		
}

void powerUp2Effect(void)
{
	_commonPowerUpEffect();
	powerUp2._coolDown = POWER_UP2_INITIAL_COOLDOWN;	
}

void _freezeEffect(void)
{
	unsigned char i;
	for(i=0;i<3;++i)
	{
		_commonPowerUpEffect();
	}	
}

void freezeEffect(void)
{
	_freezeEffect();
	freeze._coolDown = FREEZE_INITIAL_COOLDOWN*2;	
}

void handle_frozen(void)
{
	if(frozen && (frozenCountDown<=0))
	{
		frozen = 0;
	}
	else if(frozenCountDown>0)
	{
		--frozenCountDown;
	}
}

void _gunEffect(void)
{
	guns = GUNS_NUMBER;
	printGunsStats();		
	points+=GUN_BONUS;		
}

void gunEffect(void)
{
	_gunEffect();
	gun._coolDown = GUN_INITIAL_COOLDOWN;	
}

void extraPointsEffect(void)
{
	points+=EXTRA_POINTS+level*EXTRA_POINTS_LEVEL_INCREASE;
	extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN*2; // second time is harder		
}

void handle_item(Item *itemPtr)
{
	// Manage item
	if(itemPtr->_character._status == 1)
	{	
		if(areCharctersAtSamePosition(&player, (Character *) itemPtr))
		{
			itemPtr->_effect();
			powerUpReached((Character *) itemPtr);			
		}
		else
		{
			_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
		}		
	}
	else if (itemPtr->_coolDown <= 0)
	{
		relocatePowerUp((Character *) itemPtr);

		_blink_draw(itemPtr->_character._x, itemPtr->_character._y, itemPtr->_character._imagePtr, &(itemPtr->_blink));
	}
	else
	{
		--itemPtr->_coolDown;
	}
}

#define handle_gun_item() handle_item(&gun);
#define handle_powerup_item() handle_item(&powerUp);
#define handle_powerup2_item() handle_item(&powerUp2);
#define handle_freeze_item() handle_item(&freeze);
#define handle_extraPoints_item() handle_item(&extraPoints);
	
#if defined(FULL_GAME)

	void powerUp3Effect(void)
	{
		_commonPowerUpEffect();
		powerUp3._coolDown = POWER_UP3_INITIAL_COOLDOWN;	
	}	
	
	void extraLifeEffect(void)
	{
		++lives;
		skullsKilled=1;
		// extraLife._coolDown = EXTRA_LIFE_COOL_DOWN*10; // second time is harder
		printLivesStats();		
	}

	void _invincibilityEffect(void)
	{
		player_invincibility = 1;
		playerInvincibilityCoolDown = PLAYER_INVINCIBILITY_COOL_DOWN;			
	}
	
	void invincibilityEffect(void)
	{
		_invincibilityEffect();
		invincibility._coolDown = INVINCIBILITY_COOL_DOWN;		
	}
	
	void superEffect(void)
	{
		_freezeEffect();
		_gunEffect();
		_invincibilityEffect();
		super._coolDown = SUPER_COOL_DOWN*10;
	}
	
	#define handle_powerup3_item() handle_item(&powerUp3)
	#define handle_extraLife_item() handle_item(&extraLife)
	#define handle_invincibility_item() handle_item(&invincibility)
	#define handle_super_item() handle_item(&super)
#endif

	// TODO: This has to be moved into level.c
	void initItems()
	{
		powerUp._effect = &powerUpEffect;
		powerUp2._effect = &powerUp2Effect;
		gun._effect = &gunEffect;
		extraPoints._effect = &extraPointsEffect;
		freeze._effect = &freezeEffect;
		#if defined(FULL_GAME)
			powerUp3._effect = &powerUp3Effect;		
			extraLife._effect = &extraLifeEffect;
			invincibility._effect = &invincibilityEffect;
			super._effect = &superEffect;
		#endif	
	}
#endif

#if defined(FULL_GAME)
	unsigned char computeArrowRange(void)
	{
			return level/10;
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
							((invincibleXCountDown==0)|| (invincibleYCountDown==0) || (loop>=INVINCIBLE_LOOP_TRIGGER) || (ghostCount<=invincibleGhostCountTrigger))) || 
		   (bossLevel() && loop>=INVINCIBLE_LOOP_TRIGGER))
		// #elif defined(TINY_GAME)
		// if(loop>=INVINCIBLE_LOOP_TRIGGER) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER))
		#else
		if(invincibleGhostAlive &&
							((invincibleXCountDown==0)     || (invincibleYCountDown==0) || 
							 (loop>=INVINCIBLE_LOOP_TRIGGER) || (ghostCount<=INVINCIBLE_GHOST_TRIGGER)))
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
		unsigned char i;
		
		CLEAR_SCREEN();			
		
		for(i=32;i<255;++i)
		{
			printf(" %u: %c", i, i);
		}
	}
	WAIT_PRESS();
}
#endif

#if defined(FULL_GAME)
	void handle_player_invincibility(void)
	{

		if(player_invincibility && playerInvincibilityCoolDown<=0)
		{
			player_invincibility = 0;
		}
		else
		{
			--playerInvincibilityCoolDown;
		}
	}

	void handle_rockets(void)
	{	
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
	}

	void _handle_enemy_missile_from_the_left(void)
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
	}
	
	void _handle_enemy_missile_from_the_right(void)
	{
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
				if((loop%2 && (player._x<= rightEnemyMissile._x)) && ((!oneMissileLevel() && player._y<=ENEMY_MISSILE_OFFSET+arrowRange) || (oneMissileLevel() && player._y<=YSize/2+arrowRange && player._y>=YSize/2-arrowRange)))			
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
	
	void handle_enemy_missiles(void)
	{	
		if(missileLevel() || bossLevel() || oneMissileLevel())
		{
			_handle_enemy_missile_from_the_left();	
			_handle_enemy_missile_from_the_right();
		}	
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

		#if defined(TINY_GAME) || defined(FULL_GAME)
			highScoreScreen();	
			WAIT_PRESS();	
		#endif
		#if !defined(TINY_GAME)

			CLEAR_SCREEN();
		#endif
		
		extraLifeThroughPointsCounter = 1;
		points = 0;
		level = INITIAL_LEVEL; 	
		lives = LIVES_NUMBER;
		ghostCount = GHOSTS_NUMBER;
		#if defined(FULL_GAME)
			skullsKilled = 0;
		#endif
		do // Level (Re-)Start
		{ 	
			#if defined(FULL_GAME)
				dead_bubbles = 0;
				extraLife._coolDown = EXTRA_LIFE_COOL_DOWN;
				invincibility._coolDown = INVINCIBILITY_COOL_DOWN;
				player_invincibility = 0;
				
				powerUp3._coolDown = POWER_UP3_INITIAL_COOLDOWN;	
				super._coolDown = SUPER_COOL_DOWN;
				if(skullsKilled==1)
				{
					invincibility._coolDown/=8;
				}
				
				// horizontalWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + level/10 + loop/10;
			#endif			
						
			#if !defined(TINY_GAME)
				extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;			
				computeStrategy();
			#endif
			
			loop = 0;
			ghostLevel = 0;

			#if !defined(TINY_GAME)
				frozen = 0;

				invincibleGhostAlive = 1;
				invincibleGhostHits = 0;
										
				guns = 0;
							
				gun._coolDown = GUN_INITIAL_COOLDOWN;
				powerUp2._coolDown = POWER_UP2_INITIAL_COOLDOWN;
				freeze._coolDown = FREEZE_INITIAL_COOLDOWN;
				
				frozenCountDown = 0;
				computeInvincibleGhostParameters();
			#endif

			ghostSlowDown = computeGhostSlowDown();
			
			CLEAR_SCREEN();
			#if !defined(NO_TEXT)
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
					sleep(1);
				}
				CLEAR_SCREEN();
				
				updateInnerWallVerticalData();	
			#endif
			
			printPressKeyToStart();
			WAIT_PRESS();
			CLEAR_SCREEN();
			
			
			#if !defined(TINY_GAME)
				DRAW_BORDERS();
			#endif
			
			fillLevelWithCharacters(ghostCount);	
			
			#if !defined(TINY_GAME)
				initItems();	
				
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
					handle_player_invincibility();
					handle_rockets();
					handle_enemy_missiles();
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
				
				_DRAW_PLAYER	
	
				#if !defined(TINY_GAME)
					handle_missile();
				#endif
				
				#if !defined(TINY_GAME)				
					if(!frozen)
					{
						chasePlayer(ghostSlowDown);
						++ghostLevel;				
					}
				
					handle_invincible_ghost();
				
					handle_frozen();

					// This detects collisions of ghosts that have just moved
					if(missile._status)
					{
						checkMissileVsGhosts(&missile);
					}
				#else
					chasePlayer(ghostSlowDown);
					++ghostLevel;						
				#endif
				
				// Check collisions bombs vs ghosts
				checkBombsVsGhosts();
				
				#if !defined(TINY_GAME)
					handle_extraPoints_item();
					handle_gun_item();
					handle_powerup_item();
					handle_powerup2_item();		
					handle_freeze_item();
				#endif
				
				#if defined(FULL_GAME)
					handle_powerup3_item();	
				
					handle_invincibility_item();

					if(skullsKilled>=2)
					{
						handle_super_item();
					}
					if(skullsKilled>=3)
					{
						handle_extraLife_item();
					}
						
					
					if(horizontalWallsLevel())
					{
						SKIP_MORE_DRAW
						{				
							horizontalWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + level/16 + (loop/HORIZONTAL_WALLS_INCREASE_LOOP);
						
							DRAW_HORIZONTAL_WALLS(horizontalWallsLength);
						}
											
						if(!player_invincibility && horizontalWallsReached(&player))
							{
								playerDies();
							}
					}
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
						SET_TEXT_COLOR(WALL_COLOR);
						DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);			
						DRAW_BOMBS();	
					}
				#else
					#if !defined(TINY_GAME)
					SKIP_MORE_DRAW
						DRAW_BOMBS();
					#endif
				#endif
				
				// Display ghosts
				SKIP_DRAW
					displayGhosts();
				
			}; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

			if(player._status) // if level finished
			{
				#if !defined(TINY_GAME)
					#if !defined(NO_TEXT)
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
					points+= LEVEL_BONUS*4;
				#endif			

				ghostCount = GHOSTS_NUMBER;
				
				#if defined(FULL_GAME)			
					if(bossLevel())
					{	
						CLEAR_SCREEN();
						sleep(1);
						PING_SOUND();
						#if !defined(NO_TEXT)
							printExtraLife();
						#endif
						sleep(2);
						++lives;
						skullsKilled = 1;
					}
					else
					{
						if(!invincibleGhostAlive)
						{
							++skullsKilled;
						}
					}
				#endif
				++level;

			}
			else // if dead
			{
				#if !defined(TINY_GAME)
					CLEAR_SCREEN();
				#endif
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
		#if !defined(NO_SLEEP)
			sleep(2);
		#else
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
