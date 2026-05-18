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

#ifndef _CREATIVISION_SOUNDS
#define _CREATIVISION_SOUNDS

#include <peekpoke.h>
#include <creativision.h>

#include "standard_libs.h"

#include "creativision_sounds.h"


void play_frequency(uint16_t value)
{
    psg_outb(value&0xFF); // Latch frequency
    psg_outb((value&0xFF00)>>8); // Frequency byte 2
    psg_outb(0x90); // Channel 0 full volume
    psg_delay(900);
    psg_silence();
}



#endif // _CREATIVISION_SOUNDS



