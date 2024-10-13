
/* --------------------------------------------------------------------------------------- */ 
// 
// CROSS SHOOT by Fabrizio Caruso
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

// TODO: REMOVE THIS
// #define DEBUG_STRATEGY
// #define DEBUG_ITEMS
// #define DEBUG_END
// #define DEBUG_ITEMS_IN_GAME
// #define DEBUG_EASY_BOSS_LEVEL
// #define NO_CHASE

#include "settings.h"

#include "character.h"
#include "item.h"
#include "strategy.h"
#include "game_text.h"
#include "level.h"
#include "ghost.h"
#include "move_player.h"
#include "sleep.h"

#include "text_strings.h"

#include "init_images.h"

#include "display_macros.h"

#include "speed_game_settings.h"

#include "sleep_macros.h"

#include "bullet.h"
#include "skull.h"
    
#include "end_screen.h"

#include "horizontal_missile.h"
#include "rocket.h"

#include "variables.h"

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
    extern Image CALM_DOWN_IMAGE;
    extern Image FIRE_CHARGE_IMAGE;
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
    extern Image SUICIDE_IMAGE;
    extern Image BROKEN_BRICK_IMAGE;

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
        _draw_stat(1, 8,&CALM_DOWN_IMAGE);
        _draw_stat(1, 9,&FIRE_CHARGE_IMAGE);
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
            _draw_stat(1,22,&SUICIDE_IMAGE);
            _draw_stat(1,23,&BROKEN_BRICK_IMAGE);
            #else
            _draw_stat(3,7,&ROCKET_IMAGE);
            _draw_stat(3,8,&FREEZE_IMAGE);
            _draw_stat(3,9,&SUPER_IMAGE);
            _draw_stat(3,10,&EXTRA_LIFE_IMAGE);
            _draw_stat(3,11,&INVINCIBILITY_IMAGE);
            _draw_stat(3,12,&CONFUSE_IMAGE);
            _draw_stat(3,13,&SUICIDE_IMAGE);
            _draw_stat(3,14,&BROKEN_BRICK_IMAGE);
            #endif
        #endif
    #endif
    _XL_PRINTD(4,3,5, 1234U);
    _XL_PRINTD(4,5,5,56789U);
    _XL_PRINT(4,7,"abcdefghijklmnopqrstuvwxyz,./|-");
    _XL_PRINT(4,9,"ABCDEFGHIJKLMNOPQRSTUVWXYZ;+{}=");
    while(1){};
}
#endif


#if !defined(NO_EXTRA_TITLE) && YSize>=17
static const uint8_t item_tile[7][2] = 
{
    { _GHOST_TILE      , _XL_WHITE },
    { _FAST_GHOST_TILE , _XL_GREEN },
    { _SKULL_TILE      , _XL_YELLOW },
    { _BOSS_TILE       , _XL_RED },
    { _BULLET_TILE     , _XL_WHITE},
    { _FIRE_POWER_TILE , _FIRE_POWER_COLOR },
    { _BOMB_TILE       , _XL_RED },
};

static const char item_name[7][9] = 
{
    _XL_G _XL_H _XL_O _XL_S _XL_T,
    _XL_S _XL_P _XL_E _XL_C _XL_T _XL_E _XL_R,
    _XL_S _XL_K _XL_U _XL_L _XL_L,
    _XL_B _XL_O _XL_S _XL_S,
    _XL_B _XL_U _XL_L _XL_L _XL_E _XL_T _XL_S,
    _XL_P _XL_O _XL_W _XL_E _XL_R _XL_SPACE _XL_U _XL_P,
    _XL_M _XL_I _XL_N _XL_E,
};

#if XSize>16 && YSize>20
    #define _NEXT_ROW 2
    #define _Y_ITEMS_OFFSET 0
#else
    #define _NEXT_ROW 1
    #define _Y_ITEMS_OFFSET 1
#endif


#define display_items() \
do \
{ \
    uint8_t i; \
    \
    for(i=0;i<7;++i) \
    { \
        _XL_DRAW(XSize/2-5,YSize/3+1+i*_NEXT_ROW+_Y_ITEMS_OFFSET, item_tile[i][0], item_tile[i][1]); \
        _XL_SET_TEXT_COLOR(_XL_GREEN); \
        _XL_PRINT(XSize/2-5+3,YSize/3+1+i*_NEXT_ROW+_Y_ITEMS_OFFSET, (char *)item_name[i]); \
    } \
} while(0)

#endif


#define handle_secret_item_at_start_up(secretFlag, item, secretIndex) \
    if(secretFlag) \
    { \
        item._coolDown = 2; \
        setSecret(secretIndex); \
        secretFlag = 0; \
    }\


