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
#include "strategy.h"

#include "cross_lib.h"

extern uint8_t level;
#if !defined(TINY_GAME) || defined(_XL_TURN_BASED)
extern uint16_t loop;
#endif
extern uint8_t ghostCount;

extern Character player; 
extern Character ghosts[GHOSTS_NUMBER];

extern uint8_t strategyArray[GHOSTS_NUMBER];

extern uint16_t ghostSlowDown;

#if defined(FULL_GAME)
    extern uint8_t zombieActive;
#endif


uint8_t moveCharacter(register uint8_t *hunterOffsetPtr, register uint8_t *preyOffsetPtr)
{
    if((uint8_t) *((uint8_t *)hunterOffsetPtr) < (uint8_t) *((uint8_t *)preyOffsetPtr))
    {
        ++(*((uint8_t *) hunterOffsetPtr));
    }
    else if((uint8_t) *((uint8_t *) hunterOffsetPtr) > (uint8_t) *((uint8_t *)preyOffsetPtr))
    {
        --(*((uint8_t *) hunterOffsetPtr));
    }    
    else
    {
        return 0;
    }
    return 1;
}


#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
    uint8_t chaseXStrategy(Character* hunterPtr, Character* preyPtr)
    {
        // deleteGhost(hunterPtr);
		uint8_t x = hunterPtr->_x;
		// uint8_t y = hunterPtr->_y;

		
        if(moveCharacter((uint8_t *)hunterPtr + X_MOVE, 
                         (uint8_t *)preyPtr + X_MOVE))
        {
			_XL_DELETE(x,hunterPtr->_y);
            return 1;
        }
        else 
		{
			uint8_t y = hunterPtr->_y;

			if(moveCharacter((uint8_t *)hunterPtr + Y_MOVE, 
                                 (uint8_t *)preyPtr + Y_MOVE))
			{
				_XL_DELETE(x,y);
				return 1;
			}
		}
		return 0;
    }

    uint8_t chaseYStrategy(Character* hunterPtr, Character* preyPtr)
    {
        // deleteGhost(hunterPtr);
		uint8_t y = hunterPtr->_y;

        if(moveCharacter((uint8_t *)hunterPtr + Y_MOVE, 
                         (uint8_t *)preyPtr + Y_MOVE))
        {
			_XL_DELETE(hunterPtr->_x,y);

            return 1;
        }
        else
        {
			uint8_t x = hunterPtr->_x;

            if(moveCharacter((uint8_t *)hunterPtr + X_MOVE, 
                                 (uint8_t *)preyPtr + X_MOVE))
			{
				_XL_DELETE(x,y);
				return 1;
			}
        }
		return 0;
    }
#endif


// strategy: 
// 4 means do no prefer horizontal to vertical movement
// 0 means always horizontal
// 9 means always vertical
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
    uint8_t moveTowardCharacter(Character* preyPtr, Character *hunterPtr, uint8_t strategy)
    {
        if((_XL_RAND()&7) > strategy) // Select blind chase strategy
            { // 0 - 4
                if(chaseXStrategy(hunterPtr, preyPtr))
				{
					displayGhost(hunterPtr);
				}	return 1;
            }
        else
            { // 5 - 9
                if(chaseYStrategy(hunterPtr, preyPtr))
				{
					displayGhost(hunterPtr);
					return 1;
				}
            }
		return 0;
    }
#elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
    uint8_t moveTowardCharacter(Character* preyPtr, Character *hunterPtr)
    {
        uint8_t offset = (uint8_t) _XL_RAND()&1;
        

		uint8_t x = hunterPtr->_x;
		uint8_t y = hunterPtr->_y;
        if(moveCharacter((uint8_t *)hunterPtr + offset, 
                      (uint8_t *)preyPtr + offset))
		{
			_XL_DELETE(x,y);
			displayGhost(hunterPtr);
			return 1;
		}
		return 0;
    }    
#elif !defined(TINY_GAME)
    uint8_t moveTowardCharacter(Character *hunterPtr)
    {
        uint8_t offset = (uint8_t) _XL_RAND()&1;
        
		uint8_t x = hunterPtr->_x;
		uint8_t y = hunterPtr->_y;
        if(moveCharacter((uint8_t *)hunterPtr + offset,
                      (uint8_t *)(&player) + offset))
		{
			_XL_DELETE(x,y);
			displayGhost(hunterPtr);
			return 1;
		}
		return 0;
	}
#else
    void moveTowardCharacter(Character *hunterPtr)
    {
        uint8_t offset = (uint8_t) _XL_RAND()&1;
        
		deleteGhost(hunterPtr)
        moveCharacter((uint8_t *)hunterPtr + offset,(uint8_t *)(&player) + offset);
			// return 1;
		// }
		// return 0;
    }
#endif


// #if !defined(TINY_GAME) 
#if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
void computeStrategy(void)
{
    uint8_t i;
    uint8_t skew = level / 8;
    
    for(i=5; i<GHOSTS_NUMBER; ++i) // 3 (if total=8)
    {
        strategyArray[i] = 4+skew; // 4,5,6 
        strategyArray[9-i] = 2-skew; // 2,1,0
            
    }
    strategyArray[0] = 3;
    strategyArray[1] = 3;                
}
#endif

#define GHOST_RANDOM_CONDITION (_XL_RAND()>ghostSlowDown)


// #if defined(FULL_GAME)
// Ghosts move to new positions if they get their chanche
#if defined(FULL_GAME)
void chaseCharacter(Character *preyPtr)
#else
void chaseCharacter(void)    
#endif
{
    uint8_t i;
    
    _XL_WAIT_VSYNC();
    #if defined(DEBUG_GHOST_DISPLAY)
        SET_DEBUG_BORDER();
    #endif
    for(i=0;i<GHOSTS_NUMBER;++i)
    {
        #if defined(FULL_GAME)
            if((ghosts[i]._status || (zombieActive && loop&1)) && GHOST_RANDOM_CONDITION)
        #else
            if((ghosts[i]._status) && GHOST_RANDOM_CONDITION)    
        #endif
            {
                #if defined(FULL_GAME) && !defined(SIMPLE_STRATEGY)
                    if(!moveTowardCharacter(preyPtr, &ghosts[i], strategyArray[i]) || !((loop+i)&7))
					{
						displayGhost(&ghosts[i]);
					}
                #elif defined(FULL_GAME) && defined(SIMPLE_STRATEGY)
                    if(!moveTowardCharacter(preyPtr, &ghosts[i]) || !(loop&7))
					{
						displayGhost(&ghosts[i]);
					}
                #else
                    moveTowardCharacter(&ghosts[i]);    
                #endif
            }
            #if !defined(TINY_GAME)
            else
            {
				// Re-display()
				if(!((loop+i)&7))
				{
					displayGhost(&ghosts[i]);
				}
            }
            #else
            displayGhost(&ghosts[i]);
            #endif
    }
    #if defined(DEBUG_GHOST_DISPLAY)
        UNSET_DEBUG_BORDER();
    #endif
}

