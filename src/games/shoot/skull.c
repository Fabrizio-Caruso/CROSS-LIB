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


#include "settings.h"

#include "skull.h"
#include "level.h"
#include "strategy.h"
#include "game_text.h"
#include "item.h"

extern uint8_t level;
extern uint16_t loop;

extern uint16_t skullSlowDown;
extern uint8_t skullXCountDown;
extern uint8_t skullYCountDown ;

extern uint16_t ghostLevel;

extern uint8_t skullsCount;

extern Character skulls[SKULLS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

extern Character skull;
extern Character player;

extern uint8_t skullActive;

extern uint8_t isBossLevel;

extern uint8_t confuseActive;

extern uint8_t freezeActive;

extern uint8_t maxGhostsOnScreen;

extern Character ghosts[]; 

extern Image GHOST_IMAGE;
extern Image FAST_GHOST_IMAGE;

// extern uint16_t levelSlowDown;

extern uint8_t exploded_bombs;

uint16_t computeSkullSlowDown(void)
{
    if(loop<MAX_SKULL_LOOP)
    {
        #if !defined(WIDE)
            return INITIAL_GHOST_SLOWDOWN - ghostLevel*4;                
        #else
            return INITIAL_GHOST_SLOWDOWN - ghostLevel*8;        
        #endif
    }
    return SKULL_MIN_SLOWDOWN; // You must die!
}

void computeSkullParameters(void)
{
    skullSlowDown = computeSkullSlowDown();
    skullXCountDown = SKULL_COUNT_DOWN;
    skullYCountDown = SKULL_COUNT_DOWN;
}


// Redability macros
#define INACTIVITY_COUNT_DOWN_REACHED ((skullXCountDown==0) || (skullYCountDown==0))
#define GHOST_COUNT_TRIGGER_REACHED (ghostCount<=SKULL_TRIGGER+(level/4))
#define BOSS_LOOP_TRIGGER_REACHED (loop>=SKULL_LOOP_TRIGGER)

#define NON_BOSS_TRIGGER_REACHED (INACTIVITY_COUNT_DOWN_REACHED || GHOST_COUNT_TRIGGER_REACHED)

#if defined(__NCURSES__)
    #define SKULL_RAND_CONDITION ((_XL_RAND())>skullSlowDown)
#else
    #define SKULL_RAND_CONDITION (_XL_RAND()>skullSlowDown)
#endif


void skullDies(register Character * skullPtr)
{
        DRAW_BROKEN_BRICK(skullPtr->_x, skullPtr->_y);
        skullPtr->_status=0;
        if(!(--skullsCount))
        {
            skullActive = 0;
        }
        _XL_EXPLOSION_SOUND();
        points+=SKULL_POINTS;
        displayScoreStats();
        deleteSkull(skullPtr);
}


void handle_skull(Character *skullPtr, uint8_t strategy)
{
    if(skullPtr->_status)
    {
        if(!freezeActive && SKULL_RAND_CONDITION)
        {
            skullMoveTowardCharacter(skullPtr, strategy);
        }
        displaySkull(skullPtr);
        if (playerKilledBy(skullPtr))
        {
            playerDies();
        }
    }
}

void handle_skulls(void)
{
    if(!skullActive)
    {   
        if((!isBossLevel && NON_BOSS_TRIGGER_REACHED) || 
           (isBossLevel && BOSS_LOOP_TRIGGER_REACHED))
        {
            #if !defined(DEBUG_NO_SKULL)
            skullActive = 1;
            #endif
            
            if(!isBossLevel && INACTIVITY_COUNT_DOWN_REACHED)
            {
                setSecret(SKULLS_ACTIVATED_SECRET_INDEX);
            }
        }
        else
        {
            --skullXCountDown;
            --skullYCountDown;
        }
    }
    else
    {     
        uint8_t i;

        if(!confuseActive || !(loop&15))
        {
            skullSlowDown = computeSkullSlowDown();

            for(i=0;i<SKULLS_NUMBER;++i)
            {
                handle_skull(&skulls[i],i*2);
            }
        }
        
        if(isBossLevel && skulls[BOSS_INDEX]._status && !(loop&63) && (ghostCount<=BOSS_LEVEL_GHOSTS_NUMBER))
        {
            i=0;
            while((i<BOSS_LEVEL_GHOSTS_NUMBER)&&(ghosts[i]._status))
            {
                ++i;
            }
            if(i<BOSS_LEVEL_GHOSTS_NUMBER)
            {
                ++ghostCount;
                if(!(_XL_RAND()&7))
                {
                    initializeCharacter(&ghosts[i],skulls[BOSS_INDEX]._x, skulls[BOSS_INDEX]._y,GHOST_LIFE/2,&FAST_GHOST_IMAGE);
                }
                else
                {
                    initializeCharacter(&ghosts[i],skulls[BOSS_INDEX]._x, skulls[BOSS_INDEX]._y,GHOST_LIFE,&GHOST_IMAGE);
                }
                printGhostCountStats();
            }
            
        }
    }
}

void checkBombsVsSkull(register Character * skullPtr)
{
    uint8_t reachedBombInd = characterReachedBombs(skullPtr);
    
    if(skullPtr->_status && reachedBombInd<BOMBS_NUMBER)
    {        
        points+=GHOST_VS_BOMBS_BONUS;
        
        bombs[reachedBombInd]._status = 0;
        deleteBomb(&bombs[reachedBombInd]);
        ++exploded_bombs;
        _XL_EXPLOSION_SOUND();
        
        if(skullPtr->_status<=SKULL_BOMB_DAMAGE)
        {
            skullDies(skullPtr);
        }
        else
        {
            skullPtr->_status-=SKULL_BOMB_DAMAGE;
        }
    }
}

void checkBombsVsSkulls(void)
{
    uint8_t i;
    for(i=0;i<SKULLS_NUMBER;++i)
      {
         checkBombsVsSkull(&skulls[i]);
      }
}   


void displaySkulls(void)
{
    uint8_t i;

    for(i=0;i<SKULLS_NUMBER;++i)
    {
        if (skulls[i]._status)
        {
            displaySkull(&skulls[i]);
        }
    }
}


