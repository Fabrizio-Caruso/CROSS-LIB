
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

#include "settings.h"

#include "character.h"
#include "item.h"
#include "strategy.h"
#include "game_text.h"
#include "level.h"
#include "ghost.h"
#include "move_player.h"
#include "text_strings.h"
#include "init_images.h"
#include "end_screen.h"
#include "variables.h"

#if !defined(TINY_GAME)
    #include "bullet.h"
    #include "skull.h"
#endif
    

#if defined(FULL_GAME)
    #include "horizontal_missile.h"
    #include "rocket.h"
#endif


#if defined(BENCHMARK)
    #include <stdio.h>
    #include <time.h>
#endif

#include "cross_lib.h"

// #define DEBUG_ITEMS

#if defined(DEBUG_ITEMS)
    #if defined(ANIMATE_PLAYER)
        extern Image PLAYER_DOWN_IMAGE;
        extern Image PLAYER_UP_IMAGE;
        extern Image PLAYER_RIGHT_IMAGE;
        extern Image PLAYER_LEFT_IMAGE;
    #else
        extern Image PLAYER_IMAGE;
    #endif
    extern Image GHOST_IMAGE;
    extern Image BOMB_IMAGE;
    extern Image DEAD_GHOST_IMAGE;
    extern Image SKULL_IMAGE;
    extern Image POWERUP_IMAGE;
    extern Image GUN_IMAGE;
    extern Image EXTRA_POINTS_IMAGE;
    extern Image BULLET_IMAGE;
    extern Image VERTICAL_BRICK_IMAGE;
    extern Image HORIZONTAL_BRICK_IMAGE;
    extern Image LEFT_HORIZONTAL_MISSILE_IMAGE;
    extern Image RIGHT_HORIZONTAL_MISSILE_IMAGE;
    extern Image ROCKET_IMAGE;
    extern Image FREEZE_IMAGE;
    extern Image SUPER_IMAGE;
    extern Image EXTRA_LIFE_IMAGE;
    extern Image INVINCIBILITY_IMAGE;
    extern Image CONFUSE_IMAGE;
    extern Image ZOMBIE_IMAGE;
    extern Image BROKEN_BRICK_IMAGE;
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
    void resetItems(void)
    {
        gun._coolDown = GUN_COOL_DOWN;
        powerUp._coolDown = POWER_UP_COOL_DOWN;
        extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;        
        
        #if defined(FULL_GAME)
            powerUp2._coolDown = POWER_UP2_COOL_DOWN;
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
        gun._effect = &gunEffect;
        extraPoints._effect = &extraPointsEffect;
        #if defined(FULL_GAME)
            powerUp2._effect = &powerUp2Effect;        
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
    _XL_CLEAR_SCREEN();                    
    printStartMessage();
    
    #if defined(FULL_GAME) && !defined(NO_HINTS) && XSize>=14
        _XL_WAIT_FOR_INPUT();
        _XL_CLEAR_SCREEN();
        printHints();    
    #endif
}
#endif


#if defined(FULL_GAME)
    
    void handle_special_triggers(void)
    {
        // confuse_present_on_level_condition is defined as bases_in_completed_levels
        zombie_present_on_level = bases_in_completed_levels>=MISSILES_FOR_ZOMBIE;
        super_present_on_level = all_skulls_killed_in_completed_levels>=SKULLS_FOR_SUPER;
        // chase_present_on_level_condition is defined as all_skulls_killed_in_completed_levels;
        extraLife_present_on_level = super_present_on_level && zombie_present_on_level;
    }
    
#endif

#if defined(DEBUG_ITEMS)

