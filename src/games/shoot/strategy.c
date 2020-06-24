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
#include "character.h"
#include "strategy.h"
#include "level.h"

extern uint8_t level;
extern uint16_t loop;
extern uint8_t ghostCount;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern uint8_t strategyArray[GHOSTS_NUMBER];

extern uint8_t innerVerticalWallX;
extern uint8_t innerVerticalWallY;
extern uint8_t innerVerticalWallLength;

extern uint8_t innerHorizontalWallX;
extern uint8_t innerHorizontalWallY;
extern uint8_t innerHorizontalWallLength;


#if defined(FULL_GAME)
extern uint8_t zombieActive;
#endif


#if defined(__NCURSES__)
    #define GHOST_RANDOM_CONDITION ((rand()&0x7fff)>slowDown)
#else
    #define GHOST_RANDOM_CONDITION (rand()>slowDown)
#endif


// Required by horizontal missile
#if defined(FULL_GAME)
    uint8_t moveCharacter(Character* hunterPtr, Character* preyPtr, uint8_t offset)
    {
        if((uint8_t) *((uint8_t *)hunterPtr+offset) < (uint8_t) *((uint8_t *)preyPtr+offset))
        {
            ++(*((uint8_t *) hunterPtr+offset));        
        }
        else if((uint8_t) *((uint8_t *) hunterPtr+offset) > (uint8_t) *((uint8_t *)preyPtr+offset))
        {
            --(*((uint8_t *) hunterPtr+offset));        
        }    
        else
        {
            return 0;
        }
        return 1;
    }
#else
    uint8_t moveCharacter(Character* hunterPtr, uint8_t offset)    
    {
        if((uint8_t) *((uint8_t *)hunterPtr+offset) < (uint8_t) *((uint8_t *)(&player)+offset))
        {
            ++(*((uint8_t *) hunterPtr+offset));        
        }
        else if((uint8_t) *((uint8_t *) hunterPtr+offset) > (uint8_t) *((uint8_t *)(&player)+offset))
        {
            --(*((uint8_t *) hunterPtr+offset));        
        }    
        else
        {
            return 0;
        }
        return 1;
    }    
#endif


#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
    void blindChaseCharacterXStrategy(Character* hunterPtr, Character* preyPtr)
    {
        if(moveCharacter(hunterPtr, preyPtr,X_MOVE))
        {
            return;
        }
        else
        {
            moveCharacter(hunterPtr, preyPtr,Y_MOVE);
        }
    }

    void blindChaseCharacterYStrategy(Character* hunterPtr, Character* preyPtr)
    {
        if(moveCharacter(hunterPtr, preyPtr,Y_MOVE))
        {
            return;
        }
        else
        {
            moveCharacter(hunterPtr, preyPtr,X_MOVE);
        }
    }
#endif


uint8_t inCorridor(Character *hunterPtr)
{
    return (hunterPtr->_y<innerVerticalWallY-1) || (hunterPtr->_y>YSize-innerVerticalWallY);
}
    
uint8_t leftSide(Character *characterPtr)
{
    return characterPtr->_x < (XSize/2);
}    

uint8_t rightSide(Character *characterPtr)
{
    return characterPtr->_x > (XSize/2);
}

uint8_t sameSide(Character *preyPtr, Character *hunterPtr)
{
    return ((leftSide(preyPtr) && leftSide(hunterPtr)) || (rightSide(preyPtr) && rightSide(hunterPtr)));
}


// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
void moveTowardCharacter(Character* preyPtr, register Character *hunterPtr, uint8_t strategy)
{
    if(sameSide(preyPtr, hunterPtr)) // same side
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("same vertical side      ");           
        #endif
        if(rand()%10 > strategy) // Select blind chase strategy
            { // 0 - 4
                blindChaseCharacterXStrategy(hunterPtr, preyPtr);    
            }
            else
            { // 5 - 9
                blindChaseCharacterYStrategy(hunterPtr, preyPtr);
            }            
    }
    else if(inCorridor(hunterPtr)) // hunter in vertical corridor
    {
        #if defined(DEBUG_STRATEGY)   
        gotoxy(4,1);cprintf("in horizontal corridor  ");           
        #endif
        blindChaseCharacterXStrategy(hunterPtr, preyPtr);            
    }
    else if((preyPtr->_x)!=(XSize/2)) // hunter behind the wall
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("behind the wall         ");
        #endif
        if(hunterPtr->_y>(YSize/2))
        {
            ++(hunterPtr->_y);
        } 
        else 
        {
            --(hunterPtr->_y);
        }                
    }
    else // prey aligned with the wall
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("aligned with the wall  ");          
        #endif
        if(hunterPtr->_x==XSize/2) // both aligned with the wall
        {
            ++(hunterPtr->_x);
        }
        else
        {
            blindChaseCharacterYStrategy(hunterPtr, preyPtr);
        }            
    }
}

