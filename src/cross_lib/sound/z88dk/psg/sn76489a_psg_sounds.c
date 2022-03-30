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
#elif defined(__SMS__)

#include <psg.h>

void _XL_INIT_SOUND(void)
{
    psg_init();
  
}
#endif

/*

psg_init();                       // questo di solito lo si sposta in main() o nella sezione di inzializzazione
               
                //set_psg(A_VOLUME,15);
                psg_volume(0, 15);
               
                //psg_envelope(0, 500, chanAll);  // con questo avresti un suono che termina da solo, senza settare volume a 0 in fondo
               
                psg_channels(chan0, chanNone); // set first channel to tone generation, reset noise channels
               
                //set_psg(A_PERIOD_LOW,255);
               
                //set_psg(A_PERIOD_HI, 15);
               
                //set_psg(CONTROL, 0xFF  - 0x01);
               
                for(i=0;i<16;i++)
                {
                                //set_psg(A_PERIOD_HI,15-i);                   
                                psg_tone(0, 500-(i*16));
                                for(j=0;j<150;++j)
                                {
                                }
                }
               
                //set_psg(A_VOLUME,0);                            
                psg_volume(0, 0);
 

*/

void _ping_sound(uint8_t freq)
{
	uint16_t i;
    
	psg_volume(1,15);
    
    psg_tone(0,freq);
    // psg_channels(1, 0);

    for(i=0;i<120;i++)
    {
    }

	// psg_volume(0,0); 
    psg_volume(1,0);
    // psg_volume(2,0);
}

void _XL_ZAP_SOUND(void) 
{
    //
}


void _explosion_sound(uint8_t freq)
{ 
    //
}

