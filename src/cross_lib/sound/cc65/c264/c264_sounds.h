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

#ifndef __C264_SOUNDS
#define __C264_SOUNDS
	#include<peekpoke.h>
    #include<stdint.h>
	
	#define NOISE 64
	#define VOICE_1 16
	#define MAX_VOLUME 15
	
	#define LO_FREQ_1 0xFF0E
	#define HI_FREQ_1 0xFF12
	#define SELECT 0xFF11 
	
	void _noise_sound(uint8_t type);
	
	#define _XL_EXPLOSION_SOUND() _noise_sound(220)
	
	#define _XL_SHOOT_SOUND() _noise_sound(160)
	
	void _short_sound(uint8_t freq);
	
	#define _XL_PING_SOUND() _short_sound(200)
	
	#define _XL_TOCK_SOUND() _short_sound(150)
	
	#define _XL_TICK_SOUND() _short_sound(250)
	
	void _XL_ZAP_SOUND(void);
		
#endif // __C264_SOUNDS