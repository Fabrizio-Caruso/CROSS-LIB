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

#include "level.h"
#include "strategy.h"

#include "../cross_lib/display/display_macros.h"

extern uint16_t loop;
extern uint8_t level;

extern Character leftHorizontalMissile;
extern Character rightHorizontalMissile;
extern Character player;

extern uint8_t arrowRange;
extern uint8_t arrowYPosition;

extern uint8_t isOneMissileLevel;
extern uint8_t isMissileLevel;
extern uint8_t isBossLevel;

	#if !defined(NO_ARROW_RANGE_CHECK)
    uint8_t _playerInArrowRange(void)
    {
        return (loop&1 && player._y<=(arrowYPosition+arrowRange) && player._y>=(arrowYPosition-arrowRange));
    }
    #endif
    
    void _handle_from_the_left(void)
    {
        if(leftHorizontalMissile._status)
        {
            if(handle_player_killed(&leftHorizontalMissile))
            {
                return;
            }

            deleteHorizontalMissile(&leftHorizontalMissile);
            if(leftHorizontalMissile._x==XSize-2)
            {
                leftHorizontalMissile._x=0;
                leftHorizontalMissile._y =  arrowYPosition;
            }
            else
            {
                ADVANCED_LEFT_MISSILE();
				#if !defined(NO_ARROW_RANGE_CHECK)
                if(_playerInArrowRange())
                {
                    if(player._x>=leftHorizontalMissile._x)
                    {
                        (void) moveCharacter((uint8_t *)&leftHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
                    }
                }
				#endif
            }
            displayHorizontalMissile(&leftHorizontalMissile);
            
            handle_player_killed(&leftHorizontalMissile);
        }        
    }
    
    void _handle_from_the_right(void)
    {
        if(rightHorizontalMissile._status)
        {
            if(handle_player_killed(&rightHorizontalMissile))
            {
                return;
            }

            deleteHorizontalMissile(&rightHorizontalMissile);
            if(rightHorizontalMissile._x==1)
            {
                rightHorizontalMissile._x= XSize-1;
                rightHorizontalMissile._y = arrowYPosition;
            }
            else
            {
                ADVANCED_RIGHT_MISSILE();

				#if !defined(NO_ARROW_RANGE_CHECK)
                if(_playerInArrowRange())
                {
                    if(player._x<= rightHorizontalMissile._x)    
                    {
                        (void) moveCharacter((uint8_t *)&rightHorizontalMissile+Y_MOVE, (uint8_t *)&player+Y_MOVE);			
                    }
                }
				#endif
            }
            displayHorizontalMissile(&rightHorizontalMissile);    
            
            handle_player_killed(&rightHorizontalMissile);
        }        
    }
    
    void handle_horizontal_missiles(void)
    {    
        if(isOneMissileLevel)
        {
            arrowYPosition = YSize/2;
            _handle_from_the_right();
        }    
        else if(isMissileLevel || isBossLevel)
        {
            arrowYPosition = HORIZONTAL_MISSILE_OFFSET;
            _handle_from_the_right();
            
            arrowYPosition = YSize-1-HORIZONTAL_MISSILE_OFFSET; 
            _handle_from_the_left();
        }

    }
    

