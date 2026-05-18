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

#ifndef _VIC_SOUNDS
#define _VIC_SOUNDS
#include <stdint.h>

	void _ping_sound(uint8_t freq);
	#define _XL_PING_SOUND() _ping_sound(0xF0);	
	#define _XL_TOCK_SOUND() _ping_sound(0x08);
	#define _XL_TICK_SOUND() _ping_sound(0xC0);	
	
	void _explosion_sound(uint8_t freq, uint8_t vol);
	#define _XL_SHOOT_SOUND() _explosion_sound(0x50, 0x0B);
	#define _XL_EXPLOSION_SOUND() _explosion_sound(0x80, 0x0F);
	
	void _XL_ZAP_SOUND();
	
#endif // _VIC_SOUNDS