void DO_DEBUG_ITEMS(void)
{
    
    #if defined(ANIMATE_PLAYER)
        _draw_stat(1, 0,&PLAYER_DOWN_IMAGE);
        _draw_stat(1, 1,&PLAYER_UP_IMAGE);
        _draw_stat(1, 2,&PLAYER_RIGHT_IMAGE);
        _draw_stat(1, 3,&PLAYER_LEFT_IMAGE);
    #else
        _draw_stat(1, 0,&PLAYER_IMAGE);
    #endif
    _draw_stat(1, 4,&GHOST_IMAGE);
    _draw_stat(1, 5,&BOMB_IMAGE);
    #if !defined(NO_DEAD_GHOST)
        _draw_stat(1, 6,&DEAD_GHOST_IMAGE);
    #endif
    #if !defined(TINY_GAME)
        _draw_stat(1, 7,&SKULL_IMAGE);
        _draw_stat(1, 8,&POWERUP_IMAGE);
        _draw_stat(1, 9,&GUN_IMAGE);
        _draw_stat(1,10,&EXTRA_POINTS_IMAGE);
        _draw_stat(1,11,&BULLET_IMAGE);
        _draw_stat(1,12,&VERTICAL_BRICK_IMAGE);
        _draw_stat(1,13,&HORIZONTAL_BRICK_IMAGE);
        #if defined(FULL_GAME)
            _draw_stat(1,14,&LEFT_HORIZONTAL_MISSILE_IMAGE);
            _draw_stat(1,15,&RIGHT_HORIZONTAL_MISSILE_IMAGE);
            #if (YSize+Y_OFFSET)>18
            _draw_stat(1,16,&ROCKET_IMAGE);
            _draw_stat(1,17,&FREEZE_IMAGE);
            _draw_stat(1,18,&SUPER_IMAGE);
            _draw_stat(1,19,&EXTRA_LIFE_IMAGE);
            _draw_stat(1,20,&INVINCIBILITY_IMAGE);
            _draw_stat(1,21,&CONFUSE_IMAGE);
            _draw_stat(1,22,&ZOMBIE_IMAGE);
            _draw_stat(1,23,&BROKEN_BRICK_IMAGE);
            #else
            _draw_stat(3,7,&ROCKET_IMAGE);
            _draw_stat(3,8,&FREEZE_IMAGE);
            _draw_stat(3,9,&SUPER_IMAGE);
            _draw_stat(3,10,&EXTRA_LIFE_IMAGE);
            _draw_stat(3,11,&INVINCIBILITY_IMAGE);
            _draw_stat(3,12,&CONFUSE_IMAGE);
            _draw_stat(3,13,&ZOMBIE_IMAGE);
            _draw_stat(3,14,&BROKEN_BRICK_IMAGE);
            #endif
        #endif
    #endif
    _XL_PRINTD(4,3,5, 1234U);
    _XL_PRINTD(4,5,5,56789U);
    // PRINT(4,7,"abcdefghijklmnopqrstuvwxyz,./|-");
    // PRINT(4,9,"ABCDEFGHIJKLMNOPQRSTUVWXYZ;+{}=");
    while(1){};
}
#endif


