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
#if !defined(TINY_GAME)

#include "settings.h"

#include "skull.h"
#include "level.h"
#include "strategy.h"

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

#if defined(FULL_GAME)
    extern uint8_t confuseActive;
#endif

uint16_t computeSkullSlowDown(void)
{
    if(loop<MAX_SKULL_LOOP)
    {
        #if defined(TURN_BASED)
            return INITIAL_SKULL_SLOWDOWN - level * 256 - ghostLevel*8;                
        #else
            return INITIAL_SKULL_SLOWDOWN - level * 256 - ghostLevel*16;        
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
#define GHOST_COUNT_TRIGGER_REACHED (ghostCount<=SKULL_TRIGGER)
#define BOSS_LOOP_TRIGGER_REACHED (loop>=SKULL_LOOP_TRIGGER)

#define NON_BOSS_TRIGGER_REACHED (INACTIVITY_COUNT_DOWN_REACHED || GHOST_COUNT_TRIGGER_REACHED)

#if defined(__NCURSES__)
    #define SKULL_RAND_CONDITION ((rand()&0x7fff)>skullSlowDown)
#else
    #define SKULL_RAND_CONDITION (rand()>skullSlowDown)
#endif


void handle_skull(Character *skullPtr)
{
    if(skullPtr->_status)
    {
        if(SKULL_RAND_CONDITION)
        {
            TOCK_SOUND();
            deleteSkull(skullPtr);
            skullMoveTowardCharacter(&player, skullPtr, 4);
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
        if(skullsCount==SKULLS_NUMBER && 
            ((!bossLevel() && NON_BOSS_TRIGGER_REACHED) || 
             (bossLevel() && BOSS_LOOP_TRIGGER_REACHED)))
        {
            #if !defined(DEBUG_NO_SKULL)
            skullActive = 1;
            #endif
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

        if(!confuseActive || !(loop&3))
        {
            skullSlowDown = computeSkullSlowDown();

            for(i=0;i<SKULLS_NUMBER;++i)
            {
                handle_skull(&skulls[i]);
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
        EXPLOSION_SOUND();
        
        if(skullPtr->_status<=10)
        {
            skullPtr->_status=0;
            --skullsCount;
            EXPLOSION_SOUND();
        }
        else
        {
            (skullPtr->_status)>>=1;
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
#endif

