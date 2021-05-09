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

#ifndef _GAMATE_SOUNDS
#define _GAMATE_SOUNDS

#include <peekpoke.h>

#include "standard_libs.h"

#include "pet_sounds.h"


void beep(uint8_t value)
{
    uint8_t i;
    
    POKE(SHIFT_REG, 15);
    POKE(AUX_CTRL_REG,16);
    POKE(TIMER_2_LOW,value);
    for(i=0;i<100;++i){};
    POKE(AUX_CTRL_REG,0);
}


void noise(uint8_t duration)
{
    uint8_t i;
    uint8_t j;
    
    POKE(SHIFT_REG,15);
    POKE(AUX_CTRL_REG,16);
    for(i=0;i<duration;++i)
    {
        POKE(TIMER_2_LOW,(uint8_t)rand()&0xFF);
        for(j=0;j<8;++j){};
    }
    POKE(AUX_CTRL_REG,0);    
}


void _XL_ZAP_SOUND(void)
{
    uint8_t i;
    
    POKE(SHIFT_REG, 15);
    POKE(AUX_CTRL_REG,16);
    for(i=255;i>0;--i)
    {
        POKE(TIMER_2_LOW,i);
    }
    POKE(AUX_CTRL_REG,0);
}


#endif // _GAMATE_SOUNDS



