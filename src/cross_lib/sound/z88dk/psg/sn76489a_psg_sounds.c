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

// PSG code for AY-3-8910

#include "psg_sounds.h"
#include <psg.h>

#include <stdint.h>


#define A_PERIOD_LOW 0
#define A_PERIOD_HI 1

#define B_PERIOD_LOW 2
#define B_PERIOD_HI 3

#define C_PERIOD_LOW 4
#define C_PERIOD_HI 5
	
#define NOISE 6
#define CONTROL 7

#define A_VOLUME 8
#define B_VOLUME 9
#define C_VOLUME 10
	
#define ENV_PERIOD_LOW 11
#define ENV_PERIOD_HI 12

#define ENV_WAVE 13


#define POKE(addr,val)     (*(uint8_t*) (addr) = (val))

#if defined(__MSX__)
void _XL_INIT_SOUND(void)
{
    POKE(0xF3DB,0);
}
// #elif defined(__SMS__)

// #include <psg.h>

// void _XL_INIT_SOUND(void)
// {
    // psg_init();
  
// }
#else
    
#include <psg.h>

void _XL_INIT_SOUND(void)
{
    psg_init();
}
#endif


void _ping_sound(uint8_t freq)
{
	uint16_t i;
    
	psg_volume(0,15);
    
    psg_tone(0,freq);

    for(i=0;i<220;i++)
    {
    }

    psg_volume(0,0);
}


void _XL_ZAP_SOUND(void) 
{
    uint8_t i;
    uint8_t j;

	psg_volume(0,15);
    
    for(i=0;i<30;i++)
    {
        for(j=0;j<i;++j)
        {
        }
        psg_tone(0,i*4);
        
    }   
    psg_volume(0,0);
}



void _explosion_sound(uint8_t freq)
{ 
    uint8_t i;
    uint8_t j;

	psg_volume(0,15);
    
    for(i=0;i<30;i++)
    {
        psg_tone(0,freq);
        for(j=0;j<i;++j)
        {
        }
        psg_tone(0,i&3);
        for(j=0;j<i;++j)
        {
        }
        
    }   
    psg_volume(0,0);
}

