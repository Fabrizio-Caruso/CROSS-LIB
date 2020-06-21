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
    CLEAR_SCREEN();                    
    printStartMessage();
    
    #if defined(FULL_GAME) && !defined(NO_HINTS)
        WAIT_PRESS();
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
            
        #else
            printPressKeyToStart();                
        #endif
        WAIT_PRESS();
        CLEAR_SCREEN();

        #if !defined(LESS_TEXT) || defined(ALT_HIGHSCORE)
            highScoreScreen();
            WAIT_PRESS();
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
            #if defined(FULL_GAME)
                isBossLevel = bossLevel();
                isRocketLevel = rocketLevel();
                isOneMissileLevel = oneMissileLevel();
                isMissileLevel = missileLevel();
                isHorizontalWallsLevel = horizontalWallsLevel();
            #endif
            #if !defined(TINY_GAME) || defined(TURN_BASED)
                loop = 0;
            #endif
            
            #if !defined(TINY_GAME)
        
                playerFire = 0;
            #endif
        
            #if defined(FULL_GAME)
                ghostLevel = 0;
                
                dead_rockets = 0;
                
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
                    skullSlowDown = INITIAL_SKULL_SLOWDOWN;
                #endif
            #endif

            #if !defined(FULL_GAME)
                ghostSlowDown = INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256;
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
            
                if(isBossLevel)
                {
                    printKillTheSkull();
                    SLEEP(2);
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
            
            fillLevelWithCharacters();
            #if !defined(TINY_GAME)
                constructItems();    
                
                #if !defined(NO_STATS)
                displayStatsTitles();
                #endif
            #endif
            
            displayStats();        
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
                    gameCompleted();
                #endif
                    
                #if !defined(TURN_BASED)
                    MOVE_PLAYER();                
                    _DRAW_PLAYER();    
                #endif
                #if SLOW_DOWN>0
                    DO_SLOW_DOWN(SLOW_DOWN);
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
                
                #if !defined(FULL_GAME)
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
                            if(confuseActive && skullActive && skull._status)
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
                                        moveTowardCharacter(chasedByGhosts, &bombs[loop&3], (uint8_t) (1+(uint8_t)(loop&3))<<1);
                                    #endif
                                }
                            #endif                            
                            ++ghostLevel;
                        #else
                            chaseCharacter(ghostSlowDown);
                        #endif
                        
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
                        chaseCharacter(ghostSlowDown);
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
                                displayStats();
                                reducePowerUpsCoolDowns();
                            }
                        }
                    }
                    
                #endif
                
                #if defined(FULL_GAME)
                    if(wallReached(&player) || 
                       (!invincibilityActive && (playerReachedGhosts() || characterReachedBombs(&player) || innerWallReached(&player) || (isHorizontalWallsLevel && horizontalWallsReached())))
                      )
                #else
                    if(wallReached(&player) || playerReachedGhosts() || characterReachedBombs(&player))
                #endif
                    {
                        playerDies();
                    }

                
                #if defined(FULL_GAME)
                    SKIP_BOMB_DRAW
                    {
                        displayBombs();    
                    }
                    SKIP_WALL_DRAW
                    {                        
                        DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);            
                
                        if(isHorizontalWallsLevel)
                        {                
                            horizontalWallsLength = HORIZONTAL_WALLS_INITIAL_LENGTH + (level>>4) + (uint8_t) (loop/HORIZONTAL_WALLS_INCREASE_LOOP);        
                            DRAW_HORIZONTAL_WALLS(horizontalWallsLength);    
                        }                        
                    }
                                        
                #else
                    SKIP_BOMB_DRAW
                    {
                        displayBombs();
                    }
                #endif
                
                // Display ghosts
                SKIP_GHOST_DRAW
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
    
            #if defined(BENCHMARK)
                TicksDelta = clock() - Ticks;
                Sec = (uint16_t) (TicksDelta / CLOCKS_PER_SEC);
                Milli = ((TicksDelta % CLOCKS_PER_SEC) * 1000) / CLOCKS_PER_SEC;
                printf ("\nTime used: %u.%03u secs = %u ticks\n", Sec, Milli, (uint16_t) TicksDelta);    
                getchar();
            #endif

            if(player._status) // if level finished
            {
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &player;
                    SHOW_DOWN();
                #endif
                
                #if defined(FULL_GAME)
                    SLEEP(1);
                    #if !defined(LESS_TEXT)
                        printVictoryMessage();
                        SLEEP(2);
                    #endif

                    points+= LEVEL_BONUS;
                #endif            

                ghostCount = GHOSTS_NUMBER;

                #if defined(FULL_GAME)            
                    if(isBossLevel)
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
                        if(!skull._status)
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
                SLEEP(2);
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