void resetSecrets(void)
{
    uint8_t i;
    
    for(i=0;i<SECRETS_NUMBER;++i)
    {
        discoveredSecrets[i]=0;
    }
}


void resetItems(void)
{

    calmDown._coolDown = CALM_DOWN_COOL_DOWN;
    firePower._coolDown = FIRE_POWER_COOL_DOWN;
    fireCharge._coolDown = FIRE_CHARGE_COOL_DOWN;
    freeze._coolDown = FREEZE_COOL_DOWN;
    bombCharge._coolDown = BOMB_CHARGE_COOL_DOWN;
    
    extraPoints._coolDown = EXTRA_POINTS_COOL_DOWN;
    invincibility._coolDown = INVINCIBILITY_COOL_DOWN;

    handle_secret_item_at_start_up(calmDownSecret, calmDown, CALM_DOWN_AT_START_SECRET_INDEX);
    handle_secret_item_at_start_up(extraPointsSecret, extraPoints, EXTRA_POINTS_AT_START_SECRET_INDEX);
    handle_secret_item_at_start_up(freezeSecret, freeze, FREEZE_AT_START_SECRET_INDEX);

    handle_secret_item_at_start_up(firePowerItemSecret, firePower, FIRE_POWER_AT_START_SECRET_INDEX);

    super._coolDown = SUPER_COOL_DOWN;
    extraLife._coolDown = EXTRA_LIFE_COOL_DOWN;
    
    confuse._coolDown = CONFUSE_COOL_DOWN;
    suicide._coolDown = SUICIDE_COOL_DOWN;                
    destroyer._coolDown = DESTROYER_COOL_DOWN;
    
    if(!level)
    {
        isBossLevel = 0;
        extraPoints._coolDown = 4;
        invincibility._coolDown = 4;
    }
    
    if(isBossLevel)
    {
        firePower._coolDown<<=1;
        freeze._coolDown<<=1;
        bombCharge._coolDown<<=1;
        super._coolDown<<=1;
        confuse._coolDown<<=1;
    }
}


// Constructor for all items
void constructItems(void)
{
    calmDown._effect = &calmDownEffect;
    fireCharge._effect = &fireChargeEffect;
    bombCharge._effect = &bombChargeEffect;
    firePower._effect = &firePowerEffect;
    extraPoints._effect = &extraPointsEffect;
    freeze._effect = &freezeEffect;
    extraLife._effect = &extraLifeEffect;
    invincibility._effect = &invincibilityEffect;
    super._effect = &superEffect;
    confuse._effect = &confuseEffect;
    suicide._effect = &suicideEffect;
    destroyer._effect = &destroyerEffect;
}    


#if !defined(NO_INITIAL_SCREEN)            
void initialScreen(void)
{    
    _XL_CLEAR_SCREEN();                    
    printStartMessage();
    
    #if !defined(NO_EXTRA_TITLE) && YSize>=17
    display_items();
    #endif 
    
    _XL_WAIT_FOR_INPUT();
    _XL_SET_TEXT_COLOR(_XL_YELLOW);
    _printCrossShoot();
    _XL_ZAP_SOUND();
    SHORT_SLEEP(3);
    
    #if !defined(NO_HINTS) && XSize>=18
        _XL_CLEAR_SCREEN();
        printHints();    
    #endif
}
#endif

void handle_special_triggers(void)
{
    confuse_present_on_level = !level || destroyed_bases_in_completed_levels;
    suicide_present_on_level = !level || (destroyed_bases_in_completed_levels>=2);
    super_present_on_level = all_skulls_killed_in_completed_levels>=2;
    extraLife_present_on_level = super_present_on_level && suicide_present_on_level && !discoveredSecrets[EXTRA_LIFE_EFFECT_SECRET_INDEX];
}