uint8_t inVerticalCorridor(Character *hunterPtr)
{
    return (hunterPtr->_x<innerHorizontalWallX-1) || (hunterPtr->_x>XSize-innerHorizontalWallX);
}
    
uint8_t topSide(Character *characterPtr)
{
    return characterPtr->_y < (YSize/2);
}    

uint8_t bottomSide(Character *characterPtr)
{
    return characterPtr->_y > (YSize/2);
}

uint8_t sameHorizontalSide(Character *preyPtr, Character *hunterPtr)
{
    return ((topSide(preyPtr) && topSide(hunterPtr)) || (bottomSide(preyPtr) && bottomSide(hunterPtr)));
}


void horizontalWallMoveTowardCharacter(Character* preyPtr, register Character *hunterPtr, uint8_t strategy)
{
    if(sameHorizontalSide(preyPtr, hunterPtr))
    {
        #if defined(DEBUG_STRATEGY)
        gotoxy(4,1);cprintf("same horizontal side  ");   
        #endif
        if(rand()%10 > strategy) // Select blind chase strategy
            { // 0 - 4
                blindChaseCharacterXStrategy(hunterPtr, preyPtr);    
            }
            else
            { // 5 - 9
                blindChaseCharacterYStrategy(hunterPtr, preyPtr);
            }            
    }
    else if(inVerticalCorridor(hunterPtr))
    {
        #if defined(DEBUG_STRATEGY) 
        gotoxy(4,1);cprintf("in vertical corridor  ");        
        #endif
        blindChaseCharacterYStrategy(hunterPtr, preyPtr);            
    }
    else if((preyPtr->_y)!=(YSize/2))
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("behind the wall       ");          
        #endif
        if(hunterPtr->_y>(YSize/2))
        {
            ++(hunterPtr->_x);
        } 
        else 
        {
            --(hunterPtr->_x);
        }                
    }
    else
    {
        #if defined(DEBUG_STRATEGY)        
        gotoxy(4,1);cprintf("aligned with the wall ");         
        #endif
        if(hunterPtr->_y==YSize/2)
        {
            ++(hunterPtr->_y);
        }
        else
        {
            blindChaseCharacterXStrategy(hunterPtr, preyPtr);
        }            
    }
}



void skullMoveTowardCharacter(Character *preyPtr, Character *hunterPtr, uint8_t strategy)
{
    if(rand()%10 > strategy) // Select blind chase strategy
        { // 0 - 4
            blindChaseCharacterXStrategy(hunterPtr, preyPtr);    
        }
        else
        { // 5 - 9
            blindChaseCharacterYStrategy(hunterPtr, preyPtr);
        }    
}


// #if !defined(TINY_GAME) 
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
void computeStrategy(void)
{
    uint8_t i;
    uint8_t skew = (level - 1) / 5;
    
    for(i=5; i<GHOSTS_NUMBER; ++i) // 3 (if total=8)
    {
        strategyArray[i] = 5+skew; // 6,7,8,(9 if GHOSTS are 9) (prefer Y (60%, 70%, 80%, 90)
        strategyArray[9-i] = 3-skew; // 4,3,2,(1 if GHOSTS are 9) prefer X (60%, 70%, 80%, 90%)
            
    }
    strategyArray[0] = 4;
    strategyArray[1] = 4;                
}
#endif


// #if defined(FULL_GAME)
// Ghosts move to new positions if they get their chanche
#if defined(FULL_GAME)
void chaseCharacter(Character *preyPtr, uint16_t slowDown)
#else
void chaseCharacter(uint16_t slowDown)    
#endif
{
    uint8_t i;
    
    if(innerVerticalWallLevel())
    {
        for(i=0;i<GHOSTS_NUMBER;++i)
        {
            #if defined(FULL_GAME)
                if((ghosts[i]._status || (zombieActive && loop&1)) && GHOST_RANDOM_CONDITION)
            #else
                if((ghosts[i]._status) && GHOST_RANDOM_CONDITION)    
            #endif
            {
                deleteGhost(&ghosts[i]);
                #if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
                    moveTowardCharacter(preyPtr, (Character *)&ghosts[i], strategyArray[i]);    
                #elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
                    moveTowardCharacter(preyPtr, (Character *)&ghosts[i]);        
                #endif
            }
        }
    }
    else
    {
        for(i=0;i<GHOSTS_NUMBER;++i)
        {
            #if defined(FULL_GAME)
                if((ghosts[i]._status || (zombieActive && loop&1)) && GHOST_RANDOM_CONDITION)
            #else
                if((ghosts[i]._status) && GHOST_RANDOM_CONDITION)    
            #endif
            {
                deleteGhost(&ghosts[i]);
                #if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
                    horizontalWallMoveTowardCharacter(preyPtr, (Character *)&ghosts[i], strategyArray[i]);    
                #elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
                    horizontalWallMoveTowardCharacter(preyPtr, (Character *)&ghosts[i]);        
                #endif
            }
        }  
    }        
}
