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

#ifndef _COMX_SOUNDS
#define _COMX_SOUNDS

#include <comx/rca_vis_sound.h>

    void _ping_sound(uint8_t freq);
    
	#define PING_SOUND() _ping_sound(0x40)
	#define TOCK_SOUND() _ping_sound(0x20)
	#define TICK_SOUND() _ping_sound(0x30)	
	
    void _explosion_sound(uint8_t range);
    
	#define SHOOT_SOUND() _explosion_sound(0x4)
	#define EXPLOSION_SOUND() _explosion_sound(0x2)
	
	void ZAP_SOUND(void);
	
#endif // _COMX_SOUNDS
