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
#include <gamate.h>

#include "standard_libs.h"

#include "gamate_sounds.h"

#define set_volume(value) \
    POKE(AUDIO_BASE+10,value)

#define silence() set_volue(0)

void play_frequency(uint16_t value)
{
    uint16_t i;
    
    set_volume(0xF);  
    POKE(AUDIO_BASE+4,value&0xFF);
    POKE(AUDIO_BASE+5,(value&0x0F00)>>8);
    for(i=0;i<1200U;++i)
    {
    }
    set_volume(0x0);
}


#endif // _GAMATE_SOUNDS



