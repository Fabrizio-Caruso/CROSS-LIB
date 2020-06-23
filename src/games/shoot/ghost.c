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


#include "ghost.h"
#include "settings.h"
#include "game_text.h"
#include "level.h"
#include "character.h"

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;

extern uint8_t ghostsOnScreen;
extern uint16_t ghostLevel;
extern uint8_t level;

extern Ghost ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];


void checkBombsVsGhost(register Ghost * ghostPtr)
{
    uint8_t reachedBombInd = characterReachedBombs((Character *)ghostPtr);
    if(((Character *)ghostPtr)->_status && reachedBombInd<BOMBS_NUMBER)
    {
        points+=GHOST_VS_BOMBS_BONUS;

        #if !defined(TINY_GAME)
            ((Character *)ghostPtr)->_x=1+GHOSTS_NUMBER-ghostCount;
        #else
            ((Character *)ghostPtr)->_x=1;
        #endif
        ((Character *)ghostPtr)->_y=1;


        bombs[reachedBombInd]._status = 0;
        deleteBomb(&bombs[reachedBombInd]);

        ghostDies(ghostPtr);
    }
}



void checkBombsVsGhosts(void)
{
    uint8_t i;
    for(i=0;i<ghostsOnScreen;++i)
      {
         checkBombsVsGhost(&ghosts[i]);
      }
}



void initializeGhost(Ghost *ghostPtr, uint8_t x, uint8_t y, uint8_t status, uint8_t type, Image *imagePtr)
{
    initializeCharacter(&(ghostPtr->_character),x,y,status,imagePtr);
    ghostPtr->type=type;
}

void spawnGhost(Ghost *ghostPtr, uint8_t ghostIndex)
{
    switch(ghostIndex % GHOSTS_NUMBER)
    {
        case 0:
            initializeGhost(ghostPtr  , 2      ,     2, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;
        case 1:
            initializeGhost(ghostPtr, XSize-3, YSize-3, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;
        case 2:
            initializeGhost(ghostPtr, 2      , YSize-3, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;
        case 3:
            initializeGhost(ghostPtr, XSize-3,       2, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;
        case 4:
            initializeGhost(ghostPtr  , 2    , YSize/2, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;
        case 5:
            initializeGhost(ghostPtr, XSize/2, YSize-3, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;
        case 6:
            initializeGhost(ghostPtr, XSize-3, YSize/2, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;
        case 7:
            initializeGhost(ghostPtr, XSize/2,       2, GHOST_LIFE, 0, &GHOST_IMAGE);
        break;    
    }        
}


void ghostDies(Ghost * ghostPtr)
{
    EXPLOSION_SOUND();
    
    ghostPtr->_character._status=0;
    displayStats();
    
    --ghostCount;
    if(ghostCount>=ghostsOnScreen)
    {
        spawnGhost(ghostPtr, ghostCount % ghostsOnScreen);
    }
    printGhostCountStats();
}


#if !defined(TINY_GAME)
uint16_t computeGhostSlowDown(void)
{
    if(ghostLevel<MAX_GHOST_LEVEL)
    {
        #if defined(TURN_BASED)
            return INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256-ghostLevel*4;        
        #else
            return INITIAL_GHOST_SLOWDOWN-(uint16_t)level*256-ghostLevel*8;
        #endif
    }
    return GHOST_MIN_SLOWDOWN;
}
#endif

#if !defined(TINY_GAME)
    void decreaseGhostLevel(void)
    {
        if(ghostLevel>GHOST_LEVEL_DECREASE)
            ghostLevel-=GHOST_LEVEL_DECREASE;
        else
            ghostLevel=0;
    }
#endif

void displayGhosts(void)
{
    uint8_t i;

    for(i=0;i<GHOSTS_NUMBER;++i)
    {
        if (ghosts[i]._character._status)
        {
            displayGhost((Character *)&ghosts[i]);
        }
    }
}

void displayBombs(void)
{
    uint8_t i;

    for(i=0;i<BOMBS_NUMBER;++i)
    {
        if(bombs[i]._status)
        {
            displayBomb(&bombs[i]);
        }
    }
}    

#if defined(FULL_GAME)
    uint8_t firstAlive(void)
    {
        uint8_t i;
        
        i=0;
        while(i<GHOSTS_NUMBER)
        {
            if(ghosts[i]._character._status)
                return i;
            ++i;
        }
        return GHOSTS_NUMBER;
    }
#endif


uint8_t sameLocationAsAnyGhostLocation(uint8_t x, uint8_t y, Ghost *ghostList, uint8_t length)
{
    uint8_t i;

    for(i=0;i<length;++i)
    {
        if(ghostList[i]._character._status && isCharacterAtLocation(x,y,&(ghostList[i]._character)))
        {
            return i;
        }
    }    
    return length;
}

