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

extern Image HORIZONTAL_BRICK_IMAGE;
extern Image VERTICAL_BRICK_IMAGE;

extern Character ghosts[GHOSTS_NUMBER];
extern Character bombs[BOMBS_NUMBER];
extern Character skulls[SKULLS_NUMBER];

extern Character player;

extern uint8_t maxGhostsOnScreen;

extern uint8_t isInnerHorizontalWallLevel;
extern uint8_t isInnerVerticalWallLevel;

extern uint8_t invincibilityActive;
extern uint8_t destroyerActive;

void _DRAW_PLAYER(void)
{
    if(destroyerActive)
    {
        DRAW_PLAYER(player._x, player._y, &DESTROYER_IMAGE);
    }
#if !defined(_XL_NO_COLOR)
    // Color case: player gets a different color when invincible
    else if(invincibilityActive)
    {
        player._imagePtr->_color = _XL_YELLOW;
        DRAW_PLAYER(player._x, player._y, player._imagePtr);
        player._imagePtr->_color = _XL_CYAN;
    }
#endif
    else
    {
        DRAW_PLAYER(player._x, player._y, player._imagePtr);
    }
}


#if !defined(NO_BLINKING)
void _blink_draw(uint8_t x, uint8_t y, Image * image, uint8_t *blinkCounter) 
{
    if(*blinkCounter) 
    {
        _draw(x,y,image);
        *blinkCounter=0;
    } 
    else 
    {
        _delete(x,y);
        *blinkCounter=1;
    }    
}
#endif


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

void playerDies(void)
{
    _XL_EXPLOSION_SOUND();
    player._status=0;
    #if !defined(LESS_TEXT)
        printDefeatMessage();
    #endif
    _XL_SLEEP(1);    
}

uint8_t playerKilledBy(Character *enemyPtr)
{
    return (!invincibilityActive) && areCharctersAtSamePosition(enemyPtr,&player);
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


uint8_t onWall(uint8_t x, uint8_t y)
{
    return(x==0)||(x==XSize-1) || 
          (y==0)||(y==YSize-1);
}

uint8_t wallReached(register Character *characterPtr)
{
    return (characterPtr->_x==0)||(characterPtr->_x==XSize-1) || 
           (characterPtr->_y==0)||(characterPtr->_y==YSize-1);
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
       return (x>=SAFETY) && (x<=XSize-SAFETY) && (y>SAFETY) && (y<=YSize-SAFETY);
}


void relocateNearBy(register Character * characterPtr)
{
    uint8_t x; 
    uint8_t y;  
    
    do
    {
        x = characterPtr->_x - (uint8_t)(RELOCATE_RANGE/2) + (uint8_t)(_XL_RAND() % RELOCATE_RANGE);
        y = characterPtr->_y - (uint8_t)(RELOCATE_RANGE/2) + (uint8_t)(_XL_RAND() % RELOCATE_RANGE);

    } while(!safeLocation(x,y));

    characterPtr->_x = x;
    characterPtr->_y = y;
}

uint8_t innerVerticalWallReached(uint8_t x, uint8_t y)
{
    return (x==innerVerticalWallX) && (y >= innerVerticalWallY) && (y <= (innerVerticalWallY + innerVerticalWallLength-1));
}


uint8_t nearInnerVerticalWall(register Character *characterPtr)
{
    return isInnerVerticalWallLevel && (characterPtr->_x>=innerVerticalWallX-1) && (characterPtr->_x<=innerVerticalWallX+1) &&
           (characterPtr->_y >= innerVerticalWallY-1) && (characterPtr->_y<= (innerVerticalWallY + innerVerticalWallLength));
}



uint8_t innerHorizontalWallReached(uint8_t x, uint8_t y)
{
    return (y==innerHorizontalWallY) && (x >= innerHorizontalWallX) && (x<= (innerHorizontalWallX + innerHorizontalWallLength-1));
}


uint8_t nearInnerHorizontalWall(register Character *characterPtr)
{
    return isInnerHorizontalWallLevel && (characterPtr->_y>=innerHorizontalWallY-1) && (characterPtr->_y<=innerHorizontalWallY+1) &&
           (characterPtr->_x >= innerHorizontalWallX-1) && (characterPtr->_x<= (innerHorizontalWallX + innerHorizontalWallLength));
}    


void DRAW_BROKEN_BRICK(uint8_t x, uint8_t y)
{
    _draw(x,y,&BROKEN_BRICK_IMAGE);        
}

