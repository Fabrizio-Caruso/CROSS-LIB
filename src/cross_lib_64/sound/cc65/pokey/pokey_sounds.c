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

#include "pokey_sounds.h"

#include <stdint.h>

void _explosion_sound(uint8_t freq)
{
    uint16_t i; 
    POKEY_WRITE.audctl = 0x00; 
    POKEY_WRITE.skctl = 2;
    POKEY_WRITE.audf1 = freq; 
    POKEY_WRITE.audc1 = 0x07; 
    for(i=150;i>0;--i) 
    { 
        POKEY_WRITE.audc1 = i/10u; 
    } 
}


void _ping_sound(uint8_t freq)
{ 
    uint16_t i; 
    POKEY_WRITE.audctl = 0x00; 
    POKEY_WRITE.skctl = 2;		
    POKEY_WRITE.audf1 = freq; 
    POKEY_WRITE.audc1 = 0xA7; 
    for(i=700;i>0;--i) 
    { 
    } 
    POKEY_WRITE.audc1 = 0x00; 
};


void _XL_ZAP_SOUND(void) 
{ 
    uint16_t i; 
    uint16_t j; 
    POKEY_WRITE.audctl = 0x00; 
    POKEY_WRITE.skctl = 2;		
    POKEY_WRITE.audf1 = 0x02;
    POKEY_WRITE.audc1 = 0xA7; 
    for(i=0;i<100;++i) 
    { 
        for(j=0;j<50;++j) { } 
        POKEY_WRITE.audf1 = 100+i; 
    } 
    POKEY_WRITE.audc1 = 0x00; 
};

