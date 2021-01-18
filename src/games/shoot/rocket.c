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


#include "../cross_lib/display/display_macros.h"

#include "settings.h"
#include "character.h"
#include "level.h"

extern uint8_t rocketsOnScreen;

extern Character rockets[ROCKETS_NUMBER];

extern uint8_t ghostCount;

extern uint8_t isRocketLevel;
extern uint8_t isBossLevel;

    void handle_rockets(void)
    {    
        if((ghostCount<=MAX_GHOST_COUNT_FOR_ROCKETS && isRocketLevel) || isBossLevel)
        { 
            uint8_t i;

            for(i=0;i<rocketsOnScreen;++i)
            {
                if(rockets[i]._status)
                {
                    if(playerKilledBy(&rockets[i]))
                    {
                        playerDies();
                    }
                    
                    #if defined(TURNED_BASED)
                    if(1)
                    #else
                    if(_XL_RAND()&1)
                    #endif
                    {
                        deleteRocket(&rockets[i]);
                        --(rockets[i]._y);
                    }

                    displayRocket(&rockets[i]);            
                    if(rockets[i]._y<=1)
                    {    
                        deleteRocket(&rockets[i]);
                        rockets[i]._x = (i+1)*(XSize/(rocketsOnScreen+1));
                        rockets[i]._y = YSize-2;                            
                    }
                }
            }
        }
    }



