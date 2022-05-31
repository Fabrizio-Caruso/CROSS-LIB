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
#include "init_images.h"

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image FAST_GHOST_IMAGE;

extern uint8_t maxGhostsOnScreen;
extern uint16_t ghostLevel;
extern uint8_t level;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];

extern uint8_t guns;

extern uint8_t bulletStrength;

extern uint8_t isBossLevel;

extern uint8_t exploded_bombs;

extern uint8_t restart;

void checkBombsVsGhost(register Character * ghostPtr)
{
    uint8_t reachedBombInd = characterReachedBombs(ghostPtr);
    
    if(ghostPtr->_status && reachedBombInd<BOMBS_NUMBER)
    {
        // points+=GHOST_VS_BOMBS_BONUS;
        increasePoints(GHOST_VS_BOMBS_BONUS);
        bombs[reachedBombInd]._status = 0;
        ghostDiesAndSpawns(ghostPtr);
        ++exploded_bombs;
    }
}



void checkBombsVsGhosts(void)
{
    uint8_t i;
    
    for(i=0;i<maxGhostsOnScreen;++i)
      {
         checkBombsVsGhost(&ghosts[i]);
      }
}


void spawnGhost(Character *ghostPtr, uint8_t ghostIndex)
{
    if(!isBossLevel)
    {
        if((ghostCount>=FAST_GHOST_COUNT_MIN_THRESHOLD) && (!(ghostIndex % 4)) && (guns>=FAST_GHOST_GUN_THRESHOLD) && !restart)
        {
            if(!(ghostIndex % 8))
            {
                initializeCharacter(ghostPtr  ,  1      ,     1, FAST_GHOST_LIFE, &FAST_GHOST_IMAGE);
            }
            else
            {
                initializeCharacter(ghostPtr  ,XSize-2, YSize-2, FAST_GHOST_LIFE, &FAST_GHOST_IMAGE);
            }
        }
        else
        {
            switch(ghostIndex % 8)
            {
                case 0:
                    initializeCharacter(ghostPtr  , 1      ,     1, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 1:
                    initializeCharacter(ghostPtr, XSize-2, YSize-2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 2:
                    initializeCharacter(ghostPtr, 1      , YSize-2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 3:
                    initializeCharacter(ghostPtr, XSize-2,       1, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 4:
                    initializeCharacter(ghostPtr, XSize-2, YSize/2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 5:
                    initializeCharacter(ghostPtr  , 1    , YSize/2, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 6:
                    initializeCharacter(ghostPtr, XSize/2,       1, GHOST_LIFE, &GHOST_IMAGE);
                break;
                case 7:
                    initializeCharacter(ghostPtr, XSize/2, YSize-2, GHOST_LIFE, &GHOST_IMAGE);
                break;    
            }
        }
    }
}


void ghostDies(register Character * ghostPtr)
{
    _XL_DRAW(ghostPtr->_x, ghostPtr->_y, _GHOST_TILE, _XL_RED);
    _XL_EXPLOSION_SOUND();
    
    if(ghostPtr->_imagePtr==&FAST_GHOST_IMAGE)
    {
        increasePoints(FAST_GHOST_EXTRA_POINTS);
    }
    
    ghostPtr->_status=0;
    // displayScoreStats();
    
    --ghostCount;
    deleteGhost(ghostPtr);
    // printGhostCountStats();
    displayStats();
}


void ghostDiesAndSpawns(Character * ghostPtr)
{
    ghostDies(ghostPtr);
    if((!isBossLevel) && (ghostCount>=maxGhostsOnScreen))
    {
        spawnGhost(ghostPtr,ghostCount);
    }
}


uint16_t computeGhostSlowDown(void)
{
    if(ghostLevel<MAX_GHOST_LEVEL)
    {
        #if !defined(WIDE)
            return INITIAL_GHOST_SLOWDOWN-ghostLevel*4;        
        #else
            return INITIAL_GHOST_SLOWDOWN-ghostLevel*8;
        #endif
    }
    return GHOST_MIN_SLOWDOWN;
}


void decreaseGhostLevel(void)
{
    if(ghostLevel>GHOST_LEVEL_DECREASE)
        ghostLevel-=GHOST_LEVEL_DECREASE;
    else
        ghostLevel=0;
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


uint8_t sameLocationAsAnyGhostLocation(uint8_t x, uint8_t y, Character *ghostList, uint8_t length)
{
    uint8_t i;

    for(i=0;i<length;++i)
    {
        if(ghostList[i]._status && isCharacterAtLocation(x,y,&(ghostList[i])))
        {
            return i;
        }
    }    
    return length;
}

