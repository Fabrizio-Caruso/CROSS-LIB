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
#include "cross_lib.h"
#include "vic20_explosion_sound.h"

#define FREQ 0x80
#define VOL  0x08

	void _explosion_sound(void)
	{ 
		// uint8_t i; 
		// uint8_t j;
		VIC.noise = FREQ; 
		VIC.volume_color |= VOL; 


        // for(i=0;i<254;++i) 
        // { 
        // }

        _XL_SLOW_DOWN(100);

        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
        // __asm__("NOP");
		VIC.volume_color &= 0x00; 
	};	