void handle_player_killed_by_ghost(void)
{
    uint8_t reachedByGhost = sameLocationAsAnyGhostLocation(player._x, player._y, ghosts, maxGhostsOnScreen);
    
    if(destroyerActive && (reachedByGhost < maxGhostsOnScreen))
    {
        points += GHOST_VS_BOMBS_BONUS;
        ghostDiesAndSpawns(&ghosts[reachedByGhost]);
    }
    
    
    if((!invincibilityActive && ((reachedByGhost<maxGhostsOnScreen))))
    {
        playerDies();
    }
}

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
            
            _XL_WAIT_FOR_INPUT();
            _XL_SET_TEXT_COLOR(_XL_YELLOW);

            _printCrossShoot();
            _XL_ZAP_SOUND();
            SHORT_SLEEP(9);
            _XL_CLEAR_SCREEN();    
        #else
            _XL_CLEAR_SCREEN();    
            
            printPressKeyToStart();                
        #endif

        #if defined(DEBUG_ITEMS)
            DO_DEBUG_ITEMS();
        #endif
        
        extraLifeThroughPointsCounter = 1;
        points = 0;
        level = INITIAL_LEVEL;     
        lives = LIVES_NUMBER;
        
        ghostCount = FIRST_LEVEL_GHOSTS_NUMBER; 
         
        destroyed_bases_in_completed_levels = 0;
        all_skulls_killed_in_completed_levels = 0;
        
        firePowerItemSecret = 0;
        calmDownSecret = 0;
        extraPointsSecret = 0;
        freezeSecret = 0;
        fireChargeSecret = 0;
        
        secretLevelActivated = 0;
        zeroLevelSecret = 0;
        
        resetSecrets();
        
        do // Level (Re-)Start
        {
            isBossLevel = bossLevel();
            // isRocketLevel = rocketLevel();
            isOneMissileLevel = oneMissileLevel();
            isMissileLevel = missileLevel();
            isInnerHorizontalWallLevel = innerHorizontalWallLevel();
            isInnerVerticalWallLevel = innerVerticalWallLevel();
            setNumberOfRocketsOnScreen();
            
            #if defined(DEBUG_STRATEGY)
            maxGhostsOnScreen = 1;
            #else
            if(!level) // TODO: Do I need this check for the zero level?
            {
                maxGhostsOnScreen = GHOSTS_NUMBER;
            }
            else if(isBossLevel)
            {
                maxGhostsOnScreen = BOSS_LEVEL_GHOSTS_NUMBER;
            }
            else if(isInnerHorizontalWallLevel || isInnerVerticalWallLevel)
            {
                maxGhostsOnScreen = GHOSTS_NUMBER-GHOST_DECREASE;
            }
            else
            {
                maxGhostsOnScreen = GHOSTS_NUMBER;
            }
            #endif
            
            skullsCount = SKULLS_NUMBER;
            
            bombCount = BOMBS_NUMBER;
            
            loop = 0;
            
            ghostLevel = 32*level;
        
            
            destroyed_bases = 0;
            exploded_bombs = 0;
            
            invincibilityActive = 1;                
            invincibility_count_down = INITIAL_INVINCIBILITY_COUNT_DOWN;
            
            destroyerActive = 0;
            missileDestroyerActivated = 0;
            bombDestroyerActivated = 0;
            
            #if !defined(INITIAL_GHOST_FREEZE)
                freezeActive = 0;
            #endif
            confuseActive = 0;

            #if defined(DEBUG_ITEMS_IN_GAME)
                destroyed_bases_in_completed_levels = 2;
                all_skulls_killed_in_completed_levels = 2;

            #endif

            handle_special_triggers();

            #if !defined(SIMPLE_STRATEGY)
                computeStrategy();            
            #endif
                            
            skullActive = 0;
            
            #if defined(INITIAL_GHOST_FREEZE)
                freezeActive = 1;
                freeze_count_down = INITIAL_FROZEN_COUNT_DOWN;
            #endif
            
            computeSkullParameters();                
            
            // levelSlowDown = INITIAL_GHOST_SLOWDOWN;
            ghostSlowDown = computeGhostSlowDown();
            
            _XL_CLEAR_SCREEN();
            #if !defined(LESS_TEXT)
                // Clear the screen, put cursor in upper left corner

                printLevel();
                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
                _XL_CLEAR_SCREEN();
            #endif
                  
            arrowRange = computeArrowRange();
            
            if(isBossLevel)
            {
                #if !defined(LESS_TEXT)
                printKillTheSkulls();
                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
                #endif
                ghostCount = 0;
            }
            _XL_CLEAR_SCREEN();
            
            updateInnerVerticalWall();    
            
            updateInnerHorizontalWall();                
            
            // printPressKeyToStart();
            // _XL_WAIT_FOR_INPUT();
            // _XL_CLEAR_SCREEN();
            
            playerFire = 0;

            if(fireChargeSecret)
            {
                guns = SECRET_GUNS;
                setSecret(FIRE_CHARGE_AT_START_SECRET_INDEX);
                fireChargeSecret = 0;
            }
            else
            {
                guns = 0;
            }

            if(firePowerLevelSecret)
            {
                bulletStrength = firePowerLevelSecret;
                setSecret(FIRE_POWER_LEVEL_SECRET_INDEX);
                firePowerLevelSecret = 0;
            }
            else
            {
                bulletStrength = 2;
            }

            fillLevelWithCharacters();            
            
            constructItems();    
            
            // displayStatsTitles();
            
            // displayStats();
            
            resetItems();
            

            displayStats();

            while(player._status && (( ((ghostCount>0)&&(skullsCount)) && !isBossLevel) || (skullsCount && isBossLevel))) // while alive && there are still ghosts
            {                
                #if defined(DEBUG_END)
                    gameCompleted();
                #endif
                    
                #if !defined(_XL_TURN_BASED)
                    MOVE_PLAYER();  
                    #if !defined(TRANSITION_ANIMATION)
                        _DRAW_PLAYER();    
                    #endif
                #endif
                        
                handle_rockets();
                handle_horizontal_missiles();
                
                ++loop;
                
                if(points>(extraLifeThroughPointsCounter*EXTRA_LIFE_THROUGH_POINTS))
                {
                    ++extraLifeThroughPointsCounter;
                    _XL_PING_SOUND();
                    ++lives;
                    printLivesStats();
                    _XL_TICK_SOUND();
                }
                
                handle_bullets();
                handle_bomb();

                handle_player_killed_by_ghost();

                #if !defined(NO_CHASE)
                if(!freezeActive)
                {
                    if(ghostCount)
                    {
                        chaseCharacter();
                    }
                    ++ghostLevel;            
                    
                }
                else // Frozen ghosts have to be displayed anyway
                {
                    for(ind=0;ind<GHOSTS_NUMBER;++ind)
                    {
                        if(ghosts[ind]._status)
                        {
                            displayGhost(&ghosts[ind]);
                        }
                    }
                }
                #else
                    for(ind=0;ind<GHOSTS_NUMBER;++ind)
                    {
                        if(ghosts[ind]._status)
                        {
                            displayGhost(&ghosts[ind]);
                        }
                    }
                #endif
                                
                
                #if !defined(DEBUG_STRATEGY) && !defined(NO_CHASE)
                handle_skulls();
                #endif
            
                // This detects collisions of ghosts that have just moved
                checkBullets();

                
                #if defined(TRANSITION_ANIMATION)
                    if(moved)
                    {
                        _XL_DELETE(old_x,old_y);
                        moved = 0;
                    }
                    _DRAW_PLAYER();
                #endif                
                
                // Check collisions bombs vs ghosts
                checkBombsVsGhosts();
                if(skullActive)
                {
                    checkBombsVsSkulls();
                }
                
                SHORT_SLEEP(3);
                if(invincibilityActive)
                {
                    #if _XL_SLOW_DOWN_FACTOR>0
                    SHORT_SLEEP(1);
                    #endif
                    _XL_DELETE(player._x, player._y);
                }

                SHORT_SLEEP(3);
                
                handle_calmDown_item();
                handle_extraPoints_item();
                handle_firePower_item();
                handle_fireCharge_item();
                handle_bombCharge_item();    
                
                handle_freeze_count_down();                    
                
                handle_freeze_item();    
                handle_invincibility_item();
                handle_invincibility_count_down();                    

                handle_destroyer_triggers();
                handle_destroyer_item();
                handle_destroyer_count_down();
                    
                if(super_present_on_level)
                {
                    handle_super_item();
                    if(extraLife_present_on_level)
                    {
                        handle_extraLife_item();
                    }
                }
                
                                    
                if(confuse_present_on_level)
                {
                    handle_confuse_item();
                    handle_confuse_count_down();
                    if(suicide_present_on_level)
                    {
                        handle_suicide_item();
                    }
                }              
                
                handle_player_killed_by_ghost();
                // reachedByGhost = sameLocationAsAnyGhostLocation(player._x, player._y, ghosts, maxGhostsOnScreen);
                
                // if(destroyerActive && (reachedByGhost < maxGhostsOnScreen))
                // {
                    // points += GHOST_VS_BOMBS_BONUS;
                    // ghostDiesAndSpawns(&ghosts[reachedByGhost]);
                // }
                
                
                // if((!invincibilityActive && ((reachedByGhost<maxGhostsOnScreen))))
                // {
                    // playerDies();
                // }

                    
                SKIP_WALL_DRAW
                {
                    ghostSlowDown = computeGhostSlowDown();
                    
                    if(isInnerVerticalWallLevel)
                    {
                        DRAW_VERTICAL_LINE(XSize/2, YSize/2-(innerVerticalWallLength/2), innerVerticalWallLength);
                    }
                    else if(isInnerHorizontalWallLevel)
                    {
                        DRAW_HORIZONTAL_LINE((XSize>>1)-(innerHorizontalWallLength>>1), YSize/2, innerHorizontalWallLength);
                    }
                    displayBombs();                                                        
                }
                
            }; // end inner while [while (player._alive && ghostCount>0), i.e., exit on death or end of level]

            if(player._status) // if level finished
            {
                _DRAW_PLAYER();
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &player;
                    SHOW_DOWN();
                #endif
                
                #if !defined(LESS_TEXT)
                    _XL_SLEEP(1);
                    printVictoryMessage();
                    _XL_WAIT_FOR_INPUT();

                    // _XL_CLEAR_SCREEN();
                #endif

                handleLevelBonus(((uint16_t)BASE_LEVEL_BONUS)+((uint16_t) LEVEL_BONUS)*((uint16_t) level)+ghostCount*GHOSTS_LEFT_BONUS);

                if(!isBossLevel)
                {
                    if(ghostCount>=CALM_DOWN_SECRET_THRESHOLD)
                    {
                        calmDownSecret = 1;
                        if(ghostCount>=EXTRA_POINTS_SECRET_THRESHOLD)
                        {
                            extraPointsSecret = 1;
                            if(ghostCount>=FREEZE_SECRET_THRESHOLD)
                            {
                                freezeSecret = 1;
                                if((ghostCount>=SECRET_LEVEL_THRESHOLD) && !secretLevelActivated)
                                {
                                    zeroLevelSecret = 1;
                                }
                            }
                        }
                    }
                }

                _XL_SLEEP(1);
                _XL_WAIT_FOR_INPUT();
                
                if(isBossLevel && level!=FINAL_LEVEL)
                {    
                    all_skulls_killed_in_completed_levels = 1;
                    destroyed_bases_in_completed_levels/=2;
                }
                else
                {
                    if(!skullsCount)
                    {
                        ++all_skulls_killed_in_completed_levels;
                    }
                    destroyed_bases_in_completed_levels+=destroyed_bases;
                }
                
                if(zeroLevelSecret)
                {
                    nextLevel = level+1;
                    level = 0;
                    secretLevelActivated = 1;
                    zeroLevelSecret = 0;
                    setSecret(ZERO_LEVEL_SECRET_INDEX);
                    ghostCount = ZERO_LEVEL_GHOSTS_NUMBER;
                }
                else
                {
                    if(!level)
                    {
                        // Restart at the next level after the one you left when entering the secret zero level
                        level = nextLevel;
                    }
                    else
                    {
                        ++level;
                    }
                    if(level>8)
                    {
                        ghostCount = NUMBER_OF_GHOSTS_ON_PART_II;
                    }
                    else
                    {
                        ghostCount = FIRST_LEVEL_GHOSTS_NUMBER + 2*level;
                    }
                }
                

            }
            else // if dead
            {        
                #if defined(BETWEEN_LEVEL)
                    chasedEnemyPtr = &skulls[0];    
                #endif
                
                _XL_CLEAR_SCREEN();
                --lives;
                if(lives>0)
                {
                    player._status = 1;
                }
                firePowerItemSecret = 0;
                calmDownSecret = 0;
                extraPointsSecret = 0;
                freezeSecret = 0;
                fireChargeSecret = 0;
                firePowerLevelSecret = 0;
                zeroLevelSecret = 0;
            }
            #if defined(BETWEEN_LEVEL)
                spiral(chasedEnemyPtr, SPIRAL_LOOPS);
                // _XL_SLEEP(1);
            #endif
            
        } while (player._status && (level<(FINAL_LEVEL+1))); // lives left and not completed game game     

        // GAME OVER    

        if(level==FINAL_LEVEL+1) // if completed game
        {
            // uint8_t i;
            // uint8_t lives_left = lives;
            do
            {
                points+=LIVES_LEFT_BONUS;
                --lives;
                displayScoreStats();
                printLivesStats();
                _XL_ZAP_SOUND();
                _XL_SLEEP(1);
            } while(lives);
            _XL_SLEEP(2);
            _XL_WAIT_FOR_INPUT();
            gameCompleted();
            _XL_SLEEP(1);
        }

        if(points>highScore)
        {
            highScore = points;
        }

        _XL_CLEAR_SCREEN();
		#if !defined(NO_ACHIEVEMENTS)
        printAchievements();
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
        
        _XL_CLEAR_SCREEN();
		#endif
        printGameOver();
        
        _XL_SLEEP(1);
        _XL_WAIT_FOR_INPUT();
        
    } // while(1) -> restart from the beginning

    return EXIT_SUCCESS;
}

