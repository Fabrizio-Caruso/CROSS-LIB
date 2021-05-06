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
#include <creativision.h>

#include "standard_libs.h"

#include "creativision_sounds.h"


void play_frequency(uint16_t value)
{
    uint16_t i;
    
    psg_outb(value&0xFF); // Latch frequency
    psg_outb((value&0xFF00)>>8); // Frequency byte 2
    psg_outb(0x90); // Channel 0 full volume
    for(i=0;i<600U;++i)
    {
    }
}

void _XL_EXPLOSION_SOUND(void)
{
    play_frequency(350);
    psg_silence();
}

void _XL_PING_SOUND(void)
{
    play_frequency(200);
    psg_silence();
}

void _XL_SHOOT_SOUND(void)
{
    play_frequency(600);
    psg_silence();
}

void _XL_TICK_SOUND(void)
{
    play_frequency(80);
    psg_silence();
}

void _XL_TOCK_SOUND(void)
{
    play_frequency(90);
    psg_silence();
}

void _XL_ZAP_SOUND(void)
{
    play_frequency(1200);
    psg_silence();
}

#endif // _GAMATE_SOUNDS



