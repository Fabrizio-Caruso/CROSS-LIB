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

#include<vic20.h>

#include <stdint.h>

#include "sleep_macros.h"

// #define _pause() _XL_SLOW_DOWN(100)

void _pause(void)
{
    _XL_SLOW_DOWN(100);
    // uint8_t i;
    // for(i=0;i<254;++i) 
        // { 
        // } 			
}

void silence(void)
{
    VIC.noise = 0x00; 
    VIC.voice1 = 0x00;
    VIC.volume_color = 0x00;    
}

void _explosion_sound(uint8_t freq, uint8_t duration)
{ 
    uint8_t j;
    VIC.noise = freq; 
    VIC.volume_color |= 15; 

    for(j=0;j<duration;++j) 
    { 
        _pause();
    } 
    silence();
};	

void _ping_sound(uint8_t freq)
{ 
    VIC.voice1 = freq; 
    VIC.volume_color |= 0x08; 
    _pause();
    silence();
};


void _XL_ZAP_SOUND() 
{ 
    uint8_t j;
    VIC.volume_color |= 0x0B; 
    for(j=0;j<14;++j) 
    { \
        VIC.voice1 = j<<4;
        _pause(); _pause();
    } \
    silence();
};		

    