int main(void)
{        
    _XL_INIT_GRAPHICS();

    _XL_INIT_INPUT();

    _XL_INIT_SOUND();

    highScore = 0;
    
    while(1)
    {
        INIT_IMAGES();
        
        #if !defined(NO_INITIAL_SCREEN)
            initialScreen();
            _XL_REFRESH();
        #else
            printPressKeyToStart();                
        #endif
        // _XL_WAIT_FOR_INPUT();
        // _XL_CLEAR_SCREEN();

        #if !defined(LESS_TEXT)
            highScoreScreen();
            _XL_REFRESH();
            _XL_WAIT_FOR_INPUT();
            _XL_CLEAR_SCREEN();
        #else
            highScoreScreen();
            // _XL_REFRESH();
            _XL_WAIT_FOR_INPUT();
            // _XL_CLEAR_SCREEN();
        #endif

        
        #if !defined(TINY_GAME)
            extraLifeThroughPointsCounter = 1;
        #endif
        points = 0;
        level = INITIAL_LEVEL;     
        lives = LIVES_NUMBER;
        ghostCount = GHOSTS_NUMBER;
        #if defined(FULL_GAME)
            #if defined(DEBUG_ITEMS_IN_GAME)
                bases_in_completed_levels = 99;
                all_skulls_killed_in_completed_levels = 99;
            #else
                bases_in_completed_levels = 0;
                all_skulls_killed_in_completed_levels = 0;            
            #endif            
        #endif
        
        
        do // Level (Re-)Start
        {
            
            #if defined(FULL_GAME)
                isBossLevel = bossLevel();
                isRocketLevel = rocketLevel();
                isOneMissileLevel = oneMissileLevel();
                isMissileLevel = missileLevel();
                ishorizWallsLevel = horizWallsLevel();
            #endif
            #if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
                loop = 0;
            #endif
            
            #if !defined(TINY_GAME)
        
                playerFire = 0;
            #endif
        
            #if defined(FULL_GAME)
                ghostLevel = 0;
                
                bases = 0;
                
                invincibilityActive = 1;                
                invincibility_count_down = INITIAL_INVINCIBILITY_COUNT_DOWN;
                
                #if !defined(INITIAL_GHOST_FREEZE)
                    freezeActive = 0;
                    freeze_count_down = 0;
                #endif
                confuseActive = 0;
                zombieActive = 0; 
                
                handle_special_triggers();
                
                #if !defined(SIMPLE_STRATEGY)
                    computeStrategy();            
                #endif
                                
            #endif            
            
            #if !defined(TINY_GAME)
                skullActive = 0;
                
                guns = 0;
                
                resetItems();
                
                #if defined(INITIAL_GHOST_FREEZE)
                    freezeActive = 1;
                    freeze_count_down = INITIAL_FROZEN_COUNT_DOWN;
                #endif
                
                skullXCountDown = SKULL_COUNT_DOWN;
                skullYCountDown = SKULL_COUNT_DOWN;        
                #if !defined(FULL_GAME)
                    skullSlowDown = INITIAL_GHOST_SLOWDOWN;
                #endif
            #endif

            #if !defined(FULL_GAME)
                #if !defined(TINY_GAME)
                    ghostSlowDown = INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256;
                #else
                    
                    ghostSlowDown = INITIAL_GHOST_SLOWDOWN;
                #endif
            #else
                levelSlowDown = INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256;
                ghostSlowDown = computeGhostSlowDown();
            #endif
            
            _XL_CLEAR_SCREEN();
            #if !defined(LESS_TEXT)
                // Clear the screen, put cursor in upper left corner
                printLevel();
                _XL_SLEEP(1);
                // _XL_CLEAR_SCREEN();
            #endif
                
            #if defined(FULL_GAME)
                        
                arrowRange = computeArrowRange();
            
                if(isBossLevel)
                {
                    printKillTheSkull();
                    _XL_SLEEP(1);
                    _XL_WAIT_FOR_INPUT();
                }
                _XL_CLEAR_SCREEN();
                
                updateInnerWallVerticalData();
            #endif
            
            printPressKeyToStart();
            _XL_REFRESH();
            #if !defined(TINY_GAME)
            _XL_WAIT_FOR_INPUT();
            #endif
            
            #if !defined(TINY_GAME)
            _XL_CLEAR_SCREEN();
            #endif
            
            fillLevelWithCharacters();
            #if !defined(TINY_GAME)
                constructItems();    
                
                #if !defined(NO_STATS)
                displayStatsTitles();
                #endif
            #endif
            
            displayScore();        
            #if !defined(NO_STATS)    
                printLevelStats();
                printLivesStats();
            #endif
            //
            #if !defined(TINY_GAME)
                #if !defined(NO_STATS)
                printGunsStats();
                #endif
                printGhostCountStats();

            #endif        
            
            #if defined(DEBUG_ITEMS)
                DO_DEBUG_ITEMS();
            #else
            
            #if defined(FULL_GAME)
                #if !defined(BENCHMARK)
                    while(player._status && ((ghostCount>0 && !isBossLevel) || (skull._status && isBossLevel))) // while alive && there are still ghosts
                #else
                    Ticks = clock();

                    while(benchmark_count<BENCHMARK_MAX && player._status && ((ghostCount>0 && isBossLevel) || (skull._status && isBossLevel))) // while alive && there are still ghosts
                #endif
            #else
                while(player._status && (ghostCount>0) )
            #endif
            {
                
                #if defined(BENCHMARK)
                    ++benchmark_count;
                #endif
                #if defined(DEBUG_END)
                    //gameCompleted();
                #endif
                    
                #if !defined(_XL_TURN_BASED)
                    MOVE_PLAYER();                
                    _DRAW_PLAYER();    
                #endif
                        
                #if defined(FULL_GAME)
                    handle_rockets();
                    handle_missiles();
                #endif
                

                #if !defined(TINY_GAME)
                if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
                {
                    ++extraLifeThroughPointsCounter;
                    _XL_PING_SOUND();
                    ++lives;
                    printLivesStats();
                }
                #endif
                
                #if !defined(FULL_GAME)
                    if(ghostSlowDown) 
                    {
                        --ghostSlowDown;
                    }
                #endif
                #if !defined(TINY_GAME)
                    handle_bullet();
                #endif
                #if !defined(TINY_GAME)                        

                    if(!freezeActive)
                    {
                        #if defined(FULL_GAME)
                            if(confuseActive && skullActive && skull._status)
                            {
                                chasedByGhosts=&skull;
                            }
                            else
                            {
                                chasedByGhosts=&player;
                            }
                            
                            chaseCharacter(chasedByGhosts);

                            #if BOMBS_NUMBER==4
                                if(isBossLevel && ghostCount<=2)
                                {
                                    #if defined(SIMPLE_STRATEGY)
                                        moveTowardCharacter(chasedByGhosts, &bombs[loop&3]);
                                    #else
                                        moveTowardCharacter(chasedByGhosts, &bombs[loop&3], (uint8_t) (1+(uint8_t)(loop&3))<<1);
                                    #endif
                                }
                            #endif                            
                            ++ghostLevel;
                        #else
                            chaseCharacter();
                        #endif
                        
                    }
                    else // Frozen ghosts must be redisplayed anyway
                    {
                        // Display ghosts
						for(ind=0;ind<GHOSTS_NUMBER;++ind)
						{
							if(!((loop+ind)&7))
							{
								displayGhost(&ghosts[ind]);
							}
						}
                    }
                    
                    if(skull._status)
                    {
                        handle_skull();
                    }

                    // This detects collisions of ghosts that have just moved
                    if(bullet._status)
                    {
                        checkBullet(&bullet);
                    }
                    
                #else
                    #if !defined(NO_CHASE)
                        chaseCharacter();
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
                    handle_freeze_count_down();                    
                #endif
                
                #if defined(FULL_GAME)
                    handle_powerup2_item();                    
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
                                displayScore();
                                reducePowerUpsCoolDowns();
                            }
                        }
                    }
                    
                #endif
                
                #if defined(FULL_GAME)
                    if(wallReached(&player) || 
                       (!invincibilityActive && (playerReachedGhosts() || characterReachedBombs(&player) || innerWallReached(&player) || (ishorizWallsLevel && horizWallsReached())))
                      )
                #else
                    if(wallReached(&player) || playerReachedGhosts() || characterReachedBombs(&player))
                #endif
                    {
                        playerDies();
                    }

                #if defined(FULL_GAME)
                    if(invincibilityActive && (loop&1))
                        {
                            DELETE_CHARACTER(player._x, player._y);
                        }    
                #endif
                
                #if defined(FULL_GAME)
                    SKIP_BOMB_DRAW
                    {
                        displayBombs();    
                    }
                    SKIP_WALL_DRAW
                    {                        
                    
                        #if defined(FULL_GAME)
                            ghostSlowDown = computeGhostSlowDown();
                        #endif
                        
                        DRAW_VERTICAL_LINE(XSize/2, YSize/2-(verticalWallLength/2), verticalWallLength);            
                        if(ishorizWallsLevel)
                        {                
                            horizWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + (level>>4) + (uint8_t) (loop/HORIZONTAL_WALLS_INCREASE_LOOP);        
                            DRAW_HORIZONTAL_WALLS(horizWallsLength);    
                        }                        
                    }
                                        
                #else
                    SKIP_BOMB_DRAW
                    {
                        displayBombs();
                    }
                #endif
                

                #if defined(_XL_TURN_BASED) 
                    #if !defined(EVEN_LOOP_MOVE)
                    if((loop<TURN_BASED_MAX_LOOP) || loop&1)
                    #else
                    if(loop<TURN_BASED_MAX_LOOP)   
                    #endif
                    {
                        MOVE_PLAYER();
                    }                    
                    _DRAW_PLAYER();    
                #endif    
                #if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
                    ++loop;
                #endif

                #if defined(_XL_SLOW_DOWN_FACTOR) && _XL_SLOW_DOWN_FACTOR>0
                    _XL_SLOW_DOWN(_XL_SLOW_DOWN_FACTOR);
                #endif
                _XL_REFRESH();
            }; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]
    
            #if defined(BENCHMARK)
                TicksDelta = clock() - Ticks;
                Sec = (uint16_t) (TicksDelta / CLOCKS_PER_SEC);
                Milli = ((TicksDelta % CLOCKS_PER_SEC) * 1000) / CLOCKS_PER_SEC;
                printf ("\nTime used: %u.%03u secs = %u ticks\n", Sec, Milli, (uint16_t) TicksDelta);    
                getchar();
            #endif

            if(player._status) // if level finished
            {
                
                #if defined(FULL_GAME)
                    _DRAW_PLAYER();
                    _XL_SLEEP(1);
                #endif
                
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &player;
                    SHOW_DOWN();
                #endif
                
                #if defined(FULL_GAME)
                    #if !defined(LESS_TEXT)
                        printVictoryMessage();
                    // #else
                        // _XL_SLEEP(2);
                        // _XL_CLEAR_SCREEN();
                    #endif
                    points+= LEVEL_BONUS;
                    displayScore();
                    _XL_SLEEP(1);
                    // _XL_WAIT_FOR_INPUT();

                #endif            

                ghostCount = GHOSTS_NUMBER;

                #if defined(FULL_GAME)            
                    if(isBossLevel)
                    {    
                        all_skulls_killed_in_completed_levels = 1;
                        bases_in_completed_levels/=2;
                    }
                    else
                    {
                        if(!skull._status)
                        {
                            ++all_skulls_killed_in_completed_levels;
                        }
                        bases_in_completed_levels+=bases;
                    }
                #endif
                #if !defined(TINY_GAME)
                ++level;
                #endif
            }
            else // if dead
            {        
                #if defined(FULL_GAME) && (defined(END_SCREEN) || defined(DANCE))
                    for(bulletDirection=0;bulletDirection<30;++bulletDirection)
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
                
                if(--lives>0)
                {
                    player._status = 1;
                }
            }
            #if defined(BETWEEN_LEVEL)
                spiral(chasedEnemyPtr);
                _XL_SLEEP(2);
            #endif                
            
            #endif
        } while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game 
            
        // _XL_SLEEP(2);
        if(level==FINAL_LEVEL+1) // if completed game
        {
            // _XL_WAIT_FOR_INPUT();
            #if !defined(NO_GAME_COMPLETED)
				gameCompleted();
			#endif
            #if !defined(NO_SLEEP)
                _XL_SLEEP(2);
            #else
                _XL_WAIT_FOR_INPUT();
            #endif
        }

        // GAME OVER    
        _XL_CLEAR_SCREEN();
        #if !defined(TINY_GAME)
        displayScore();
        printGameOver();
        _XL_WAIT_FOR_INPUT();

        #endif

        
        if(points>highScore)
        {
            highScore = points;
        }
        
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}
