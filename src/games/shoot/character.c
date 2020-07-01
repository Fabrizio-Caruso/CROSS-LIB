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

#if defined(__CMOC__) && !defined(__WINCMOC__)
    #include <cmoc.h>
#else
    #include <stdlib.h>
    #include <string.h>
    #include <stdio.h>
#endif

#include "character.h"
#include "settings.h"
#include "level.h"

#include "../cross_lib/cross_lib.h"

#include "game_text.h"

extern uint16_t points;

extern uint8_t skullsCount;

extern uint8_t ghostCount;
extern uint16_t loop;
extern uint8_t level;

extern Image DEAD_GHOST_IMAGE;
extern Image GHOST_IMAGE;
extern Image BOMB_IMAGE;
extern Image BROKEN_BRICK_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character skulls[SKULLS_NUMBER];

extern Character player;

extern     uint8_t ghostsOnScreen;

void displayCharacter(register Character * characterPtr)
{
    DRAW_CHARACTER(characterPtr->_x, characterPtr->_y, characterPtr->_imagePtr);
}

void deleteCharacter(Character * characterPtr)
{
    DELETE_CHARACTER(characterPtr->_x, characterPtr->_y);
}

extern uint8_t invincibilityActive;

extern uint8_t innerVerticalWallX;
extern uint8_t innerVerticalWallY;
extern uint8_t innerVerticalWallLength;

extern uint8_t innerHorizontalWallX;
extern uint8_t innerHorizontalWallY;
extern uint8_t innerHorizontalWallLength;

extern uint8_t zombieActive;    

void playerDies(void)
{
    EXPLOSION_SOUND();
    player._status=0;
    #if !defined(LESS_TEXT)
        printDefeatMessage();
    #endif
    SLEEP(1);    
}

uint8_t playerKilledBy(Character *enemyPtr)
{
    return !invincibilityActive && areCharctersAtSamePosition(enemyPtr,&player);
}

void initializeCharacter(register Character* characterPtr, uint8_t x, uint8_t y, uint8_t status, Image * imagePtr)
{
    characterPtr->_x = x;
    characterPtr->_y = y;
    characterPtr->_status = status;
    characterPtr->_imagePtr = imagePtr;
    
    #if defined(DEBUG)
        displayCharacter(characterPtr);
    #endif
}


uint8_t isCharacterAtLocation(uint8_t x, uint8_t y, Character * characterPtr)
{
    return(characterPtr->_x==x) && (characterPtr->_y==y);
}


uint8_t wallReached(register Character *characterPtr)
{
    return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
           (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
}


uint8_t sameLocationAsAnyLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length)
{
    uint8_t i;

    for(i=0;i<length;++i)
    {
        if(isCharacterAtLocation(x,y,&characterList[i]))
        {
            return i;
        }
    }    
    return length;
}


uint8_t sameLocationAsAnyActiveLocation(uint8_t x, uint8_t y, Character *characterList, uint8_t length)
{
    uint8_t i;

    for(i=0;i<length;++i)
    {
        if(characterList[i]._status && isCharacterAtLocation(x,y,&characterList[i]))
        {
            return i;
        }
    }    
    return length;
}


#if YSize<XSize
    #define SAFETY YSize/4
#else
    #define SAFETY XSize/4
#endif

// TODO: To be replaced with something cleaner
// also used with things different from global bombs
uint8_t safeLocation(uint8_t x, uint8_t y)
{
    return !((sameLocationAsAnyGhostLocation(x,y,ghosts,ghostsOnScreen)<ghostsOnScreen)
          || (sameLocationAsAnyLocation(x,y,bombs, BOMBS_NUMBER)<BOMBS_NUMBER)
          || (x<SAFETY) || (x>XSize-SAFETY) || (y<=SAFETY) || (y>YSize-SAFETY));
}


void relocateCharacter(register Character * characterPtr)
{
    uint8_t x; 
    uint8_t y;  
    
    do
    {
        x = characterPtr->_x - (uint8_t)(RELOCATE_RANGE/2) + (uint8_t)(rand() % RELOCATE_RANGE);
        y = characterPtr->_y - (uint8_t)(RELOCATE_RANGE/2) + (uint8_t)(rand() % RELOCATE_RANGE);

    } while(!safeLocation(x,y));

    characterPtr->_x = x;
    characterPtr->_y = y;
}

uint8_t innerWallReached(Character *characterPtr)
{
    return (characterPtr->_x==innerVerticalWallX) && (characterPtr->_y >= innerVerticalWallY) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength-1));
}


uint8_t nearInnerWall(register Character *characterPtr)
{
    return innerVerticalWallLevel() && (characterPtr->_x>=innerVerticalWallX-1) && (characterPtr->_x<=innerVerticalWallX+1) &&
           (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
}


uint8_t innerHorizontalWallReached(Character *characterPtr)
{
    return (characterPtr->_y==innerHorizontalWallY) && (characterPtr->_x >= innerHorizontalWallX) && (characterPtr->_x<= (innerHorizontalWallX + innerHorizontalWallLength-1));
}


uint8_t nearInnerHorizontalWall(register Character *characterPtr)
{
    return innerHorizontalWallLevel() && (characterPtr->_y>=innerHorizontalWallY-1) && (characterPtr->_y<=innerHorizontalWallY+1) &&
           (characterPtr->_x >= innerHorizontalWallX-1) && (characterPtr->_x<= (innerHorizontalWallX + innerHorizontalWallLength));
}    


void DRAW_BROKEN_BRICK(uint8_t x, uint8_t y)
{
    _draw(x,y,&BROKEN_BRICK_IMAGE);        
}

